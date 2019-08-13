/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_vr.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data/data_definition.h"
#include "render/renderer_3d.h"
#include "render/pick_3d.h"
#include "render/measure_3d.h"
#include "render/orientation_3d.h"
#include "data/pixel_data.h"
#include "render/transfer_function.h"
#include "data/window_level.h"
#include "orientation/orientation_marker.h"

using namespace DW::IMAGE;
using namespace DW::Orientation;

namespace DW {
	namespace Render{

		class VolumeRenderer : public IThreedRenderer
			, public IThreedPickHandler
			, public IThreedMeasureHandler
			, public IThreedOrientationHandler
		{
		public:
			VolumeRenderer();
			virtual ~VolumeRenderer();

			void SetRenderParam(RenderParam* param) override;
			void Render() override;
			ShowBuffer* GetShowBuffer() override;
			void SetData(VolData* data) override;
			void ClearData() override;
			/// Convert world coordinate to display coordinate
			void ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos) override;
			void ComputeWorldToDisplay(double x, double y, double z, double display_point[3]) override;

			/// Shading On/Off
			void SetShading(bool);
			/// Lighting Parameters.
			void SetAmbient(double);
			void SetDiffuse(double);
			void SetSpecular(double);
			void SetSpecularPower(double);

			/// Determines The bounding box that defines the volume cutting plans
			void SetClippingPlanes(vtkPlaneCollection *clipping);
			/// Get cutting plans that have been defined on the volume
			vtkPlaneCollection *GetClippingPlanes() const;
			/// Get/Set the current transfer function
			void SetTransferFunction(const TransferFunction &transferFunction);
			/// Set render mode to use
			void SetRenderingMode(RenderMode mode);
			/// Set blend mode to use
			void SetBlendMode(BlendMode mode);
			/// Computes and sets the transformation matrix for the volume to convert from the DICOM space to the world space.
			void SetVolumeTransformation();
			/// Sets the "independent components" property of the volume. The default is true and this should be set to false for a color image.
			/// In a 4-component image with non-independent components, the first 3 represent RGB and the fourth will be mapped through the opacity transfer function.
			void SetIndependentComponents(bool independent);
			/// Change color/opacity
			void SetWindowLevel(int ww, int wl);

			/// get handler
			IThreedPickHandler* GetThreedPickHandler() { return this; }
			IThreedMeasureHandler* GetThreedMeasureHandler() { return this; }
			IThreedOrientationHandler* GetThreedOrientationHandler() { return this; }

			/// IThreedPickHandler implementation
			bool PickByImagePos(const Vector2i& in_pnt, Vector3d& out_point) override;
			bool PickByImagePos(const int& x, const int& y, Vector3d& out_point) override;
			bool PickBy3DPoint(const Vector3d&, int&, int&) override;
			/// IThreedMeasureHandler implementation
			float GetVoxelVolume() override;
			float GetVoxelVolume(BoundingBox*) override;
			float CalculateLength(Vector3d&, Vector3d&) override;
			/// IThreedOrientationHandler implementation
			void GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&) override;
			
			/// methods for testing
			void SetOffScreenRendering(bool);
		protected:
			virtual void DoRender(vtkSmartPointer<vtkImageData> imagedata);
			virtual void FitRenderingIntoViewport(float clip, bool zoom_after_clip);

		protected:
			bool is_off_screen_rendering_;
			/// 输出图像的vtkImageData格式
			vtkSmartPointer<vtkImageData> vtk_image_data_;
			/// 输入MASK的vtkImageData格式
			vtkSmartPointer<vtkImageData> input_mark_data_;
			/// vtkImageMask
			vtkSmartPointer<vtkImageMask> vtk_mask_filter_;
			vtkSmartPointer<vtkRenderWindow> vtk_render_window_;
			vtkSmartPointer<vtkRenderer> vtk_renderer_;
			/// The main mapper for volume rendering.
			vtkSmartPointer<vtkSmartVolumeMapper> vtk_volume_mapper_;
			/// Properties of volume rendering.
			vtkSmartPointer<vtkVolumeProperty> vtk_volume_property_;
			/// The volume actor.
			vtkSmartPointer<vtkVolume> vtk_volume_;
			/// The clipping vtkPlanes
			vtkSmartPointer<vtkPlaneCollection> vtk_clipping_planes_;
			/// Current transfer function.
			TransferFunction transfer_function_;
			/// MIP transfer function.
			TransferFunction transfer_function_mip_;
			/// Window width and level that are used when rendering in MIP mode
			WindowLevel window_level_;
			/// Orientation marker widget
			OrientationMarker *orientation_marker_;
			/// Output spacing values
			double output_spacings[2];

		};
	}
}