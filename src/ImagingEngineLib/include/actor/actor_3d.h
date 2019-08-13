/*=========================================================================

  Program:   ImagingEngine
  Module:    actor_3d.h
  author: 	 zhangjian
  Brief:	 ��άͼ������������

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/vector3.h"
#include "render/imaging_3d.h"

using namespace DW::Render;
using namespace std;
//
namespace DW {
	namespace ACTOR{
		
		class ActorArgs;

		///
		// ��άͼ������������
		///
		class IThreeDActor
		{
		public:
			///
			// ִ������
			///
			virtual void Execute(ActorArgs* args) = 0;
			///
			// ����IThreedImaging����
			///
			void SetImaging(IThreedImaging* obj) { imaging_ = obj; }

		protected:
			IThreedImaging* imaging_;
			void* result_;
		};

		///
		// �����������
		///
		class ActorArgs
		{
		public:
			ActorArgs() {};
			virtual ~ActorArgs() {};

		protected:
			// �Ƿ�����ƶ��¼�
			bool mouse_moving_;
			// ��Ӧ�¼�����꣺���1/�м�2/�Ҽ�3
			BYTE mouse_;
		};
	}
}