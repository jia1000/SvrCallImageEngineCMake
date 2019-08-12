﻿/*
 *
 *  $Id: dicommanager.cpp $
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
//#define _GINKGO_TRACE

//#define USE_V364_MY_DCMTK  // 187服务器和本地的DCMTK的版本不一致，导致它们的接口不一样。


//para la comprobacion de si esta correcto el fichero
#include <sstream>
#include <queue>
#include <stack>
#include <cstdio>
#include <fstream>

//#include <wx/file.h>
//#include <wx/strconv.h>
//#include <wx/filename.h>
#include <api/globals.h>
//#include <api/internationalization/internationalization.h>
#include "dicommanager.h"
//#include <api/ientorno.h>
#include <api/dicom/idicom.h>
//#include <api/controllers/icontroladorlog.h>
//#include <main/controllers/dcmtk/ginkgouid.h>
//#include <api/math/geometry.h>

#ifdef verify
#define MACRO_QUE_ESTORBA verify
#undef verify
#endif
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
#include <dcmtk/dcmjpeg/djdecode.h>
#include <dcmtk/dcmjpeg/djencode.h>
#include <dcmtk/dcmjpls/djdecode.h>		//for JPEG-LS decode
#include <dcmtk/dcmjpls/djencode.h>		//for JPEG-LS encode
#include <dcmtk/dcmdata/dcrledrg.h>
#include <dcmtk/dcmdata/dcrleerg.h>
#include <api/dicom/dcmdictionary.h>
//#include <main/utils/UtilString.h>
//#include <main/utils/UtilNumber.h>
//#include <main/entorno.h>
//#include "core_global_function.h"

#include "tools/logger.h"

#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif



#define AI_SERIES_NUMBER 10000
#define AI_SERIES_INSTANCE_PRIFIX "AI"
#define AI_SERIES_DESC "DW Key Series"

#define PRIVATE_CREATOR_NAME "Deep Wise" 
#define PRV_PrivateCreator   DcmTag(DW_PRIVATE_CREATOR_TAG, EVR_UT)
#define PRV_PrivateAIResult  DcmTag(DW_TAG_AI_RESULT_NUM, EVR_UT)
#define PRV_PrivateOldSeries DcmTag(DW_TAG_OLD_SERIES_NUM, EVR_UT)

namespace GIL
{
	namespace DICOM
	{

		// Constructor
		DICOMManager::DICOMManager() {
			m_pDCMSourceFile = NULL;
			m_pDCMSourceDataset = NULL;
			//m_pConv = NULL;

			// 3种Dicom格式，都要编解码注册
			DJEncoderRegistration::registerCodecs(
				ECC_lossyYCbCr,
				EUC_default, // UID generation (never create new UID's)
				OFFalse, // verbose
				0, 0, 0, true, ESS_444, true); // optimize huffman table
			DJDecoderRegistration::registerCodecs();

			DJLSEncoderRegistration::registerCodecs();		//JPEG-LS encoder registerCodecs
			DJLSDecoderRegistration::registerCodecs();		//JPEG-LS decoder registerCodecs

			// DcmRLEEncoderRegistration::registerCodecs();
			// DcmRLEDecoderRegistration::registerCodecs();
		}

		DICOMManager::DICOMManager(DcmDataset* dataset, const std::string& defaultCharset) {
			m_pDCMSourceFile = NULL;			
			m_pDCMSourceDataset = dataset;
			//m_pConv = NULL;
			FindCharset(defaultCharset);

			// 3种Dicom格式，都要编解码注册
			DJEncoderRegistration::registerCodecs(
				ECC_lossyYCbCr,
				EUC_default, // UID generation (never create new UID's)
				OFFalse, // verbose
				0, 0, 0, true, ESS_444, true); // optimize huffman table
			DJDecoderRegistration::registerCodecs();

			DJLSEncoderRegistration::registerCodecs();		//JPEG-LS encoder registerCodecs
			DJLSDecoderRegistration::registerCodecs();		//JPEG-LS decoder registerCodecs

			// DcmRLEEncoderRegistration::registerCodecs();
			// DcmRLEDecoderRegistration::registerCodecs();
		}

		// Destructor
		DICOMManager::~DICOMManager() {
			if (m_pDCMSourceFile != NULL) {
				delete m_pDCMSourceFile;
				m_pDCMSourceFile = NULL;
			}
			//if (m_pConv != NULL) {
			//	delete m_pConv;
			//	m_pConv = NULL;
			//}

			// 3种Dicom格式，都要编解码解注册
			DJEncoderRegistration::cleanup();
			DJDecoderRegistration::cleanup();

			DJLSEncoderRegistration::cleanup();		//JPEG-LS encoder cleanup
			DJLSDecoderRegistration::cleanup();		//JPEG-LS decoder cleanup

			// DcmRLEEncoderRegistration::cleanup();
			// DcmRLEDecoderRegistration::cleanup();
		}

		DcmDataset* DICOMManager::getSourceDataSet() {
			if (m_pDCMSourceFile != NULL) {
				return m_pDCMSourceFile->getDataset();
			}
			else {
				return m_pDCMSourceDataset;
			}
		}

//region "Interfaz"

		/*Almacena la jerarquia cargada en un fichero*/
		bool DICOMManager::AlmacenarFichero(const std::string& inputFile, IInspectCallBack* , bool )
		{
			if (m_pDCMSourceFile == NULL) {
				return false;
			}
			FindCharset();
			OFCondition cond;
			cond = m_pDCMSourceFile->saveFile(inputFile.c_str());
			if(cond.bad()) {
				CGLogger::Error("save dicom file has error\n");
			}

			//std::string tempDir = GNC::GCS::IEntorno::Instance()->CreateGinkgoTempDir();
			//std::string tempFile;
			//{
			//	std::ostringstream ostr;
			//	ostr << tempDir;
			//	char c;
			//	if (wxFileName::GetPathSeparator().GetAsChar(&c)) {
			//		ostr << c;
			//	}
			//	ostr << "1.dcm";
			//	tempFile = ostr.str();
			//}

			//cond = m_pDCMSourceFile->saveFile(tempFile.c_str());
			//if (cond.bad()) {
			//	std::cerr << "Error: " << cond.text() << std::endl;
			//	wxRemoveFile(FROMPATH(tempFile));
			//	//return false;
			//} else {
			//	wxRenameFile(FROMPATH(tempFile), FROMPATH(inputFile), true);
			//	m_pDCMSourceFile->loadFile(inputFile.c_str(), EXS_Unknown, EGL_noChange, DCM_TagInfoLength);
			//}
   //         DeleteTempDir(tempDir);
			
            return !cond.bad();
		}

		/*Actualiza la jerarquia cargada*/
		int DICOMManager::ActualizarJerarquia(DicomDataset& base)
		{
			if (getSourceDataSet() == NULL) {
				return 0;
			}

			return InsertarJerarquia(base, NULL, NULL);
		}

		/*anonimiza los tags privados*/
		void DICOMManager::AnonimizarTagsPrivados()
		{
			OFCondition cond;
			DcmElement* element;
			unsigned int g=DW_PRIVATE_TAG_GROUP;
			unsigned int e=0x0010;

			DcmDataset* ds = getSourceDataSet();
			if (ds == NULL) {
				return;
			}

			for(;e<=0xFFFF;e++){
				DcmTagKey key(g,e);
				cond = ds->findAndGetElement(key,element,false);
				if(element!=NULL){
					ds->remove(key);
				}
			}
		}

		/*actualiza los tags privados*/
		int DICOMManager::UpdatePrivateTags(TipoPrivateTags& tags){
			if (getSourceDataSet() == NULL) {
				return 0;
			}

			return InsertPrivateTags(tags);
		}

		int DICOMManager::LoadPrivateTags(GIL::DICOM::TipoPrivateTags& tags) {
			if (getSourceDataSet() == NULL) {
				return 0;
			}

			return PrivateCargarTagsPrivados(tags);
		}

        std::string getDestFileFullPath(std::string oldFileFullPath, std::string destFolder, std::string suffix) {
            //Get file name
            int start = oldFileFullPath.find_last_of('/');
            if (start < 0) {
                start = oldFileFullPath.find_last_of('\\');
            }
            int end = oldFileFullPath.find_last_of('.');

            std::string fileName = oldFileFullPath.substr(start+1, end-start-1);
			
            // get dest file path
            char lastChar = destFolder[destFolder.length()-1];
            if (lastChar == '/' || lastChar == '\\') {
                std::stringstream ss;
                ss << destFolder << fileName << suffix;
                return ss.str();
            }
            else {
                std::stringstream ss;
                ss << destFolder << "/" << fileName << suffix;
                return ss.str();
            }
        }
        void registerPrivateTags() {
            DcmDataDictionary &dict = dcmDataDict.wrlock();
            dict.addEntry(new DcmDictEntry(DW_TAG_AI_RESULT_NUM, EVR_UT, "AI Result",    1, 1, "private", OFTrue, PRIVATE_CREATOR_NAME));
            dict.addEntry(new DcmDictEntry(DW_TAG_OLD_SERIES_NUM, EVR_UT, "Old Series",    1, 1, "private", OFTrue, PRIVATE_CREATOR_NAME));
#ifdef USE_V364_MY_DCMTK
            dcmDataDict.wrunlock();
#else
	dcmDataDict.unlock();
#endif
        }

        void addPrivateElements(DcmItem &item, std::string aiResult, std::string oldSeries) {
            if (!item.tagExists(PRV_PrivateCreator)){
                item.putAndInsertString(PRV_PrivateCreator, PRIVATE_CREATOR_NAME);
                item.putAndInsertString(PRV_PrivateAIResult, aiResult.c_str());
                item.putAndInsertString(PRV_PrivateOldSeries, oldSeries.c_str());
            }
        }

        //std::string getRandomSeriesNum() {
        //    int randNum = DW::Util::UtilNumber::RandInt(10000, 19999);
        //    return DW::Util::UtilString::IntToString(randNum);
        //}

		//int DICOMManager::GenerateKeySeriesAndGSPS(std::vector<std::string>& inputFileList, DW::MODEL::AIResult* aiResult, const std::string& aiInfor, std::string destFolder, boolean isGenerateKeySeries, boolean isGenerateGSPS, std::list<string>& listOfFileToUpload, std::string& seriesUID){
			//int totalFileNum = 0;
   //         std::string aiResultStr = "";
   //         std::string keySeriesNewUID = "";
   //         std::string keySeriesNumber = "";

			//if (inputFileList.size()>0 && aiResult && (isGenerateGSPS || isGenerateKeySeries)) {
			//	//1. get map. key:sliceNumber; value:AIResultNoduleSlice
			//	std::map<int, std::vector<DW::MODEL::AIResultNoduleSlice *>> sliceMap;
			//	aiResult->getMapOfSliceIndexAndNoduleSlice(sliceMap, false, true);

   //             //2. create private tag to store ai result for key series
   //             if (isGenerateKeySeries) {
   //                 aiResultStr = aiInfor;//aiResult->ModelToJson();//DW::Util::UtilString::UrlEncode(aiResult->ModelToJson();
   //                 seriesUID = keySeriesNewUID = GIL::DICOM::MakeUID(GIL::DICOM::GUID_SeriesRoot);
   //                 keySeriesNumber = getRandomSeriesNum();
   //             }
   //             if (destFolder.length() == 0) {
   //                 destFolder = GNC::Entorno::Instance()->CreateGinkgoTempDir();
   //             }

			//	//3. loop all the files
			//	int size = inputFileList.size();
   //             registerPrivateTags();
			//	for (int i=0; i<size; i++) {
			//		std::string filePath = inputFileList[i];

			//		DcmFileFormat imageFile;
			//		if (imageFile.loadFile(filePath.c_str()).bad()) {
   //                     continue;
			//		}
   //                 DcmDataset *  imageDataSet = imageFile.getDataset();

   //                 //3. get image instance id
   //                 int instanceId = -1;
   //                 DcmElement* tagElement = NULL;
   //                 DcmTagKey key = DcmTagKey(0x0020,0x0013); //0020, 0013
   //                 imageFile.getDataset()->findAndGetElement(key,tagElement);
   //                 if(tagElement != NULL){
   //                     char * value;
   //                     tagElement->getString(value);
   //                     instanceId = atoi((value));
   //                 }

			//		//4. get pixels from AI Result
   //                 std::vector<DW::MODEL::AIResultNoduleSlice *> * slicesAndPixes = NULL; 
   //                 if (instanceId >= 0) {
			//			std::map<int, std::vector<DW::MODEL::AIResultNoduleSlice *>>::iterator itr = sliceMap.find(instanceId);
			//			if (itr != sliceMap.end()) {
			//				slicesAndPixes = &(itr->second);
			//			}
   //                 }

   //                 //5. save the key series
   //                 if (slicesAndPixes != NULL) {
   //                     //--------------generate key series
   //                     if (isGenerateKeySeries) {
   //                         string oldSeriesInstanceId = "";
   //                         
   //                         //5.1. Check patient name, because for DCM4CHEE opensource pacs, if the patient name is empty, the upload will be refused.
   //                         tagElement = NULL;
   //                         key = DcmTagKey(0x0010, 0x0010); //Patient Name
   //                         imageDataSet->findAndGetElement(key, tagElement);
   //                         if (tagElement != NULL) {
   //                             char * value;
   //                             tagElement->getString(value);
   //                             string patientName = std::string(value);
   //                             if (patientName == "") {
   //                                 tagElement->putString("Anonymized");
   //                             }
   //                         }
   //                         //5.2. Series Number
   //                         tagElement = NULL;
   //                         key = DcmTagKey(0x0020, 0x0011); //Series Number: 0020, 0011
   //                         imageDataSet->findAndGetElement(key, tagElement);
   //                         if (tagElement != NULL) {
   //                             tagElement->putString(keySeriesNumber.c_str());
   //                         }

   //                         //5.3. Series Instance
   //                         key = DcmTagKey(0x0020,0x000e); //Series Instance UID: 0020, 000e
   //                         imageDataSet->findAndGetElement(key, tagElement);
   //                         if (tagElement != NULL) {
   //                             char * value;
   //                             tagElement->getString(value);
   //                             oldSeriesInstanceId = std::string(value);

   //                             string newSeriesInstanceId = keySeriesNewUID;
   //                             tagElement->putString(newSeriesInstanceId.c_str());
   //                         }

   //                         //5.4. Series Description
   //                         key = DcmTagKey(0x0008, 0x103E); //Series Description: 0008, 103E
   //                         imageDataSet->findAndGetElement(key, tagElement);
   //                         if (tagElement != NULL) {
   //                             tagElement->putString(AI_SERIES_DESC);
   //                         }

   //                         //5.5. SOP instance id
   //                         key = DcmTagKey(0x0008, 0x0018); //SOP instance id: 0008, 103E
   //                         imageDataSet->findAndGetElement(key, tagElement);
   //                         if (tagElement != NULL) {
   //                             string newSOPUID = GIL::DICOM::MakeUID(GIL::DICOM::GUID_InstanceRoot).c_str();
   //                             tagElement->putString(newSOPUID.c_str());
   //                         }

   //                         //5.6. save AI result into private tag
   //                         addPrivateElements(*imageFile.getDataset(), aiResultStr, oldSeriesInstanceId);
   //                         //cout << "aiResult length :" << aiResultStr.length() << endl;
   //                         //GIL::DICOM::TagPrivadoString * aiTag = new TagPrivadoString(aiResultStr);
   //                         //DcmElement *aiElement = aiTag->ToElement(DW_PRIVATE_TAG_GROUP, DW_TAG_AI_RESULT_Element);
   //                         //aiElement->setVR(EVR_UT);

   //                         //OFCondition cond = imageDataSet->insert(aiElement, true, false);
   //                         //if (cond.bad()) {
   //                         //    std::cerr << "error occurs when insert private AI Result" <<std::endl;
   //                         //    return 0;
   //                         //}

   //                         ////5.7. Save old Series UID, so that when we import the key series, the AI result can be shown on the original series as well.
   //                         //GIL::DICOM::TagPrivadoString * oldSeriesUIDTag = new TagPrivadoString(oldSeriesInstanceId);
   //                         //DcmElement * oldSeriesElement = oldSeriesUIDTag->ToElement(DW_PRIVATE_TAG_GROUP, DW_TAG_OLD_SERIES_ELEMENT);
   //                         //cond = imageDataSet->insert(oldSeriesElement, true, false);
   //                         //if (cond.bad()) {
   //                         //    std::cerr << "error occurs when insert private old series id" <<std::endl;
   //                         //    return 0;
   //                         //}

   //                         // 5.8 Save key series
   //                         std::string destSeriesFilePath = getDestFileFullPath(filePath, destFolder, "_key.dcm");
   //                         E_TransferSyntax xfer = imageFile.getDataset()->getOriginalXfer();
   //                         if (imageFile.saveFile(destSeriesFilePath.c_str(), xfer, EET_ExplicitLength,EGL_recalcGL,EPD_noChange,0,2,EWM_updateMeta).bad()) {
   //                             CERR << "Error: cannot save file '" << destSeriesFilePath << "'" << OFendl;
   //                             return -1;
   //                         }
   //                         else {
   //                             listOfFileToUpload.push_back(destSeriesFilePath);
   //                         }
   //                     }

   //                     //---------generate gsps
   //                     if (isGenerateGSPS) { 
   //                         DVPresentationState pstate;

   //                         if (pstate.createFromImage(*imageDataSet).bad())
   //                             CERR << "Error: cannot create GSPS object from first image" << OFendl;
   //                         if (pstate.attachImage(imageDataSet, OFFalse).bad())
   //                             CERR << "Error: cannot attach first image to GSPS object" << OFendl;

   //                         int nodules = slicesAndPixes->size();

   //                         for (int i=0; i<nodules; i++) {
   //                             DW::MODEL::AIResultNoduleSlice * pixels = (*slicesAndPixes)[i];

   //                             std::stringstream ss;
   //                             ss << "AIResult" << i;
   //                             std::string layerName = ss.str();

   //                             if (pstate.addGraphicLayer(layerName.c_str()).good()) {
   //                                 DVPSGraphicObject *graphic = pstate.addGraphicObject(pstate.getGraphicLayerIndex(layerName.c_str()));
   //                                 if (graphic != NULL){
   //                                     int pixelNum = pixels->mListOfPoints.size()+1;
   //                                     Float32 * data = new Float32[pixelNum * 2];
   //                                     int index = 0;
   //                                     std::list<GNC::GCS::Vector>::iterator pixelItr;
   //                                     for (pixelItr = pixels->mListOfPoints.begin(); pixelItr != pixels->mListOfPoints.end(); pixelItr++) {
   //                                         data[index++] = (*pixelItr).x;
   //                                         data[index++] = (*pixelItr).y;
   //                                     }
   //                                     data[index++] = data[0];
   //                                     data[index++] = data[1];

   //                                     graphic->setGraphicType(DVPST_interpolated);
   //                                     graphic->setData(pixelNum, data, DVPSA_pixels);
   //                                 } 
   //                             }
   //                         }

   //                         //write GSPS
   //                         DcmFileFormat gspsFile;
   //                         /* save GSPS object first to a DICOM data set ... */
   //                         if (pstate.write(*gspsFile.getDataset(), OFTrue).good()){
   //                             string destGSPSFilePath = getDestFileFullPath(filePath, destFolder, ".gsps");
   //                             if (gspsFile.saveFile(destGSPSFilePath.c_str(), EXS_LittleEndianExplicit).bad()) {
   //                                 CERR << "Error: cannot save file '" << destGSPSFilePath << "'" << OFendl;
   //                                 return -1;
   //                             }
   //                             else {
   //                                 listOfFileToUpload.push_back(destGSPSFilePath);
   //                             }
   //                             totalFileNum++;
   //                         }
   //                     }
   //                 }
			//	}
			//}

			//return totalFileNum;
		//}

		/*carga la MetaInfo*/
		int DICOMManager::CargarMetaInfo(const std::string& inputFile, GIL::DICOM::TipoMetaInfo& tags) {

			//comprobamos que es dicom(numero magico, si no el load file a veces casca estrepitosamente)
			if(!EsDicom(inputFile))
			{
				return 0;
			}

			OFCondition cond;
			DcmMetaInfo metaInfo;

			cond = metaInfo.loadFile(inputFile.c_str(), EXS_Unknown, EGL_noChange);
			//metaInfo.loadAllDataIntoMemory();
			//metaInfo.print(std::cout);

			int numInsertados = 0;

			unsigned long elemCount = metaInfo.card();

			for (unsigned long i = 0; i < elemCount; i++) {
				DcmElement *e = metaInfo.getElement(i);
				GTRACE("Procesando objeto MetaInfo: " << e->getTag().toString())
				std::ostringstream os;
				os << std::hex << std::setw(4) << std::setfill('0') << e->getGTag() << "|" << std::hex << std::setw(4) << std::setfill('0') << e->getETag();
				std::string tag = os.str();

				OFString v;
				std::string ov;

				cond = e->getOFStringArray(v, OFTrue);
				if (cond.good()) {
					ov = v.c_str();
				}

				tags.tags[tag] = ov;

				numInsertados++;
			}


			if (cond.bad()) {
				// TODO: Lanzar excepción
				return 0;
			}

			return numInsertados;
		}

		/* Obtains the hierarchy of DICOM file Fields */
		bool DICOMManager::CargarFichero(const std::string& inputFile, GIL::DICOM::DicomDataset& jerarquia, bool cargarSoloTagsInfo, IInspectCallBack* /*pICallback*/)
		{
			//comprobamos que es dicom(numero magico, si no el load file a veces casca estrepitosamente)
			if(!EsDicom(inputFile))
			{
				CGLogger::Info("file is not dicom : " + inputFile);
				return false;
			}
			//

			if (m_pDCMSourceFile != NULL) {
				delete m_pDCMSourceFile;
				m_pDCMSourceFile = NULL;
			}
			//if (m_pConv != NULL) {
			//	delete m_pConv;
			//	m_pConv = NULL;
			//}

			m_pDCMSourceFile = new DcmFileFormat();
			OFCondition cond;

			if (true ||cargarSoloTagsInfo) { // FIXME Review
				cond = m_pDCMSourceFile->loadFile(inputFile.c_str(), EXS_Unknown, EGL_noChange, DCM_TagInfoLength);
			}
			else {
				//cond = dcmff.loadFile(inputFile.c_str(), EXS_Unknown, EGL_noChange, DCM_MaxReadLength);
			}
			//dcmff.loadAllDataIntoMemory();

			if (cond.bad()) {
				// TODO: Lanzar excepción
				CGLogger::Error("load dicom status  is bad.");
				return false;
			}

			FindCharset();

			if(cargarSoloTagsInfo) {
				int ret = CargarJerarquia(jerarquia, DCM_OptPrintLineLength);
                if (ret == 0)
                {
					CGLogger::Error("CargarJerarquia error 1");
                    return false;
                }
			} else {
				int ret = CargarJerarquia(jerarquia, 0);
                if (ret == 0)
                {
					CGLogger::Error("CargarJerarquia error 2");
                    return false;
                }
			}
			return true;
		}


		/******************/
		//This is to load the file without any Memory, only what we are interested in gettag 
		bool DICOMManager::CargarFichero(const std::string& inputFile, bool /*cargarSoloTagsInfo*/)
		{
			//comprobamos que es dicom(numero magico, si no el load file a veces casca estrepitosamente)
			if(!EsDicom(inputFile))
			{
				return false;
			}
			//

			if (m_pDCMSourceFile != NULL) {
				delete m_pDCMSourceFile;
				m_pDCMSourceFile = NULL;
			}
			//if (m_pConv != NULL) {
			//	delete m_pConv;
			//	m_pConv = NULL;
			//}

			m_pDCMSourceFile = new DcmFileFormat();
			OFCondition cond;

			//if (true || cargarSoloTagsInfo) {
				cond = m_pDCMSourceFile->loadFile(inputFile.c_str(), EXS_Unknown, EGL_noChange, DCM_TagInfoLength);
			//}
			//else {
			//	cond = dcmff.loadFile(inputFile.c_str());

			//}

			if (cond.bad()) {
				// TODO: Lanzar excepción
				return false;
			}

			//cond = dcmff.loadAllDataIntoMemory();

			if (cond.bad()) {
				// TODO: Lanzar excepción
				return false;
			}

			FindCharset();

			return true;
		}

		bool DICOMManager::GetTag(unsigned int grupo,unsigned int elemento, std::string & valor)
		{
			DcmDataset* ds = getSourceDataSet();
			if (ds == NULL) {
				return false;
			}

			FindCharset();

			DcmTagKey key(grupo,elemento);
			DcmElement* pElement;
			ds->findAndGetElement(key,pElement);
			if(pElement != NULL)
			{
			    if(pElement->containsUnknownVR()){
			     return false;
			    } else {
                 OFString v;
                 OFCondition cond = pElement->getOFStringArray(v, OFTrue);
                 if (cond.good()) {
					 //valor = wxString(v.c_str(), *m_pConv).ToUTF8();
					 valor = v.c_str();
                     return true;
                 } else {
                     return false;
                 }
			    }
			} else {
				return false;
			}
		}

		bool DICOMManager::GetTag(unsigned int grupo,unsigned int elemento, TagPrivadoUndefined& tagBinario)
		{
			DcmDataset* ds = getSourceDataSet();
			if (ds == NULL) {
				return false;
			}

			if(grupo == 0x7fe0 && elemento == 0x0010) {
				//es necesario para pillar el pixeldata en explicit little endian
				OFCondition cond = ds->chooseRepresentation(EXS_DeflatedLittleEndianExplicit,NULL);
				if(cond.bad()) {
					return false;
				}
			}

			FindCharset();

			DcmTagKey key(grupo,elemento);
			DcmElement* pElement;
			ds->findAndGetElement(key,pElement);
			if(pElement != NULL)
			{
				OFCondition cond = pElement->loadAllDataIntoMemory();
				if(cond.good()) {
					Uint8 * ptr;
					cond = pElement->getUint8Array(ptr);
					if(cond.good()) {
						tagBinario.Copiar(ptr,pElement->getLength());
						return true;
					} else {
						return false;
					}
				} else {
					return false;
				}
			} else {
				return false;
			}
		}

		bool DICOMManager::GetTag(unsigned int grupo,unsigned int elemento, float& valor)
		{
			std::string valorstr;
			if(GetTag(grupo,elemento,valorstr))
			{
				valor = 0.0f;
				std::sscanf(valorstr.c_str(),"%f",&valor);
				return true;
			} else {
			    //se intenta interpretar como US
			    DcmDataset* ds = getSourceDataSet();
				if (ds == NULL) {
					return false;
				}
                DcmTagKey key(grupo,elemento);
                DcmElement* pElement;
                ds->findAndGetElement(key,pElement);
                if(pElement != NULL)
                {
                    Uint8 * ptr;
                    OFCondition cond = pElement->getUint8Array(ptr);
                    if(cond.good()) {
                        switch (pElement->getLength()) {
                            case 4:
                                valor = *((Float32*)ptr);
                                return true;
                            break;
                            case 8:
                                valor = *((Float64*)ptr);
                                return true;
                            break;
                            default:
                                return false;
                        }
                    } else {
                        return false;
                    }
                }
				return false;
            }
		}

		bool DICOMManager::GetTag(unsigned int grupo,unsigned int elemento, int& valor)
		{
			std::string valorstr;
			if(GetTag(grupo,elemento,valorstr))
			{
				valor = atoi(valorstr.c_str());
				return true;
			} else {
			    //se intenta interpretar como US
			    DcmDataset* ds = getSourceDataSet();
				if (ds == NULL) {
					return false;
				}
                DcmTagKey key(grupo,elemento);
                DcmElement* pElement;
                ds->findAndGetElement(key,pElement);
                if(pElement != NULL)
                {
                    Uint8 * ptr;
                    OFCondition cond = pElement->getUint8Array(ptr);
                    if(cond.good()) {
                        switch (pElement->getLength()) {
                            case 2:
                                valor = *((Uint16*)ptr);
                                return true;
                            break;
                            case 4:
                                valor = *((Uint32*)ptr);
                                return true;
                            break;
                            default:
                                return false;
                        }
                    } else {
                        return false;
                    }
                }
				return false;
			}
		}

		bool DICOMManager::ExtractTagToFile(unsigned int grupo, unsigned int element, const std::string& outputFile
			/*, GNC::IProxyNotificadorProgreso* pNotificador*/)
		{
			DcmTagKey key(grupo,element);
			DcmElement* pElement;
			DcmDataset* pDset = getSourceDataSet();
			if (pDset == NULL) {
				return false;
			}
			pDset->findAndGetElement(key,pElement);
			if(pElement != NULL)
			{
				DcmPixelData* pPixelData = dynamic_cast<DcmPixelData*>(pElement);				
				if (pPixelData != NULL) {
				//	OFCondition cond = pElement->loadAllDataIntoMemory();
					DcmPixelSequence* pPixelSeq = NULL;
					OFCondition cond = pPixelData->getEncapsulatedRepresentation(pDset->getOriginalXfer(), NULL, pPixelSeq);
					if(cond.good()) {
						for (unsigned long i = 0; i < pPixelSeq->card(); ++i) {
							DcmPixelItem *pixelItem  = NULL;;
							cond  = pPixelSeq->getItem(pixelItem, i);
							if (cond.good()) {
								if (pixelItem->getLength(pDset->getOriginalXfer())> 0) {
									char readingBuffer[8*1024];
									int length = pixelItem->getLengthField();
									int numBytes, offset = 0;
									DcmFileCache cache;
									std::ofstream outfile (outputFile.c_str(), std::ofstream::binary);
									int numIterations = 0;
									do {
										//if (pNotificador != NULL && numIterations%100 == 0) {
										//	if (!pNotificador->NotificarProgreso((float)offset/length, _Std("Extracting file..."))) {
										//		outfile.close();
										//		wxRemoveFile(FROMPATH(outputFile));
										//		return false;
										//	}
										//}
										numBytes = ((offset + (int)sizeof(readingBuffer)) > length)? length - offset: (int)sizeof(readingBuffer);
										pixelItem->getPartialValue(readingBuffer, offset, numBytes, &cache);
										outfile.write(readingBuffer, numBytes);
										offset += numBytes;
										numIterations ++;
									} while (offset < length);
									outfile.close();
									return true;
								}
							}								
						}
					}
				} else {
					char readingBuffer[8*1024];
					int length = pElement->getLengthField();
					int numBytes, offset = 0;
					DcmFileCache cache;
					std::ofstream outfile (outputFile.c_str(), std::ios_base::binary);
					int numIterations = 0;
					do {
						//if (pNotificador != NULL && numIterations%100 == 0) {
							//if (!pNotificador->NotificarProgreso((float)offset/length, _Std("Extracting file..."))) {
							//	outfile.close();
							//	wxRemoveFile(FROMPATH(outputFile));
							//	return false;
							//}
						//}
						numBytes = ((offset + (int)sizeof(readingBuffer)) > length)? length - offset: (int)sizeof(readingBuffer);
						pElement->getPartialValue(readingBuffer, offset, numBytes, &cache);
						outfile.write(readingBuffer, numBytes);
						offset += numBytes;
						numIterations ++;
					} while (offset < length);
					outfile.close();
					return true;
				}
			}
			//LOG_ERROR("Dicommanager", "error extracting tag");
			return false;
		}

		bool DICOMManager::GetTag(unsigned int grupo,unsigned int elemento, GIL::DICOM::DicomDataset& secuencia)
		{
			DcmDataset* ds = getSourceDataSet();
			if (ds == NULL) {
				return false;
			}

			DcmTagKey key(grupo,elemento);
			DcmElement* pSequence;
			ds->findAndGetElement(key,pSequence);
			if(pSequence != NULL)
			{
				CargarJerarquia(secuencia,DCM_OptPrintLineLength,pSequence);
				return true;
			} else {
				return false;
			}
		}

		bool DICOMManager::FindTag(unsigned int grupo,unsigned int elemento, TagPrivadoUndefined& tagBinario)
		{
			DcmDataset* ds = getSourceDataSet();
			if (ds == NULL) {
				return false;
			}

			if( (grupo == 0x7fe0 && elemento == 0x0010) || (grupo == 0x5400 && elemento == 0x1010) ) {
				//es necesario para pillar el pixeldata en explicit little endian
				OFCondition cond = ds->chooseRepresentation(EXS_DeflatedLittleEndianExplicit,NULL);
				if(cond.bad()) {
					return false;
				}
			}

			DcmTagKey key(grupo,elemento);
			DcmElement* pElement;
			ds->findAndGetElement(key, pElement, OFTrue);
			if(pElement != NULL)
			{
				OFCondition cond = pElement->loadAllDataIntoMemory();
				if(cond.good()) {
					Uint8 * ptr;
					cond = pElement->getUint8Array(ptr);
					if(cond.good()) {
						tagBinario.Copiar(ptr,pElement->getLength());
						return true;
					} else {
						Uint16 * ptr2;
						cond = pElement->getUint16Array(ptr2);
						if(cond.good()) {
							tagBinario.Copiar(ptr2, pElement->getLength());
							return true;
						} else {
							return false;
						}
					}
				} else {
					return false;
				}
			} else {
				return false;
			}
		}

		/* returns the oid name or empty string if not found */
		std::string DICOMManager::GetOIDName(const std::string &oid)
		{
			const char* name = dcmFindNameOfUID(oid.c_str());
			if (name != NULL) {
				return std::string(name);
			}
			else {
				return std::string("");
			}


		}

		/* devuelve la descripcion de este tag*/
		std::string DICOMManager::GetDescription(const std::string &clave) const
		{
			unsigned int g = 0xffff;
			unsigned int e = 0xffff;
			int n;

			n = sscanf(clave.c_str(), "%x|%x", &g, &e);
			if (n  == 2) // if at least no tag could be parsed
			{
				DcmTagKey key(g,e);
				const DcmDataDictionary& globalDataDict = dcmDataDict.rdlock();
				const DcmDictEntry *dicent = globalDataDict.findEntry(key,NULL);
#ifdef USE_V364_MY_DCMTK
				dcmDataDict.rdunlock();
#else
				dcmDataDict.unlock();
#endif
				if (dicent != NULL) {
					return std::string(dicent->getTagName());
				}
			}
			return std::string("");

		};

		//std::string DICOMManager::GetNewUID()
		//{
		//	return GIL::DICOM::MakeUID(GIL::DICOM::GUID_StudyRoot);
		//}

		bool DICOMManager::EsDicom(const std::string& inputFile) {
			//comprobamos que es dicom(numero magico, si no el load file a veces casca estrepitosamente)
			bool esDicom = false;
#ifdef USE_WXWIDGETS
			wxFile fichero(FROMPATH(inputFile),wxFile::read);
			if(fichero.IsOpened()){
				char buffer[128];
				if(fichero.Read(buffer,128) == 128){
					char numeroMagico[4];
					if(fichero.Read(numeroMagico,4) == 4){
						std::string str(numeroMagico,4);
						if(str == "DICM") {
							esDicom = true;
						}
					}
				}
			}
#else 
			FILE* fp = fopen(inputFile.c_str(), "r");
			if (fp) {
				const int BUFFER_SIZE = 128;
				char buffer[BUFFER_SIZE] = {0};
				if (BUFFER_SIZE == fread(buffer, 1, BUFFER_SIZE, fp)) {
					const int MAGIC_NUM = 4;
					char numeroMagico[MAGIC_NUM] = {0};
					if (MAGIC_NUM == fread(numeroMagico, 1, MAGIC_NUM, fp)) {
						std::string str(numeroMagico, MAGIC_NUM);
						if (str == "DICM") {
							esDicom = true;
						}
					}
				}
				fclose(fp);
			}
#endif
			return esDicom;
		}

