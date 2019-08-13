/*=========================================================================

  Program:   ImagingEngine
  Module:    light.h
  author: 	 zhangjian
  Brief:	 �ƹ�

=========================================================================*/
#pragma once

#include <vtkLight.h>
#include <vtkSmartPointer.h>
#include "data/data_definition.h"

using namespace std;

namespace DW {
	namespace IMAGE{
		
		///
		// �ƹ���
		///
		class Light
		{
		public:
			Light();
			~Light();
			vtkSmartPointer<vtkLight> GetLight();
			void SetLight(vtkSmartPointer<vtkLight> light);
		protected:
			vtkSmartPointer<vtkLight> vtk_light_;
		private:

		};
	}
}