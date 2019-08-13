/*=========================================================================

  Program:   ImagingEngine
  Module:    image_plane.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data/image_plane.h"
#include "vtkSmartPointer.h"
#include "vtkMatrix4x4.h"

using namespace DW::IMAGE;

ImagePlane::ImagePlane()
	: row_length_(0.0), column_length_(0.0), thickness_(1.0)
{
	image_orientation_.SetRowAndColumnVectors(Vector3d(1.0, 0.0, 0.0), Vector3d(0.0, 1.0, 0.0));
	ww_ = 400;
	wl_ = 50;
}

const ImageOrientation& ImagePlane::GetImageOrientation() const
{
	return image_orientation_;
}

void ImagePlane::SetImageOrientation(ImageOrientation imageOrientation)
{
	image_orientation_ = std::move(imageOrientation);
}

const Point3d& ImagePlane::GetOrigin() const
{
	return origin_;
}

void ImagePlane::SetOrigin(Point3d origin)
{
	origin_ = std::move(origin);
}

void ImagePlane::SetOrigin(double x, double y, double z)
{
	origin_.x = x;
	origin_.y = y; 
	origin_.z = z;
}

const PixelSpacing2D& ImagePlane::GetSpacing() const
{
	return spacing_;
}

void ImagePlane::SetSpacing(PixelSpacing2D spacing)
{
	spacing_ = std::move(spacing);
}

float ImagePlane::GetRowLength() const
{
	return row_length_;
}

void ImagePlane::SetRowLength(double length)
{
	row_length_ = length;
}

float ImagePlane::GetColumnLength() const
{
	return column_length_;
}

void ImagePlane::SetColumnLength(double length)
{
	column_length_ = length;
}

float ImagePlane::GetThickness() const
{
	return thickness_;
}

void ImagePlane::SetThickness(double thickness)
{
	thickness_ = thickness;
}

bool ImagePlane::operator ==(const ImagePlane &imagePlane) const
{
	return image_orientation_ == imagePlane.image_orientation_ && 
		(origin_.x - imagePlane.origin_.x > MathTool::kEpsilon &&
		origin_.y - imagePlane.origin_.y > MathTool::kEpsilon &&
		origin_.z - imagePlane.origin_.z > MathTool::kEpsilon) && 
		spacing_.IsEqual(imagePlane.spacing_)
		&& row_length_ == imagePlane.row_length_ && 
		column_length_ == imagePlane.column_length_ && 
		thickness_ - imagePlane.thickness_ < MathTool::kEpsilon;
}

bool ImagePlane::operator !=(const ImagePlane &imagePlane) const
{
	return !(*this == imagePlane);
}


bool ImagePlane::GetIntersections(const ImagePlane* plane, Point3d& intersectionPoint1, Point3d& intersectionPoint2, CornersLocation cornersLocation) const
{
	//TODO implement later..
	return false;
}

double ImagePlane::GetDistanceToPoint(const Point3d& point) const
{
	// TODO An alternative method would be computing distance with QVector3D::distanceToPlane()
	return 0.0;
}

Point3d ImagePlane::ProjectPoint(Point3d point, bool vtkReconstructionHack) const
{
	// First gather the needed plane data to project the point
	Vector3d row_vector = image_orientation_.GetRowVector();
	Vector3d column_vector = image_orientation_.GetColumnVector();
	Vector3d normal_vector = image_orientation_.GetNormalVector();

	// Then create the projection matrix
	auto projectionMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
	projectionMatrix->Identity();

	for (int column = 0; column < 3; column++)
	{
		projectionMatrix->SetElement(0, column, row_vector[column]);
		projectionMatrix->SetElement(1, column, column_vector[column]);
		projectionMatrix->SetElement(2, column, normal_vector[column]);
	}

	// Check if we have to apply vtk hack on projection matrix
	if (vtkReconstructionHack)
	{
		// HACK Patch for the cranium cases that are not working properly (gantry tilt).
		// TODO This is a first aproximation, must be thoroughly tested
		projectionMatrix->SetElement(0, 0, 0);
		projectionMatrix->SetElement(0, 1, 1);
		projectionMatrix->SetElement(0, 2, 0);
	}

	// Once we have the matrix, we can project the point in homogeneous coordinates
	// We must shift the point to project to the plane origin
	point.x -= origin_.x;
	point.y -= origin_.y;
	point.z -= origin_.z;
	double homogeneousPoint[4] = { point.x, point.y, point.z, 1.0 };

	// Project the point with the matrix
	double homogeneousProjectedPoint[4];
	projectionMatrix->MultiplyPoint(homogeneousPoint, homogeneousProjectedPoint);

	// Convert the homogeneous projected point to the 3D projected point to return
	Point3d result;
	result.x = homogeneousProjectedPoint[0];
	result.y = homogeneousProjectedPoint[1];
	result.z = homogeneousProjectedPoint[2];
	return result;
}

void ImagePlane::DeepCopy(ImagePlane *plane)
{
	if (plane){
		this->row_length_ = plane->row_length_;
		this->column_length_ = plane->column_length_;
		this->thickness_ = plane->thickness_;
		this->spacing_ = plane->spacing_;
		this->image_orientation_ = plane->image_orientation_;
		this->origin_.x = plane->origin_.x;
		this->origin_.y = plane->origin_.y;
		this->origin_.z = plane->origin_.z;
	}
}


void ImagePlane::GetWindowLevel(int &ww, int &wl)
{
	ww = ww_;
	wl = wl_;
}

void ImagePlane::SetWindowLevel(int &ww, int &wl)
{
	ww_ = ww;
	wl_ = wl;
}