/*=========================================================================

  Program:   ImagingEngine
  Module:    iexception.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include <string>
#include <exception>

namespace DW {
	namespace Exception {

		class IException : public std::exception {
		public:
			IException(const int& code, const std::string& msg, const std::string& component, bool fatal = true) throw();

			virtual ~IException() throw();

			const int& GetErrorCode() const;

			const std::string& GetComponent() const;

			const std::string& GetCause() const;

			virtual const char* what() const throw();

			std::string GetFullCause() const;

			bool IsFatal()  const;

			operator std::string () const;

			inline const std::string str() const { return (std::string) *this; }

		protected:
			int error_code_;
			std::string cause_;
			std::string component_;
			bool fatal_;
		};

	}
}