/*=========================================================================

  Program:   ImagingEngine
  Module:    orientation_marker.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "orientation/orientation_marker.h"
#include "data/patient_orientation.h"

using namespace DW::IMAGE;
using namespace DW::Orientation;


OrientationMarker::OrientationMarker(vtkRenderer *renderer)
{
	// Extracted from http://public.kitware.com/cgi-bin/viewcvs.cgi/* checkout */Examples/GUI/Tcl/ProbeWithSplineWidget.tcl? root = VTK & Content-type = text/plain
	// Create A composite orientation marker using
	// vtkAnnotatedCubeActor and vtkAxesActor.
	// //
	// \TODO is supposed to be this orientation is correct if we are viewing the volume on axial. We should look at the system how to put the labels correctly,
	// For example the dicom tag or leave it in the hands of the class to use and provide methods of this class to put
	// Labels correctly

	//////////////////////////////////////////////////////////////////////////
	// 立方体形式
	cube_actor_ = vtkAnnotatedCubeActor::New();
	this->SetOrientationText(PatientOrientation::kRightLabel, 
		PatientOrientation::kLeftLabel, 
		PatientOrientation::kPosteriorLabel, 
		PatientOrientation::kAnteriorLabel, 
		PatientOrientation::kHeadLabel,
		PatientOrientation::kFeetLabel);

	cube_actor_->SetXFaceTextRotation(180);
	cube_actor_->SetYFaceTextRotation(180);
	cube_actor_->SetZFaceTextRotation(-90);
	cube_actor_->SetFaceTextScale(0.65);

	vtkProperty *property = cube_actor_->GetCubeProperty();
	property->SetColor(0.1647, 0.1647, 0.5098);
	
	//property = cube_actor_->GetTextEdgesProperty();
	//property->SetLineWidth(1);
	//property->SetDiffuse(0);
	//property->SetAmbient(1);
	//property->SetColor(0.18, 0.28, 0.23);
	cube_actor_->SetTextEdgesVisibility(false);
	cube_actor_->SetCubeVisibility(true);
	cube_actor_->SetFaceTextVisibility(true);

	//0.1647, 0.1647, 0.5098
	property = cube_actor_->GetXPlusFaceProperty();
	property->SetColor(0.8784, 0.8784, 0.8784);
	property->SetInterpolationToFlat();

	property = cube_actor_->GetXMinusFaceProperty();
	property->SetColor(0.8784, 0.8784, 0.8784);
	property->SetInterpolationToFlat();

	property = cube_actor_->GetYPlusFaceProperty();
	property->SetColor(0.8784, 0.8784, 0.8784);
	property->SetInterpolationToFlat();

	property = cube_actor_->GetYMinusFaceProperty();
	property->SetColor(0.8784, 0.8784, 0.8784);
	property->SetInterpolationToFlat();

	property = cube_actor_->GetZPlusFaceProperty();
	property->SetColor(0.8784, 0.8784, 0.8784);
	property->SetInterpolationToFlat();

	property = cube_actor_->GetZMinusFaceProperty();
	property->SetColor(0.8784, 0.8784, 0.8784);
	property->SetInterpolationToFlat();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 坐标系形式
	vtkAxesActor *axes = vtkAxesActor::New();
	axes->SetShaftTypeToCylinder();
	// \TODO We could apply a rotation to the axes so that the annotations were seen in a system of hand-left (RAF) instead of hand-right (LPH)
	axes->SetXAxisLabelText("L");
	axes->SetYAxisLabelText("P");
	axes->SetZAxisLabelText("H");
	axes->SetTotalLength(1.5, 1.5, 1.5);
	vtkTextProperty *textProp = vtkTextProperty::New();
	textProp->ItalicOn();
	textProp->ShadowOn();
	textProp->SetFontFamilyToArial();
	axes->GetXAxisCaptionActor2D()->SetCaptionTextProperty(textProp);
	vtkTextProperty *textProp2 = vtkTextProperty::New();
	textProp2->ShallowCopy(textProp);
	axes->GetYAxisCaptionActor2D()->SetCaptionTextProperty(textProp2);
	vtkTextProperty *textProp3 = vtkTextProperty::New();
	textProp3->ShallowCopy(textProp);
	axes->GetZAxisCaptionActor2D()->SetCaptionTextProperty(textProp3);
	//////////////////////////////////////////////////////////////////////////

	// Combine the two actors into one with vtkPropAssembly ...
	//
	vtkPropAssembly *assembly = vtkPropAssembly::New();
	//     vtkAssembly *assembly = vtkAssembly::New();
	assembly->AddPart (cube_actor_);
	//assembly->AddPart (axes);

	//     cube_actor_->SetOrientation(10, 99, 103); // aquest no fa cas
	//     axes->SetOrientation(10, 87, 15); // aquest fa cas
	//     assembly->SetOrientation(10, 87, 15);

	//     Add the composite marker to the widget.  The widget
	//     should be kept in non-interactive mode and the aspect
	//     ratio of the render window taken into account explicitly,
	//     since the widget currently does not take this into
	//     account in a multi-renderer environment.
	marker_widget_ = new ManualOrientationMarkderWidget();
	//TODO 没有interactor，直接传入vtkRenderer
	//marker_widget_->SetInteractor(interactor);
	marker_widget_->SetCurrentRenderer(renderer);
	marker_widget_->SetOrientationMarker(assembly);
	marker_widget_->SetViewport(0.0, 0.0, 0.15, 0.15);
	this->SetEnabled(true);

	axes->Delete();
	textProp->Delete();
	textProp2->Delete();
	textProp3->Delete();
	assembly->Delete();
}

OrientationMarker::~OrientationMarker()
{
	cube_actor_->Delete();
	marker_widget_->Delete();
}

void OrientationMarker::Update()
{
	marker_widget_->Update();
}

void OrientationMarker::SetOrientationText(string right, string left, string posterior, string anterior, string superior, string inferior)
{
	cube_actor_->SetXPlusFaceText(right.c_str());
	cube_actor_->SetXMinusFaceText(left.c_str());
	cube_actor_->SetYPlusFaceText(posterior.c_str());
	cube_actor_->SetYMinusFaceText(anterior.c_str());
	cube_actor_->SetZPlusFaceText(inferior.c_str());
	cube_actor_->SetZMinusFaceText(superior.c_str());
}

void OrientationMarker::SetEnabled(bool enable)
{
	marker_widget_->SetEnabled(enable);
}