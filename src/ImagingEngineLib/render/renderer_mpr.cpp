/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_mpr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render/renderer_mpr.h"
#include "render/render_param_mpr.h"
#include "tools/vtk_image_data_creator.h"
#include "tools/convert_vtk_image_data_to_rgba.h"
#include "tools/math.h"
#include "tools/timer.h"
#include "tools/logger.h"
#include "processing/image_process.h"
#include "exception/data_exception.h"
#include "exception/render_exception.h"

using namespace std;
using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::CV;
using namespace DW::Exception;


MPRRenderer::MPRRenderer()
	: IThreedRenderer()
{
	render_mode_ = RenderMode::MPR;
	is_first_render_ = true;
	show_buffer_ = new ShowBuffer();
	image_plane_ = new ImagePlane();
}

MPRRenderer::~MPRRenderer()
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

void MPRRenderer::Render()
{
	if (volume_data_ == NULL) {
		CGLogger::Error("MPRRenderer::Render >> volume data is null.");
		return;
	}

	DoRender();
}

void MPRRenderer::SetData(VolData* data)
{
	volume_data_ = data;
	if (NULL == volume_data_){
		CGLogger::Info("MPRRenderer::SetData null");
		return;
	}
	if (volume_data_->GetPixelData()){
		volume_data_->GetPixelData()->GetSpacing(voxel_spacing_);
	}
	else{
		CGLogger::Error("MPRRenderer::SetData >> volume data has no pixel data.");
		throw DataException(RET_STATUS_ENGINE_DATA_PIXELDATA_NULL, "volume data has no pixel data.");
	}
}

ShowBuffer *MPRRenderer::GetShowBuffer()
{
	return show_buffer_;
}

