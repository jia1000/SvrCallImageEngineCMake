/*=========================================================================

  Program:   ImagingEngine
  Module:    get_set.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

// PropertyBuilderByName 用于生成类的成员变量
// 并生成set和get方法
// type 为变量类型
// access_permission 为变量的访问权限(public, priavte, protected)

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

