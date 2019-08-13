/*=========================================================================

  Program:   ImagingEngine
  Module:    curve_line.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data/curve_line.h"
#include "vtk_include.h"
//#include "vtkPolyDataReader.h"
#include "vtkSplineFilter.h"
#include "vtkParametricSpline.h"
#include "vtkParametricFunctionSource.h"
#include "vtkCardinalSpline.h"
#include "tools/math.h"
#include "tools/string_util.h"
#include "exception/data_exception.h"
#include "exception/render_exception.h"

using namespace DW::IMAGE;
using namespace DW::Exception;

/// Curve线按照正常体位排序
bool CurvePointCompare(Point3f first, Point3f second)
{
	float first_pos = first.z;
	float second_pos = second.z;
	return second_pos < first_pos;
}


VolCurve::VolCurve()
{
	using_consistent_normal_ = true;
	head_to_feet_ = true;
	curve_id_ = GenerateGUID();  
}

VolCurve::~VolCurve()
{
}

void VolCurve::AddControlPoint(Point3f point)
{
	control_points_.push_back(point);
}
void VolCurve::AddControlPoint(float x, float y, float z)
{
	Point3f point;
	point.x = x;
	point.y = y;
	point.z = z;
	control_points_.push_back(point);
}

void VolCurve::RemoveControlPoint(int pos)
{
	if (control_points_.empty()) {
		return;
	}
	auto it = control_points_.begin();
	it += pos;
	if (it != control_points_.end())
	{
		control_points_.erase(it);
	}
}

void VolCurve::GetControlPoint(int pos, float& x, float& y, float& z)
{
	int point_count = control_points_.size();
	if(pos > point_count-1)
		return;
	x = control_points_[pos].x;
	y = control_points_[pos].y;
	z = control_points_[pos].z;
}

void VolCurve::GetSamplePoint(int pos, float& x, float& y, float& z)
{
	int point_count = sample_points_.size();
	if(pos > point_count-1)
		return;

	if (!head_to_feet_){
		pos = point_count-1 - pos;
	}
	x = sample_points_[pos].x;
	y = sample_points_[pos].y;
	z = sample_points_[pos].z;
}

void VolCurve::Update()
{
	// 0. Prepare data
	sample_points_.clear();
	// 设置生成曲线的间隔距离，单位同curve_length
	float unit_legnth =	1;
	// 控制点总数
	int control_points = GetNumberOfControlPoint();
	float tmp_point[3];

	if (control_points <= 0 || 
		voxel_spacing_[0] < MathTool::kEpsilon ||
		voxel_spacing_[1] < MathTool::kEpsilon ||
		voxel_spacing_[2] < MathTool::kEpsilon){

		throw DataException(RET_STATUS_ENGINE_INPUT_INVALID_PARAM, "no control point or invalid pixel spacings.");
	}
	
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	// 由三维图像坐标系转换到患者坐标系是为了计算真实的长度，用来生成等距离的采样点
	int i;
	for(i=0; i<control_points-1; ++i){
		tmp_point[0] = control_points_[i].x * voxel_spacing_[0];
		tmp_point[1] = control_points_[i].y * voxel_spacing_[1];
		tmp_point[2] = control_points_[i].z * voxel_spacing_[2];
		points->InsertNextPoint(tmp_point);
	}
	// 1. Create
	vtkSmartPointer<vtkParametricSpline> splinepp =
		vtkSmartPointer<vtkParametricSpline>::New();
	splinepp->SetPoints(points);

	vtkSmartPointer<vtkParametricFunctionSource> functionSource =
		vtkSmartPointer<vtkParametricFunctionSource>::New();
	functionSource->SetParametricFunction(splinepp);
	functionSource->Update();

	vtkSmartPointer<vtkCardinalSpline> cardinal_spine = vtkSmartPointer<vtkCardinalSpline>::New();

	vtkSmartPointer<vtkSplineFilter> spline =
		vtkSmartPointer<vtkSplineFilter>::New();
	spline->SetInputConnection(functionSource->GetOutputPort());
	// Use cardinal spine to generate sample points
	spline->SetSpline(cardinal_spine);
	// Get subdivisions with equal length
	spline->SetSubdivideToLength();
	//TODO 此处生成更密的采样点，在生成CPR时等间距重建
	spline->SetLength(voxel_spacing_[0] / 3.0);
	// Get a certain number of subdivisions 
	//spline->SetSubdivideToSpecified();
	//spline->SetNumberOfSubdivisions(resolution);
	spline->Update();

	unsigned int rows = spline->GetOutput()->GetNumberOfPoints();
	unsigned int row;
	for (row = 0; row < rows; row++)
	{
		// 由患者坐标系转换到三维图像坐标系
		double p[3];
		spline->GetOutput()->GetPoint(row, p);
		Point3f pnt;
#ifdef  CURVE_PATIENT_COORDINATE
		pnt.x = (float)p[0];
		pnt.y = (float)p[1];
		pnt.z = (float)p[2];
#else
		pnt.x = (float)p[0] / voxel_spacing_[0];
		pnt.y = (float)p[1] / voxel_spacing_[1];
		pnt.z = (float)p[2] / voxel_spacing_[2];
#endif //  CURVE_PATIENT_COORDINATE

		sample_points_.push_back(pnt);
	}

	if (rows > 0){
		// 按照头上脚下的顺序排序
		//std::sort(sample_points_.begin(), sample_points_.end(), CurvePointCompare);
		if (sample_points_[0].z > sample_points_[rows - 1].z){
			head_to_feet_ = false;
		}
		else{
			head_to_feet_ = true;
		}
		CGLogger::Info("curve line has " + to_string(rows) + " sample points. " + to_string(head_to_feet_));
	}
	else{
		throw DataException(RET_STATUS_ENGINE_INPUT_INVALID_PARAM, "curve line has no sample point.");
	}

	// CPR行列间距都为voxel_spacing_[0]
	curve_length_in_mm_ = rows * voxel_spacing_[0];
	// 生成每个采样点切线向量以及局部平面坐标
	ComputeSampleVectors(); 
}

void VolCurve::ComputeCurveLengthBySamplePoints()
{
	float curve_length = 0.0f;
	float point1[3], point2[3];
	int number_of_points = sample_points_.size();
	for(int i=0; i<number_of_points-1; ++i)
	{
#ifdef CURVE_PATIENT_COORDINATE
		point1[0] = sample_points_[i].x;
		point1[1] = sample_points_[i].y;
		point1[2] = sample_points_[i].z;

		point2[0] = sample_points_[i+1].x;
		point2[1] = sample_points_[i+1].y;
		point2[2] = sample_points_[i+1].z;
#else
		point1[0] = sample_points_[i].x * voxel_spacing_[0];
		point1[1] = sample_points_[i].y * voxel_spacing_[1];
		point1[2] = sample_points_[i].z * voxel_spacing_[2];

		point2[0] = sample_points_[i+1].x * voxel_spacing_[0];
		point2[1] = sample_points_[i+1].y * voxel_spacing_[1];
		point2[2] = sample_points_[i+1].z * voxel_spacing_[2];
#endif

		curve_length += MathTool::ComputeDistanceBetweenPoints(point1, point2);
	}
	curve_length_in_mm_ = curve_length;
}

float VolCurve::CalculateDistance(int from_index, int to_index)
{
	int total_points = GetNumberOfSamplePoint();
	if(from_index <0)
		from_index = 0;
	if(to_index > total_points - 1)
		to_index = total_points - 1;
	float length =0;
	float x1,y1,z1,x2,y2,z2;
	int i;
	for(i=from_index; i<to_index; i++)
	{
		x1=sample_points_[i].x;
		y1=sample_points_[i].y;
		z1=sample_points_[i].z;

		x2=sample_points_[i+1].x;
		y2=sample_points_[i+1].y;
		z2=sample_points_[i+1].z;

		float len =sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1));
		length += len;
	}

	return length;
}

float VolCurve::CalculateDistanceInMM(int from_index, int to_index)
{
	int total_points = GetNumberOfSamplePoint();
	if(from_index <0)
		from_index = 0;
	if(to_index > total_points - 1)
		to_index = total_points - 1;
	float length =0;
	float x1,y1,z1,x2,y2,z2;
	int i;
	for(i=from_index; i<to_index; i++)
	{
		x1=sample_points_[i].x;
		y1=sample_points_[i].y;
		z1=sample_points_[i].z;

		x2=sample_points_[i+1].x;
		y2=sample_points_[i+1].y;
		z2=sample_points_[i+1].z;

		float len =sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1));
		length += len;
	}

	return length;
}

void VolCurve::GenerateCurve()
{
	// 采用Cardinal插值生成曲线
	sample_points_.clear();
	// 控制点总数
	int control_points = GetNumberOfControlPoint();

	float tmp_point[3];
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	int i;
	for(i=0;i<control_points-1;i++){
		tmp_point[0]=control_points_[i].x;
		tmp_point[1]=control_points_[i].y;
		tmp_point[2]=control_points_[i].z;
		
		points->InsertNextPoint(tmp_point);
	}

	vtkSmartPointer<vtkParametricSpline> spline =
		vtkSmartPointer<vtkParametricSpline>::New();
	spline->SetPoints(points);
	
	vtkSmartPointer<vtkParametricFunctionSource> functionSource =
		vtkSmartPointer<vtkParametricFunctionSource>::New();
	functionSource->SetParametricFunction(spline);
	functionSource->Update();

	vtkSmartPointer<vtkPolyData> line = functionSource->GetOutput();
	unsigned int rows = line->GetNumberOfPoints();
	double p[3];
	unsigned int row;
	for (row = 0; row < rows; row++)
	{
		line->GetPoint(row, p);
		Point3f sample_point;
		sample_point.x = p[0];
		sample_point.y = p[1];
		sample_point.z = p[2];
		sample_points_.push_back(sample_point);
	}
}

bool VolCurve::IsSame3DPoint(Point3f point)
{
	float x = point.x - sample_points_[sample_points_.size() - 1].x;
	float y = point.y - sample_points_[sample_points_.size() - 1].y;
	float z = point.z - sample_points_[sample_points_.size() - 1].z;
	if (fabs(x) < MathTool::kEpsilon && fabs(y) < MathTool::kEpsilon && fabs(z) < MathTool::kEpsilon)
		return true;
	else
		return false;
}

void VolCurve::GetCurveDirection(float& x, float& y, float& z)
{
	// 获取曲线的趋势方向
	// 目前以首尾点的方向来计算
	int point_number = sample_points_.size();
	if (point_number < 1) return;

	int index_first = 0;
	int index_last = point_number-1;
	if (!head_to_feet_){
		index_first = point_number-1;
		index_last = 0;
	}

	Point3f first = sample_points_[index_first];
	Point3f last = sample_points_[index_last];

	float dir[3];
	dir[0] = last.x - first.x;
	dir[1] = last.y - first.y;
	dir[2] = last.z - first.z;
	MathTool::Normalize(dir);

	x = dir[0];
	y = dir[1];
	z = dir[2];
}

void VolCurve::ComputeSampleVectors()
{
	tangent_vectors_.clear();
	normal_vectors_.clear();
	binormal_vectors_.clear();

	int sample_count = GetNumberOfSamplePoint();
	// compute tangent vectors
	int i;
	for(i = 0 ; i<sample_count ; ++i){
		Vector3f tangent;
		if( i == 0 )
			this->ComputeTangentVector( sample_points_[0], sample_points_[1], tangent );
		else if( i == sample_count-1 )
			this->ComputeTangentVector( sample_points_[sample_count-2], sample_points_[sample_count-1],tangent );
		else
			this->ComputeTangentVector( sample_points_[i], sample_points_[i+1],tangent );

		MathTool::Normalize(tangent);
		// add to vector list
		tangent_vectors_.push_back(tangent);
	}
	// compute normal and binormal vectors
	for(i = 0 ; i<sample_count; ++i)
	{
		Vector3f normal;
		if( !this->using_consistent_normal_ || i == 0){
			Vector3f tangentLast, tangentNext;
			if ( i == 0 ){
				tangentLast = tangent_vectors_[0];
			}
			else{
				tangentLast = tangent_vectors_[i-1];
			}
			if ( i == sample_count-1 ){
				tangentNext = tangent_vectors_[i];
			}
			else{
				tangentNext = tangent_vectors_[i];	//i+1
			}
			ComputeNormalVector( tangentLast, tangentNext, normal );
		}

		if( this->using_consistent_normal_ && i != 0)
		{

			Vector3f tangent, lastNormal;
			lastNormal = normal_vectors_[i-1];
			tangent = tangent_vectors_[i];

			ComputeConsistentNormalVector( tangent, lastNormal, normal );
		}

		MathTool::Normalize( normal );
		// add to vector list
		normal_vectors_.push_back(normal);
		// compute binormal vector
		Vector3f tangent, binormal;
		tangent = tangent_vectors_[i];
		MathTool::Cross( tangent, normal, binormal );
		MathTool::Normalize( binormal );
		// add to vector list
		binormal_vectors_.push_back(binormal);
	}
}

void VolCurve::ComputeTangentVector(Point3f last_pnt, Point3f next_pnt, Vector3f& tangent)
{
	tangent[0] = next_pnt.x - last_pnt.x;
	tangent[1] = next_pnt.y - last_pnt.y;
	tangent[2] = next_pnt.z - last_pnt.z;
}

void VolCurve::ComputeNormalVector(Vector3f last_tangent, Vector3f next_tangent, Vector3f& normal)
{
	int comp;
	for( comp = 0; comp < 3; comp++ )
		normal[comp] = ( next_tangent[comp] - last_tangent[comp] );

	if( MathTool::Norm(normal) == 0 )	// last_tangent == next_tangent
	{
		Vector3f unit(1,0,0);
		MathTool::Cross( last_tangent, unit, normal );
	}
}

void VolCurve::ComputeConsistentNormalVector(Vector3f tangent, Vector3f last_normal, Vector3f& normal)
{
	Vector3f temp;
	MathTool::Cross( last_normal, tangent, temp);
	MathTool::Cross( tangent, temp, normal );
}

void VolCurve::GetTangentVector(int index, Vector3f& tangent)
{
	if (index < tangent_vectors_.size()){
		tangent = tangent_vectors_[index];
	}
}

void VolCurve::GetLocalPlaneVector(int index, Vector3f& x, Vector3f& y)
{
	if (index < normal_vectors_.size()){
		x = normal_vectors_[index];
	}
	if (index < binormal_vectors_.size()){
		y = binormal_vectors_[index];
	}
}