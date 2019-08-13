/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_3d.h
  author: 	 zhangjian
  Brief:	 定义渲染接口，流程是接收三维数据，执行Render方法，输出显示结果ShowBuffer对象

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "render/render_param.h"
#include "data/vol_data.h"
#include "data/camera.h"
#include "data/light.h"
#include "render/pick_3d.h"
#include "render/measure_3d.h"
#include "render/orientation_3d.h"
#include "data/show_buffer.h"
#include "data/render_mode.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{
		///
		// 接收数据，render，输出pixeldata
		///
		class IThreedRenderer
		{
		public:
			IThreedRenderer()
			{
				volume_data_ = NULL;
				render_param_ = NULL;
				show_buffer_ = NULL;
				image_plane_ = NULL;
				camera_ = NULL;
				light_ = NULL;
			}
			/// 设置三维数据
			virtual void SetData(VolData* data)
			{
				volume_data_ = data;
				if (volume_data_ && volume_data_->GetPixelData()){
					volume_data_->GetPixelData()->GetSpacing(voxel_spacing_);

					CGLogger::Info("VolumeRenderer::SetData null");
				}
			}
			virtual void ClearData() = 0;
			virtual void Render() = 0;
			virtual ShowBuffer* GetShowBuffer() = 0;
			virtual void SetRenderParam(RenderParam* param) { render_param_ = param; }
			virtual RenderParam* GetRenderParam() { return render_param_; }
			virtual Camera* GetCamera() { return camera_; }
			virtual RenderMode GetRenderMode() 
			{
				return render_mode_;
			}
			/// Convert world coordinate to display coordinate
			virtual void ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos) = 0;
			virtual void ComputeWorldToDisplay(double x, double y, double z, double display_point[3]) = 0;

			///
			// 返回各种handler
			///
			virtual IThreedPickHandler* GetThreedPickHandler() { return NULL; }
			virtual IThreedMeasureHandler* GetThreedMeasureHandlerr() { return NULL; }
			virtual IThreedOrientationHandler* GetThreedOrientationHandlerr() { return NULL; }

			/// 将vtkImageData格式转换为ShowBuffer
			virtual void BufferTransform() {}

			/// 从三维图像坐标系转换到患者坐标系
			void ImageToPatientCoordinate(float &x, float &y, float &z)
			{
				x *= voxel_spacing_[0];
				y *= voxel_spacing_[1];
				z *= voxel_spacing_[2];
			}
			/// 从患者坐标系转换到三维图像坐标系
			void PatientToImageCoordinate(float &x, float &y, float &z)
			{
				x /= voxel_spacing_[0];
				y /= voxel_spacing_[1];
				z /= voxel_spacing_[2];
			}
			/// 返回ImagePlane，存储了图像方向，间距，层厚，位置等信息
			ImagePlane *GetCurrentImagePlane() { return image_plane_; }
			/// 设置Render标识
			void SetFirstRenderFlag(bool flag)
			{
				is_first_render_ = flag;
			}

		protected:
			/// 图像渲染方式
			RenderMode render_mode_;
			/// 原始体数据
			VolData* volume_data_;
			/// 绘制参数
			RenderParam* render_param_;
			/// 图像显示缓存
			ShowBuffer* show_buffer_;
			/// 图像所在平面数据
			ImagePlane *image_plane_;
			/// 相机
			Camera* camera_;
			/// 灯光
			Light* light_;
			/// 首次渲染标识
			bool is_first_render_;
			/// 存储缓存图片的地址
			string dump_file_name_;
			/// 体素间距
			double voxel_spacing_[3];

		private:


		};
	}
}