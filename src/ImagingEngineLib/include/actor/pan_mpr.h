/*=========================================================================

  Program:   ImagingEngine
  Module:    pan_mpr.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "vector3.h"
#include "imaging_3d.h"
#include "actor_3d.h"

using namespace DW;
using namespace DW::ACTOR;

namespace DW {
	namespace ACTOR {

		///
		// ͼ��ƽ������
		///
		class PanActor : public IThreeDActor
		{
		public:
			PanActor();
			~PanActor();
			// ִ������
			void Execute(ActorArgs* args) override;
		protected:

		};

		///
		// ƽ���������
		///
		class PanArgs : public ActorArgs
		{
		public:
			PanArgs() {}
			~PanArgs() {}
			void SetOffetX(float x) { offet_x = x; }
			float GetOffetX() { return offet_x; }
			void SetOffetY(float y) { offet_y = y; }
			float GetOffetY() { return offet_y; }
			
		protected:
			// ���λ��x
			float offet_x;
			// ���λ��y
			float offet_y;
		};

	}
}