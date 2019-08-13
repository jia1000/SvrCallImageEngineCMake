/*=========================================================================

  Program:   ImagingEngine
  Module:    camera.h
  author: 	 zhangjian
  Brief:	 ����࣬��װ��vtkCamera�Լ���������ӿ�
			 �ο���https://blog.csdn.net/wzheng92/article/details/79935059

=========================================================================*/
#pragma once

#include <vtkCamera.h>
#include <vtkPlane.h>
#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>
#include "data_definition.h"
#include "vector3.h"
#include "render_mode.h"

using namespace std;

namespace DW {
	namespace IMAGE{
		
		///
		// ����࣬��װ��vtkCamera�Լ���������ӿ�
		// �ο���https://blog.csdn.net/wzheng92/article/details/79935059
		///		
		class Camera
		{
		public:
			Camera();
			~Camera();
			///// ����ƽ�淨����(ƽ����)��ͼ�����ĵ���ת
			//void Rotate(float angle);
			///// ����ƽ�淨����(ƽ����)��ƽ���ڵ���ת
			//void Rotate(float angle, float x, float y);
			/// ������ͼX����ת
			void RotateX(float angle);
			/// ������ͼY����ת
			void RotateY(float angle);
			/// ������ͼZ����ת
			void RotateZ(float angle);
			/// ������������ת
			void RotateWXYZ(float angle, float x, float y, float z);
			/// Zooms in on the scene with the given factor
			/// @param zoom Factor factor that we want to apply to the camera
			void Zoom(float factor);
			/// Moves in direction
			void Move(float offset[3]);
			/// ��ȡvtkCamera
			vtkSmartPointer<vtkCamera> GetVtkCamera();
			/// ����vtkCamera
			void SetVtkCamera(vtkSmartPointer<vtkCamera> camera);
			/// �������
			void DeepCopy(Camera*);
			/// �������λ�úͷ���
			void SetCameraViewPlane(OrientationType ori);

		private:
			// vtk�����
			vtkCamera* vtk_camera_;
			// �������ƽ��
			vtkSmartPointer<vtkPlane> view_plane_;
			// �������λ�ã�������ƽ�����ĵ�
			Point3f focal_point_;
			// ���ĵ�λ��
			Point3f position_;
			// ����ƽ�����ĵ�: �����һ��offsetƫ����
			// ������������ͼ��������ģ�Focal Point�����ڴ��ڵ����ģ���ôWindowCenter����(0, 0)
			// WindowCenter�����������������ϵġ�������ƽ�������ϵ�������������µ�
			// ƽ�ƵĶ�����ͼ�񣬶���������ά�ռ��ƶ����
			Point3f window_center_;
			// ͼ���������
			Vector3f view_up_;
			// �ӽǣ�Ĭ����30��
			// ��������ͼ�������ݵı�����С��ͨ��������һ��������ʵ��ͼ��ķ���
			float view_angle_;
			// ���ΪTrue��ô����ƽ��ͶӰ������Ⱦ������Ĭ���ǰ���͸��ͶӰPerspectiveProjection������Ⱦ
			bool parallel_projection_;
			// �ӹ���λ��Positionָ�򽹵�λ��FocalPoint
			Vector3f direction_of_projection_;
			// ͶӰ�棨�����棩�ķ���������direction_of_projection_ʸ�������෴
			Vector3f view_plane_normal_;
			// ����
			float distance_;
			// �������������
			vtkSmartPointer<vtkMatrix4x4> view_transform_;
			// Ӧ�����������ı任����
			vtkSmartPointer<vtkMatrix4x4> camera_light_transform_;

		};
	}
}