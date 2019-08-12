#include "dicomdataset.h"

static const std::string empty;


GIL::DICOM::DicomDataset::DicomDataset()
{
}

GIL::DICOM::DicomDataset::~DicomDataset()
{
}


//ade referencias a las imagenes a las que hace referencia
void GIL::DICOM::DicomDataset::AddReference(const std::string& sopClassUID, const std::string& sopInstanceUID) {
	//se busca la secuencia de referencias
	GIL::DICOM::DicomDataset referencia;
	referencia.tags[std::string("0008|1150")] =  sopClassUID;
	referencia.tags[std::string("0008|1155")] =  sopInstanceUID;
	DicomDataset* pSecuencia_referencias = buscar_secuencia("0008|1140");
	if (  pSecuencia_referencias== NULL) {
		DicomDataset secuencia_referencias;
		secuencia_referencias.items.push_back(referencia);
		secuencia_referencias.tagName = "0008|1140";
		secuencias.push_back(secuencia_referencias);
	} else {
		//se busca si ya esta metida la referencia
		bool encontrado = false;
		for(GIL::DICOM::DicomDataset::DatasetList::iterator it = pSecuencia_referencias->items.begin(); it!= pSecuencia_referencias->items.end(); ++it) {
			std::string uid;
			if((*it).getTag("0008|1155",uid)){
				if (uid == sopInstanceUID)
				{
					encontrado = true;
					break;
				}
			}
		}
		if(!encontrado) {
			pSecuencia_referencias->items.push_back(referencia);
		}
	}
}

//ade referencias a las imagenes originales
void GIL::DICOM::DicomDataset::AddReferenceSource(const std::string& sopClassUID, const std::string& sopInstanceUID) {
	//se busca la secuencia de referencias
	GIL::DICOM::DicomDataset referencia;
	referencia.tags[std::string("0008|1150")] =  sopClassUID;
	referencia.tags[std::string("0008|1155")] =  sopInstanceUID;
	DicomDataset* pSecuencia_referencias = buscar_secuencia("0008|2112");
	if (  pSecuencia_referencias== NULL) {
		DicomDataset secuencia_referencias;
		secuencia_referencias.items.push_back(referencia);
		secuencia_referencias.tagName = "0008|2112";
		secuencias.push_back(secuencia_referencias);
	} else {
		//se busca si ya esta metida la referencia
		bool encontrado = false;
		for(GIL::DICOM::DicomDataset::DatasetList::iterator it = pSecuencia_referencias->items.begin(); it!= pSecuencia_referencias->items.end(); ++it) {
			std::string uid;
			if((*it).getTag("0008|1155",uid)){
				if (uid == sopInstanceUID)
				{
					encontrado = true;
					break;
				}
			}
		}
		if(!encontrado) {
			pSecuencia_referencias->items.push_back(referencia);
		}
	}
}

void GIL::DICOM::DicomDataset::clear()
{
	secuencias.clear();
	items.clear();
	tags.clear();
}

GIL::DICOM::DicomDataset* GIL::DICOM::DicomDataset::buscar_secuencia(const char* tag)
{
	std::string stag = tag;
	return buscar_secuencia(stag);
}

GIL::DICOM::DicomDataset* GIL::DICOM::DicomDataset::buscar_secuencia(const std::string & tag)
{
	for (DatasetList::iterator it = secuencias.begin(); it != secuencias.end(); ++it) {
		if ((*it).tagName == tag) {
			return &(*it);
		}
	}
	return NULL;
}

/* Obtiene un tag y lo asigna en el par醡etro de salida valor.
Devuelve true si el tag existe y false si el tag no existe */
bool GIL::DICOM::DicomDataset::getTag(const std::string& tag, std::string& valor) const
{
	ListaTags::const_iterator it = tags.find(tag);
	bool found = false;
	if (it != tags.end()) {
		valor = (*it).second;
        //wxString tryUTF8 = valor;
        //valor = tryUTF8.ToUTF8();
		found = true;
	}
	return found;
}

/* Obtiene un tag y lo asigna en el par醡etro de salida valor.
Devuelve true si el tag existe y false si el tag no existe */
bool GIL::DICOM::DicomDataset::getTag(const char* tag, std::string& valor) const
{
	ListaTags::const_iterator it = tags.find(tag);
	bool found = false;
	if (it != tags.end()) {
		valor = (*it).second;
		found = true;
	}
	return found;
}

const std::string& GIL::DICOM::DicomDataset::getTag(const std::string& tag) const
{
	ListaTags::const_iterator it = tags.find(tag);
	if (it != tags.end()) {
		return (*it).second;
	}
	return empty;
}

const std::string& GIL::DICOM::DicomDataset::getTag(const char* tag) const
{
	ListaTags::const_iterator it = tags.find(tag);
	if (it != tags.end()) {
		return (*it).second;
	}
	return empty;
}


void GIL::DICOM::DicomDataset::dump(std::ostream& out, int indentLevel) const
{
	for (int i = 0; i < indentLevel; i++) {
		out << " ";
	}
	out << "[ " << std::endl;

	for (DatasetList::const_iterator it = secuencias.begin(); it != secuencias.end(); ++it) {
		for (int i = 0; i < indentLevel + 3; i++) {
			out << " ";
		}
		out << "SEQ " << (*it).tagName << std::endl;
		(*it).dump(out, indentLevel + 6);
	}
	for (DatasetList::const_iterator it = items.begin(); it != items.end(); ++it) {
		for (int i = 0; i < indentLevel + 3; i++) {
			out << " ";
		}
		out << "ITEM " << std::endl;

		(*it).dump(out, indentLevel + 6);
	}
	for (ListaTags::const_iterator it = tags.begin(); it != tags.end(); ++it) {
		for (int i = 0; i < indentLevel + 3; i++) {
			out << " ";
		}
		out << "TAG " << (*it).first << " = " << (*it).second << std::endl;

	}
	for (int i = 0; i < indentLevel; i++) {
		out << " ";
	}
	out << "]" << std::endl;
}
