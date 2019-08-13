/*=========================================================================

  Program:   ImagingEngine
  Module:    aabb.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data/aabb.h"
#include "tools/math.h"

AABB::AABB(void)
{

}

//��AABB��Χ����С���������Point3f�ṹ����Ϊ�����Ĺ��캯��
AABB::AABB(Point3f pointMin, Point3f pointMax)
{
	Xmin=pointMin.x;
	Ymin=pointMin.y;
	Zmin=pointMin.z;
	Xmax=pointMax.x;
	Ymax=pointMax.y;
	Zmax=pointMax.z;

}

//��AABB��Χ����С�������������������Ϊ�����Ĺ��캯��
AABB::AABB(float m_Xmin,float m_Ymin,float m_Zmin,float m_Xmax,float m_Ymax,float m_Zmax)
{
	Xmin=m_Xmin;
	Xmax=m_Xmax;
	Ymin=m_Ymin;
	Ymax=m_Ymax;
	Zmin=m_Zmin;
	Zmax=m_Zmax;
}

AABB::~AABB(void)
{

}

/*-----IsInterection(AABB *a)---------
�������ܣ��ж�����AABB��Χ���Ƿ��ཻ
����������aΪAABB����ָ��,�������Ϊb��Χ��
--------------------------------------------------*/
//�ж�����AABB��Χ���Ƿ��ཻ
BOOL AABB::IsInterection(AABB *a)
{
	//���a��Χ����С��X�������b��Χ�е����X����,a��Χ����X��������b���ұ�
	if (a->Xmin>Xmax){
		return false;
	}
	else if (a->Xmax<Xmin){
		//���a��Χ������X����С��b��Χ�е���С��X���꣬a��Χ����X��������b�����
		return false;
	}
	else if (a->Ymin>Ymax){
		//���a��Χ����С��Y�������b��Χ�е����Y���꣬a��Χ����Y��������b���ұ�
		return false;
	}
	else if (a->Ymax<Ymin){
		//���a��Χ������Y����С��b��Χ�е���С��Y����,a��Χ����Y��������b�����
		return false;
	}
	else if (a->Zmin>Zmax){
		//���a��Χ����С��Z�������b��Χ�е����Z���꣬a��Χ����Z��������b���Ϸ�
		return false;
	}
	else if (a->Zmax<Zmin){
		//���a��Χ������Z����С��b��Χ�е���С��Z����,a��Χ����Z��������b���·�
		return false;
	}
	else{
		return true;
	}
}

/*-----IsPointInBox�жϵ��Ƿ�����AABB��Χ��֮��------
�������ܣ��ж���ά���Ƿ�����AABB��Χ��֮��
����������ptΪPoint3f�ṹ�壬boxΪAABB��Χ�ж���ָ��
---------------------------------------------------------------------------------------------*/
BOOL AABB::IsPointInBox(Point3f pt)
{
	return (pt.x>=Xmin)&&(pt.x<=Xmax)&&
		(pt.y>=Ymin)&&(pt.y<=Ymax)&&
		(pt.z>=Zmin)&&(pt.z<=Zmax);
}

BOOL AABB::IsPointInBox(float x, float y, float z)
{
	return (x>=Xmin)&&(x<=Xmax)&&
		(y>=Ymin)&&(y<=Ymax)&&
		(z>=Zmin)&&(z<=Zmax);
}

/*-----IsPointInBoxPlane()-------------------------------------------
�������ܣ����Ƿ��ڰ�Χ�е���������
-------------------------------------------------------------------*/
BOOL AABB::IsPointInBoxPlane(float x, float y, float z)
{
	// �Ƿ�������yzƽ����
	if (fabs(x) < MathTool::kEpsilon ||
		fabs(Xmax - x) < MathTool::kEpsilon)
	{
		if (y >= Ymin && y <= Ymax && z >= Zmin && z <= Zmax)
			return true;
	}

	// �Ƿ�������xzƽ����
	if (fabs(y) < MathTool::kEpsilon ||
		fabs(Ymax - y) < MathTool::kEpsilon)
	{
		if (x >= Xmin && x <= Xmax && z >= Zmin && z <= Zmax)
			return true;
	}

	// �Ƿ�������xyƽ����
	if (fabs(z) < MathTool::kEpsilon ||
		fabs(Zmax - z) < MathTool::kEpsilon)
	{
		if (x >= Xmin && x <= Xmax && y >= Ymin && y <= Ymax)
			return true;
	}

	return false;
}

