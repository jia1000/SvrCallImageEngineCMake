#include "render/imaging_mpr.h"
#include "render/renderer_mpr.h"
#include "data/vector3.h"
#include "render/render_param_mpr.h"
#include "processing/image_process.h"
#include "tools/math.h"

using namespace DW::Render;
using namespace DW::CV;

MPRImaging::MPRImaging(string id)
	: IThreedImaging(id)
{
	bounding_box_ = new BoundingBox();
}

MPRImaging::~MPRImaging()
{

}

// 获取原始三维图像数据场
VolData* MPRImaging::GetData()
{
	return volume_data_;
}
// 设置原始三维图像数据场
void MPRImaging::SetData(VolData* data)
{
	volume_data_ = data;
	if (volume_data_){
		bounding_box_->Update(0,0,0,data->GetSliceWidth()-1,data->GetSliceHeight()-1,data->GetSliceCount()-1);
	}
	else{
		bounding_box_->Update(0,0,0,0,0,0);
	}
}
// 获取输出显示图像
ShowBuffer* MPRImaging::GetShowBuffer()
{
	if (renderer_){
		return renderer_->GetShowBuffer();
	}
	return NULL;
}
// 渲染图像，缓存到show_buffer_
void MPRImaging::Render()
{
	if (renderer_){
		// Call renderer's Render method
		renderer_->Render();
	}
}

void MPRImaging::SetRenderer(IThreedRenderer* renderer)
{
	renderer_ = renderer;
}

void MPRImaging::Zoom(float scale) 
{
	//TODO cannot go here
	if (renderer_){
		MPRRenderParam* param = dynamic_cast<MPRRenderParam*>(renderer_->GetRenderParam());
		if (param){

			scale_factor_ *= scale;
			param->SetScale(scale_factor_);
		}
	}
}

void MPRImaging::ZoomToFitWindow()
{
	Point3f top_left, bottom_right;
	Point3f display_top_left, display_bottom_right;
	bounding_box_->GetCornerPoints(top_left, bottom_right);

	renderer_->ComputeWorldToDisplay(top_left, display_top_left);
	renderer_->ComputeWorldToDisplay(bottom_right, display_bottom_right);

	// We recalculate taking into account the display
	double display_width, display_height;
	display_width = fabs(display_top_left.x - display_bottom_right.x);
	display_height = fabs(display_top_left.y - display_bottom_right.y);

	/// Adjust the image depending on whether the window is narrower by width or height. 
	/// If we want to see the whole region that we have chosen, we will adjust Be narrower, 
	/// if we adjust through wider image to the other side
	int image_width = renderer_->GetRenderParam()->GetWidth();
	int image_height = renderer_->GetRenderParam()->GetHeight();
	double ratio = min(image_width / display_width, image_height / display_height);
	double margin_rate = 0.01;
	double factor = ratio * (1.0 - margin_rate);

	/// Update zoom factor to camera
	renderer_->GetCamera()->Zoom(factor);
	/// Call render
	renderer_->Render();
}

void MPRImaging::Move(float dx, float dy) 
{
	//TODO cannot go here
	if (renderer_){

		MPRRenderParam* param = dynamic_cast<MPRRenderParam*>(renderer_->GetRenderParam());
		if (param){

			// Cache offset number to let opencv generate a new image
			offset_x_ += dx;
			offset_y_ += dy;
			param->SetOffset(offset_x_, offset_y_);
		}
	}
}

void MPRImaging::Move(float motionVector[3]) 
{
	if (renderer_){
		MPRRenderParam* param = dynamic_cast<MPRRenderParam*>(renderer_->GetRenderParam());
		if (param){
			
			// Set image center to a new position
			param->SetImageCenter(motionVector[0], motionVector[1], motionVector[2]);
			// Call render to recalculate mpr plane according to the new position
			renderer_->Render();
		}
	}
}

void MPRImaging::Rotate(float angle) 
{
	//TODO cannot go here
	if (renderer_){

		MPRRenderParam* param = dynamic_cast<MPRRenderParam*>(renderer_->GetRenderParam());
		if (param){

			// Cache angle number to let opencv generate a new image
			roll_angle_ += angle;
		}
	}
}

void MPRImaging::Rotate3D(Vector3f axis, float angle) 
{
	// No implementation
}

void MPRImaging::Rotate3D(Vector3f &axis, Point3f &point, float angle) 
{
	// No implementation
}

void MPRImaging::WindowWidthLevel(int width, int level) 
{
	//TODO cannot go here
	if (renderer_){
		MPRRenderParam* param = dynamic_cast<MPRRenderParam*>(renderer_->GetRenderParam());
		if (param){
			param->SetWindowWidthLevel(width, level);

			renderer_->BufferTransform();
		}
	}
}

void MPRImaging::WindowWidthLevelOffset(int offset_width, int offset_level)
{
	//TODO cannot go here
	if (renderer_){
		MPRRenderParam* param = dynamic_cast<MPRRenderParam*>(renderer_->GetRenderParam());
		if (param){
			int width, level;
			param->GetWindowWidthLevel(width, level);
			param->SetWindowWidthLevel(width + offset_width, width + offset_level);

			renderer_->BufferTransform();
		}
	}
}