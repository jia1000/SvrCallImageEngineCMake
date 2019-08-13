/*=========================================================================

  Program:   ImagingEngine
  Module:    show_buffer.h
  author: 	 zhangjian
  Brief:	 UI��ʾ����࣬����UNITDATASHOWָ�����͵�buffer
			 UNITDATASHOW����Ӧ����Ϊ����������

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "data/image_plane.h"

using namespace std;

namespace DW {
	namespace IMAGE{

		///
		// show buffer is associated with one imaging object
		///
		class ShowBuffer
		{
		public:
			ShowBuffer();
			~ShowBuffer();
			/// ����һ��ָ����С��buffer
			void InitBufferData(int width, int height, BYTE bits_per_pixel);
			/// ���ֽ����ݸ�ֵ����buffer��
			void SetBufferData(UNITDATASHOW* bytes, int width, int height, BYTE bits_per_pixel);
			/// ��ȡ��ǰbuffer
			UNITDATASHOW* GetShowBuffer();
			/// ���
			int GetWidth() {return buffer_width_;}
			/// �߶�
			int GetHeight() {return buffer_height_;}
			/// λ��
			BYTE GetBitsPerPixel() {return bits_per_pixel_;}
			/// ͨ����
			BYTE GetNumberOfComponent() { return bits_per_pixel_ / 8; }
			/// ͼ�����зֱ���
			void SetDPI(float x, float y)
			{
				x_dpi_ = x;
				y_dpi_ = y;
			}
			void GetDPI(float &x, float &y)
			{
				x = x_dpi_;
				y = y_dpi_;
			}
			/// Copy 
			void ShallowCopy(ShowBuffer *from);
			void DeepCopy(ShowBuffer *from);
			
			/// methods for testing
			void DumpBufferToFile(const char* file_path);

		protected:
			BYTE bits_per_pixel_;				//bits allocated for display 
			UNITDATASHOW* buffer_data_;			//pixel data for display
			int buffer_width_;					//width of the pixel data
			int buffer_height_;					//height of the pixel data
			float x_dpi_;						//indicate how long in mm unit is each pixel in x direction
			float y_dpi_;						//indicate how long in mm unit is each pixel in y direction
			string time_stamp_;					//time stamp uid

		private:


		};

		///
		// Struct to store image buffer and information
		///
		struct BufferResult
		{
			~BufferResult()
			{
				if (buffer_data) {
					delete buffer_data;
					buffer_data = NULL;
				}
				if (image_plane) {
					delete image_plane;
					image_plane = NULL;
				}
			}
			ShowBuffer *buffer_data;
			ImagePlane *image_plane;
			string file_name;
			float step;
			int instance_number;
			int window_width;
			int window_level;

		};

	}
}