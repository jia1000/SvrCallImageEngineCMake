/*=========================================================================

  Program:   ImagingEngine
  Module:    dicom_attribute.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

using namespace std;

namespace GIL {
	namespace DICOM {

		class DICOMAttribute {

		public:
			DICOMAttribute();

			virtual ~DICOMAttribute();

			/// Assigna/Obtenir el Tag associat a l'atribut
			void SetTag(const DICOMTag &tag);
			DICOMTag* getTag();

			/// M¨¨tode abstracte per saber si l'atribut ¨¦s de tipus valor
			virtual bool IsValueAttribute() = 0;

			/// M¨¨tode abstracte per saber si l'atribut ¨¦s de tipus seq¨¹¨¨ncia
			virtual bool IsSequenceAttribute() = 0;

			/// M¨¨tode abstracte que retorna el contingut de l'atribut formatat
			virtual string ToString(bool verbose = false) = 0;

		private:
			/// Guarda el Tag associat a l'atribut
			DICOMTag *m_tag;

		};

	}
}