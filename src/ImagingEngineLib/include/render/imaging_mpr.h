/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_mpr.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "../data/data_definition.h"
#include "imaging_3d.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class MPRImaging : public IThreedImaging
		{
		public:
			MPRImaging(string id);
			~MPRImaging();

			/// ��ȡԭʼ��άͼ�����ݳ�
			VolData* GetData() override;
			/// ����ԭʼ��άͼ�����ݳ� 
			void SetData(VolData* data) override;
			/// ��ȡ�����ʾͼ��
			ShowBuffer* GetShowBuffer() override;
			/// ��Ⱦͼ��
			void Render() override;
			/// ����renderer
			void SetRenderer(IThreedRenderer* renderer) override;
			/// ����
			void Zoom(float scale) override;
			/// ��������Ӧ����
			void ZoomToFitWindow() override;
			/// �ƶ�
			void Move(float dx, float dy) override;
			/// ƽ��
			void Move(float motionVector[3]) override;
			/// ƽ������ת
			void Rotate(float angle) override;
			/// ������������תһ���Ƕ�
			void Rotate3D(Vector3f axis, float angle) override;
			/// �����������ָ������תһ���Ƕ�
			void Rotate3D(Vector3f &axis, Point3f &point, float angle) override;
			/// ����λ
			void WindowWidthLevel(int width, int level) override;
			void WindowWidthLevelOffset(int offset_width, int offset_level) override;
			
		private:


		};
	}
}