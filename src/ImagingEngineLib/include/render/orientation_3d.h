/*=========================================================================

  Program:   ImagingEngine
  Module:    orientation_3d.h
  author: 	 zhangjian
  Brief:	 ����ͼ�����ܵĽӿ�

=========================================================================*/
#pragma once

#include "data/vector2.h"
#include "data/vector3.h"
#include "data/vol_data.h"

namespace DW {
	namespace Render {

		class IThreedOrientationHandler
		{
		public:
			///
			// ���㵱ǰ����ͼ����ĸ�����ֵ
			// params: left, right, top, bottom
			///
			virtual void GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&) = 0;
		};
	}
}