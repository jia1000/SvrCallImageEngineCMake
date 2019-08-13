/*=========================================================================

  Program:   ImagingEngine
  Module:    iod.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data_definition.h"

using namespace DW::IMAGE;
using namespace std;

namespace DW {
	namespace IMAGE{

		class DicomTagReaderWriter;

		///
		// BaseIOD
		///
		class BaseIOD
		{
		public:
			BaseIOD();
			virtual ~BaseIOD();

			virtual void SetDicomTagReaderWriter(DicomTagReader *reader);
			virtual void Read();
			virtual void Write(std::string);
		};

		class DicomTagReader
		{
		public:
			virtual bool GetValueString(unsigned int grupo, unsigned int elemento, std::string &value);
			virtual bool GetValueFloat(unsigned int grupo, unsigned int elemento, float &value);
			virtual bool GetValueDouble(unsigned int grupo, unsigned int elemento, double &value);
			virtual bool GetValueInt(unsigned int grupo, unsigned int elemento, int &value);

		};

	}
}