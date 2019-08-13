/*=========================================================================

  Program:   ImagingEngine
  Module:    rotate_vr.h
  author: 	 zhangjian
  Brief:	 VRͼ����ת����

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/vector3.h"
#include "render/imaging_3d.h"
#include "actor/actor_3d.h"

using namespace DW::Render;
using namespace std;

namespace DW {
	namespace ACTOR{
		
		///
		// VR��ת����
		///
		class VRRotateActor : public IThreeDActor
		{
		public:
			VRRotateActor();
			~VRRotateActor();
			// ִ������
			void Execute(ActorArgs* args) override;
			// ����ָ������ת: not in use
			void Rotate(Vector3d axis, float angle);
			// ����ƽ������ת: not in use
			void Rotate(float x, float y, float angle);
		protected:

		};

		///
		// VR��ת�������
		///
		class VRRotateArgs : public ActorArgs
		{
		public:
			VRRotateArgs() {}
			~VRRotateArgs() {}
			void SetOffetX(float x) { offet_x = x; }
			float GetOffetX() { return offet_x; }
			void SetOffetY(float y) { offet_y = y; }
			float GetOffetY() { return offet_y; }
			void SetAngle(float angle) { angle_ = angle; }
			float GetAngle() { return angle_; }
			void SetRotationAxis(float* vec) 
			{ 
				axis_[0] = vec[0];
				axis_[1] = vec[1]; 
				axis_[2] = vec[2]; 
			}
			float* GetRotationAxis() { return axis_; }
		protected:
			// ���λ��x
			float offet_x;
			// ���λ��y
			float offet_y;
			// ��ת�Ƕ�
			float angle_;
			// �����������ת��
			float* axis_;
		};
	}
}