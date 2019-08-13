/*=========================================================================

  Program:   ImagingEngine
  Module:    image_control_2d.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once


#include "data/data_definition.h"
#include "render/renderer_3d.h"
#include "render/imaging_3d.h"
#include "processing/image_transform.h"
#include "data/show_buffer.h"
#include "control/image_control.h"

using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::CV;

namespace DW {
	namespace Control {

		///
		// ��άͼ��ؼ����洢��ǰͼ��/����Batchͼ��
		///
		class TwodImageControl : public IImageControl, public IImageSouceCallback
		{
		public:
			TwodImageControl(string control_id);
			~TwodImageControl();

			/// ��ȡ�ؼ�����
			ImageControlType GetType() override { return ImageControlType::CONTROL_TWOD; }
			/// ��ȡ�ؼ�Ψһ���
			string GetControlID() override;
			/// ����ͼ������Դ
			void SetInput(int, void *) override;
			/// ��ȡͼ������Դ
			void *GetInput(int) override;
			/// ����ͼ��任ִ����
			void SetTransform(IImageTransform *) override;
			/// Ӧ��ͼ��任
			void Update() override;
			/// �ɻ�ȡ����ͼ�������
			int GetOutputNumber() override;
			/// ��ȡͼ����
			IBitmap *GetOutput() override;
			IBitmap *GetOutput(int) override;
			/// ��ȡͼ����Ϣ
			IBitmapDcmInfo *GetOutputInfo() override;
			IBitmapDcmInfo *GetOutputInfo(int) override;
			/// ������ʾͼ��ĳߴ�
			void SetDisplaySize(int width, int height) override;
			/// ��ҳ
			void GoTo(int pos) override;
			/// ����
			void Zoom(float scale) override;
			/// ��ת
			void Roll(float angle) override;
			/// ƽ��
			void Pan(float dx, float dy) override;
			/// ����λ
			void WWWL(int ww, int wl) override;
			/// ������ָ����С
			void ZoomToSize(int width, int height) override;
			
			/// NA for twod image control
			void Move(float motion_vector[3]) override {}
			void Rotate3D(Vector3f axis, float angle) override {}

			// IImageSourceCallback implementation
			void OnImageAdded() override;

			/// ����ǰ����Դͼ��洢Ϊbitmap
			void SaveAsBitmap(string output_dir) override;
			/// ����ǰ����Դͼ��洢Ϊdicomͼ��
			void SaveAsDicom(string output_dir) override;
			/// �������
			void Clear() override;
			
		private:
			int port_;
			/// Control ID
			string control_id_;
			/// Object that is hold images
			IImageSource *image_source_;
			/// Object that is rendering images
			IThreedImaging *imaging_;
			///// Calculate data
			//ICalculator *calculator_;
			/// Handle image transforming
			IImageTransform *transform_;
			/// �ۼƵķ�������
			float scale_factor_;
			/// �ۼƵ���ת�Ƕ�
			float roll_angle_;
			/// �ۼƵ�ˮƽƫ����
			float offset_x_;
			/// �ۼƵĴ�ֱƫ����
			float offset_y_;
			/// ����λ
			int ww_, wl_;
			/// ��ǰ��ʾͼ������
			int display_index_;
			/// ��ʾͼ��Ŀ��
			int display_width_;
			int display_height_;

		};

	}
}
