/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_cpr.h
  author: 	 zhangjian
  Brief:	 ��ʾһ�������CPRͼ��
			 CPRͼ���ΪStretched(����CPR)��Straightened(��ֱCPR)����	������CPRͼ��ͨ����ͬ����Ⱦ����renderer��ʵ��
			 �ο���������
			 Kanitsar et al. "CPR - Curved Planar Reformation", Proc. IEEE  Visualization, 2002, 37-44

=========================================================================*/
#pragma once

#include "../data/data_definition.h"
#include "imaging_3d.h"
#include "data/render_mode.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class CPRImaging : public IThreedImaging
		{
		public:
			CPRImaging(string id);
			~CPRImaging();
			
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
			/// ��¼�ۼӵ���ת�Ƕ�
			float rotation_angle_;

		};
	}
}