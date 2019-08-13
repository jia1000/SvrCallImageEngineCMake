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

			/// ��ȡͼ��
			BufferResult *GetImage(int index) override;
			/// ��ӻ���ͼ��
			void AddImage(BufferResult *buffer) override;
			/// ��ȡ����ͼ������
			int GetImageNumber() override;
			/// �����������
			void Destroy() override;

			/// IBatchCallback implementation
			void OnBatchOneGenerated(BufferResult* results, BYTE status) override;
			void OnBatchAllGenerated(BYTE status) override;

		private:
			/// ͼ�����ݼ���
			vector<BufferResult *> buffer_list_;
			/// ControlID
			const string control_id_;

		};

	}
}