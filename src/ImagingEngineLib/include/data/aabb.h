/*=========================================================================

  Program:   ImagingEngine
  Module:    aabb.h
  author: 	 zhangjian
  Brief:	 ʹ��Max-min��ʾ����AABB��Χ��
			 ʹ��һ�����ϽǺ����½ǵĵ���Ψһ�Ķ���һ����Χ��

=========================================================================*/
#pragma once

#include "data/data_definition.h"

class AABB
{
public:
	AABB(void);
	~AABB(void);
	///
	// �Զ�����вι��캯��������Ϊ����Pointf�ṹ��AABB��Χ����С��������
	///
	AABB(Point3f pointMin, Point3f pointMax);
	///
	// �Զ�����вεĹ��캯��������ΪAABB��Χ����С���������
	///
	AABB(float m_Xmin,float m_Xmax,float m_Ymin,float m_Ymax,float m_Zmin,float m_Zmax);
	///
	// ��һ�����󿽱�
	///
	void DeepCopy(AABB* a);
	///
	// �ж�����AABB��Χ���Ƿ��ཻ������ཻ����TRUE��������ཻ������FALSE
	///
	BOOL IsInterection(AABB* a);
	///
	// �ж�һ��ά���Ƿ�����AABB��Χ����
	///
	BOOL IsPointInBox(Point3f pt);
	BOOL IsPointInBox(float x, float y, float z);
	///
	// �ж�һ��ά���Ƿ�����AABB��Χ��ĳ������
	///
	BOOL IsPointInBoxPlane(float x, float y, float z);
	///
	// ��AABB��Χ�е����Ļ�������
	///
	Point3f GetBoxCenter();
	///
	// �ж�AABB��Χ���Ƿ�Ϊ��
	///
	BOOL IsOrNotEmpty();
	///
	// �õ�X�����ϵĳ���
	///
	float GetXLength();
	///
	// �õ�Y�����ϵĳ���
	///
	float GetYLength();
	///
	// �õ�Z�����ϵĳ���
	///
	float GetZLength();
	///
	// �õ�AABB��Χ�е����
	///
	float GetVolume();
	///
	// ����AABB��Χ�У�������
	///
	void DrawBox();
	///
	// ���¶���ֵ
	///
	void Update(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax);

protected:
	//X����Сֵ
	float Xmin;
	//X�����ֵ
	float Xmax;
	//Y����Сֵ
	float Ymin;
	//Y�����ֵ
	float Ymax;
	//Z����Сֵ
	float Zmin;
	//Z�����ֵ
	float Zmax;

};