void MPRRenderer::DoRender()
{
	Timer::begin("MPR::DoRender");

	MPRRenderParam* param_imp = dynamic_cast<MPRRenderParam *>(render_param_);
	if (param_imp == NULL){
		CGLogger::Error("MPRRenderer::DoRender >> no render param exists.");
		throw DataException(RET_STATUS_ENGINE_INPUT_INVALID_PARAM, "no render param exists.");
	}

	// 图像属性
	int image_width = volume_data_->GetSliceWidth();
	int image_height = volume_data_->GetSliceHeight();
	int image_count = volume_data_->GetSliceCount();
	BoundingBox *box = volume_data_->GetBoundingBox();

	// 层厚设置
	float thickness = param_imp->GetThickness();
	float half_thickness = thickness * 0.5f;
	// 行列向量
	Vector3f row_vector, col_vector, normal_vector;
	param_imp->GetPlaneVector(row_vector, col_vector);
	// 平面法向量
	MathTool::Cross(row_vector, col_vector, normal_vector);
	// 图像中心点
	//TODO 单位mm？
	Point3f center_point;
	param_imp->GetImageCenter(center_point);

	// MPR图像宽高
	int width = param_imp->GetWidth();
	int height = param_imp->GetHeight();
	CalculateImageSize(box, normal_vector, center_point, width, height);

	//////////////////////////////////////////////////////////////////////////
	double r = 0.0, d = 0.0;
	// 计算最大宽度
	double whole_extent[3] = { box->GetXLength(), box->GetYLength(), box->GetZLength() };
	int j;
	for (j = 0; j < 3; j++)
	{
		double tmp = row_vector[j] * row_vector[j];
		d += tmp * whole_extent[j] * fabs(voxel_spacing_[j]);
		r += tmp;
	}
	d /= r*sqrt(r);
	width = fabs(d / voxel_spacing_[0]);
	// 临时值复位
	r = d = 0.0;
	// 计算最大高度
	for (j = 0; j < 3; j++)
	{
		double tmp = col_vector[j] * col_vector[j];
		d += tmp * whole_extent[j] * fabs(voxel_spacing_[j]);
		r += tmp;
	}
	d /= r*sqrt(r);
	height = fabs(d / voxel_spacing_[1]);
	//////////////////////////////////////////////////////////////////////////

	width = param_imp->GetWidth();
	height = param_imp->GetHeight();
	
	// 初始化buffer
	show_buffer_->InitBufferData(width, height, 16);
	short *ptr = reinterpret_cast<short *>(show_buffer_->GetShowBuffer());

	float x, y, z;
	float gR, gC;
	int i;
	for (i=0; i<height; ++i){

		gC = (i - (float)(height / 2.0)) * voxel_spacing_[0];

		for (j=0; j<width; ++j){
			// 计算三维坐标点: 以center_point为图像几何中心点
			//x = center_point.x + (j - center_point.x) * row_vector[0] + (i - center_point.y) * col_vector[0];
			//y = center_point.y + (j - center_point.x) * row_vector[1] + (i - center_point.y) * col_vector[1];
			//z = center_point.z + (j - center_point.x) * row_vector[2] + (i - center_point.y) * col_vector[2];

			gR = (j - (float)(width / 2.0)) * voxel_spacing_[0];

			x = (center_point.x + gR * row_vector[0] + gC * col_vector[0]) / voxel_spacing_[0];
			y = (center_point.y + gR * row_vector[1] + gC * col_vector[1]) / voxel_spacing_[1];
			z = (center_point.z + gR * row_vector[2] + gC * col_vector[2]) / voxel_spacing_[2];

			if (x < MathTool::kEpsilon || x > image_width - 1 || 
				y < MathTool::kEpsilon || y > image_height - 1 || 
				z < MathTool::kEpsilon || z > image_count - 1){
					continue;
			}

			short val = -1024;			
			TrilinearInterpolation(x, y, z, val);
			*(ptr + i * width + j) = val;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	// 以下设置输出图像平面信息
	// 图像方向
	Vector3d row_vec_d(row_vector[0], row_vector[1], row_vector[2]);
	Vector3d col_vec_d(col_vector[0], col_vector[1], col_vector[2]);
	ImageOrientation orientation(row_vec_d, col_vec_d);
	image_plane_->SetImageOrientation(orientation);
	// 行列间距
	PixelSpacing2D spacing_2d(voxel_spacing_[0], voxel_spacing_[0]);
	image_plane_->SetSpacing(spacing_2d);
	// 层厚
	image_plane_->SetThickness(param_imp->GetThickness());	
	// 左上角第一个像素在患者坐标系中的坐标
	double left_top_origin[3];
	double tmp_dist_w = ((double)width / 2.0) * voxel_spacing_[0];
	double tmp_dist_h = ((double)height / 2.0) * voxel_spacing_[0];
	left_top_origin[0] = center_point.x + tmp_dist_w * row_vector[0] + tmp_dist_h * col_vector[0];
	left_top_origin[1] = center_point.y + tmp_dist_w * row_vector[1] + tmp_dist_h * col_vector[1];
	left_top_origin[2] = center_point.z + tmp_dist_w * row_vector[2] + tmp_dist_h * col_vector[2];
	image_plane_->SetOrigin(left_top_origin[0], left_top_origin[1], left_top_origin[2]);
	// 宽高
	image_plane_->SetRowLength(width * voxel_spacing_[0]);
	image_plane_->SetColumnLength(height * voxel_spacing_[0]);
	//////////////////////////////////////////////////////////////////////////

	//TODO show buffer不再需要存储间距了
	show_buffer_->SetDPI(voxel_spacing_[0], voxel_spacing_[0]);

	//Timer::end("MPR::DoRender::Loop");

	Timer::end("MPR::DoRender");
	CGLogger::Info(Timer::summery());
}

void MPRRenderer::BufferTransform()
{
	//if (output_vtk_image_data_ && show_buffer_){

	//	//vtkSmartPointer<vtkImageData> formatted_image_data;
	//	//vtkTransform *imageTransform = vtkTransform::New();
	//	//imageTransform->PostMultiply();
	//	///// Apply scale factor
	//	//float scale_factor = render_param_->GetScale();
	//	//imageTransform->Scale(scale_factor, scale_factor, scale_factor);
	//	///// Apply translation
	//	//float move_vector[3];
	//	//render_param_->GetMove(move_vector);
	//	//imageTransform->Translate(move_vector[0], move_vector[1], move_vector[2]);
	//	//vtkTransformFilter *transform_filter = vtkTransformFilter::New(); 
	//	//transform_filter->SetTransform(imageTransform);
	//	//transform_filter->SetInputData(output_vtk_image_data_);
	//	//transform_filter->Update();
	//	//formatted_image_data = (vtkImageData *)transform_filter->GetOutput();
	//	//if (formatted_image_data == NULL){
	//	//	return;
	//	//}

	//	// 原来的方法，用来对比生成的图像
	//	int width = output_vtk_image_data_->GetDimensions()[0];
	//	int height = output_vtk_image_data_->GetDimensions()[1];

	//	int ww, wl;
	//	(dynamic_cast<MPRRenderParam *>(render_param_))->GetWindowWidthLevel(ww, wl);
	//	// convert to 32 bits bitmap
	//	vtkImageData *pTmpImageData = NULL;	
	//	ConvertVtkImagedataToRGBA* convert = new ConvertVtkImagedataToRGBA();
	//	if (false == convert->ConvertImageScalarsToRGBA(output_vtk_image_data_, &pTmpImageData, -1, ww, wl))
	//	{
	//		return;
	//	}

	//	//// 写入磁盘文件
	//	//vtkSmartPointer<vtkBMPWriter> writer = vtkSmartPointer<vtkBMPWriter>::New();
	//	//writer->SetFileName(dump_file_name_.c_str());
	//	//writer->SetInputData(pTmpImageData);
	//	//writer->Write();

	//	UNITDATASHOW* pdata = reinterpret_cast<UNITDATASHOW*>(pTmpImageData->GetScalarPointer());
	//	int number_of_components = pTmpImageData->GetNumberOfScalarComponents();

	//	show_buffer_->SetBufferData(pdata, width, height, number_of_components * 8);

	//}
}

void MPRRenderer::SetOffScreenRendering(bool flag)
{
	is_off_screen_rendering_ = flag;
}

bool MPRRenderer::PickByImagePos(const Vector2i& in_point, Vector3d& out_point)
{
	return false;
}

bool MPRRenderer::PickByImagePos(const int& x, const int& y, Vector3d& out_point)
{
	return false;
}

bool MPRRenderer::PickBy3DPoint(const Vector3d& in_point, int& x, int& y)
{
	return false;
}

float MPRRenderer::CalculateLength(Vector3d&, Vector3d&)
{
	return 0.0;
}

void MPRRenderer::GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&)
{

}

///
// 三线性插值
///
void MPRRenderer::TrilinearInterpolation(float ix, float iy, float iz, short& ctv)
{
	short gO,gP,gQ,gR,gS,gT,gU,gV,gA,gB,gC,gD,gE,gF; 
	int nnx,nny,nnz;
	nnx = (int)ix;
	nny = (int)iy;
	nnz = MathTool::Round(iz);
	//int width = volume_data_->GetSliceWidth();
	//int height = volume_data_->GetSliceHeight();
	int slice_number = volume_data_->GetSliceCount();
	if (nnz > slice_number - 1) {
		//ctv = 0;	
		return;
	}

	//int nID = nny*width+nnx;
	//int nID1 = (nny+1)*width+nnx;
	float dZ = (iz-nnz);
	// 最后一层不超出范围
	int nnz_next = nnz < slice_number-1 ? nnz+1 : nnz-1;
	gO = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx, nny, nnz)));
	gP = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx, nny, nnz_next)));
	gQ = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx+1, nny, nnz)));
	gR = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx+1, nny, nnz_next)));
	gS = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx, nny+1, nnz)));
	gT = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx, nny+1, nnz_next)));
	gU = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx+1, nny+1, nnz)));
	gV = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx+1, nny+1, nnz_next)));

	gA = (short)(dZ*(gP-gO))+gO;
	gB = (short)(dZ*(gR-gQ))+gQ;
	gC = (short)(dZ*(gT-gS))+gS;
	gD = (short)(dZ*(gV-gU))+gU;
	gE = (short)((ix-nnx)*(gB-gA))+gA;
	gF = (short)((ix-nnx)*(gD-gC))+gC;
	ctv = (short)((iy-nny)*(gF-gE))+gE;
}

