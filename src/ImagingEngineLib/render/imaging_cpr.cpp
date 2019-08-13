#include "render/imaging_cpr.h"
#include "render/renderer_cpr.h"
#include "data/vector3.h"
#include "render/render_param_cpr.h"
#include "tools/math.h"
#include "processing/image_process.h"

using namespace DW::Render;
using namespace DW::CV;

CPRImaging::CPRImaging(string id)
	: IThreedImaging(id)
{
	bounding_box_ = new BoundingBox();
	rotation_angle_ = 0.0f;
}

CPRImaging::~CPRImaging()
{

}

VolData* CPRImaging::GetData()
{
	return volume_data_;
}

void CPRImaging::SetData(VolData* data)
{
	volume_data_ = data;
	if (volume_data_){
		bounding_box_->Update(0,0,0,data->GetSliceWidth()-1,data->GetSliceHeight()-1,data->GetSliceCount()-1);
	}
	else{
		bounding_box_->Update(0,0,0,0,0,0);
	}
}

ShowBuffer* CPRImaging::GetShowBuffer()
{
	if (renderer_){
		return renderer_->GetShowBuffer();
	}
	return NULL;
}

void CPRImaging::Render()
{
	if (renderer_){
		renderer_->Render();
	}
}

void CPRImaging::SetRenderer(IThreedRenderer* renderer)
{
	renderer_ = renderer;
}

void CPRImaging::Zoom(float scale)
{
	//TODO cannot go here
	if (renderer_){
		RenderParam* param = renderer_->GetRenderParam();
		if (param){

			scale_factor_ *= scale;
			param->SetScale(scale_factor_);
		}
	}
}

void CPRImaging::ZoomToFitWindow()
{

}

void CPRImaging::Move(float dx, float dy)
{
	//TODO cannot go here
	RenderParam* param = renderer_->GetRenderParam();
	if (param){

		offset_x_ += dx;
		offset_y_ += dy;
		param->SetOffset(offset_x_, offset_y_);
	}
}

void CPRImaging::Move(float motionVector[3])
{
	// Not applicable
}

void CPRImaging::Rotate(float angle)
{
	//TODO cannot go here
	if (renderer_){

		RenderParam* param = renderer_->GetRenderParam();
		if (param){

			roll_angle_ += angle;

		}
	}
}

void CPRImaging::Rotate3D(Vector3f axis, float angle)
{
	if (renderer_){
		CPRRenderParam* param = dynamic_cast<CPRRenderParam*>(renderer_->GetRenderParam());
		if (param){

			//if (param->GetRenderMode() == CPRRenderMode::STRETCHED_CPR_VER ||
			//	param->GetRenderMode() == CPRRenderMode::STRETCHED_CPR_HOR){
			float direction[3]={0}, new_direction[3]={0};
			//TODO CPR旋转轴需要由上层应用设置
			// 旋转轴和切割线方向都应该基于<患者坐标系>. -->>图像库坐标都建立在患者坐标系基础上
			param->SetAxis(axis[0], axis[1], axis[2]);
			param->GetDirection(direction[0], direction[1], direction[2]);

			MathTool::RotateAroundAxis(axis[0], axis[1], axis[2], 
				direction[0], direction[1], direction[2], 
				new_direction[0], new_direction[1], new_direction[2],
				angle);

			param->SetDirection(new_direction[0], new_direction[1], new_direction[2]);

			//}
			//else if (param->GetRenderMode() == CPRRenderMode::STRAIGHTENED_CPR_VER ||
			//	param->GetRenderMode() == CPRRenderMode::STRAIGHTENED_CPR_HOR){

			// 累加旋转角度，用于拉直cpr
			rotation_angle_ += angle;

			param->SetAngle(rotation_angle_);
			//}
			renderer_->Render();
		}
	}
}


void CPRImaging::Rotate3D(Vector3f &axis, Point3f &point, float angle) 
{
	// No implementation
}


void CPRImaging::WindowWidthLevel(int width, int level)
{
	//TODO cannot go here
	if (renderer_){
		CPRRenderParam* param = dynamic_cast<CPRRenderParam*>(renderer_->GetRenderParam());
		if (param){
			param->SetWindowWidthLevel(width, level);

			renderer_->BufferTransform();

		}
	}
}

void CPRImaging::WindowWidthLevelOffset(int offset_width, int offset_level)
{
	//TODO cannot go here
	if (renderer_){
		CPRRenderParam* param = dynamic_cast<CPRRenderParam*>(renderer_->GetRenderParam());
		if (param){
			int width, level;
			param->GetWindowWidthLevel(width, level);
			param->SetWindowWidthLevel(width + offset_width, width + offset_level);

			renderer_->BufferTransform();

		}
	}
}