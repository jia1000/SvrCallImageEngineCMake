/*=========================================================================

  Program:   ImagingEngine
  Module:    БъЭЗ.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include <iostream>
#include <string.h>
#include "get_set.h"
#include "api/dicom/dicomdataset.h"
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

namespace GIL {
	namespace DICOM {

		class IODModule
		{
		public:
			virtual void Write(DcmItem *dataset) = 0;
			virtual void Read(DicomDataset *dataset) = 0;

			virtual DcmTag ToDcmTag(const string &tag_string)
			{
				DcmTag tag(0x0008, 0x0001);
				return tag;
			}

			virtual DcmTag ToDcmTag(const DcmTagKey &tag_key)
			{
				DcmTag tag(tag_key);
				return tag;
			}

		private:

		};
		
	}
}