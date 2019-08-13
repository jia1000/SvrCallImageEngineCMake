/*=========================================================================

  Program:   ImagingEngine
  Module:    orientation_interpreter.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "orientation/orientation_interpreter.h"

using namespace DW::Orientation;

//TODO 约定与x/y/z轴夹角在75度范围之内，均显示x/y/z轴方向标识
// 对应弧度值约为0.258819
const float OrientationInterpreter::kFixedRadianLimit = 0.258819f;

void OrientationInterpreter::Interpret(const Vector3d &direction_unit, char orientation[4])
{
	char *optr = orientation;

	char orientationX = direction_unit[0] < 0 ? 'R' : 'L';
	char orientationY = direction_unit[1] < 0 ? 'A' : 'P';
	char orientationZ = direction_unit[2] < 0 ? 'F' : 'H';

	double absX = fabs(direction_unit[0]);
	double absY = fabs(direction_unit[1]);
	double absZ = fabs(direction_unit[2]);

	if (absX > kFixedRadianLimit) {
		*optr++ = orientationX;
	}
	if (absY>kFixedRadianLimit) {
		*optr++ = orientationY;
	}
	if (absZ>kFixedRadianLimit) {
		*optr++ = orientationZ;
	}
}

void OrientationInterpreter::Interpret(const ImageOrientation &ori, char left[4], char right[4], char up[4], char down[4])
{
	Vector3d row_vector = ori.GetRowVector();
	Vector3d colume_vector = ori.GetColumnVector();

	Interpret(row_vector, right);
	Interpret(-row_vector, left);

	Interpret(colume_vector, down);
	Interpret(-colume_vector, up);
}