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


using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class MPRRenderer : public IThreedRenderer
			, public IThreedPickHandler
			, public IThreedMeasureHandler
			, public IThreedOrientationHandler
		{
		public:
			MPRRenderer();
			virtual ~MPRRenderer();
			void Render() override;
			void SetData(VolData* data) override;
			ShowBuffer* GetShowBuffer() override;
			void ClearData() override {}
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
			void TrilinearInterpolation(float ix, float iy, float iz, short& ctv);
			void BilinearInterpolation(float ix, float iy, float iz, short& ctv, short max_ctv);
			void CalculateImageSize(BoundingBox *box, const Vector3f &normal_vector, const Point3f &origin, int &width, int &height);
			int ComputePlaneLineIntersection(const Vector3f &normal_vector, const Point3f &plane_point, const Vector3f &line_vector, const Point3f &line_point, Point3f &intersection);

		protected:
			bool is_off_screen_rendering_;
			//// �����ڴ�pixeldataӦ�ô���λ���������
			//vtkSmartPointer<vtkImageData> output_vtk_image_data_;
			//vtkSmartPointer<vtkRenderWindow> render_window_;
			//vtkSmartPointer<vtkRenderer> vtk_renderer_;
			// ��ת��
			Vector3f rotation_axis_;
			// ͼ�����ĵ�(Ҳ����ת���ĵ�)����ͼ�񼸺����ĵ�
			// ���㷽����������ͼ���Ͼ��η�Χ�����ĵ�
			Point3f rotation_center_;

		};
	}
}