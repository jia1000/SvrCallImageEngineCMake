/*=========================================================================

  Program:   ImagingEngine
  Module:    batch_image_source.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/show_buffer.h"
#include "data/image_source.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Batch {

		///
		// IImageSource represents a data source
		///
		class BatchImageSource : public IImageSource, public IBatchCallback
		{
		public:
			BatchImageSource();
			~BatchImageSource();

			/// 获取图像
			BufferResult *GetImage(int index) override;
			/// 添加缓存图像
			void AddImage(BufferResult *buffer) override;
			/// 获取缓存图像总数
			int GetImageNumber() override;
			/// 清空所有数据
			void Destroy() override;

			/// IBatchCallback implementation
			void OnBatchOneGenerated(BufferResult* results, BYTE status) override;
			void OnBatchAllGenerated(BYTE status) override;

		private:
			/// 图像数据集合
			vector<BufferResult *> buffer_list_;
			/// ControlID
			const string control_id_;

		};

	}
}