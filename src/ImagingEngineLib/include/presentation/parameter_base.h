/*=========================================================================

  Program:   ImagingEngine
  Module:    parameter_base.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

namespace DW {

	class RenderParameter
	{
	public:

	protected:
		/// ͼ����ʾ���
		int display_width_;
		int display_height_;
		/// ͼ�񴰿�λ
		int window_width_;
		int window_level_;
		/// ͼ���������
		float scale_factor_;
		/// (ƽ�ƺ�)ͼ����ʾ�����ĵ�
		float display_center_;
		/// ͼ�����ĵ�
		float image_center_;
		/// ����/������ID
		int curve_id_;
		/// MIP = 1/MinIP = 2/AIP = 3
		int blend_mode_;
		/// ��ֵ����: Cubic = 1/Linear = 2/NearestNeighbor = 3
		int interpolation_mode_;
		/// ���
		float thickness_;
		/// VR�Ƿ������������
		bool fit_window_;

	private:
	};

}
