/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_mpr.h
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
#include "data/orthogonal_plane.h"


using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class MPRRendererVtk : public IThreedRenderer
			, public IThreedPickHandler
			, public IThreedMeasureHandler
			, public IThreedOrientationHandler
		{
		public:
			MPRRendererVtk();
			virtual ~MPRRendererVtk();
			void Render() override;
			void SetData(VolData* data) override;
			ShowBuffer* GetShowBuffer() override;
			void ClearData() override;

			/// Convert world coordinate to display coordinate
			void ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos) override;
			void ComputeWorldToDisplay(double x, double y, double z, double display_point[3]) override;

			/// get handler
			IThreedPickHandler* GetThreedPickHandler() { return this; }
			IThreedMeasureHandler* GetThreedMeasureHandler() { return this; }
			IThreedOrientationHandler* GetThreedOrientationHandler() { return this; }

			/// IThreedPickHandler implementation
			bool PickByImagePos(const Vector2i& in_pnt, Vector3d& out_point) override;
			bool PickByImagePos(const int& x, const int& y, Vector3d& out_point) override;
			bool PickBy3DPoint(const Vector3d&, int&, int&) override;
			/// IThreedMeasureHandler implementation
			float CalculateLength(Vector3d&, Vector3d&) override;
			/// IThreedOrientationHandler implementation
			void GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&) override;
			/// Convert vtkImageData to showbuffer object
			void BufferTransform() override;

			/// methods for testing
			void SetOffScreenRendering(bool);
		protected:
			virtual void DoRender();
			void SetCameraViewPlane(const OrthogonalPlane &viewPlane);
			

		protected:
			bool is_off_screen_rendering_;
			//// �����ڴ�pixeldataӦ�ô���λ���������
			//vtkSmartPointer<vtkImageData> output_vtk_image_data_;
			//vtkSmartPointer<vtkRenderWindow> vtk_render_window_;
			//vtkSmartPointer<vtkRenderer> vtk_renderer_;
			//vtkSmartPointer<vtkImageActor> vtk_image_actor_;
			vtkSmartPointer<vtkImageSlabReslice> vtk_image_reslice_;
			// ��ת��
			Vector3f rotation_axis_;
			// ͼ�����ĵ�(Ҳ����ת���ĵ�)����ͼ�񼸺����ĵ�
			// ���㷽����������ͼ���Ͼ��η�Χ�����ĵ�
			Point3f rotation_center_;
			OrthogonalPlane current_view_plane_;

		};
	}
}