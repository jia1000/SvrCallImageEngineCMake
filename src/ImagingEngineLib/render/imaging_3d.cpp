#include "render/imaging_3d.h"
#include "tools/convert_vtk_image_data_to_rgba.h"

using namespace DW::Render;

//// ��ȡԭʼ��άͼ�����ݳ�
//VolData* ThreeDImaging::GetData()
//{
//	return volume_data_;
//}
//// ����ԭʼ��άͼ�����ݳ�
//void ThreeDImaging::SetData(VolData* data)
//{
//	volume_data_ = data;
//}
//// ��ȡ�����ʾͼ��
//ShowBuffer* ThreeDImaging::GetShowBuffer()
//{
//	return show_buffer_;
//}
//// ��Ⱦͼ�񣬻��浽show_buffer_
//void ThreeDImaging::Render()
//{
//	
//}
//// ����
//void ThreeDImaging::Zoom(int scale)
//{
//
//}
//// �ƶ�
//void ThreeDImaging::Move(int distance) 
//{
//
//}
//// ƽ������ת
//void ThreeDImaging::Rotate(float angle)
//{
//
//}
//// ������������һ���Ƕ�
//void ThreeDImaging::Rotate3D(Vector3D axis, float angle) 
//{
//
//}
//// ����λ
//void ThreeDImaging::WindowWidthLevel(int width, int level) 
//{
//
//}
//
//void ThreeDImaging::SetRenderer(ThreeDRenderer* renderer)
//{
//	renderer_ = renderer;
//}