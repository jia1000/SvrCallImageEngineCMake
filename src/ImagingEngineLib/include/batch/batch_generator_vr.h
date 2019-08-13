/*=========================================================================

  Program:   ImagingEngine
  Module:    batch_vr.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "batch/batch_generator.h"
#include "data/data_definition.h"
#include "data/camera.h"
#include "data/render_mode.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Batch{

		typedef struct  {
			string control_id;
			float start_pos;
			float end_pos;
			float step_angle;
			RotationDirection direction;
			int image_number;	
			bool clockwise;
			IThreedImaging *imaging;
			IBatchGenerator *owner;
		} BATCHTHREADPARAM;

		///
		// VR批处理
		///
		class VRRotationBatchGenerator : public IBatchGenerator
		{
		public:
			VRRotationBatchGenerator();
			~VRRotationBatchGenerator();
			void Execute(BatchArgs *) override;
			void Destroy() override;

		private:
			
		};

		///
		// VR批处理参数
		///
		class VRRotationBatchArgs : public BatchArgs
		{
		public:
			/// Sets/Gets rotation direction
			void SetDirection(RotationDirection direction)
			{
				direction_ = direction;
			}
			RotationDirection GetDirection()
			{
				return direction_;
			}
			/// Sets/Gets clip percent
			void SetClipPercent(float percent)
			{
				clip_percent_ = percent;
			}
			float GetClipPercent()
			{
				return clip_percent_;
			}

			/// Not used for now
			void SetAxis(float x, float y, float z) 
			{
				rotate_axis_[0] = x;
				rotate_axis_[1] = y;
				rotate_axis_[2] = z;
			}
			void GetAxis(float& x, float& y, float& z) 
			{
				x = rotate_axis_[0];
				y = rotate_axis_[1];
				z = rotate_axis_[2];
			}

		private:
			Vector3f rotate_axis_;
			float clip_percent_;
			RotationDirection direction_;

		};

	}
}