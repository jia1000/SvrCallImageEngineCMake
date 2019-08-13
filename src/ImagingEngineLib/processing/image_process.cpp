/*=========================================================================

  Program:   ImagingEngine
  Module:    image_process.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "processing/image_process.h"

#include <algorithm>
#include <fstream>
//#include <io.h>
#include "tools/math.h"

using namespace DW::CV;

ImageProcessBase::ImageProcessBase(IBitmap *in_image_data)
	: in_image_data_(in_image_data)
	, image_width_(in_image_data->GetWidth())
	, image_height_(in_image_data->GetHeight())
	, bits_per_pixel_(in_image_data->GetNumberOfComponent())
{

}

ImageProcessBase::~ImageProcessBase()
{

}
//
//void ImageProcessBase::Mat2Buffer(const cv::Mat &img, UNITDATASHOW *output) 
//{
//	// һ�����ص�bits
//	int nFlag = img.channels() * 8;
//	int nHeight = img.rows;
//	int nWidth = img.cols;
//	// ͼ���ܵ��ֽ�
//	int nBytes = nHeight * nWidth * nFlag / 8;
//
//	if(output)
//		delete output;
//
//	// new�ĵ�λΪ�ֽ�
//	output = new UNITDATASHOW[nBytes];
//	// ת������,ע��Mat��data��Ա
//	memcpy(output, img.data, nBytes);
//}
//
//void ImageProcessBase::Buffer2Mat(const UNITDATASHOW *input, int width, int height, int bits, cv::Mat &img)
//{
//	if (input == NULL) return;
//
//	int bytes = width * height * bits / 8;//�ֽڼ���
//	int type = bits == 8 ? CV_8UC1 : CV_8UC3;
//
//	img = cv::Mat::zeros(width, height, bits / 8);
//	memcpy(img.data, input, bytes);
//
//
//
//	//img = cv::Mat(height,width, , input).clone(); // make a copy
//}
//
//BOOL ImageProcessBase::HBitmapToMat(IBitmap *_hBmp, cv::Mat& _mat)
//{
//	//TODO to implement
//	//BITMAP bmp;    
//	//GetObject(_hBmp,sizeof(BITMAP),&bmp);    
//	//int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8 ;
//	//int cv_channel = nChannels == 4 ? CV_8UC4 : CV_8UC3;
//	//int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;    
//	//cv::Mat v_mat;
//	//v_mat.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(cv_channel, nChannels));
//	//GetBitmapBits(_hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, v_mat.data);  
//	//_mat=v_mat;
//	return TRUE;   
//}
//
//BOOL ImageProcessBase::MatToHBitmap(cv::Mat& _mat, IBitmap *_hBmp)
//{
//	//MAT���TYPE=��nChannels-1+ CV_8U��<<3
//	int nChannels = (_mat.type() >> 3) - CV_8U + 1;
//	int cv_channel = nChannels == 4 ? CV_8UC4 : CV_8UC3;
//	int iSize=_mat.cols * _mat.rows * nChannels;
//	//TODO to implement
//	//_hBmp = CreateBitmap(_mat.cols, _mat.rows, 1, nChannels * 8, _mat.data);
//	return TRUE;
//}


//// hbitmap convert to IplImage   
//IplImage* ImageProcessBase::HBitmapToIplImage(HBITMAP hBmp)   
//{   
//	BITMAP bmp;    
//	GetObject(hBmp,sizeof(BITMAP),&bmp);    
//
//	// get channels which equal 1 2 3 or 4    
//	// bmBitsPixel :   
//	// Specifies the number of bits    
//	// required to indicate the color of a pixel.    
//	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel/8 ;   
//
//	// get depth color bitmap or grayscale   
//	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;    
//
//
//	// create header image   
//	IplImage* img = cvCreateImage(cvSize(bmp.bmWidth,bmp.bmHeight),depth,nChannels);    
//
//	// allocat memory for the pBuffer   
//	BYTE *pBuffer = new BYTE[bmp.bmHeight*bmp.bmWidth*nChannels];    
//
//	// copies the bitmap bits of a specified device-dependent bitmap into a buffer   
//	GetBitmapBits(hBmp,bmp.bmHeight*bmp.bmWidth*nChannels,pBuffer);    
//
//	// copy data to the imagedata   
//	memcpy(img->imageData,pBuffer,bmp.bmHeight*bmp.bmWidth*nChannels);   
//	delete pBuffer;    
//
//	// create the image   
//	IplImage *dst = cvCreateImage(cvGetSize(img),img->depth,3);   
//	// convert color   
//	cvCvtColor(img,dst,CV_BGRA2BGR);   
//	cvReleaseImage(&img);   
//	return dst;   
//}


//////////////////////////////////////////////////////////////////////////
ImageZoomProcess::ImageZoomProcess(IBitmap *in_image_data, float scale)
	: ImageProcessBase(in_image_data)
	, scale_(scale)
{
}

ImageZoomProcess::~ImageZoomProcess()
{
}

bool ImageZoomProcess::Excute(IBitmap *out_image_data)
{
	//cv::Mat src_image;
	//HBitmapToMat(in_image_data_, src_image);
	////cv::imwrite("C:\\ztest2\\a.png", src_image);
	//
	//// ����ͼ��
	//cv::Mat dst_image;
	//cv::resize(src_image, dst_image, cv::Size(), scale_, scale_);

	//MatToHBitmap(dst_image, out_image_data);
	////cv::imwrite("C:\\ztest2\\b.png", dst_image);
	
	return false;
}


//////////////////////////////////////////////////////////////////////////
ImageRotateProcess::ImageRotateProcess(IBitmap *in_image_data, float angle)
	: ImageProcessBase(in_image_data)
	, angle_(angle)
{
}

ImageRotateProcess::~ImageRotateProcess()
{
}

bool ImageRotateProcess::Excute(IBitmap *out_image_data)
{
	//// ����ͼ���Եfull
	//int padding_val = 0;
	//cv::Mat src_image;
	//HBitmapToMat(in_image_data_, src_image);

	//// ��תͼ��
	//double m[6];
	//cv::Mat dst_image;
	////cv::rotate(src_image, dst_image, angle_);

	////// warpAffineĬ�ϵ���ת��������ʱ�룬���ԼӸ��ű�ʾת��Ϊ˳ʱ��
	////angle_ = -angle_;
	//// ת��Ϊ����  
	//double angle_radian = MathTool::DegreesToRadians(angle_);
	//double a = sin(angle_radian), b = cos(angle_radian);
	//int width = src_image.cols;
	//int height = src_image.rows;
	//int width_rotate = int(width * fabs(b) + height * fabs(a));
	//int height_rotate = int(height * fabs(b) + width * fabs(a));

	//if(width_rotate <= 20 || height_rotate <= 20){
	//	width_rotate = 20;
	//	height_rotate = 20;
	//}

	//// ��ת����map
	//// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
	//// [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]
	//float map[6];
	//cv::Mat map_matrix = cv::Mat(2, 3, CV_32F, map);
	//// ��ת����
	//CvPoint2D32f center = cvPoint2D32f(width / 2, height / 2);
	//CvMat map_matrix2 = map_matrix;
	//// �����ά��ת�ķ���任����
	//cv2DRotationMatrix(center, angle_, 1.0, &map_matrix2);
	//map[2] += (width_rotate - width) / 2;
	//map[5] += (height_rotate - height) / 2;

	//// Mat img_rotate;
	//// ��ͼ��������任
	//// CV_WARP_FILL_OUTLIERS - ����������ͼ������ء�
	//// �������������������ͼ��ı߽��⣬��ô���ǵ�ֵ�趨Ϊ fillval.
	//// CV_WARP_INVERSE_MAP - ָ�� map_matrix �����ͼ������ͼ��ķ��任��
	//int chnnel = src_image.channels();
	//if(chnnel == 3)
	//	warpAffine(src_image, dst_image, map_matrix, cv::Size(width_rotate, height_rotate), 1, 0, cv::Scalar(padding_val,padding_val,padding_val));
	//else
	//	warpAffine(src_image, dst_image, map_matrix, cv::Size(width_rotate, height_rotate), 1, 0, padding_val);

	//MatToHBitmap(dst_image, out_image_data);

	return false;
}

//////////////////////////////////////////////////////////////////////////
ImageMoveProcess::ImageMoveProcess(IBitmap *in_image_data, float dx, float dy)
	: ImageProcessBase(in_image_data)
	, offset_x_(dx)
	, offset_y_(dy)
{

}

ImageMoveProcess::~ImageMoveProcess()
{

}

bool ImageMoveProcess::Excute(IBitmap *out_image_data)
{
	//cv::Mat src_image;
	//HBitmapToMat(in_image_data_, src_image);
	//
	//int dx = offset_x_;
	//int dy = offset_y_;

	//const int rows = src_image.rows;
	//const int cols = src_image.cols;

	//// ƽ��ͼ��
	//cv::Mat dst_image;
	//dst_image.create(rows, cols, src_image.type());

	//int channel = src_image.channels();
	//uchar val = 0;
	//cv::Vec4b padding(val);
	//cv::Vec4b *p;
	//int i, j;
	//for (i = 0; i < rows ; i++) {
	//	p = dst_image.ptr<cv::Vec4b>(i);
	//	for (j = 0; j < cols ; j++) {
	//		// ƽ�ƺ�����ӳ�䵽ԭͼ��
	//		int x = j - dx;
	//		int y = i - dy;

	//		// ��֤ӳ����������ԭͼ��Χ��
	//		if (x >=0 && y >= 0 && x < cols && y < rows) {
	//			p[j] = src_image.ptr<cv::Vec4b>(y)[x];
	//		}
	//		else{
	//			p[j] = padding;
	//		}
	//	}		
	//}

	//MatToHBitmap(dst_image, out_image_data);

	return false;
}