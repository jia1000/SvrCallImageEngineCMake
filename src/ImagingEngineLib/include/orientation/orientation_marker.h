/*=========================================================================

  Program:   ImagingEngine
  Module:    orientation_marker.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once
#include "data/data_definition.h"
#include "vtk_include.h"
#include "filter/manual_orientation_marker_widget.h"

namespace DW {
	namespace Orientation {

		///
		// A Class that enraptures a vtkOrienatationMarker that will serve as an orientation element. 
		///
		class OrientationMarker
		{
		public:
			OrientationMarker(vtkRenderer *renderer);
			~OrientationMarker();

			/// Updates display on orientation marker widget
			void Update();
			/// We put the text labels that must go for each face of the cube
			void SetOrientationText(string right, string left, string posterior, string anterior, string superior, string inferior);
			/// Methods to control whether the widget is enabled or not
			void SetEnabled(bool enable);				

		private:
			/// vtk Widget for displaying orientation marker
			ManualOrientationMarkderWidget *marker_widget_;
			///
			vtkAnnotatedCubeActor *cube_actor_;
		};

	}
}
