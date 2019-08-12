/*=========================================================================

  Program:   ImagingEngine
  Module:    get_set.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

// PropertyBuilderByName ����������ĳ�Ա����
// ������set��get����
// type Ϊ��������
// access_permission Ϊ�����ķ���Ȩ��(public, priavte, protected)

#define PropertyBuilderByName(type, name)\
private:\
	type m_##name;\
	public:\
	 void Set##name(type v) {\
	m_##name = v;\
	}\
	type Get##name() {\
	return m_##name;\
	}\

#define PointerPropertyBuilderByName(type, name)\
private:\
	type* m_##name;\
	public:\
	void Set##name(type* v){\
	m_##name = v;\
		}\
		type* Get##name(){\
		return m_##name;\
		}\