void MPRRenderer::BilinearInterpolation(float ix, float iy, float iz, short& ctv, short max_ctv)
{
	short gO, gP, gR, gS, gA, gB, gC;
	int nnx,nny,nnz;
	nnx = (int)ix;
	nny = (int)iy;
	nnz = MathTool::Round(iz);
	int width = volume_data_->GetSliceWidth();
	int height = volume_data_->GetSliceHeight();
	int slice_number = volume_data_->GetSliceCount();
	// x/y方向权重
	float dx = ix - nnx;
	float dy = iy - nny;

	gO = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx, nny, nnz)));
	gP = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx+1, nny, nnz)));
	gR = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx, nny+1, nnz)));
	gS = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx+1, nny+1, nnz)));

	if (gO > max_ctv || gP > max_ctv || gR > max_ctv || gS > max_ctv){

		// 沿x方向线性插值
		gA = dx * gP + (1 - dx) * gO;
		gB = dx * gS + (1 - dx) * gR;
		// 沿y方向线性插值
		gC = dy * gB + (1 - dy) * gA;

		ctv = gC;
	}
	else{
		ctv = max_ctv;
	}	
}

void MPRRenderer::ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos)
{

}

void MPRRenderer::ComputeWorldToDisplay(double x, double y, double z, double display_point[3])
{

}

