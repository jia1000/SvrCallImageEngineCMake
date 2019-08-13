/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_cpr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render/renderer_cpr.h"
#include <vtkWindowToImageFilter.h>
#include <vtkBMPWriter.h>
#include <vtkNew.h>
#include <vtkCamera.h>
#include <vtkColorTransferFunction.h>
#include <vtkDataArray.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkImageData.h>
#include <vtkImageReader.h>
#include <vtkImageShiftScale.h>
#include <vtkNew.h>
#include <vtkOutlineFilter.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
//#include <vtkStructuredPointsReader.h>
#include <vtkTimerLog.h>
#include <vtkVolumeProperty.h>
#include <vtkDICOMImageReader.h>
#include <vtkCallbackCommand.h>

#include "tools/math.h"
#include "render/render_param_cpr.h"
#include "tools/convert_vtk_image_data_to_rgba.h"
#include "tools/timer.h"
#include "tools/logger.h"
#include "processing/image_process.h"
#include "exception/data_exception.h"
#include "exception/render_exception.h"

using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::CV;
using namespace DW::Exception;


CPRRenderer::CPRRenderer()
	: IThreedRenderer()
{
	is_first_render_ = true;
	show_buffer_ = new ShowBuffer();
	image_plane_ = new ImagePlane();
}

CPRRenderer::~CPRRenderer()
{
	if (show_buffer_){
		delete show_buffer_;
		show_buffer_ = NULL;
	}
	if (image_plane_){
		delete image_plane_;
		image_plane_ = NULL;
	}
}

void CPRRenderer::SetData(VolData* data)
{
	volume_data_ = data;
	if (NULL == volume_data_){
		CGLogger::Info("VolumeRenderer::SetData null");
		return;
	}
	if (volume_data_->GetPixelData()){
		volume_data_->GetPixelData()->GetSpacing(voxel_spacing_);
	}
	else{
		CGLogger::Error("CPRRenderer::SetData >> volume data has no pixel data.");
		throw DataException(RET_STATUS_ENGINE_DATA_PIXELDATA_NULL, "volume data has no pixel data.");
	}
}

void CPRRenderer::Render()
{
	if (volume_data_ == NULL) {
		CGLogger::Error("CPRRenderer::Render >> volume data is null.");
		return;
	}

	DoRender();
}

ShowBuffer *CPRRenderer::GetShowBuffer()
{
	return show_buffer_;
}

vtkSmartPointer<vtkPolyData> CPRRenderer::SweepLine (VolCurve *curve,
										float direction[3],
										float distance,
										unsigned int cols)
{
	unsigned int rows = curve->GetNumberOfControlPoint();
	float spacing = distance / cols;
	vtkSmartPointer<vtkPolyData> surface =
		vtkSmartPointer<vtkPolyData>::New();

	// Generate the points
	cols++;
	unsigned int numberOfPoints = rows * cols;
	unsigned int numberOfPolys = (rows - 1) * (cols - 1);
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	points->Allocate(numberOfPoints);
	vtkSmartPointer<vtkCellArray> polys =
		vtkSmartPointer<vtkCellArray>::New();
	polys->Allocate(numberOfPolys*4);

	float x[3];
	unsigned int cnt = 0;
	unsigned int row, col;
	for (row = 0; row < rows; row++)
	{
		for (col = 0; col < cols; col++)
		{
			float p[3];
			curve->GetControlPoint(row, p[0], p[1], p[2]);
			x[0] = p[0] + direction[0] * col * spacing;
			x[1] = p[1] + direction[1] * col * spacing;
			x[2] = p[2] + direction[2] * col * spacing;
			points->InsertPoint(cnt++, x);
		}
	}
	// Generate the quads
	vtkIdType pts[4];
	for (row = 0; row < rows - 1; row++)
	{
		for (col = 0; col < cols - 1; col++)
		{
			pts[0] = col + row * (cols);
			pts[1] = pts[0] + 1;
			pts[2] = pts[0] + cols + 1;
			pts[3] = pts[0] + cols;
			polys->InsertNextCell(4,pts);
		}
	}
	surface->SetPoints(points);
	surface->SetPolys(polys);

	return surface;
}