//endregion

		DcmElement* DICOMManager::CrearElementoConValor(const char* s)
		{
			unsigned int g = 0xffff;
			unsigned int e = 0xffff;
			int n = 0;
			char* val = new char[strlen(s)+1];
			OFString dicName, valStr;
			OFString msg;

			val[0] = '\0';

			// try to parse group and element number
			n = sscanf(s, "%x|%x=%s", &g, &e, val);
			OFString toParse = s;
			size_t eqPos = toParse.find('=');
			if (n < 2) // if at least no tag could be parsed
			{
				// if value is given, extract it (and extrect dictname)
				if (eqPos != OFString_npos) {
					dicName = toParse.substr(0, eqPos).c_str();
					valStr = toParse.substr(eqPos + 1, toParse.length());
				} else // no value given, just dictionary name
					dicName = s; // only dictionary name given (without value)
				// try to lookup in dictionary
				DcmTagKey key(0xffff, 0xffff);
				const DcmDataDictionary& globalDataDict = dcmDataDict.rdlock();
				const DcmDictEntry *dicent = globalDataDict.findEntry(dicName.c_str());
#ifdef USE_V364_MY_DCMTK
				dcmDataDict.rdunlock();
#else
				dcmDataDict.unlock();
#endif
				if (dicent != NULL) {
					// found dictionary name, copy group and element number
					key = dicent->getKey();
					g = key.getGroup();
					e = key.getElement();
				} else {
					// not found in dictionary
					std::cerr <<  "bad key format or dictionary name not found in dictionary: " << dicName << std::endl;
					delete[] val;
					return NULL;
				}
			}// tag could be parsed, copy value if it exists
			else {
				if (eqPos != OFString_npos) {
					valStr = toParse.substr(eqPos + 1, toParse.length());
				}
			}
			DcmTag tag(g, e);
			if (tag.error() != EC_Normal) {
				std::cerr << "Tag desconocido: (" <<
				std::hex << std::setw(4) << std::setfill('0') << g << "|" <<
				std::hex << std::setw(4) << std::setfill('0') << e << ")" << std::endl;
				delete[] val;
				return NULL;
			}
			DcmElement *elem = DcmItem::newDicomElement(tag);
			if (elem == NULL) {
				std::cerr << "No se pudo crear el elemento para el tag: (" <<
				std::hex << std::setw(4) << std::setfill('0') << g << "|" <<
				std::hex << std::setw(4) << std::setfill('0') << e << ")" << std::endl;
				delete[] val;
				return NULL;
			}
			if (valStr.length() > 0) {
				if (elem->putString(valStr.c_str()).bad()) {
					std::cerr << "No se pudo asignar el valor al elemento: (" <<
					std::hex << std::setw(4) << std::setfill('0') << g << "|" <<
					std::hex << std::setw(4) << std::setfill('0') << e << ")=" << valStr.c_str() << std::endl;
					delete elem;
					delete[] val;
					return NULL;
				}
			}
			delete[] val;
			return elem;

		}

		int DICOMManager::RemoveTags(const DicomDataset& base)
		{
			DcmDataset* ds = getSourceDataSet();
			if (ds == NULL) {
				return 0;
			}

			for (ListaTags::const_iterator it = base.tags.begin(); it != base.tags.end(); ++it) {
				std::string claveSecuencia = (*it).first;
				
				unsigned int sg = 0xffff;
				unsigned int se = 0xffff;
				int sn = 0;

				sn = sscanf(claveSecuencia.c_str(), "%x|%x", &sg, &se);
				if (sn < 2) {
					std::cerr << "Formato invalido (" << claveSecuencia.c_str() << "). Solo se soporta (FFFF|FFFF) como formato de tag para secuencias" << std::endl;
					continue;
				}
				DcmTag stag(sg, se);

				OFCondition cond;
				ds->remove(stag);				
			}
			return 1;
		}
		
		int DICOMManager::InsertarJerarquia(const DicomDataset& base, DcmItem* itemPadre, DcmSequenceOfItems* seqPadre)
		{

			int numTotalInsertados = 0;
			int numTagsInsertados  = 0;
			int numItemsInsertados = 0;
			int numSeqsInsertadas  = 0;

			DcmDataset* ds = getSourceDataSet();
			if (ds == NULL) {
				return 0;
			}

			// Insertamos todos los tags correspondientes a este nivel en la raiz (de haberla, si no: en el dataset).
			for (ListaTags::const_iterator it = base.tags.begin(); it != base.tags.end(); ++it) {
				DcmElement* e = this->CrearElementoConValor((*it).first.c_str());

				if (e != NULL) {
					e->putString( (*it).second.c_str() );
					OFCondition cond;
					if (itemPadre == NULL) {
						cond = ds->insert(e, true, false);
					}
					else {
						cond = itemPadre->insert(e, true, false);
					}
					if (cond.bad()) {
						//LOG_ERROR("DICOMMANAGER", "No se pudo insertar el elemento: (" << e->getTag().toString().c_str() << "): " << cond.text());
					}
					else {
						numTotalInsertados++;
						numTagsInsertados++;
					}
				}
			}

			// Insertamos todos los items correspondientes a este nivel en la raiz (de haberla, si no: en el dataset).

			for (DicomDataset::DatasetList::const_iterator it = base.items.begin(); it != base.items.end(); ++it) {
				DcmItem *item = new DcmItem();

				int nItems = InsertarJerarquia((*it), item, NULL);

				if (nItems > 0) {

					OFCondition cond;

					if (seqPadre == NULL) {
						//cond = i2d->getOverrideKeys()->insert(item, OFTrue);
						std::cerr << "No se pudo insertar el item directamente a la raiz. Deben insertarse en secuencias o en otros items.  " << nItems << " elementos perdidos: " << cond.text() << std::endl;
						std::cerr << "tag << " << (*it) <<std::endl;
						delete item;
					}
					else {
						cond = seqPadre->insert(item, true, false);
						GTRACE(seqPadre->getTag().toString().c_str() << " << " << item->getTag().toString())
					}

					if (cond.bad()) {
						std::cerr << "No se pudo insertar el item a la raiz.  " << nItems << " elementos perdidos: " << cond.text() << std::endl;
						delete item;
					}
					else {
						numTotalInsertados += nItems + 1;
						numItemsInsertados++;
					}
				}
				else {
					delete item;
				}
			}

			// Insertamos todas las secuencias correspondientes a este nivel en la raiz (de haberla, si no: en el dataset).
			for (DicomDataset::DatasetList::const_iterator it = base.secuencias.begin(); it != base.secuencias.end(); ++it) {

				std::string claveSecuencia = (*it).tagName;
				const DicomDataset& nbase = (*it);

				unsigned int sg = 0xffff;
				unsigned int se = 0xffff;
				int sn = 0;

				sn = sscanf(claveSecuencia.c_str(), "%x|%x", &sg, &se);
				if (sn < 2) {
					std::cerr << "Formato invalido (" << claveSecuencia.c_str() << "). Solo se soporta (FFFF|FFFF) como formato de tag para secuencias" << std::endl;
					continue;
				}
				DcmTag stag(sg, se);
				if (stag.error() != EC_Normal) {
					std::cerr << "Tag desconocido: " << claveSecuencia << std::endl;
					continue;
				}
				DcmSequenceOfItems* seq = new DcmSequenceOfItems(stag);
				if (seq == NULL) {
					std::cerr << "No se pudo crear la secuencia para el tag: " << claveSecuencia << std::endl;
					continue;
				}

				int nItems = InsertarJerarquia(nbase, NULL, seq);

				if (nItems > 0) {

					OFCondition cond;

					if (seqPadre != NULL) {
						DcmItem* item = new DcmItem();
						cond = item->insert(seq);
						seqPadre->insert(item);
						GTRACE(seqPadre->getTag().toString().c_str() << " << " << seq->getTag().toString());
					}
					else if (itemPadre != NULL) {
						cond = itemPadre->insert(seq, OFTrue);
						GTRACE(itemPadre->getTag().toString().c_str() << " << " << seq->getTag().toString());

					}
					else {
						cond = ds->insert(seq, OFTrue);
						GTRACE("raiz << " << seq->getTag().toString());
					}

					if (cond.bad()) {
						std::cerr << "No se pudo insertar el item a la raiz.  " << nItems << " elementos perdidos: " << cond.text() << std::endl;
						delete seq;
					}
					else {
						numTotalInsertados += nItems + 1;
						numSeqsInsertadas++;
					}
				}
				else {
					delete seq;
				}
			}
			return numTotalInsertados;
		}

		//----------------------Hierarchy load-------------------------------------------------------
		int DICOMManager::CargarJerarquia(DicomDataset& base, unsigned int maximaLongitud, DcmElement * pElementoInicial, bool mustExistPixel) {
			int numInsertados = 0;

			std::queue<DcmElement*>                pilaElementos;
			std::queue<GIL::DICOM::DicomDataset*> pilaPadres;

			OFCondition cond;

			DcmDataset* ds = getSourceDataSet();
			if (ds == NULL) {
				CGLogger::Error("SourceDataSet is null.");
				return 0;
			}

			FindCharset();

			unsigned long elemCount = ds->card();

            bool hasPixel = false;
			//If is Zero, we carry the entire Hierarchy
			if(pElementoInicial == NULL) {
				for (unsigned long i = 0; i < elemCount; i++) {
					DcmElement *e = ds->getElement(i);
                    if (IsTagValid(e->getGTag()))
                    {
                        if (e->getGTag() == 0x7fe0 && e->getETag() == 0x0010) {
                            // Excluir pixel data
                            hasPixel = true;
                        }
                        else {
                            pilaElementos.push(e);
                            pilaPadres.push(&base);
                        }
                    }
				}
			} else {
                if (IsTagValid(pElementoInicial->getGTag()))
                {
                    pilaElementos.push(pElementoInicial);
                    pilaPadres.push(&base);
                }
			}

            if (mustExistPixel && !hasPixel)
            {
				CGLogger::Error("dicom file has not pixel data.");
                return 0;
            }

			while(!pilaElementos.empty()) {
				DcmElement* e = pilaElementos.front(); pilaElementos.pop();
				GIL::DICOM::DicomDataset* cbase = pilaPadres.front(); pilaPadres.pop();

				if (e->ident() == EVR_item) {
					//GTRACE("Procesando item: " << e->getTag().toString())
					GIL::DICOM::DicomDataset newbase;

					cbase->items.push_front(newbase);

					numInsertados++;

					DcmObject* e2 = NULL;
					while ( (e2 = e->nextInContainer(e2)) != NULL) {
						pilaElementos.push( static_cast<DcmElement*>(e2) );
						pilaPadres.push( &(cbase->items.front()) );
					}

				}
				else if (e->ident() == EVR_SQ) {
					//GTRACE("Procesando secuencia: " << e->getTag().toString())
					std::ostringstream os;
					os << std::hex << std::setw(4) << std::setfill('0') << e->getGTag() << "|" << std::hex << std::setw(4) << std::setfill('0') << e->getETag();
					std::string tag = os.str();

					GIL::DICOM::DicomDataset newbase;
					newbase.tagName = tag;

					cbase->secuencias.push_front(newbase);

					numInsertados++;

					DcmObject* e2 = NULL;
					while ( (e2 = e->nextInContainer(e2)) != NULL) {
						pilaElementos.push( static_cast<DcmElement*>(e2) );
						pilaPadres.push( &(cbase->secuencias.front()) );
					}
				}
				else {
					GTRACE("Procesando objeto: " << e->getTag().toString())
					std::ostringstream os;
					os << std::hex << std::setw(4) << std::setfill('0') << e->getGTag() << "|" << std::hex << std::setw(4) << std::setfill('0') << e->getETag();
					std::string tag = os.str();

					//solo carga los tags no privados
					//if(e->getGTag() != DW_PRIVATE_TAG_GROUP && ( maximaLongitud == 0 || e->getLength() < maximaLongitud ) ){
						if (e->getGTag() == DW_PRIVATE_TAG_GROUP) {
							std::cout << "long=" << e->getLength() << std::endl;
						}
						OFString v;
						std::string ov;

						cond = e->getOFStringArray(v, OFTrue);
						if (cond.good()) {
							ov = v.c_str();
						} 
						if (ov.size() == 0 && v.size() != ov.size()) {
							//workarround try iso...
							//wxCSConv tryIso(wxFONTENCODING_ISO8859_1);
							ov = v.c_str();
							//last chance... try ascii
							if (ov.size() == 0 && v.size() != ov.size()) {
								ov = v.c_str();
							}
						}

                        //wxString tryUtf8 = wxString::FromUTF8(ov.c_str());
                        //if (tryUtf8.size() > 0)
                        //{
                        //    ov = tryUtf8.ToStdString();
                        //}

						cbase->tags[tag] = ov;

						numInsertados++;
					//} else {
						
					//}
				}
			}
			return numInsertados;
		}

		//----------------------INSERTAR TAGS PRIVADOS-------------------------------------------------------
		int DICOMManager::InsertPrivateTags(TipoPrivateTags& tags){
			//Deep wise private tag
			unsigned int g=DW_PRIVATE_TAG_GROUP;
			unsigned int e=0;
			OFCondition cond;
			DcmElement* element;

			DcmDataset* ds = getSourceDataSet();
			if (ds == NULL) {
				return 0;
			}

			e=GetElementIdentifier(tags);

			if(e>0x00FF || e==0){
				//I should not get here
				std::cerr<<"error al almacenar los tags privados, todos los slots ocupados" <<std::endl;
				return 0;
			}

			//ya tenemos el e del modulo hay que hacer un desplazamiento de 8 bits a la izquierda para obtener el rango
			//si el elemento es 00xx el rango sera xx00-xxFF
			e = e << 8;

			unsigned int eTemp;
			TipoPrivateTags::ListaTags& ListaTags = tags.GetListaTags();
			for (TipoPrivateTags::ListaTags::iterator it = ListaTags.begin(); it != ListaTags.end(); ++it)
			{
				eTemp = e | (*it).first;
				element = (*it).second->ToElement(g,eTemp);
				if (element != NULL)
				{
					cond = ds->insert(element, true, false);
					if (cond.bad()) {
						std::cerr << "error al almacenar los tags privados, error al escribir en el dataset: (" << g << ","<<eTemp<<")" <<std::endl;
						return 0;
					}
				} else {
					std::cerr << "error al almacenar los tags privados, error al crear el elemento: (" << g << ","<<eTemp<<")" <<std::endl;
					return 0;
				}
			}

			return 1;
		}

		//----------------------CARGAR TAGS PRIVADOS-------------------------------------------------------
		int DICOMManager::PrivateCargarTagsPrivados(GIL::DICOM::TipoPrivateTags& tags){
			//se busca el uid en el rango (DW_PRIVATE_TAG_GROUP,0010-00FF)
			unsigned int g=DW_PRIVATE_TAG_GROUP;
			unsigned int e=0;
			OFCondition cond;
			DcmElement* element;

			DcmDataset* ds = getSourceDataSet();
			if (ds == NULL) {
				return 0;
			}

			e=GetElementIdentifier(tags);

			if(e>0x00FF || e==0){
				//no deberia llegar aqui
				std::cerr<<"error al almacenar los tags privados, todos los slots ocupados" <<std::endl;
				return 0;
			}

			//ya tenemos el e del modulo hay que hacer un desplazamiento de 8 bits a la izquierda para obtener el rango
			//si el elemento es 00xx el rango sera xx00-xxFF
			e = e << 8;

			unsigned int numElementos=0;
			DcmVR vr(EVR_UN);
			DcmTagKey key(g,e);
			unsigned int eTemp;

			//se pillan todos los atributos del slot
			for(unsigned char eTag=0x0;eTag<0xFF;++eTag){
				eTemp = e | eTag;

				key.setElement(eTemp);
				DcmTag tag(key,vr);

				if (tag.error() != EC_Normal) {
					std::cerr << "error al almacenar los tags privados, tag desconocido: (" << g << ","<<eTemp<<")" <<std::endl;
					return 0;
				}

				cond = ds->findAndGetElement(key,element,false);

				if (element!=NULL && cond.good()){
					if (element->getVR() == EVR_LT) {
						char* copiaString = NULL;
						if (!element->isEmpty() && element->getString(copiaString).good()) {
							tags.SetTag(eTag, copiaString);
						}
						numElementos++;
					} else if (element->getVR() == EVR_UN) {
						//se lee el elemento
						int size = element->getLength();
						if (size>0) {
							TagPrivadoUndefined* pTag = tags.NewTagUndefined(eTag, size);

							Uint8 * ptr;
							cond = element->getUint8Array(ptr);

							memcpy(pTag->GetValor(),ptr,size*sizeof(Uint8));

							if (cond.bad()) {
								std::cerr << "error al cargar los tags privados, error al cargar el tag: (" << g << ","<<eTemp<<")" <<std::endl;
								return 0;
							}
							numElementos++;
						}
					}
				}
			}

			return numElementos;
		}

		//devuelve el element identifier donde el modulo puede escribir sus tags privados
		unsigned int DICOMManager::GetElementIdentifier(GIL::DICOM::TipoPrivateTags& tags){
			OFCondition cond;
			DcmElement* element;
			unsigned int g=DW_PRIVATE_TAG_GROUP;
			unsigned int e=0x0010;

			DcmDataset* ds = getSourceDataSet();
			if (ds == NULL) {
				return 0;
			}

			for(;e<=0x00FF;e++){
				DcmTagKey key(g,e);
				cond = ds->findAndGetElement(key,element,false);
				if(element==NULL){
					//esta libre el slot
					//se introduce el uid
					DcmTagKey key(g,e);
					DcmTag tag(key);
					//tipo short string!!!!
					DcmVR vr(EVR_SH);
					tag.setVR(vr);
					if (tag.error() != EC_Normal) {
						std::cerr << "error al almacenar los tags privados, tag desconocido: (" << g << ","<<e<<")" <<std::endl;
						return 0;
					}

					element = DcmItem::newDicomElement(tag);

					if(element==NULL){
						std::cerr << "error al almacenar los tags privados, error al crear el elemento uid: (" << g << ","<<e<<")" <<std::endl;
						return 0;
					}

					cond=element->putString(tags.UIDModulo.c_str());

					if (cond.bad()) {
						std::cerr << "error al almacenar los tags privados, error al escribir el uid: (" << g << ","<<e<<")" <<std::endl;
						return 0;
					}

					cond = ds->insert(element, true, false);
					break;
				}

				if(element!=NULL){
					char* cadena;
					std::string ov;

					cond = element->getString(cadena);
					if (cond.good() && cadena!=NULL) {
						ov = std::string(cadena);
					} else {
						continue;
					}

					if(ov==tags.UIDModulo){
						//estamos en el g y e del modulo deseado
						break;
					}
				}
			}

			return e;
		}

		//endregion

		void DICOMManager::FindCharset(const std::string& defaultCharset)
		{
			DcmDataset* ds = getSourceDataSet();
			if (ds == NULL) {
				return;
			}

			//if (m_pConv != NULL) {
			//	delete m_pConv;
			//	m_pConv = NULL;
			//}

			OFString OFCharSet;

			std::string Charset;

			if (ds->findAndGetOFString(DCM_SpecificCharacterSet, OFCharSet).good()) {
				Charset.assign(OFCharSet.c_str());
			}
			else {
				Charset = defaultCharset.c_str();
				ds->putAndInsertOFStringArray(DCM_SpecificCharacterSet, Charset.c_str());
			}

			//if (Charset == "ISO_IR 192" || Charset == "ISO_IR 6") {
			//	m_pConv = new wxCSConv(wxFONTENCODING_UTF8);
			//} else if (Charset == "ISO 2022 IR 6") {
			//	m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_1);
			//} else if (Charset == "ISO_IR 100") {
			//	m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_1);
			//}
			//else if (Charset == "ISO_IR 101") {
			//	m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_2);
			//}
			//else if (Charset == "ISO_IR 109") {
			//	m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_3);
			//}
			//else if (Charset == "ISO_IR 110") {
			//	m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_4);
			//}
			//else if (Charset == "ISO_IR 148") {
			//	m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_9);
			//}
			//else if (Charset == "ISO_IR 144") {
			//	m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_5);
			//}
			//else if (Charset == "ISO_IR 127") {
			//	m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_6);
			//}
			//else if (Charset == "ISO_IR 126") {
			//	m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_7);
			//}
			//else if (Charset == "ISO_IR 138") {
			//	m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_8);
			//}
			//else {
			//	m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_1);
			//}

		}

		
		void DICOMManager::CreateTempFile()
		{
		}

		void DICOMManager::CleanupTempFile()
		{
		}
	}
}