void MPRRenderer::CalculateImageSize(BoundingBox *box, const Vector3f &normal_vector, const Point3f &origin, int &width, int &height)
{
	//// 包围盒的12条线
	//double point1[3], point2[3];
	//point1[0] = 0;
	//vtkPlane::IntersectWithLine(b)
}


/// <summary>
/// 求一条直线与平面的交点
/// </summary>
/// <param name="normal_vector">平面的法线向量，长度为3</param>
/// <param name="plane_point">平面经过的一点坐标，长度为3</param>
/// <param name="line_vector">直线的方向向量，长度为3</param>
/// <param name="line_point">直线经过的一点坐标，长度为3</param>
/// <returns>返回交点坐标，长度为3</returns>
int ComputePlaneLineIntersection(const Vector3f &normal_vector, const Point3f &plane_point, const Vector3f &line_vector, const Point3f &line_point, Point3f &intersection)
{
	int result;
	float vp1, vp2, vp3, n1, n2, n3, v1, v2, v3, m1, m2, m3, t,vpt;
	vp1 = normal_vector[0];
	vp2 = normal_vector[1];
	vp3 = normal_vector[2];
	n1 = plane_point.x;
	n2 = plane_point.y;
	n3 = plane_point.z;
	v1 = line_vector[0];
	v2 = line_vector[1];
	v3 = line_vector[2];
	m1 = line_point.x;
	m2 = line_point.y;
	m3 = line_point.z;
	vpt = v1 * vp1 + v2 * vp2 + v3 * vp3;
	// 首先判断直线是否与平面平行
	if (vpt == 0){
		result = 0;
	}
	else{
		t = ((n1 - m1) * vp1 + (n2 - m2) * vp2 + (n3 - m3) * vp3) / vpt;
		intersection.x = m1 + v1 * t;
		intersection.y = m2 + v2 * t;
		intersection.z = m3 + v3 * t;

		result = 1;
	}

	return result;
}