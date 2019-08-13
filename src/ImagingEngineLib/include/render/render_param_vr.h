/*=========================================================================

  Program:   ImagingEngine
  Module:    render_param_vr.h
  author: 	 zhangjian
  Brief:	 VR图像成像参数，包含了VR协议的参数

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "render/render_param.h"
#include "vtk_include.h"
#include "data/render_mode.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class VRRenderParam : public RenderParam
		{
		public:
			VRRenderParam();
			~VRRenderParam();

			/// Set the desired update rate
			void SetDesiredUpdateRate(float rate);
			/// Get the desired update rate
			float GetDesiredUpdateRate();

			/// Set volume reconstruction method
			void SetRenderingMode(RenderMode mode);
			/// Get volume reconstruction method
			RenderMode GetRenderMode();

			/// Sets the blend mode to use.
			void SetBlendMode(BlendMode mode);
			/// Gets the blend mode to use.
			BlendMode GetBlendMode();

			/// Sets flag to update center point and zoom factor
			void SetUpdateViewportToFitTissue(bool update);
			/// Sets flag to update center point and zoom factor
			bool GetUpdateViewportToFitTissue();
			/// Sets clipping percent
			void SetClipping(float clip);
			/// Gets clipping percent
			float GetClipping();

		private:
			/// The desired update rate which tells how quickly they need to render
			/// When camera is still, the desired update rate should also be updated to a small value
			float desired_update_rate_;
			/// Render mode: what kind of mapper is used.
			RenderMode rendering_mode_;
			/// Blend mode: how the samples are combined to create a pixel.
			BlendMode blend_mode_;
			/// Indicates whether to update center point and zoom factor to fit rendering tissue
			bool update_viewport_to_fit_tissue_;
			/// Clipping percent
			float clipping_percent_;

		};
	}
}