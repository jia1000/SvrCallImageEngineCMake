/*=========================================================================

  Program:   ImagingEngine
  Module:    orientation_interpreter.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/vector3.h"
#include "data/image_orientation.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Orientation {

		class OrientationInterpreter
		{
		public:
			/// ����������ת��Ϊ�����ʶ�ַ���
			/// direction_unitΪ��λ��������
			static void Interpret(const Vector3d &direction_unit, char orientation[4]);
			/// ��ͼ����ת��Ϊ�ĸ������ʶ�ַ���
			static void Interpret(const ImageOrientation &ori, char left[4], char right[4], char up[4], char down[4]);
			
		private:
			const static float kFixedRadianLimit;
		};

	}
}