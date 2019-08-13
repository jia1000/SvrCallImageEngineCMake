/*=========================================================================

  Program:   ImagingEngine
  Module:    math.h
  author: 	 zhangjian
  Brief:	 �ṩ��ֵ����ķ���

=========================================================================*/
#pragma once
#include <cmath>
#include <ctime>
#include <iostream>
#include "data/vector3.h"

class MathTool
{
public:
	///
	/// ��������
	///
	static const double kNumberEBase2Logarithm;
	static const double kReversePiAsDouble;
	static const float kPi;
	static const long double kPiAsDouble;
	static const double kDegreesToRadiansAsDouble;
	static const double kRadiansToDegreesAsDouble;
	/// Epsilon ����������ж�ֵ
	static const double kEpsilon;

	///
	// ������һ��
	///
	static float Normalize(float x[3]);
	static float Normalize(Vector3f& x);
	static double Normalize(double x[3]);
	static double Normalize(Vector3d& x);
	///
	// ��������֮������ؾ���
	///
	static float ComputeDistanceBetweenPoints(const float StartPnt[3], const float EndPnt[3]);
	///
	// ����㵽ֱ�߾���
	///
	static double ComputeDistancePointToLine(const float line_vec[3],
		const float point_on_line[3],
		const float point_outside[3]);
	///
	// ��������֮������ؾ��룬����������
	///
	static float ComputeDistanceBetweenPoints(const float StartPnt[3], const float EndPnt[3], const float direction[3]);
	///
	// PL���߶˵㣬PPƽ��һ�㣬e���ߵ�λ������qƽ����Ϣ��P���ؽ���
	///
	static void ComputeLineAndPlane(float PL[], float PP[], float e[], float* q, float* P);
	///
	// ��ռ���һ����һ��ֱ���ϵ�ͶӰ
	///
	static void Compute3DProjectionOnLine(const float line_vec[],
		const float pnt_on_line[],								  
		const float pnt_outside[],
		float *projection);
	///
	// �жϵ�ǰ����ԭ���Ҳ໹�����
	// ����ֵ��true�Ҳࣻfalse���
	///
	static bool IsPointOnTheRightSide(const float line_vec[],
		const float origin[],
		const float current_point[]);
	///
	// ������ת
	///
	static void RotateAroundAxis(
		float u, float v, float w,        /*Axis*/
		float x, float y, float z,        /*The Point That We Want to Roate */
		float& nx, float& ny, float& nz,  /*Result*/
		float degree );

	//----------------------------------------------------------------------------
	// Cross product of two 3-vectors. Result (a x b) is stored in z[3].
	static void Cross(const float x[3], const float y[3], float z[3]);
	static void Cross(Vector3f x, Vector3f y, Vector3f& z);
	static void Cross(Vector3d x, Vector3d y, Vector3d& z);

	///
	// Scalar Product of the two vectors passed by parameter
	///
	static float DotProduct(const float vector1[3], const float vector2[3]);

	// Description:
	// Compute the norm of 3-vector.
	static float Norm(const float x[3]);
	static float Norm(Vector3f x);
	static double Norm(const double x[3]);
	static double Norm(Vector3d x);
	
	///
	// Converts the value passed by degrees to radians and vice-versa
	///
	static float DegreesToRadians(float degrees);
	static float RadiansToDegrees(float radians);

	///
	// Calculates the angle between two vectors. Returns the value in radians or degrees
	///
	static float AngleInRadians(const Vector3f &vec1, const Vector3f &vec2);
	static float AngleInDegrees(const Vector3f &vec1, const Vector3f &vec2);

	///
	// Calculates the intersection of two plans defined by a point and a normal (P, N) and (Q, m) respectively 
	// Returns 0 if the plans are not intersected because they are parallel, otherwise > 0
	///
	static int PlaneIntersection(float p[3], float n[3], float q[3], float m[3], float r[3], float t[3]);
	
	///
	// Rounds a float to the nearest integer.
	///
	static int Round(float f) { return static_cast<int>( f + ( f >= 0.0 ? 0.5 : -0.5 ) ); }
	static int Round(double f) { return static_cast<int>( f + ( f >= 0.0 ? 0.5 : -0.5 ) ); }

	///
	//
	//
	static bool ComputeRotationAngleAxis(const Vector3f &old_vector, const Vector3f &new_vector, float &angle, Vector3f &axis);
};