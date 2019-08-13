/*=========================================================================

  Program:   ImagingEngine
  Module:    image_lut.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once
#include "data/data_definition.h"
#include "data/window_level.h"

using namespace DW::IMAGE;

namespace DW {
	namespace CV {

		class VoiLut
		{
		public:
			/// Creates a VOI LUT that represents a default WindowLevel.
			VoiLut();
			/// Creates a VOI LUT that represents the given window level.
			VoiLut(const WindowLevel &window_level);
			~VoiLut();

			/// Set window width & level
			void SetWindowLevel(const WindowLevel &window_level);
			/// Get lut table for mapping color
			unsigned int *GetLutTable() const;

		private:
			/// Create lut table
			void CreateLUT();

		private:
			/// color mapping table
			unsigned int *lut_table_;
			/// Current WindowLevel
			WindowLevel window_level_;
			/// Maximium ct value
			const int kMaxCTV;

		};

	}
}
