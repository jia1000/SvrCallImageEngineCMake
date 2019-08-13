/*=========================================================================

  Program:   ImagingEngine
  Module:    image_plane.h
  author: 	 zhangjian
  Brief:	 The ImagePlane class represents the bounded plane of an image.
			 The plane stores the image orientation, origin, spacing, 
			 row and column length, and thickness of the image.

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/vector3.h"
#include "data/image_orientation.h"
#include "data/pixel_spacing_2d.h"

namespace DW {
	namespace IMAGE {

		///
		// The ImagePlane class represents the bounded plane of an image.
		// The plane stores the image orientation, origin, spacing, row and column length, and thickness of the image.		///
		class ImagePlane {

		public:
			ImagePlane();

			/// Returns the image plane orientation.
			const ImageOrientation& GetImageOrientation() const;
			/// Sets the image plane orientation.
			void SetImageOrientation(ImageOrientation imageOrientation);

			/// Returns the top left corner of the plane.
			const Point3d& GetOrigin() const;
			/// Sets the top left corner of the plane.
			void SetOrigin(Point3d origin);
			/// Sets the top left corner of the plane.
			void SetOrigin(double x, double y, double z);

			/// Returns the pixel spacing.
			const PixelSpacing2D& GetSpacing() const;
			/// Sets the pixel spacing.
			void SetSpacing(PixelSpacing2D spacing);

			/// Returns the plane width.
			float GetRowLength() const;
			/// Sets the plane width.
			void SetRowLength(double length);

			/// Returns the plane height.
			float GetColumnLength() const;
			/// Sets the plane height.
			void SetColumnLength(double length);

			/// Returns the plane thickness.
			float GetThickness() const;
			/// Sets the plane thickness.
			void SetThickness(double thickness);

			/// Returns window width & level
			void GetWindowLevel(int &ww, int &wl);
			/// Sets window width & level
			void SetWindowLevel(int &ww, int &wl);

			///// Returns the plane center.
			//Point3f GetCenter() const;
			///// Modifies the origin so that the given point becomes the center.
			//void SetCenter(const Vector3f& center);
			///// Modifies the origin so that the given point becomes the center.
			//void SetCenter(float x, float y, float z);

			bool operator ==(const ImagePlane& plane) const;
			bool operator !=(const ImagePlane& plane) const;

			/// Location of the bounds. Central = middle of the plane, Upper = middle of the plane + thickness/2, Lower = middle of the plane - thickness/2.
			enum CornersLocation { Central, Upper, Lower };

			/// Contains the 4 corners of an ImagePlane at the specified location.
			struct Corners
			{
				CornersLocation location;
				Point3d topLeft, topRight, bottomRight, bottomLeft;
			};

			///// Returns the corners of the plane in the specified location.
			//Corners GetCorners(CornersLocation location) const;

			/// Computes the intersection between this ImagePlane and \a planeToIntersect.
			/// \param plane Plane to intersect with this one. It's considered an infinite plane.
			/// \param intersectionPoint1 The first intersection point will be stored here.
			/// \param intersectionPoint2 The second intersection point will be stored here.
			/// \param cornersLocation Specify which corners of this ImagePlane will be used to compute the intersection (upper by default)
			///                        (\a plane is always used with the central location).
			/// \return True if there are intersections, and false otherwise.
			bool GetIntersections(const ImagePlane* plane, Point3d& intersectionPoint1, Point3d& intersectionPoint2, CornersLocation cornersLocation = Upper) const;

			/// Returns the distance from the given point to this ImagePlane.
			double GetDistanceToPoint(const Point3d& point) const;

			/// Projects the given point onto this ImagePlane and returns the result.
			/// The projection consists in changing the coordinates so that the ImagePlane origin is the new (0,0,0),
			/// and the row and column vector the new X and Y axes, respectively.
			Point3d ProjectPoint(Point3d point, bool vtkReconstructionHack = false) const;

			/// Clone from given object
			void DeepCopy(ImagePlane *plane);

		private:
			/// The image plane orientation.
			ImageOrientation image_orientation_;
			/// Top left corner of the plane in patient coordinate system.
			/// This value may be saved into a dicom file with a world coordinate system,
			/// which could be achieved by adding the world position of the left top pixel stored in dicom file
			Point3d origin_;
			/// Pixel spacing.
			PixelSpacing2D spacing_;
			/// Plane width.
			double row_length_;
			/// Plane height.
			double column_length_;
			/// Plane thickness.
			double thickness_;
			/// Window width & level
			int ww_, wl_;

		};

	}
}