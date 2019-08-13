/*=========================================================================

  Program:   ImagingEngine
  Module:    rotate_cpr.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/vector3.h"
#include "render/imaging_3d.h"
#include "actor/actor_3d.h"

using namespace DW;
using namespace DW::ACTOR;

namespace DW {
	namespace ACTOR {
		
		///
		// VR��ת����
		///
		class CPRRotateActor : public IThreeDActor
		{
		public:
			CPRRotateActor();
			~CPRRotateActor();
			// ִ������
			void Execute(ActorArgs* args) override;
		protected:

		};

		///
		// CPR��ת�������
		///
		class CPRRotateArgs : public ActorArgs
		{
		public:
			CPRRotateArgs() {}
			~CPRRotateArgs() {}
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