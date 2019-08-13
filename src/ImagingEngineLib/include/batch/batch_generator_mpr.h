/*=========================================================================

  Program:   ImagingEngine
  Module:    batch_generator_mpr.h
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

		///
		// MPR批处理
		///
		class MPRSlabBatchGenerator : public IBatchGenerator
		{
		public:
			MPRSlabBatchGenerator();
			~MPRSlabBatchGenerator();
			void Execute(BatchArgs *) override;
			void Destroy() override;

		private:

		};

		///
		// MPR批处理参数
		///
		class MPRSlabBatchArgs : public BatchArgs
		{
		public:
			/// Sets/Gets image thickness
			void SetThickness(float thickness)
			{
				thickness_ = thickness;
			}
			float GetThickness()
			{
				return thickness_;
			}
			/// Sets/Gets image thickness
			void SetSpacingBetweenSlice(float spacing)
			{
				spacing_between_slice_ = spacing;
			}
			float GetSpacingBetweenSlice()
			{
				return spacing_between_slice_;
			}
			/// Sets/Gets image orientation
			void SetOrientation(OrientationType ori)
			{
				orientation_ = ori;
			}
			OrientationType GetOrientation()
			{
				return orientation_;
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

		private:
			float thickness_;
			float spacing_between_slice_;
			OrientationType orientation_;
			float clip_percent_;

		};

	}
}
