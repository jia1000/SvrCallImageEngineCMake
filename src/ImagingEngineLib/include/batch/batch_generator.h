/*=========================================================================

  Program:   ImagingEngine
  Module:    batch.h
  author: 	 zhangjian
  Brief:	 生成批处理

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/show_buffer.h"
#include "render/imaging_3d.h"

using namespace DW::IMAGE;
using namespace DW::Render;

namespace DW {
	namespace Batch{
		
		class BatchArgs;
		class IBatchCallback;

		///
		// 生成批处理基类
		///
		class IBatchGenerator
		{
		public:
			virtual void Execute(BatchArgs *) = 0;
			virtual void SetCallback(IBatchCallback *callback)
			{
				callback_ = callback;
			}
			virtual IBatchCallback *GetCallback()
			{
				return callback_;
			}
			virtual void Destroy() = 0;
			ShowBuffer** GetResults() {return buffer_data_set_;}
		protected:
			ShowBuffer** buffer_data_set_;
			IBatchCallback* callback_;
		};

		///
		// 批处理参数基类
		///
		class BatchArgs
		{
		public:
			/// Sets/Gets IThreedImaging object
			void SetImaging(IThreedImaging *imaging)
			{
				imaging_ = imaging;
			}
			IThreedImaging *GetImaging()
			{
				return imaging_;
			}
			/// Sets/Gets output directory path
			void SetOutputPath(string dir)
			{
				output_path_ = dir;
			}
			string GetOutputPath()
			{
				return output_path_;
			}
			/// Sets/Gets image number generated
			void SetImageNumber(int count) {image_number_=count;}
			int GetImageNumber() {return image_number_;}
			/// Sets/Gets step
			void SetStep(float step) {step_=step;}
			float GetStep() {return step_;}
			/// Sets/Gets blend mode
			void SetBlendMode(BlendMode mode)
			{
				blend_mode_ = mode;
			}
			BlendMode GetBlendMode()
			{
				return blend_mode_;
			}
			/// Sets/Gets orientation
			void SetOrientation(OrientationType ori)
			{
				orientation_ = ori;
			}
			OrientationType GetOrientation()
			{
				return orientation_;
			}
			/// Sets/Gets window level
			void SetWWWL(int ww, int wl)
			{
				ww_ = ww;
				wl_ = wl;
			}
			void GetWWWL(int &ww, int &wl)
			{
				ww = ww_;
				wl = wl_;
			}

		protected:
			IThreedImaging *imaging_;
			string output_path_;
			int image_number_;
			float step_;
			BlendMode blend_mode_;
			OrientationType orientation_;
			int ww_;
			int wl_;

		};

		class IBatchCallback
		{
		public:
			virtual void OnBatchOneGenerated(BufferResult* results, BYTE status) = 0;
			virtual void OnBatchAllGenerated(BYTE status) = 0;
		};
	}
}