/*=========================================================================

  Program:   ImagingEngine
  Module:    window_width_level.h
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

		class WindowWidthLevelActor : IThreeDActor
		{
		public:
			WindowWidthLevelActor();
			~WindowWidthLevelActor();
			// ִ������
			void Execute(ActorArgs* args) override;

		protected:

		};


		///
		// CPR��ת�������
		///
		class WindowWidthLevelArgs : public ActorArgs
		{
		public:
			WindowWidthLevelArgs() {}
			~WindowWidthLevelArgs() {}
			void SetWindowWidth(int ww) { window_width_ = ww; }
			int GetWindowWidth() { return window_width_; }
			void SetWindowLevel(int wl) { window_level_ = wl; }
			int GetWindowLevel() { return window_level_; }

		protected:
			// ����
			int window_width_;
			// ��λ
			int window_level_;
		};

	}
}