vtkSmartPointer<vtkPolyData> CPRRenderer::SweepLine2 (vtkPolyData *line,
													 float direction[3],
													 float distance,
													 unsigned int cols)
{
	unsigned int rows = line->GetNumberOfPoints();
	float spacing = distance / cols;
	vtkSmartPointer<vtkPolyData> surface =
		vtkSmartPointer<vtkPolyData>::New();

	// Generate the points
	cols++;
	unsigned int numberOfPoints = rows * cols;
	unsigned int numberOfPolys = (rows - 1) * (cols - 1);
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	points->Allocate(numberOfPoints);
	vtkSmartPointer<vtkCellArray> polys =
		vtkSmartPointer<vtkCellArray>::New();
	polys->Allocate(numberOfPolys*4);

	double x[3];
	unsigned int cnt = 0;
	for (unsigned int row = 0; row < rows; row++)
	{
		for (unsigned int col = 0; col < cols; col++)
		{
			double p[3];
			line->GetPoint(row, p);
			x[0] = p[0] + direction[0] * col * spacing;
			x[1] = p[1] + direction[1] * col * spacing;
			x[2] = p[2] + direction[2] * col * spacing;
			points->InsertPoint(cnt++, x);
		}
	}
	// Generate the quads
	vtkIdType pts[4];
	for (unsigned int row = 0; row < rows - 1; row++)
	{
		for (unsigned int col = 0; col < cols - 1; col++)
		{
			pts[0] = col + row * (cols);
			pts[1] = pts[0] + 1;
			pts[2] = pts[0] + cols + 1;
			pts[3] = pts[0] + cols;
			polys->InsertNextCell(4,pts);
		}
	}
	surface->SetPoints(points);
	surface->SetPolys(polys);

	return surface;
}


void CPRRenderer::SetOffScreenRendering(bool flag)
{
	is_off_screen_rendering_ = flag;
}

bool CPRRenderer::PickByImagePos(const Vector2i& in_point, Vector3d& out_point)
{
	return false;
}

bool CPRRenderer::PickByImagePos(const int& x, const int& y, Vector3d& out_point)
{
	return false;
}

bool CPRRenderer::PickBy3DPoint(const Vector3d& in_point, int& x, int& y)
{
	return false;
}

float CPRRenderer::CalculateLength(Vector3d&, Vector3d&)
{
	return 0.0;
}

void CPRRenderer::GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&)
{

}
///
// ����ֱ�����Χ�е���������
///
int CPRRenderer::Compute3DCrossPoint(const float curr_point[],
						 float direction[],
						 BoundingBox * box,
						 float *cross_point_one, 
						 float *cross_point_two)
{
	float tmp_point[3], img_tmp_point[3];
	int counter = 0;
	float length_arr[3] = { box->GetXLength(), box->GetYLength(), box->GetZLength() };

	// ��Χ��ת������������ϵ
	ImageToPatientCoordinate(length_arr[0], length_arr[1], length_arr[2]);
	int i;
	for (i=0; i<3; ++i) {
		if ( fabs(direction[i]) < MathTool::kEpsilon )
			continue;

		tmp_point[0] = i==0 ? 0 : (-curr_point[i]) * direction[0] / direction[i] + curr_point[0];
		tmp_point[1] = i==1 ? 0 : (-curr_point[i]) * direction[1] / direction[i] + curr_point[1];
		tmp_point[2] = i==2 ? 0 : (-curr_point[i]) * direction[2] / direction[i] + curr_point[2];

		// ת����ͼ������ϵ�жϰ�Χ�н���
		img_tmp_point[0] = tmp_point[0];
		img_tmp_point[1] = tmp_point[1];
		img_tmp_point[2] = tmp_point[2];
		PatientToImageCoordinate(img_tmp_point[0], img_tmp_point[1], img_tmp_point[2]);

		//TODO 
		if (box->IsPointInBoxPlane(img_tmp_point[0], img_tmp_point[1], img_tmp_point[2])) {
			if (++counter == 1) {
				cross_point_one[0] = tmp_point[0];
				cross_point_one[1] = tmp_point[1];
				cross_point_one[2] = tmp_point[2];
			}
			else {
				cross_point_two[0] = tmp_point[0];
				cross_point_two[1] = tmp_point[1];
				cross_point_two[2] = tmp_point[2];
			}
		}

		tmp_point[0] = i==0 ? length_arr[i] : (length_arr[i] -curr_point[i])*direction[0] / direction[i] + curr_point[0];
		tmp_point[1] = i==1 ? length_arr[i] : (length_arr[i] -curr_point[i])*direction[1] / direction[i] + curr_point[1];
		tmp_point[2] = i==2 ? length_arr[i] : (length_arr[i] -curr_point[i])*direction[2] / direction[i] + curr_point[2];

		// ת����ͼ������ϵ�жϰ�Χ�н���
		img_tmp_point[0] = tmp_point[0];
		img_tmp_point[1] = tmp_point[1];
		img_tmp_point[2] = tmp_point[2];
		PatientToImageCoordinate(img_tmp_point[0], img_tmp_point[1], img_tmp_point[2]);

		if (box->IsPointInBoxPlane(img_tmp_point[0], img_tmp_point[1], img_tmp_point[2])) {
			if (++counter == 1) {
				cross_point_one[0] = tmp_point[0];
				cross_point_one[1] = tmp_point[1];
				cross_point_one[2] = tmp_point[2];
			}
			else {
				cross_point_two[0] = tmp_point[0];
				cross_point_two[1] = tmp_point[1];
				cross_point_two[2] = tmp_point[2];
			}
		}
	}
	
	return counter;
}

