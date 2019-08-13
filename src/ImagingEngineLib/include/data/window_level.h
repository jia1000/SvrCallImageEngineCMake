/*=========================================================================

  Program:   ImagingEngine
  Module:    window_width_level.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once
#include "data/data_definition.h"
#include <vtkWindowLevelLookupTable.h>

namespace DW {
	namespace IMAGE {

		class WindowLevel {

		public:
			WindowLevel();
			WindowLevel(int width, int level, const string &name);
			~WindowLevel();

			/// Define a name for the window level
			void SetName(const string &name);
			const string& GetName() const;

			/// Set a window width
			void SetWindowWidth(int width);
			int GetWindowWidth() const;

			/// Set a window level
			void SetWindowLevel(int level);
			int GetWindowLevel() const;

			/// window width is greater than or equal to 1
			bool IsValid() const;

			/// Compare to the WindowLevel
			bool ValuesAreEqual(const WindowLevel &window_level) const;

			/// Compare 
			bool operator==(const WindowLevel &window_level) const;

			/// Returns a vtkLookupTable representing this WindowLevel.
			vtkLookupTable* ConvertToVtkLookupTable() const;

		private:
			string window_level_name_;
			int window_width_;
			int window_level_;

		};

	}
}
