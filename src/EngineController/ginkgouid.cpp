/*
 *  
 *  $Id: ginkgouid.cpp $
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
#include "ginkgouid.h"
#include <sstream>
// #include <api/ientorno.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/ofstd/ofstd.h>

#ifdef HAVE_WINDOWS_H
#else
#include "sys/time.h"
#endif

#include "tools/logger.h"

static const unsigned int maxUIDLen = 64;    /* A UID may be 64 chars or less */

// MetaEmotion.Healthcate.GinkgoCADx.MayorVersion.MinorVersion

#define GINKGO_UID_ROOT                           "1.6.89.8.4.0.17031.1.1"

#define GINKGO_STUDY_UID_ROOT                     ".1.2"
#define GINKGO_SERIES_UID_ROOT                    ".1.3"
#define GINKGO_INSTANCE_UID_ROOT                  ".1.4"
#define GINKGO_UID_PRIVATE_GENERIC_FILE_SOP_CLASS ".1.0.1"


std::string GIL::DICOM::MakeUID(GinkgoUIDType uidType, int counter) {
	char newUID[128] = {0};
	std::ostringstream  buf;
	// buf << GINKGO_UID_ROOT << "." << GNC::GCS::IEntorno::Instance()->GetGinkgoMayorVersionNumber() << "." << GNC::GCS::IEntorno::Instance()->GetGinkgoMinorVersionNumber();
	// buf << GINKGO_UID_ROOT << "." << "1" << "." << "0";
	buf << GINKGO_UID_ROOT ;

	switch(uidType) {
		case GUID_StudyRoot:
			buf << GINKGO_STUDY_UID_ROOT;
			break;
		case GUID_SeriesRoot:
			buf << GINKGO_SERIES_UID_ROOT;
			break;
		case GUID_InstanceRoot:
			buf << GINKGO_INSTANCE_UID_ROOT;
			break;
		case GUID_PrivateGenericFileSOPClass:
			buf << GINKGO_UID_PRIVATE_GENERIC_FILE_SOP_CLASS;
			break;
		default:
			break;
	}

	if (counter >= 0 ) {
		buf << ".";
		buf << counter;
	}
	// add a random number

	// struct timeval tv;
	// gettimeofday(&tv, NULL);
	// // buf << ".";
	// // buf << tv.tv_sec;
	// buf << ".";
	// buf.fill('0');
	// buf.width(4);
	// buf << tv.tv_usec / 1000;

	dcmGenerateUniqueIdentifierSelf(newUID, buf.str().c_str());
	return std::string(newUID);
}

char* GIL::DICOM::dcmGenerateUniqueIdentifierSelf(char* uid, const char* prefix)
{
    char buf[128]; /* be very safe */

    uid[0] = '\0'; /* initialize */

#ifdef WIN32
    unsigned long hostIdentifier = 0;//jia
#else
    /* On 64-bit Linux, the "32-bit identifier" returned by gethostid() is
		sign-extended to a 64-bit long, so we need to blank the upper 32 bits */
	unsigned long hostIdentifier = OFstatic_cast(unsigned long, gethostid() & 0xffffffff);
#endif    
	unsigned int counterOfCurrentUID = initCounterOfCurrentUID();

    unsigned int counter = counterOfCurrentUID++;

    if (prefix != NULL ) {
        addUIDComponent(uid, prefix);
    } else {
        addUIDComponent(uid, SITE_INSTANCE_UID_ROOT);
    }

    sprintf(buf, ".%lu", hostIdentifier);
    addUIDComponent(uid, buf);

	// too long ,not use processid
    // sprintf(buf, ".%lu", forcePositive(OFStandard::getProcessID()));
    // addUIDComponent(uid, buf);

    sprintf(buf, ".%lu", forcePositive(OFstatic_cast(long, time(NULL))));
    addUIDComponent(uid, buf);

    sprintf(buf, ".%u", counter);

    addUIDComponent(uid, buf);

    return uid;
}

unsigned int GIL::DICOM::initCounterOfCurrentUID()
{
	unsigned int counterOfCurrentUID = 0;
    /* Code taken from oftime.cc */
#ifdef HAVE_WINDOWS_H
    /* Windows: no microseconds available, use milliseconds instead */
    SYSTEMTIME timebuf;
    GetSystemTime(&timebuf);
    counterOfCurrentUID = timebuf.wMilliseconds; /* This is in the range 0 - 999 */
#else /* Unix */
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == 0)
        counterOfCurrentUID = OFstatic_cast(Uint32, tv.tv_usec); /* This is in the range 0 - 999999 */
#endif
    /* Do not ever use "0" for the counter */
    counterOfCurrentUID++;
	return  counterOfCurrentUID;
}

void GIL::DICOM::addUIDComponent(char* uid, const char* s)
{
    /* copy into UID as much of the contents of s as possible */
    if (OFStandard::strlcat(uid, s, maxUIDLen + 1) >= maxUIDLen + 1) // maxUIDLen+1 because strlcat() wants the size of the buffer, not the permitted number of characters.
    {
        CGLogger::Error("Truncated UID in dcmGenerateUniqueIdentifier(), SITE_UID_ROOT too long?");
    }
    stripTrailing(uid, '.');
}

unsigned long GIL::DICOM::forcePositive(long i)
{
    return (i < 0) ? OFstatic_cast(unsigned long, -i) : OFstatic_cast(unsigned long, i);
}

char* GIL::DICOM::stripTrailing(char* s, char c)
{
    if (s == NULL) return s;
    for
    (
        char* it = s + strlen(s) - 1;
        it >= s && *it == c;
        *it-- = '\0'
    );
    return s;
}