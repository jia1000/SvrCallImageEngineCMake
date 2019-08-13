/*=========================================================================

  Program:   ImagingEngine
  Module:    batch_generator_cpr.h
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
		// CPR批处理
		///
		class CPRRotationBatchGenerator : public IBatchGenerator
		{
		public:
			CPRRotationBatchGenerator();
			~CPRRotationBatchGenerator();
			void Execute(BatchArgs *) override;
			void Destroy() override;

		private:

		};

		///
		// CPR批处理参数
		///
		class CPRRotationBatchArgs : public BatchArgs
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
			/// Sets/Gets curve id
			void SetCurveId(string id)
			{
				curve_id_ = id;
			}
			string GetCurveId()
			{
				return curve_id_;
			}

		private:
			string curve_id_;
			RotationDirection direction_;

		};

	}
}