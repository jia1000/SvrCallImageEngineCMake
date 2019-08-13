/*=========================================================================

  Program:   ImagingEngine
  Module:    measure_3d.h
  author: 	 zhangjian
  Brief:	 ����������ܵĽӿ�

=========================================================================*/
#pragma once

#include "data/vector2.h"
#include "data/vector3.h"
#include "data/vol_data.h"

namespace DW {
	namespace Render {

		class IThreedMeasureHandler
		{
		public:
			///
			// ������ά���ݳ����
			///
			virtual float GetVoxelVolume() { return 0.0; };
			///
			// ����ָ����Χ�����ݳ����
			///
			virtual float GetVoxelVolume(BoundingBox*) { return 0.0; };
			///
			// �������ݳ�������֮�䳤��
			///
			virtual float CalculateLength(Vector3d&, Vector3d&) { return 0.0; };
		};
	}
}