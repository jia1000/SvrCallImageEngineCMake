/*=========================================================================

  Program:   ImagingEngine
  Module:    IThreeDPick.h
  author: 	 zhangjian
  Brief:	 ������ά��Ͷ�ά���ת��

=========================================================================*/
#pragma once

#include "data/vector2.h"
#include "data/vector3.h"

namespace DW {
	namespace Render {

		class IThreedPickHandler
		{
		public:
			///
			// ͼ�������ת��λ��ά���ݳ��еĵ�
			///
			virtual bool PickByImagePos(const Vector2i&, Vector3d&) = 0;
			///
			// ͼ�������ת��λ��ά���ݳ��еĵ�
			///
			virtual bool PickByImagePos(const int&, const int&, Vector3d&) = 0;
			///
			// ��ά��ת�������ɺ��ͼ�������
			///
			virtual bool PickBy3DPoint(const Vector3d&, int&, int&) = 0;
		};

	}
}