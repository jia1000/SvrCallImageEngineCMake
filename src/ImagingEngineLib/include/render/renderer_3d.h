/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_3d.h
  author: 	 zhangjian
  Brief:	 ������Ⱦ�ӿڣ������ǽ�����ά���ݣ�ִ��Render�����������ʾ���ShowBuffer����

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
		// �������ݣ�render�����pixeldata
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
			/// ������ά����
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
			// ���ظ���handler
			///
			virtual IThreedPickHandler* GetThreedPickHandler() { return NULL; }
			virtual IThreedMeasureHandler* GetThreedMeasureHandlerr() { return NULL; }
			virtual IThreedOrientationHandler* GetThreedOrientationHandlerr() { return NULL; }

			/// ��vtkImageData��ʽת��ΪShowBuffer
			virtual void BufferTransform() {}

			/// ����άͼ������ϵת������������ϵ
			void ImageToPatientCoordinate(float &x, float &y, float &z)
			{
				x *= voxel_spacing_[0];
				y *= voxel_spacing_[1];
				z *= voxel_spacing_[2];
			}
			/// �ӻ�������ϵת������άͼ������ϵ
			void PatientToImageCoordinate(float &x, float &y, float &z)
			{
				x /= voxel_spacing_[0];
				y /= voxel_spacing_[1];
				z /= voxel_spacing_[2];
			}
			/// ����ImagePlane���洢��ͼ���򣬼�࣬���λ�õ���Ϣ
			ImagePlane *GetCurrentImagePlane() { return image_plane_; }
			/// ����Render��ʶ
			void SetFirstRenderFlag(bool flag)
			{
				is_first_render_ = flag;
			}

		protected:
			/// ͼ����Ⱦ��ʽ
			RenderMode render_mode_;
			/// ԭʼ������
			VolData* volume_data_;
			/// ���Ʋ���
			RenderParam* render_param_;
			/// ͼ����ʾ����
			ShowBuffer* show_buffer_;
			/// ͼ������ƽ������
			ImagePlane *image_plane_;
			/// ���
			Camera* camera_;
			/// �ƹ�
			Light* light_;
			/// �״���Ⱦ��ʶ
			bool is_first_render_;
			/// �洢����ͼƬ�ĵ�ַ
			string dump_file_name_;
			/// ���ؼ��
			double voxel_spacing_[3];

		private:


		};
	}
}