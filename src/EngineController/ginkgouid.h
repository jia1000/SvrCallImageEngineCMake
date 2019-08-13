/*
 *  
 *  $Id: ginkgouid.h $
 *  DeepWise Dicom Viewer
 *
 *  Copyright 2017-2030 DeepWise All rights reserved.
 *  http://www.deepwise.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once
#include <string>
#include <api/api.h>

namespace GIL {
	namespace DICOM {

		typedef enum GinkgoUIDType {
			GUID_StudyRoot,
			GUID_SeriesRoot,
			GUID_InstanceRoot,
			GUID_PrivateGenericFileSOPClass
		} GinkgoUIDType;

		/***
		* Generates a unique UID on given root. If counter != -1 adds a level with those counter value
		**/
		std::string EXTAPI MakeUID(GinkgoUIDType uidType, int counter = -1);

		// overwrite from dcuid.cc
		char* dcmGenerateUniqueIdentifierSelf(char* uid, const char* prefix);
		unsigned int initCounterOfCurrentUID();
		void addUIDComponent(char* uid, const char* s);
		unsigned long forcePositive(long i);
		char* stripTrailing(char* s, char c);
	}
}