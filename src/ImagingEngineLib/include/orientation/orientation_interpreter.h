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
			/// 将方向向量转换为方向标识字符串
			/// direction_unit为单位方向向量
			static void Interpret(const Vector3d &direction_unit, char orientation[4]);
			/// 将图像方向转换为四个方向标识字符串
			static void Interpret(const ImageOrientation &ori, char left[4], char right[4], char up[4], char down[4]);
			
		private:
			const static float kFixedRadianLimit;
		};

	}
}