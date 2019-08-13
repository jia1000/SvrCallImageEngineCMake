/*=========================================================================

  Program:   ImagingEngine
  Module:    image_source.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/show_buffer.h"
#include "batch/batch_generator.h"

using namespace DW::Batch;

namespace DW {
	namespace IMAGE {

		class IImageSouceCallback;

		///
		// IImageSource represents a data source
		///
		class IImageSource
		{
		public:
			virtual ~IImageSource() {}
			virtual void SetCallback(IImageSouceCallback *callback)
			{
				image_changed_callback_ = callback;
			}
			/// ��ȡͼ��
			virtual BufferResult *GetImage(int index) = 0;
			/// ���һ������ͼ��
			virtual void AddImage(BufferResult *buffer) = 0;
			/// ��ȡ����ͼ������
			virtual int GetImageNumber() = 0;
			/// �����������
			virtual void Destroy() = 0;

		private:
			IImageSouceCallback *image_changed_callback_;

		};

		class IImageSouceCallback
		{
		public:
			virtual void OnImageAdded() = 0;
		};

	}
}