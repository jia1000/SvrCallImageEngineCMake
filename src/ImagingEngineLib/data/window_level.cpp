/*=========================================================================

  Program:   ImagingEngine
  Module:    window_level.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data/window_level.h"

using namespace DW::IMAGE;


WindowLevel::WindowLevel()
{
	window_width_ = 1;
	window_level_ = 0;
}

WindowLevel::WindowLevel(int width, int level, const string &name)
{
	window_width_ = width;
	window_level_ = level;
	window_level_name_ = name;
}

WindowLevel::~WindowLevel()
{

}

void WindowLevel::SetName(const string &name)
{
	window_level_name_ = name;
}

const string& WindowLevel::GetName() const
{
	return window_level_name_;
}

void WindowLevel::SetWindowWidth(int width)
{
	window_width_ = width;
}

int WindowLevel::GetWindowWidth() const
{
	return window_width_;
}

void WindowLevel::SetWindowLevel(int level)
{
	window_level_ = level;
}

int WindowLevel::GetWindowLevel() const
{
	return window_level_;
}

bool WindowLevel::IsValid() const
{
	return window_width_ >= 1;
}

bool WindowLevel::ValuesAreEqual(const WindowLevel &window_level) const
{
	return window_width_ == window_level.window_width_ && window_level_ == window_level.window_level_;
}

bool WindowLevel::operator==(const WindowLevel &window_level) const
{
	return window_width_ == window_level.window_width_ && window_level_ == window_level.window_level_ && window_level_name_ == window_level.window_level_name_;
}

vtkLookupTable* WindowLevel::ConvertToVtkLookupTable() const
{
	vtkWindowLevelLookupTable *lut = vtkWindowLevelLookupTable::New();
	lut->SetWindow(window_width_);
	lut->SetLevel(window_level_);
	//lut->SetInverseVideo(std::signbit(window_width_));
	lut->Build();
	lut->BuildSpecialColors();
	return lut;
}