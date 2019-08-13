/*=========================================================================

  Program:   ImagingEngine
  Module:    render_param_mpr.h
  author: 	 zhangjian
  Brief:	 MPRͼ��������

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
		// MPRͼ��������
		///
		class MPRRenderParam : public RenderParam
		{
		public:
			MPRRenderParam();
			~MPRRenderParam();
			/// ����ɨ��켣�ķ���
			void GetPlaneVector(Vector3f& row, Vector3f& col)
			{
				row = row_vector;
				col = column_vector;
			}
			/// ����ɨ��켣�ķ���
			void SetPlaneVector(Vector3f row, Vector3f col)
			{
				row_vector = row;
				column_vector = col;
			}
			/// ���ز��0��ʾĬ�ϲ��
			float GetThickness()
			{
				return thickness_;
			}
			/// ����ͼ�����ĵ�
			void SetImageCenter(float x, float y, float z)
			{
				center_.x = x;
				center_.y = y;
				center_.z = z;
			}
			/// ��ȡͼ�����ĵ�
			void GetImageCenter(Point3f& center)
			{
				center.x = center_.x;
				center.y = center_.y;
				center.z = center_.z;
			}
			/// ���ò��0��ʾĬ�ϲ��
			void SetThickness(float thickness)
			{
				thickness_ = thickness;
			}
			/// ������ת��
			void SetRotationAxis(float x, float y, float z)
			{
				rotation_axis_[0] = x;
				rotation_axis_[1] = y;
				rotation_axis_[2] = z;
			}
			/// ��ȡ��ת��
			void GetRotationAxis(Vector3f& axis)
			{
				axis = rotation_axis_;
			}
			/// ������ת����
			void SetRotationCenter(float x, float y, float z)
			{
				rotation_center_.x = x;
				rotation_center_.y = y;
				rotation_center_.z = z;
			}
			/// ��ȡ��ת����
			void GetRotationCenter(float& x, float& y, float& z)
			{
				x = rotation_center_.x;
				y = rotation_center_.y;
				z = rotation_center_.z;
			}
			/// ��ȡ/���ý�ȡ����
			void SetClip(float percent)
			{
				clip_percent_ = percent;
			}
			float GetClip()
			{
				return clip_percent_;
			}

		private:
			/// mprƽ��������
			Vector3f row_vector;
			/// mprƽ��������
			Vector3f column_vector;
			/// ͼ�����ĵ㣬���ϲ�ָ����Ĭ�ϼ���ƽ���е�
			Point3f center_;
			/// ���
			float thickness_;
			/// ��ת��
			Vector3f rotation_axis_;
			/// ��ת����
			Point3f rotation_center_;
			/// ��ͷ���ŷ����ȡ����
			float clip_percent_;
		};

	}
}