/*
 *  
 *  $Id: dicommanager.h $
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
#include <api/globals.h>
#include <api/api.h>
#include <api/dicom/idicommanager.h>
//#include <api/iproxynotificadorprogreso.h>
//#include <model/AIResult.h>

class DcmMetaInfo;
class DcmDataset;
class DcmFileFormat;
class DcmElement;
class OFCondition;
class DcmItem;
class DcmSequenceOfItems;

class wxCSConv;

namespace GIL
{
	namespace DICOM
	{

		class EXTAPI DICOMManager : public IDICOMManager
		{
		public:

			DICOMManager();
			DICOMManager(DcmDataset* dataset, const std::string& defaultCharset);

			~DICOMManager();

//region "Interfaz"
			/*Almacena la jerarquia cargada en un fichero*/
			virtual bool AlmacenarFichero(const std::string& inputFile, IInspectCallBack* pICallback = NULL, bool debug = false);

			/*Updated the hierarchy loaded*/
			virtual int ActualizarJerarquia(DicomDataset& base);

			/* remove certain tags */
			virtual int RemoveTags(const DicomDataset& base);

			/*Updates the private tags*/
			virtual int UpdatePrivateTags(TipoPrivateTags& tags);

			/*Anonymised Private tags*/
			virtual void AnonimizarTagsPrivados();
			
			/*load MetaInfo*/
			virtual int CargarMetaInfo(const std::string& inputFile, GIL::DICOM::TipoMetaInfo& tags);

			//virtual int GenerateKeySeriesAndGSPS(std::vector<std::string>& inputFileList, DW::MODEL::AIResult* aiResult, const std::string& aiInfor, std::string destFolder, boolean isGenerateKeySeries, boolean isGenerateGSPS, std::list<string>& listOfFileToUpload, std::string& seriesUID);

			/*load Private tags*/
			virtual int LoadPrivateTags(GIL::DICOM::TipoPrivateTags& tags);

			/* Obtains the hierarchy of DICOM file Fields */
			virtual bool CargarFichero(const std::string& inputFile, GIL::DICOM::DicomDataset& jerarquia, bool cargarSoloTagsInfo = true, IInspectCallBack* pICallback = NULL);

			/*Load the file and responds to Primitive Get TAG, useful to catch specific tags without Loading the entire Hierarchy*/
			virtual bool CargarFichero(const std::string&  inputFile, bool cargarSoloTagsInfo = true);
			virtual bool GetTag(unsigned int grupo,unsigned int elemento, std::string & valor);
			virtual bool GetTag(unsigned int grupo,unsigned int elemento, TagPrivadoUndefined& tagBinario);
			virtual bool GetTag(unsigned int grupo,unsigned int elemento, float& valor);
			virtual bool GetTag(unsigned int grupo,unsigned int elemento, int& valor);virtual bool GetTag(unsigned int grupo,unsigned int elemento, GIL::DICOM::DicomDataset& secuencia);
			virtual bool ExtractTagToFile(unsigned int group, unsigned int element, const std::string& outputFile
				/*, GNC::IProxyNotificadorProgreso* pNotificador = NULL*/);

			virtual bool FindTag(unsigned int group,unsigned int element, TagPrivadoUndefined& tagBinario);
			
			/* returns the oid name or empty string if not found */
			virtual std::string GetOIDName(const std::string &oid);
			
			/* devuelve la descripcion de este tag*/
			virtual std::string GetDescription(const std::string &clave) const;
			

			//virtual std::string GetNewUID();
//endregion

//region "External helpers"
			/*Check the magic number if DICOM*/
			static bool EsDicom(const std::string& inputFile);

			/* Creates a dcmelement parsing the string (G, e) = value */
			static DcmElement* CrearElementoConValor(const char* s);

			/* Inserts the hierarchy in the dataset */
			int InsertarJerarquia(const DicomDataset& base, DcmItem* itemPadre, DcmSequenceOfItems* seqPadre);

			/* Load the hierarchy from the dataset */
			int CargarJerarquia(DicomDataset& base, unsigned int maximaLongitud, DcmElement * pElementoInicial = NULL, bool mustExistPixel = true);

			/*The Private tags inserted in the dataset*/
			int InsertPrivateTags(TipoPrivateTags& tags);
			
			/*Private load tags*/
			int PrivateCargarTagsPrivados(GIL::DICOM::TipoPrivateTags& tags);

			/*Returns the identifier of the element which must store the private tags*/
			unsigned int GetElementIdentifier(GIL::DICOM::TipoPrivateTags& tags);

//endregion

//region "Internal helpers"

		private:
			void FindCharset(const std::string& defaultCharset = "ISO_IR 100");
			void CreateTempFile();
			void CleanupTempFile();
			DcmDataset* getSourceDataSet();
//endregion

		private:

			DcmFileFormat*  m_pDCMSourceFile;
			DcmDataset*     m_pDCMSourceDataset;
			//wxCSConv* m_pConv;
		};

	}
}
