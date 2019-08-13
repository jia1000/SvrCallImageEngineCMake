/*=========================================================================

  Program:   ImagingEngine
  Module:    render_mode.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "data_convert.h"

using namespace std;

namespace DW {
	namespace IMAGE{

		enum class RenderMode
		{
			UNDEFINED,
			SMART_RAYCASTING,
			RAYCASTING_GPU,
			RAYCASTING,
			TEXTURE_3D_GPU,
			ISO_SURFACE,
			SSD,
			MAXIP,
			MINIP,
			AVRIP,
			MPR,
			STRAIGHTENED_CPR,
			STRETECHED_CPR,
			CROSS_SECTIONAL_MPR,
		};

		enum class RenderControlType
		{
			NONE,
			VR, 
			MPR, 
			STRETECHED_CPR, 
			STRAIGHTENED_CPR,
			CROSS_SECTIONAL_MPR,
		};

		enum OrientationType
		{
			AXIAL, 
			SAGITTAL,
			CORONAL,
			LEFT,
			RIGHT,
			HEAD,
			FEET,
			POSTERIOR,
			ANTERIOR,
			OBLIQUE,
			NA,
		};

		enum RotationAxis
		{
			AXIS_X,
			AXIS_Y,
			AXIS_Z
		};

		enum RotationDirection
		{
			LEFT_TO_RIGHT,
			RIGHT_TO_LEFT,
			HEAD_TO_FEET,
			FEET_TO_HEAD,
		};

		enum BlendMode 
		{ 
			Composite, 
			MaximumIntensity, 
			MinimumIntensity, 
			AverageIntensity, 
			Additive, 
			Isosurface 
		};

		enum InterpolationMode 
		{ 
			NearestNeighbor, 
			Linear, 
			Cubic 
		};

	}
}