/*=========================================================================

  Program:   ImagingEngine
  Module:    iexception.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "exception/iexception.h"

using namespace DW::Exception;


IException::IException(const int& code, const std::string& msg, const std::string& component, bool fatal) throw()
{
	this->cause_ = msg;
	this->component_ = component;
	this->fatal_ = fatal;
	this->error_code_ = code;
}

IException::~IException() throw()
{
}

const int& IException::GetErrorCode() const
{
	return error_code_;
}


const std::string& IException::GetComponent() const
{
	return component_;
}

const std::string& IException::GetCause() const {
	return cause_;
}

const char* IException::what() const throw()
{
	return cause_.c_str();
}

std::string IException::GetFullCause() const {
	return *this;
}

bool IException::IsFatal()  const {
	return fatal_;
}

IException::operator std::string() const
{
	return std::string("Exception in component") + std::string(" ") + GetComponent() + " Code: " + std::to_string(GetErrorCode()) + std::string(" :\n")  + GetCause();
}