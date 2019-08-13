/*=========================================================================

  Program:   ImagingEngine
  Module:    manual_orientation_marker_widget.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "vtk_include.h"

class ManualOrientationMarkderWidget : public vtkOrientationMarkerWidget
{
public:
	/// Interface to call update event
	void Update();
	/// Enable/disable the widget. Default is 0 (disabled).
	void SetEnabled(int) override;
};