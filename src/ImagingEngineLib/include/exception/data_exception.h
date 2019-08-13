/*=========================================================================

  Program:   ImagingEngine
  Module:    data_exception.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "exception/iexception.h"

namespace DW {
	namespace Exception {

		class DataException : public IException
		{
		public:
			DataException(const int& code, const std::string& msg)
				: IException(code, msg, "Data", true)
			{

			}
			~DataException()
			{

			}

		private:

		};
		
	}
}