/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_straightened_cpr.h
  author: 	 zhangjian
  Brief:	 CPRͼ��Straightened������Ⱦ��

=========================================================================*/
#pragma once
#include "vtk_include.h"
#include "data/data_definition.h"
#include "render/renderer_cpr.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render {
		///
		// ƽ����ֱCPR
		///
		class StraightededCPRRenderer : public CPRRenderer
		{
		public:
			StraightededCPRRenderer();
			~StraightededCPRRenderer();
		protected:
			void DoRender() override;
		private:

		};

	}
}