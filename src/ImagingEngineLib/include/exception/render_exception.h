/*=========================================================================

  Program:   ImagingEngine
  Module:    render_exception.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "exception/iexception.h"

namespace DW {
	namespace Exception {

		class RenderException : public IException
		{
		public:
			RenderException(const int& code, const std::string& msg)
				: IException(code, msg, "Render", true)
			{

			}
			~RenderException()
			{

			}

		private:

		};

	}
}
