/*=========================================================================

  Program:   ImagingEngine
  Module:    pick_point.h
  author: 	 zhangjian
  Brief:	 ѡ�������

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "vector3.h"
#include "imaging_3d.h"
#include "actor_3d.h"
#include "render/pick_3d.h"

using namespace DW::Render;
using namespace std;

namespace DW {
	namespace ACTOR{
		
		class ActorArgs;

		///
		// ѡ�������
		///
		class PickPointActor : public IThreeDActor
		{
		public:
			PickPointActor();
			~PickPointActor();
			// ִ������
			void Execute(ActorArgs* args) override;
			
		protected:

		};

		///
		// ʰȡ������
		///
		class PickPointArgs : public ActorArgs
		{
		public:
			PickPointArgs() {}
			~PickPointArgs() {}
			float GetImageX() { return image_x; }
			void SetImageX(float x) { image_x = x; }
			float GetImageY() { return image_y; }
			void SetImageY(float y) { image_y = y; }
			Vector3d GetPicked3DPoint() { return volume_coordinate_; }
			void SetGetPicked3DPoint(float x, float y, float z){
				volume_coordinate_[0] = x;
				volume_coordinate_[1] = y;
				volume_coordinate_[2] = z;
			}
		protected:
			// ��Ļ����x
			float image_x;
			// ��Ļ����y
			float image_y;
			// ���ص���ά���ݳ�����
			Vector3d volume_coordinate_;
		};
	}
}