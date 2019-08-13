/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_3d.h
  author: 	 zhangjian
  Brief:	 ��ʾһ����άͼ��������ά���ݣ�ִ�о������Ⱦ���̣��������ʾ���
			 �ṩ���ڲ���ͼ��Ľӿڣ�����ת/����/ƽ��/����λ��

=========================================================================*/
#pragma once

#include "data/data_definition.h"
//#include "image_source.h"
#include "data/vol_data.h"
#include "render/renderer_3d.h"
#include "render/render_param.h"
#include "data/show_buffer.h"
#include "data/vector3.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class IThreedImaging //: public IImageSource
		{
		public:
			IThreedImaging(string id)
				: image_uid_(id)
				, scale_factor_(1.0f)
				, offset_x_(0.0f)
				, offset_y_(0.0f)
				, roll_angle_(0.0f)
			{

			}
			virtual ~IThreedImaging()
			{
				if (bounding_box_){
					delete bounding_box_;
					bounding_box_ = NULL;
				}
				//if (volume_data_){
				//	delete volume_data_;
				//	volume_data_ = NULL;
				//}
				if (renderer_){
					delete renderer_;
					renderer_ = NULL;
				}
			}
			/// ��ȡͼ������
			virtual RenderMode GetRenderMode()
			{
				if (renderer_){
					return renderer_->GetRenderMode();
				}
				return RenderMode::UNDEFINED;
			}
			/// ����ԭʼ��άͼ�����ݳ� 
			virtual void SetData(VolData* data) = 0;
			/// ��ȡԭʼ��άͼ�����ݳ�
			virtual VolData* GetData() = 0;
			/// ��ȡ�����ʾͼ��
			virtual ShowBuffer* GetShowBuffer() = 0;
			// Imaging����������BITMAP����ֻ�ṩShowBuffer���� [5/24/2019 Modified by zhangjian]
			//virtual HBITMAP GetImageBuffer() { return image_buffer_; }
			/// ��Ⱦͼ��
			virtual void Render() = 0;
			/// ����
			virtual void Zoom(float scale) = 0;
			/// ��������Ӧ����
			virtual void ZoomToFitWindow() = 0;
			/// ƽ�����ƶ�
			virtual void Move(float dx, float dy) = 0;
			/// Move the camera according to the motion vector we spend
			/// @param motionVector[] motion Vector that determines where and how much camera moves
			virtual void Move(float motionVector[3]) = 0;
			/// ��ת
			virtual void Rotate(float angle) = 0;
			/// ����ָ����������תһ���Ƕ�
			virtual void Rotate3D(Vector3f axis, float angle) = 0;
			/// �����������ָ������תһ���Ƕ�
			virtual void Rotate3D(Vector3f &axis, Point3f &point, float angle) = 0;
			/// ����λ
			virtual void WindowWidthLevel(int width, int level) = 0;
			virtual void WindowWidthLevelOffset(int offset_width, int offset_level) = 0;
			/// ����renderer
			virtual void SetRenderer(IThreedRenderer* renderer) {renderer_=renderer;}
			/// ��ȡrenderer
			virtual IThreedRenderer* GetRenderer() {return renderer_;}
			/// ��ȡ��ǰ��Χ�ж���
			BoundingBox* GetBoundingBox() { return bounding_box_; }
			/// ͼ��ID
			string GetImagingID() {return image_uid_;}
			/// ����ImagePlane���洢��ͼ���򣬼�࣬���λ�õ���Ϣ
			ImagePlane *GetCurrentImagePlane() 
			{ 
				if (renderer_){
					return renderer_->GetCurrentImagePlane();
				}
				return NULL; 
			}

			/// return true to do zooming in this
			virtual bool CanZoom() { return false; }
			/// return true to do rotating in plane (roll) in this
			virtual bool CanRotate() { return false; }
			/// return true to do moving in plane (pan) in this
			virtual bool CanMove() { return false; }
			/// return true to do changing ww/wl in this
			virtual bool CanWWWL() { return false; }
			
		protected:
			/// ͼ��ID
			const string image_uid_;
			/// ��ǰ���ڵİ�Χ��
			BoundingBox *bounding_box_;
			/// ͼ������
			RenderMode render_mode_;
			/// ��ά����
			VolData* volume_data_;
			/// ִ����Ⱦ�Ķ���
			IThreedRenderer* renderer_;
			// Imaging����������BITMAP����ֻ�ṩShowBuffer���� [5/24/2019 Modified by zhangjian]
			//HBITMAP image_buffer_;
			/// �ۼƵķ�������
			float scale_factor_;
			/// �ۼƵ�ƫ����
			float offset_x_;
			float offset_y_;
			/// �ۼƵ�ƽ������ת�Ƕ�
			float roll_angle_;

		private:


		};
	}
}