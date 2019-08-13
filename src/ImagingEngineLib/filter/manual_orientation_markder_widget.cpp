/*=========================================================================

  Program:   ImagingEngine
  Module:    manual_orientation_markder_widget.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "filter/manual_orientation_marker_widget.h"
#include "vtkActor2D.h"
#include "vtkCallbackCommand.h"
#include "vtkCamera.h"
#include "vtkCoordinate.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkProperty2D.h"
#include "vtkProp.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

void ManualOrientationMarkderWidget::Update()
{
	// Call event callback to update marker
	ExecuteCameraUpdateEvent(NULL, -1, NULL);
}

void ManualOrientationMarkderWidget::SetEnabled(int enabling)
{
	if (enabling)
	{
		if (this->Enabled)
		{
			return;
		}

		if (!this->OrientationMarker)
		{
			vtkErrorMacro("An orientation marker must be set prior to enabling/disabling widget");
			return;
		}

		if (!this->CurrentRenderer)
		{
			vtkErrorMacro("No renderer was set from parent renderer window.");
			return;
		}

		this->Enabled = 1;

		// Compute the viewport for the widget w.r.t. to the current renderer
		double currentViewport[4];
		this->CurrentRenderer->GetViewport(currentViewport);
		double vp[4], currentViewportRange[2];
		for (int i = 0; i < 2; ++i)
		{
			currentViewportRange[i] = currentViewport[i+2] - currentViewport[i];
			vp[i] = this->Viewport[i] * currentViewportRange[i] +
				currentViewport[i];
			vp[i+2] = this->Viewport[i+2] * currentViewportRange[i] +
				currentViewport[i];
		}
		this->Renderer->SetViewport(vp);

		vtkRenderWindow* renwin = this->CurrentRenderer->GetRenderWindow();
		renwin->AddRenderer( this->Renderer );
		if (renwin->GetNumberOfLayers() < 2)
		{
			renwin->SetNumberOfLayers( 2 );
		}

		this->CurrentRenderer->AddViewProp( this->OutlineActor );
		this->OutlineActor->VisibilityOff();
		this->Renderer->AddViewProp( this->OrientationMarker );
		this->OrientationMarker->VisibilityOn();

		// Sets again for no interaction here
		this->Interactive = 0;
		
		vtkCamera* pcam = this->CurrentRenderer->GetActiveCamera();
		vtkCamera* cam = this->Renderer->GetActiveCamera();
		if (pcam && cam)
		{
			cam->SetParallelProjection( pcam->GetParallelProjection() );
		}

		//// We need to copy the camera before the compositing observer is called.
		//// Compositing temporarily changes the camera to display an image.
		//this->StartEventObserverId = this->CurrentRenderer->AddObserver(
		//	vtkCommand::StartEvent, this->Observer, 1 );
		//this->InvokeEvent( vtkCommand::EnableEvent, NULL );
	}
	else
	{
		if (!this->Enabled)
		{
			return;
		}

		this->Enabled = 0;

		this->OrientationMarker->VisibilityOff();
		this->Renderer->RemoveViewProp( this->OrientationMarker );
		this->OutlineActor->VisibilityOff();
		this->CurrentRenderer->RemoveViewProp( this->OutlineActor );

		// if the render window is still around, remove our renderer from it
		if (this->CurrentRenderer->GetRenderWindow())
		{
			this->CurrentRenderer->GetRenderWindow()->
				RemoveRenderer( this->Renderer );
		}

		this->SetCurrentRenderer( NULL );
	}
}
