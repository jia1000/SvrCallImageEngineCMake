/*=========================================================================

  Program:   ImagingEngine
  Module:    batch_image_source.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "batch/batch_image_source.h"

using namespace DW::IMAGE;
using namespace DW::Batch;


BatchImageSource::BatchImageSource()
{

}

BatchImageSource::~BatchImageSource()
{
	Destroy();
}

BufferResult *BatchImageSource::GetImage(int index)
{
	if (index < buffer_list_.size())
		return buffer_list_[index];

	return NULL;
}

void BatchImageSource::AddImage(BufferResult *buffer)
{
	buffer_list_.push_back(buffer);
}

int BatchImageSource::GetImageNumber()
{
	return buffer_list_.size();
}

void BatchImageSource::OnBatchOneGenerated(BufferResult* results, BYTE status)
{
	buffer_list_.push_back(results);

	//TODO 通知前端生成进度
}

void BatchImageSource::OnBatchAllGenerated(BYTE status)
{
	//TODO 通知前端生成完成
}

void BatchImageSource::Destroy()
{
	auto it = buffer_list_.begin();
	while (it != buffer_list_.end()){
		delete *it;
		++it;
	}
	buffer_list_.clear();
}