///
// �����Բ�ֵ
///
void CPRRenderer::TrilinearInterpolation(short& ctv, float ix, float iy, float iz, VolData *data)
{
	short gO,gP,gQ,gR,gS,gT,gU,gV,gA,gB,gC,gD,gE,gF; 
	int nnx,nny,nnz;
	nnx = (int)ix;
	nny = (int)iy;
	nnz = MathTool::Round(iz);
	int width = data->GetSliceWidth();
	int slice_number = data->GetSliceCount();
	int height = data->GetSliceHeight();
	if (nnx < 0 || nnx > width-2 || nny < 0 || nny >height-2 || nnz < 0 || nnz > slice_number - 1) {
		ctv = -1024;	
		return;
	}
	int nID = nny*width+nnx;
	int nID1 = (nny+1)*width+nnx;
	double dZ = (iz-nnz);
	// ���һ�㲻������Χ
	int nnz_next = nnz < slice_number-1 ? nnz+1 : nnz-1;
	gO = *(reinterpret_cast<short *>(data->GetDataPointer(nnx, nny, nnz)));
	gP = *(reinterpret_cast<short *>(data->GetDataPointer(nnx, nny, nnz_next)));
	gQ = *(reinterpret_cast<short *>(data->GetDataPointer(nnx+1, nny, nnz)));
	gR = *(reinterpret_cast<short *>(data->GetDataPointer(nnx+1, nny, nnz_next)));
	gS = *(reinterpret_cast<short *>(data->GetDataPointer(nnx, nny+1, nnz)));
	gT = *(reinterpret_cast<short *>(data->GetDataPointer(nnx, nny+1, nnz_next)));
	gU = *(reinterpret_cast<short *>(data->GetDataPointer(nnx+1, nny+1, nnz)));
	gV = *(reinterpret_cast<short *>(data->GetDataPointer(nnx+1, nny+1, nnz_next)));
	
	gA = (short)(dZ*(gP-gO))+gO;
	gB = (short)(dZ*(gR-gQ))+gQ;
	gC = (short)(dZ*(gT-gS))+gS;
	gD = (short)(dZ*(gV-gU))+gU;
	gE = (short)((ix-nnx)*(gB-gA))+gA;
	gF = (short)((ix-nnx)*(gD-gC))+gC;
	ctv = (short)((iy-nny)*(gF-gE))+gE;
}

void CPRRenderer::UpdateRotation()
{
	
}

void CPRRenderer::SetRotationAxis(float x, float y, float z)
{
	rotation_axis_[0] = x;
	rotation_axis_[1] = y;
	rotation_axis_[2] = z;
}

void CPRRenderer::GetRotationAxis(float& x, float& y, float& z)
{
	x = rotation_axis_[0];
	y = rotation_axis_[1];
	z = rotation_axis_[2];
}

void CPRRenderer::GetRotationCenter(float& x, float& y, float& z)
{
	x = rotation_center_.x;
	y = rotation_center_.y;
	z = rotation_center_.z;
}

void CPRRenderer::CalculateImageSize(VolCurve *curve,
											  float direction[3],
											  float thickness,
											  float& left_distance,
											  float& right_distance)
{
	BoundingBox* box = volume_data_->GetBoundingBox();
	MathTool::Normalize(direction);

	valid_sample_point_indexs_.clear();
	int total_count = curve->GetNumberOfSamplePoint();
	// �õ���һ���������
	float origin[3] = {0,0,0};
	curve->GetSamplePoint(0, origin[0], origin[1], origin[2]);
#ifndef CURVE_PATIENT_COORDINATE
	// CURVEת������������ϵ
	ImageToPatientCoordinate(origin[0], origin[1], origin[2]);
#endif // CURVE_PATIENT_COORDINATE

	float curr_point[3], curr_proj_point[3], cross_point1[3], cross_point2[3], img_curr_point[3], last_point[3];
	cross_point1[0] = cross_point1[1] = cross_point1[2] =0;
	cross_point2[0] = cross_point2[1] = cross_point2[2] =0;
	
	// ����һ��������ؽ�Curve��������
	valid_sample_point_indexs_.push_back(0);
	double threshold = voxel_spacing_[0];
	double accumulated_distance = 0.0;
	int i;
	for(i=1; i<total_count; i++){	

		curve->GetSamplePoint(i - 1, last_point[0], last_point[1], last_point[2]);
		curve->GetSamplePoint(i, curr_point[0], curr_point[1], curr_point[2]);

#ifndef CURVE_PATIENT_COORDINATE
		//if (!box->IsPointInBox(curr_point[0], curr_point[1], curr_point[2])){
		//	// ��Χ�в�����ģ�Ҳ��������������...
		//}
		// CURVEת������������ϵ
		ImageToPatientCoordinate(curr_point[0], curr_point[1], curr_point[2]);
#else
		//img_curr_point[0] = curr_point[0];
		//img_curr_point[1] = curr_point[1];
		//img_curr_point[2] = curr_point[2];
		//PatientToImageCoordinate(img_curr_point[0], img_curr_point[1], img_curr_point[2]);
		//if (!box->IsPointInBox(img_curr_point[0], img_curr_point[1], img_curr_point[2])){
		//	// ��Χ�в�����ģ�Ҳ��������������...
		//}
#endif // CURVE_PATIENT_COORDINATE
		
		double tmp_distance = MathTool::ComputeDistancePointToLine(direction, last_point, curr_point);
		accumulated_distance += tmp_distance;
		if (accumulated_distance > threshold ||
			i == total_count - 1){
			valid_sample_point_indexs_.push_back(i);
			accumulated_distance = 0.0;
		}
		else{
			//TODO Ӧ��������curveʱ�����һЩ�����㣬���ڴ˴��ж����������Ҫ��ĵ���в�ֵ
			continue;
		}

		int cross_count = Compute3DCrossPoint(curr_point, direction, box, cross_point1, cross_point2);
		if (cross_count >= 2){
			GetBoundingBoxBorder(cross_point1, direction, origin, curr_proj_point, left_distance, right_distance);
			GetBoundingBoxBorder(cross_point2, direction, origin, curr_proj_point, left_distance, right_distance);
		}
	}
}

