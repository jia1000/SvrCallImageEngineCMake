/*=========================================================================

  Program:   ImagingEngine
  Module:    convert_vtk_image_data_to_rgba.h
  author: 	 zhangjian
  Brief:	 �ṩ��vtkimagedataתΪRGBA��ʽ�ķ���

=========================================================================*/
///  @file ConvertVtkImagedataToRGBA.h
///  @brief ��vtkimagedataתΪRGBA��ʽ
///  @author lizhiguo
///  @date 2018
///
///
#ifndef CONVERTVTKIMAGEDATATORGBA_H
#define CONVERTVTKIMAGEDATATORGBA_H

//#include <QImage>

class vtkImageData;
class vtkScalarsToColors;

class ConvertVtkImagedataToRGBA
{
public:
    ConvertVtkImagedataToRGBA();

    /// \brief ��vtkImageData����ת��ΪRGBA��ʽ
    /// \param pSrcData Դ����
    /// \param pResult  �������
    /// \param slice    ת���Ĳ㣬��1��ʾȫ��ת��
    /// \param win      ����
    /// \param level    ��λ
    /// \return true:�ɹ� false:ʧ��
    /// @note pResult�ɺ����ڲ��������������ͷš�
    /// ������ֻ��Ҫ����vtkImageData *pResult = nullptr;���ɡ�
    bool ConvertImageScalarsToRGBA(vtkImageData *pSrcData,
                                   vtkImageData **pResult,
                                   int slice=-1,
                                   float win = 255.0,
                                   float level = 127.5);

    ///
    /// \brief ��vtkImageData���ݸ�����ɫ��ת��ΪRGBA��ʽ
    /// \param pSrcData     Դ����
    /// \param pColorTable  ��ɫ��
    /// \param slice        ת������Ƭ����1��ʾת�����е�
    /// \param pResult      �������
    /// \return true:�ɹ� false:ʧ��
    /// @note pResult�ɺ����ڲ��������������ͷš�
    /// ������ֻ��Ҫ����vtkImageData *pResult = nullptr;���ɡ�
    bool ConvertImageWithColorTableScalarsToRGBA(vtkImageData *pSrcData,
                                                 vtkScalarsToColors *pColorTable,
                                                 int slice,
                                                 vtkImageData **pResult);

    /////
    ///// \brief ��ȡvtkimagedata������ͼ
    ///// \param pSrcData     Դ����
    ///// \param win          ����
    ///// \param level        ��λ
    ///// \param imageOut     ���
    ///// \param slice        ��ȡ������ͼ���ڵ���Ƭ��-1��ʾȡ�м��
    ///// \return true:�ɹ� false:ʧ��
    /////
    //bool GetThumbnail(vtkImageData *pSrcData, float win, float level, QImage &imageOut, int slice=-1);


    ///
    /// \brief �ϲ�����vtkImageData,���Դ���ɫ��
    /// \param pSrcData         Դ����
    /// \param pSrcColorTable   Դ������ɫ������Ϊ��
    /// \param pMaskData        ģ������
    /// \param pMaskColorTable  ģ��������ɫ������Ϊ��
    /// \param slice            �ϲ�����Ƭ����1��ʾ�ϲ����е�
    /// \param pResult          �ϲ���Ľ��
    /// \return true:�ɹ� false:ʧ��
    ///
    bool CombineVtkImageData(vtkImageData *pSrcData,
                             vtkScalarsToColors *pSrcColorTable,
                             vtkImageData *pMaskData,
                             vtkScalarsToColors *pMaskColorTable,
                             int slice,
                             vtkImageData *pResult);


};

#endif // CONVERTVTKIMAGEDATATORGBA_H
