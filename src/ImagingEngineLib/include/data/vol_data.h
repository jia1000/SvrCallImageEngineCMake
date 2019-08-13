/*=========================================================================

  Program:   ImagingEngine
  Module:    vol_data.h
  author: 	 zhangjian
  Brief:	 三维数据类

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data_definition.h"
#include "data_convert.h"
// 在linux上box3.h编译不过 [5/24/2019 Modified by zhangjian]
//#include "data/box3.h"
#include "data/aabb.h"
#include "data/pixel_data.h"

using namespace DW::IMAGE;
using namespace std;

namespace DW {
	namespace IMAGE{
		class BoundingBox;

		class VolData
		{
		public:
			VolData();
			~VolData();
			IPixelData* GetPixelData();
			void SetPixelData(IPixelData*);

			BYTE GetBitsPerPixel() { return bits_per_pixel_; }
			void SetBitsPerPixel(BYTE bits);
			BYTE GetBitsStored() { return bits_stored_; }
			void SetBitsStored(BYTE bits);
			void SetData(UNITDATA3D* data);
			void SetMark(UNITMARK3D* mark);
			UNITMARK3D *GetMark() { return series_mark_; }

			int GetSliceWidth() { return slice_width_; }
			void SetSliceWidth(int width);
			int GetSliceHeight() { return slice_height_; }
			void SetSliceHeight(int height);
			int GetSliceCount() { return slice_count_; }
			void SetSliceCount(int count);
			int GetSliceFOVWidth() { return slice_fov_width_; }
			void SetSliceFOVWidth(int fov_w);
			int GetSliceFOVHeight() { return slice_fov_height_; }
			void SetSliceFOVHeight(int fov_h);
			void SetStudyInstanceUID(string uid);
			void SetSeriesInstanceUID(string uid);
			/// 设置包围盒的顶点
			void SetBoundingBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax);
			/// 获取当前包围盒对象
			BoundingBox* GetBoundingBox() { return bounding_box_; }
			/// 设置分割结果包围盒的顶点
			void SetMarkBoundingBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax);
			///TODO 临时用于存储clipping包围盒
			void SetMarkBoundingBoxClipping(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax);
			/// 获取当前分割结果包围盒对象
			BoundingBox* GetMarkBoundingBox() { return mark_bounding_box_; }
			///TODO 临时用于存储clipping包围盒
			BoundingBox* GetMarkBoundingBoxClipping() { return mark_bounding_box_clipping_; }
			/// 从数据指针中获取指定坐标位置的值
			void *GetDataPointer(int x, int y, int z);
			/// 设置获取数据修改标识
			void SetModified(bool flag) {modified_=flag;}
			bool GetModified() {return modified_;}
			void Destroy();
			/// 窗宽窗位
			void GetDefaultWindowWidthLevel(int &width, int &level);
			void SetDefaultWindowWidthLevel(int width, int level);

		protected:
			BYTE bits_per_pixel_;				//bits allocated for display 
			BYTE bits_stored_;					//bits stored, the max ct value is 4095 with bits stored value 12，and is 10000 with value 16
			IPixelData* pixel_data_;			//3d pixel data of the series
			//UNITDATA3D* series_data_;			//3d pixel data of the series
			UNITMARK3D* series_mark_;			//3d mark data of the series
			//vtkSmartPointer<vtkImageData> vtk_series_mark_;
			int slice_width_;					//value stored in tag Columns(0028,0011)
			int slice_height_;					//value stored in tag Rows(0028,0010)
			int slice_count_;					//slice count in the series
			float slice_fov_width_;				//Reconstruction Field of View (width), pixel space = fov(width) / columns
			float slice_fov_height_;			//Reconstruction Field of View (width), pixel space = fov(height) / rows
			string study_instance_uid_;			//study instance uid, stored in tag (0020,000D)
			string series_instance_uid_;		//series instance uid, stored in tag (0020,000E)
			BoundingBox *bounding_box_;			//
			BoundingBox *mark_bounding_box_;	//
			BoundingBox *mark_bounding_box_clipping_;	//
			int volume_id_;						//
			bool modified_;						//
			int window_width_;					// default window width
			int window_level_;					// default window level

		private:


		};

		class BoundingBox : public AABB
		{
		public:
			BoundingBox() {}
			~BoundingBox() {}

			void SetSpacing(double spacings[3]){
				int i;
				for(i=0; i<3; ++i){
					voxel_spacings[i] = spacings[i];
				}
			}

			const float& operator[](const uint32_t index) const
			{
				if (index == 0){
					return this->Xmin;
				}
				else if (index == 1){
					return this->Ymin;
				}
				else if (index == 2){
					return this->Zmin;
				}
				else if (index == 3){
					return this->Xmax;
				}
				else if (index == 4){
					return this->Ymax;
				}
				else if (index == 5){
					return this->Zmax;
				}
			}

			/// 得到患者坐标系的
			void GetCornerPoints(Point3f& top_left, Point3f& bottom_right)
			{
				top_left.x = Xmin * voxel_spacings[0];
				top_left.y = Ymin * voxel_spacings[1];
				top_left.z = Zmin * voxel_spacings[2];
				bottom_right.x = Xmax * voxel_spacings[0];
				bottom_right.y = Ymax * voxel_spacings[1];
				bottom_right.z = Zmax * voxel_spacings[2];
			}

			/// 得到患者坐标系的
			void GetCornerPoints(double top_left[3], double bottom_right[3])
			{
				top_left[0] = Xmin * voxel_spacings[0];
				top_left[1] = Ymin * voxel_spacings[1];
				top_left[2] = Zmin * voxel_spacings[2];
				bottom_right[0] = Xmax * voxel_spacings[0];
				bottom_right[1] = Ymax * voxel_spacings[1];
				bottom_right[2] = Zmax * voxel_spacings[2];
			}

		private:
			double voxel_spacings[3];

		};

	}
}