/*-----GetBoxCenter()-------------------------------------------
�������ܣ��õ���AABB��Χ�ж�������ģ����ģ�����һ��Point3f�ṹ��
-------------------------------------------------------------------*/
Point3f AABB::GetBoxCenter()
{
	Point3f m_point;
	m_point.x=(Xmin+Xmax)*0.5f;
	m_point.y=(Ymin+Ymax)*0.5f;
	m_point.z=(Zmin+Zmax)*0.5f;
	return m_point;
}

/*-----IsOrNotEmpty()---------------------------------------------
�������ܣ��ж�AABB��Χ���Ƿ�Ϊ�գ�Ϊ�շ���TRUE����Ϊ�շ���false
------------------------------------------------------------------*/
BOOL AABB::IsOrNotEmpty()
{
	return(Xmin>Xmax)||(Ymin>Ymax)||(Zmin>Zmax);
}

/*-----GetXLength()---------
�������ܣ��õ�X�����ϵĳ���
--------------------------*/
float AABB::GetXLength()
{
	return Xmax-Xmin;
}

/*-----GetYLength()---------
�������ܣ��õ�Y�����ϵĳ���
--------------------------*/
float AABB::GetYLength()
{
	return Ymax-Ymin;
}

/*-----GetZLength()---------
�������ܣ��õ�Z�����ϵĳ���
--------------------------*/
float AABB::GetZLength()
{
	return Zmax-Zmin;
}

/*-----GetVolume()-------------
�������ܣ��õ�AABB��Χ�е����
------------------------------*/
float AABB::GetVolume()
{
	return (Xmax-Xmin)*(Ymax-Ymin)*(Zmax-Zmin);
}
/*-----DeepCopy()-------------
�������ܣ���������
------------------------------*/
void AABB::DeepCopy(AABB *a)
{
	Xmin=a->Xmin;
	Xmax=a->Xmax;
	Ymin=a->Ymin;
	Ymax=a->Ymax;
	Zmin=a->Zmin;
	Zmax=a->Zmax;
}
/*-----Update()-------------
�������ܣ����¶���
------------------------------*/
void AABB::Update(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax)
{
	Xmin=xmin;
	Xmax=xmax;
	Ymin=ymin;
	Ymax=ymax;
	Zmin=zmin;
	Zmax=zmax;
}

/*-----DrawAABBBoundingBox()-------------------
�������ܣ�����AABB��Χ��
-----------------------------------------------*/
void AABB::DrawBox()
{
	////��һ����
	//glBegin(GL_LINE);
	//glVertex3f(Xmin,Ymin,Zmin);
	//glVertex3f(Xmax,Ymin,Zmin);
	//glEnd();
	////�ڶ�����
	//glBegin(GL_LINE);
	//glVertex3f(Xmin,Ymin,Zmin);
	//glVertex3f(Xmin,Ymin,Zmax);
	//glEnd();
	////��������
	//glBegin(GL_LINE);
	//glVertex3f(Xmin,Ymin,Zmin);
	//glVertex3f(Xmin,Ymax,Zmin);
	//glEnd();
	////���ĸ���
	//glBegin(GL_LINE);
	//glVertex3f(Xmin,Ymin,Zmax);
	//glVertex3f(Xmax,Ymin,Zmax);
	//glEnd();
	////�������
	//glBegin(GL_LINE);
	//glVertex3f(Xmax,Ymin,Zmin);
	//glVertex3f(Xmax,Ymin,Zmax);
	//glEnd();
	////��������
	//glBegin(GL_LINE);
	//glVertex3f(Xmax,Ymin,Zmin);
	//glVertex3f(Xmax,Ymax,Zmin);
	//glEnd();
	////���߸���
	//glBegin(GL_LINE);
	//glVertex3f(Xmax,Ymax,Zmin);
	//glVertex3f(Xmin,Ymax,Zmin);
	//glEnd();
	////�ڰ˸���
	//glBegin(GL_LINE);
	//glVertex3f(Xmin,Ymax,Zmin);
	//glVertex3f(Xmin,Ymax,Zmax);
	//glEnd();
	////�ھŸ���
	//glBegin(GL_LINE);
	//glVertex3f(Xmax,Ymax,Zmin);
	//glVertex3f(Xmax,Ymax,Zmax);
	//glEnd();
	////��ʮ����
	//glBegin(GL_LINE);
	//glVertex3f(Xmin,Ymin,Zmax);
	//glVertex3f(Xmin,Ymax,Zmax);
	//glEnd();
	////��ʮһ����
	//glBegin(GL_LINE);
	//glVertex3f(Xmax,Ymin,Zmax);
	//glVertex3f(Xmax,Ymax,Zmax);
	//glEnd();
	////��ʮ������
	//glBegin(GL_LINE);
	//glVertex3f(Xmin,Ymax,Zmax);
	//glVertex3f(Xmax,Ymax,Zmax);
	//glEnd();
}