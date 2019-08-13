/*=========================================================================

  Program:   ImagingEngine
  Module:    batch_vr.h
  author: 	 zhangjian
  Brief:	 VR����������

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "vector3.h"
#include "imaging_3d.h"
#include "actor_3d.h"

using namespace DW::Render;
using namespace std;

namespace DW {
	namespace ACTOR{
		
		class ActorArgs;

		///
		// VR��ת����
		///
		class VRBatchActor : public IThreeDActor
		{
		public:
			VRBatchActor();
			~VRBatchActor();
			// ִ������
			void Execute(ActorArgs* args) override;
		protected:

		};

		///
		// VR�������������
		///
		class VRBatchArgs : public ActorArgs
		{
		public:
			VRBatchArgs() {}
			~VRBatchArgs() {}
			void SetImageNumber(uint16_t num) { image_number_ = num; }
			uint16_t GetImageNumber() { return image_number_; }
			void SetRotationDegree(float degree) { rotation_degree_ = degree; }
			float GetRotationDegree() { return rotation_degree_; }
			void SetRotationAxis(float* axis) { rotation_axis_ = axis; }
			float* GetRotationAxis() { return rotation_axis_; }
			void SetRotationCenter(Point3d pnt) { rotation_center_ = pnt; }
			Point3d GetRotationCenter() { return rotation_center_; }
			
		protected:
			// ����ͼ����Ŀ
			uint16_t image_number_;
			// ת���Ƕ�
			float rotation_degree_;
			// ��ת��
			float* rotation_axis_;
			// ��ת����
			Point3d rotation_center_;
		};
	}
}