void CPRRenderer::GetBoundingBoxBorder(float cross_pnt[3],
									float direction[3],
									float origin[3],
									float curr_proj_point[3],
									float& left_distance, 
									float& right_distance)
{
	// �����Χ�н����ڵ�һ��������ֱ�߷����ϵ�ͶӰ��
	MathTool::Compute3DProjectionOnLine(direction,origin,cross_pnt,curr_proj_point);
	// ����ͶӰ�����һ��������֮��ľ���
	float tmp_distance = MathTool::ComputeDistanceBetweenPoints(origin,curr_proj_point);

	if (MathTool::IsPointOnTheRightSide(direction, origin, curr_proj_point)){
		if (tmp_distance > right_distance)
			right_distance = tmp_distance;
	}
	else{
		if (tmp_distance > left_distance)
			left_distance = tmp_distance;
	}
}

void CPRRenderer::ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos)
{

}

void CPRRenderer::ComputeWorldToDisplay(double x, double y, double z, double display_point[3])
{

}

void CPRRenderer::BufferTransform()
{
//	if (output_vtk_image_data_ && show_buffer_){
//		
//		/// Apply window width/level
//		int ww = 0, wl = 0;
//		/// ����Ⱦ�����������õ�����λ
//		CPRRenderParam* param_imp = dynamic_cast<CPRRenderParam *>(render_param_);
//		if (param_imp == NULL){
//			return;
//		}
//		param_imp->GetWindowWidthLevel(ww, wl);
//		vtkSmartPointer<vtkLookupTable> colorTable =
//			vtkSmartPointer<vtkLookupTable>::New();
//		/// ����λת������С���ֵ
//		colorTable->SetRange(wl - ww / 2, wl + (ww + 1) / 2);
//		colorTable->SetValueRange(0.0, 1.0);
//		colorTable->SetSaturationRange(0.0, 0.0);		//�Ҷ�ͼ��
//		colorTable->SetRampToLinear();
//		colorTable->Build();
//
//		vtkSmartPointer<vtkImageMapToColors> colorMap =
//			vtkSmartPointer<vtkImageMapToColors>::New();
//		colorMap->SetLookupTable(colorTable);
//#if VTK_MAJOR_VERSION > 5
//		colorMap->SetInputData(output_vtk_image_data_);
//#else
//		colorMap->SetInput(output_vtk_image_data_);
//#endif	
//		colorMap->Update();
//		vtkSmartPointer<vtkImageData> formatted_image_data = colorMap->GetOutput();
//		if (formatted_image_data == NULL){
//			return;
//		}
//
//		//ԭ���ķ����������Ա����ɵ�ͼ��
//		int width = formatted_image_data->GetDimensions()[0];
//		int height = formatted_image_data->GetDimensions()[1];		
//		// ��8λת����32λ
//		int slice = -1;
//
//		UNITDATASHOW* pdata = reinterpret_cast<UNITDATASHOW*>( formatted_image_data->GetScalarPointer() );
//		int number_of_components = formatted_image_data->GetNumberOfScalarComponents();
//
//		show_buffer_->SetBufferData(pdata, width, height, number_of_components * 8);
//		show_buffer_->SetDPI(voxel_spacing_[0], voxel_spacing_[0]);
//	}
}