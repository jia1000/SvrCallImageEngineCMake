/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_stretched_cpr.h
  author: 	 zhangjian
  Brief:	 CPRͼ��Stretched������Ⱦ��

=========================================================================*/
#pragma once
#include "vtk_include.h"
#include "data/data_definition.h"
#include "render/renderer_cpr.h"
#include "render/render_param_cpr.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render {

		struct ThreadParam;
		///
		// ƽ������CPR
		///
		class StretchedCPRRenderer : public CPRRenderer
		{
		public:
			StretchedCPRRenderer();
			~StretchedCPRRenderer();
		protected:
			void DoRender() override;

			static void DoThreadWork(ThreadParam *param);

		private:

		};

		struct ThreadParam
		{
			int flag;
			int start_row;
			int end_row;
			int cols;
			int image_width;
			int image_height;
			int image_count;
			short *ptr;
			CPRRenderParam *param_imp;
			float origin_padding_left;
			float voxel_spacing[3];
			float direction[3];
			float origin[3];
			vector<int> sample_point_indexs;
			StretchedCPRRenderer *owner;
			VolData* volume_data;
		};

	}
}