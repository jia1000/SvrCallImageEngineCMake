/*=========================================================================

  Program:   ImagingEngine
  Module:    dicom_writer.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once
#include<iostream>
#include <string.h>
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dctk.h>
#include "dcmtk/dcmimgle/dcmimage.h"
#include <dcmtk/dcmdata/dcrledrg.h>
#include <dcmtk/dcmjpeg/djdecode.h>  /* for dcmjpeg decoders */
#include <dcmtk/dcmdata/dcpxitem.h>
#include <dcmtk/dcmdata/dcdict.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmpstat/dvpstat.h>
#include <dcmtk/dcmpstat/dvpsgr.h>
#include <dcmtk/ofstd/ofstring.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <api/dicom/dcmdictionary.h>

using namespace std;

namespace GIL {
	namespace DICOM {

		class IDICOMWriter
		{
		public:
			/// Sets output path
			virtual void SetPath(std::string path) = 0;
			/// Gets output path
			virtual std::string GetPath() = 0;
			/// Adds a dicom item
			virtual void AddItem(DcmElement *attribute) = 0;
			/// Adds a empty dicom item
			virtual void AddEmptyItem(const DcmTag &tag) = 0;
			/// Adds a dicom sequence
			virtual bool AddSequenceItem(const DcmTag &tag, DcmItem *attribute) = 0;
			/// Writes to file
			virtual bool write() = 0;
		
		};

	}
}