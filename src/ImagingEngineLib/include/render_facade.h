/*=========================================================================

  Program:   ImagingEngine
  Module:    render_manager.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/render_mode.h"
#include "data/vol_data.h"
#include "data/show_buffer.h"
#include "render/imaging_3d.h"
#include "builder/imaging_builder_3d.h"
#include "data_source.h"
#include "render_source.h"
#include "presentation/parameter_base.h"
#include "control/image_control.h"

using namespace DW::IMAGE;
using namespace DW::Builder;

namespace DW {
	
	class RenderFacade
	{
	public:
		~RenderFacade() { instance_ = nullptr; }
	
		static RenderFacade *Get();

		/// Zoom
		void Zoom(string control_id, float scale);
		/// Zoom to specified width and height
		void ZoomToSize(string control_id, int width, int height);
		/// Move in current plane
		void Pan(string control_id, float dx, float dy);
		/// Move to a new 3d position
		void MoveTo(string control_id, float move_vector[3]);
		/// Rotate image by a certain angle in the plane
		void Roll(string control_id, float angle);
		/// Rotate image by a certain angle around a specified axis
		void Rotate(string control_id, float angle, float axis[3]);

		/// Change image to a predefined orientation
		int SetOrientation(string control_id, OrientationType ori);
		/// Change image to a arbitrary orientation which is defined by angle rotated around specified axis
		int SetOrientation(string control_id, OrientationType ori, float vec[3]);

		/// CPR related interface
		/// Set curve id to CPR control
		void SetCPRCurveID(string control_id, string curve_id);
		/// END

		/// Cross-sectional related interface
		/// Set position on curve line by a certain point index
		void SetPositionAlongCurve(string control_id, string curve_id, int point_index);
		/// END

		/// VR related interface
		void SetDesiredUpdateRate(string control_id, float rate);
		/// END

		/// Render image
		void RenderControl(string control_id);
		/// Reset to default
		void ResetControl(string control_id);

		/// Change series data for all IThreedImaging objects
		int ChangeSeries(string series_uid);
		/// Unloads series data for specified series
		int UnloadSeries(string series_uid);

		/// Set image width and height for display
		void SetImageSize(string control_id, int width, int height);
		/// Set window width and level
		void SetWindowWidthLevel(string control_id, int width, int level);
		/// Set blend mode: MIP = 1/MinIP = 2/AIP = 3
		void SetBlendMode(string control_id, int mode);
		/// Set thickness in slab mode (mip/minip/aip)
		void SetSlabThickness(string control_id, float thickness);

		/// Create vr batch
		int CreateVRRotationBatch(string control_id, 
			string output, 
			BlendMode mode, 
			OrientationType init_ori, 
			RotationDirection direction, 
			float step_angle, 
			float clip_percent,
			int image_number,
			int ww,
			int wl);

		/// Create cpr batch
		int CreateCPRRotationBatch(string control_id,
			string output,
			string curve_id,
			OrientationType init_ori,
			RotationDirection direction, 
			float step_angle, 
			int image_number,
			int ww,
			int wl);

		/// Create mpr batch
		int CreateMPRSlabBatch(string control_id,
			string output,
			BlendMode mode,
			OrientationType ori,
			float clip_percent, 
			float thickness,
			float spacing,
			int ww,
			int wl);

		/// Get updated image buffer
		IBitmap *GetImageBuffer(string control_id);
		/// Clear data in control
		int ClearContorl(string control_id);

	private:
		RenderFacade() {}
		IImageControl *GetControl(string control_id);

	private:
		static RenderFacade *instance_;
		map<string, IThreedImagingBuilder *> builder_set_;
		string current_series_uid_;
		
	};

}