/*=========================================================================

  Program:   ImagingEngine
  Module:    render_param_cpr.h
  author: 	 zhangjian
  Brief:	 CPRͼ��������

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "render/render_param.h"
#include "vtk_include.h"
#include "data/curve_line.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		///
		// CPUͼ����ʾ��ʽ������������
		// Kanitsar et al. "CPR - Curved Planar Reformation", Proc. IEEE  Visualization, 2002, 37-44
		///
		enum CPRRenderMode
		{
			STRETCHED_CPR_AUTO,
			STRETCHED_CPR_HOR,
			STRETCHED_CPR_VER,
			STRAIGHTENED_CPR_AUTO,
			STRAIGHTENED_CPR_HOR,
			STRAIGHTENED_CPR_VER,
		};

		///
		// CPRͼ��������
		// ���ֲ��պ��˶ʿ���ġ������״���������ؽ������о���
		// Kanitsar et al. "CPR - Curved Planar Reformation", Proc. IEEE  Visualization, 2002, 37-44
		///
		class CPRRenderParam : public RenderParam
		{
		public:
			CPRRenderParam();
			~CPRRenderParam();
			// ����curve
			VolCurve* GetCurve()
			{
				return curve_;
			}
			// ����curve
			void SetCurve(VolCurve* curve)
			{
				curve_ = curve;
			}
			// ����rendre mode
			CPRRenderMode GetRenderMode()
			{
				return render_mode_;
			}
			// ����rendre mode
			void SetRenderMode(CPRRenderMode mode)
			{
				render_mode_ = mode;
			}
			// ����ɨ��켣�ķ���
			void GetDirection(float& x, float& y, float& z)
			{
				x = vector_of_interest_[0];
				y = vector_of_interest_[1];
				z = vector_of_interest_[2];
			}
			// ����ɨ��켣�ķ���
			void SetDirection(const float& x, const float& y, const float& z)
			{
				vector_of_interest_[0] = x;
				vector_of_interest_[1] = y;
				vector_of_interest_[2] = z;
			}
			// ���ز��0��ʾĬ�ϲ��
			float GetThickness()
			{
				return thickness_;
			}
			// ���ò��0��ʾĬ�ϲ��
			void SetThickness(float thickness)
			{
				thickness_ = thickness;
			}
			// ������ת��
			void SetAxis(float x, float y, float z)
			{
				rotation_axis_[0] = x;
				rotation_axis_[1] = y;
				rotation_axis_[2] = z;
			}
			// ��ȡ��ת��
			void GetAxis(float& x, float& y, float& z)
			{
				x = rotation_axis_[0];
				y = rotation_axis_[1];
				z = rotation_axis_[2];
			}
			// ������ת����
			void SetCenter(float x, float y, float z)
			{
				rotation_center_.x = x;
				rotation_center_.y = y;
				rotation_center_.z = z;
			}
			// ��ȡ��ת����
			void GetCenter(float& x, float& y, float& z)
			{
				x = rotation_center_.x;
				y = rotation_center_.y;
				z = rotation_center_.z;
			}
			// ������ת�Ƕ�
			void SetAngle(float angle)
			{
				rotation_angle_ = angle;
			}
			// ������ת�Ƕ�
			float GetAngle()
			{
				return rotation_angle_;
			}
		private:
			// ɨ��켣(�и���)�ķ������ڿ���stretched cpr��ת
			Vector3f vector_of_interest_;
			// ��ת�ĽǶȣ����ڿ���straightened cpr��ת
			float rotation_angle_;
			// ����ʽ��ȫ��/��ֱ/�����/�����
			CPRRenderMode render_mode_;
			// ��ά����
			VolCurve* curve_;
			// ���
			float thickness_;
			// ��ת��
			Vector3f rotation_axis_;
			// ��ת����
			Point3f rotation_center_;
		};

	}
}