/*=========================================================================

  Program:   ImagingEngine
  Module:    image_control.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "render/renderer_3d.h"
#include "render/imaging_3d.h"
#include "processing/image_transform.h"
#include "data/show_buffer.h"
#include "data/image_source.h"
#include "bitmap/ibitmap.h"
#include "bitmap/ibitmap_info.h"

using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::CV;
using namespace DW::Bitmap;

namespace DW {
	namespace Control {

		enum ImageControlType
		{
			CONTROL_TWOD,
			CONTROL_THREED,
		};

		///
		// ͼ��ؼ����࣬�洢��ǰͼ��/����Batchͼ��
		///
		class IImageControl
		{
		public:
			/// ��ȡ�ؼ�����
			virtual ImageControlType GetType() = 0;
			/// ��ȡ�ؼ�Ψһ���
			virtual string GetControlID() = 0;
			/// ����ͼ������Դ
			virtual void SetInput(int, void *) = 0;
			/// ��ȡͼ������Դ
			virtual void *GetInput(int) = 0;
			/// ����ͼ��任ִ����
			virtual void SetTransform(IImageTransform *) = 0;
			/// Ӧ��ͼ��任
			virtual void Update() = 0;
			/// �ɻ�ȡ����ͼ�������
			virtual int GetOutputNumber() = 0;
			/// ��ȡͼ����
			virtual IBitmap *GetOutput() = 0;
			virtual IBitmap *GetOutput(int) = 0;
			/// ��ȡͼ����Ϣ
			virtual IBitmapDcmInfo *GetOutputInfo() = 0;
			virtual IBitmapDcmInfo *GetOutputInfo(int) = 0;
			/// ������ʾͼ��ĳߴ�
			virtual void SetDisplaySize(int width, int height) = 0;
			/// ��ҳ
			virtual void GoTo(int pos) = 0;
			/// ����
			virtual void Zoom(float scale) = 0;
			/// ��ת
			virtual void Roll(float angle) = 0;
			/// ƽ��
			virtual void Pan(float dx, float dy) = 0;
			/// ����λ
			virtual void WWWL(int ww, int wl) = 0;
			/// ������ָ����С
			virtual void ZoomToSize(int width, int height) = 0;
			/// Move the camera according to the motion vector we spend
			/// @param motionVector[] motion Vector that determines where and how much camera moves
			virtual void Move(float motion_vector[3]) = 0;
			/// ����ָ����������תһ���Ƕ�
			virtual void Rotate3D(Vector3f axis, float angle) = 0;
			/// ����ǰ����Դͼ��洢Ϊbitmap
			virtual void SaveAsBitmap(string output_dir) = 0;
			/// ����ǰ����Դͼ��洢Ϊdicomͼ��
			virtual void SaveAsDicom(string output_dir) = 0;
			/// �������
			virtual void Clear() = 0;
			
		};

	}
}