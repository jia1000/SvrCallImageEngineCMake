/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_vr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render/renderer_vr.h"
#include "render/render_param_vr.h"

#include <vtkWindowToImageFilter.h>
#include <vtkBMPWriter.h>
#include <vtkNew.h>
#include <vtkCamera.h>
#include <vtkColorTransferFunction.h>
#include <vtkDataArray.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkImageData.h>
#include <vtkImageReader.h>
#include <vtkImageShiftScale.h>
#include <vtkNew.h>
#include <vtkOutlineFilter.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
//#include <vtkStructuredPointsReader.h>
#include <vtkTimerLog.h>
#include <vtkVolumeProperty.h>
#include <vtkDICOMImageReader.h>
#include <vtkCallbackCommand.h>
#include "tools/vtk_image_data_creator.h"
#include "tools/timer.h"
#include "tools/logger.h"
#include <vtkVolumeMapper.h>
#include "exception/data_exception.h"
#include "exception/render_exception.h"

using namespace DW::Render;
using namespace DW::Exception;

VolumeRenderer::VolumeRenderer()
	: IThreedRenderer()
{
	camera_ = new Camera();
	render_mode_ = RenderMode::RAYCASTING;
	show_buffer_ = new ShowBuffer();
	image_plane_ = new ImagePlane();
	is_off_screen_rendering_ = true;
	is_first_render_ = true;
	window_level_.SetWindowWidth(2000);
	window_level_.SetWindowLevel(400);
	/// initialize vtk objects
	vtk_render_window_ = vtkSmartPointer<vtkRenderWindow>::New();
	vtk_renderer_ = vtkSmartPointer<vtkRenderer>::New();
	vtk_volume_mapper_ = vtkSmartPointer<vtkSmartVolumeMapper>::New();
#if VTK_MAJOR_VERSION > 5
#ifdef WIN32
	//TODO Linux报错：没有InteractiveAdjustSampleDistancesOff成员函数
	//// Workaround for vtkSmartVolumeMapper bug (https://gitlab.kitware.com/vtk/vtk/issues/17323)
	//vtk_volume_mapper_->InteractiveAdjustSampleDistancesOff(); 
#endif
#endif
	vtk_volume_property_ = vtkSmartPointer<vtkVolumeProperty>::New();
	vtk_volume_property_->SetInterpolationTypeToLinear();
	vtk_volume_ = vtkSmartPointer<vtkVolume>::New();
	vtk_volume_->SetProperty(vtk_volume_property_);
	vtk_volume_->SetMapper(vtk_volume_mapper_);

	/// We Create an TODO default transfer function We have only to have something by default
	/// Opacity
	transfer_function_.SetScalarOpacity(-800.0, 0.0);
	transfer_function_.SetScalarOpacity(-750.0, 0.0);
	transfer_function_.SetScalarOpacity(-350.0, 0.0);
	transfer_function_.SetScalarOpacity(-300.0, 0.0);
	transfer_function_.SetScalarOpacity(-200.0, 0.0);
	transfer_function_.SetScalarOpacity(0.0, 0.0);
	transfer_function_.SetScalarOpacity(10.0, 0.05);
	transfer_function_.SetScalarOpacity(500.0, 0.65);
	transfer_function_.SetScalarOpacity(1200.0, 0.9);
	transfer_function_.SetScalarOpacity(2750.0, 1.0);
	/// Colors
	transfer_function_.SetColor(-190.0, 0.4f, 0.2f, 0.0f);
	transfer_function_.SetColor(2000, 1.0f, 1.0f, 1.0f);
	/// Gradient
	transfer_function_.SetGradientOpacity(10, 0.0);
	transfer_function_.SetGradientOpacity(90, 0.5);
	transfer_function_.SetGradientOpacity(100, 1.0);

	/// Transfer function for MIP
	int min_ctv = window_level_.GetWindowLevel() - 0.5 * window_level_.GetWindowWidth();
	int max_ctv = window_level_.GetWindowLevel() + 0.5 * window_level_.GetWindowWidth();
	transfer_function_mip_.SetColor(min_ctv, 0.0f, 0.0f, 0.0f);
	transfer_function_mip_.SetColor(max_ctv, 1.0f, 1.0f, 1.0f);
	transfer_function_mip_.SetScalarOpacity(min_ctv, 1.0);
	transfer_function_mip_.SetScalarOpacity(max_ctv, 1.0);
	transfer_function_mip_.SetGradientOpacity(10, 0.0);
	transfer_function_mip_.SetGradientOpacity(90, 0.5);
	transfer_function_mip_.SetGradientOpacity(100, 1.0);

	/// Set lighting parameters
	vtk_volume_property_->SetAmbient(0.2);
	vtk_volume_property_->SetDiffuse(0.7);
	vtk_volume_property_->SetSpecular(0.3);
	vtk_volume_property_->SetSpecularPower(8.0);
	/// Set shade on
	vtk_volume_property_->ShadeOn();
	/// Set blend mode
	vtk_volume_mapper_->SetBlendModeToComposite();
	/// Set requested render mode
#if VTK_MAJOR_VERSION > 5
	//vtk_volume_mapper_->SetRequestedRenderModeToRayCast();
#else
	vtk_volume_mapper_->SetRequestedRenderMode(vtkSmartVolumeMapper::RayCastRenderMode);
#endif
	///// Set transfer function
	//vtk_volume_property_->SetColor(transfer_function_mip_.TovtkColorTransferFunction());
	//vtk_volume_property_->SetScalarOpacity(transfer_function_mip_.vtkScalarOpacityTransferFunction());

	vtk_volume_property_->SetColor(transfer_function_.TovtkColorTransferFunction());
	vtk_volume_property_->SetScalarOpacity(transfer_function_.vtkScalarOpacityTransferFunction());
	vtk_volume_property_->SetGradientOpacity(transfer_function_.vtkGradientOpacityTransferFunction());

	vtk_renderer_->AddViewProp(vtk_volume_);
	vtk_render_window_->AddRenderer(vtk_renderer_);
	vtk_renderer_->ResetCamera();

	camera_->SetVtkCamera(vtk_renderer_->GetActiveCamera());

	orientation_marker_ = new OrientationMarker(vtk_renderer_);
	orientation_marker_->SetEnabled(true);
	//vtk_mask_filter_ = vtkSmartPointer<vtkImageMask>::New();

	SetOffScreenRendering(true);
}

VolumeRenderer::~VolumeRenderer()
{
	if (camera_){
		delete camera_;
		camera_ = NULL;
	}
	if (show_buffer_){
		delete show_buffer_;
		show_buffer_ = NULL;
	}
	if (image_plane_){
		delete image_plane_;
		image_plane_ = NULL;
	}
}

void VolumeRenderer::SetRenderParam(RenderParam* param)
{
	render_param_ = param;
}

ShowBuffer* VolumeRenderer::GetShowBuffer()
{
	return show_buffer_;
}

void VolumeRenderer::SetData(VolData* data)
{
	if (volume_data_ == data)
		return;

	is_first_render_ = true;

	Timer::begin("VolumeRenderer::SetData");

	volume_data_ = data;
	if (NULL == volume_data_){
		// 卸载数据
		vtk_volume_mapper_->RemoveAllClippingPlanes();
		vtk_volume_mapper_->RemoveAllInputs();

		CGLogger::Info("VolumeRenderer::SetData null");
		return;
	}

	// Workaround for vtkSmartVolumeMapper bug (https://gitlab.kitware.com/vtk/vtk/issues/17328)
	IPixelData *pixel_data = volume_data_->GetPixelData();
	if (pixel_data == NULL){
		CGLogger::Error("VolumeRenderer::SetData >> volume data has no pixel data.");
		throw DataException(RET_STATUS_ENGINE_DATA_PIXELDATA_NULL, "volume data has no pixel data.");
	}
	vtkImageData *image_data = pixel_data->GetVtkImageData();
	image_data->Modified();
	// 在此处将mask转换为vtkImageData格式
	UNITMARK3D *mark = volume_data_->GetMark();
	// 获取数据场的pixel spacing
	volume_data_->GetPixelData()->GetSpacing(voxel_spacing_);
	// 输入到Mapper里的vtkImageData
	vtkSmartPointer<vtkImageData> mapper_input_data = vtkSmartPointer<vtkImageData>::New();
	if (mark){

		int dims[3];
		volume_data_->GetPixelData()->GetDimensions(dims);

		//////////////////////////////////////////////////////////////////////////
		// Set volume mask data
		vtkSmartPointer<vtkImageImport> image_import = vtkSmartPointer<vtkImageImport>::New();
		image_import->SetDataSpacing(voxel_spacing_);
		image_import->SetDataOrigin(0, 0, 0);
		image_import->SetWholeExtent(0, dims[0] - 1, 0, dims[1] - 1, 0, dims[2] - 1);
		image_import->SetDataExtentToWholeExtent();
		image_import->SetDataScalarTypeToUnsignedChar();
		image_import->SetNumberOfScalarComponents(1);
		image_import->SetImportVoidPointer(mark, 1);
		image_import->Update();
		input_mark_data_ = image_import->GetOutput();

		int extent[6];
		input_mark_data_->GetExtent(extent);
		
		vtkSmartPointer<vtkImageMask> mask_filter = vtkSmartPointer<vtkImageMask>::New();
		mask_filter->SetInputData(0, image_data);
		mask_filter->SetInputData(1, input_mark_data_);
		mask_filter->SetMaskedOutputValue(-1024, -1024, -1024);
		try{
			mask_filter->Update();
		}
		catch(std::bad_alloc &e){
			CGLogger::Info("out of memory in VolumeRenderer::SetData.");
			throw DataException(RET_STATUS_ENGINE_EXCEPTION_OUT_OF_MEMORY, "out of memory in VolumeRenderer::SetData.");
		}

		mapper_input_data->ShallowCopy(mask_filter->GetOutput());
		//////////////////////////////////////////////////////////////////////////
	}
	else{
		mapper_input_data->ShallowCopy(image_data);
	}
	
#if VTK_MAJOR_VERSION > 5
	vtk_volume_mapper_->SetInputData(mapper_input_data);
#ifdef WIN32
	//TODO Linux报错：没有SetSampleDistance成员函数
	/// force the mapper to compute a sample distance based on data spacing
	vtk_volume_mapper_->SetSampleDistance(-1.0);
#endif
#else
	vtk_volume_mapper_->SetInput(volume_data_->GetPixelData()->GetVtkImageData());
#endif
	is_first_render_ = true;

	Timer::end("VolumeRenderer::SetData");
	CGLogger::Debug(Timer::summery());
}

void VolumeRenderer::ClearData()
{
	volume_data_ = NULL;
	is_first_render_ = true;
	// 卸载数据
	vtk_volume_mapper_->RemoveAllClippingPlanes();
	vtk_volume_mapper_->RemoveAllInputs();

	CGLogger::Info("VolumeRenderer::ClearData");
}

void VolumeRenderer::SetVolumeTransformation()
{
	//Image *imageReference = getMainInput()->getImage(0);
	//ImagePlane currentPlane;
	//currentPlane.fillFromImage(imageReference);
	//std::array<double, 3> currentPlaneRowVector = Vector3(currentPlane.getImageOrientation().getRowVector());
	//std::array<double, 3> currentPlaneColumnVector = Vector3(currentPlane.getImageOrientation().getColumnVector());
	//double stackDirection[3];
	//getMainInput()->getStackDirection(stackDirection);

	//DEBUG_LOG(QString("currentPlaneRowVector: %1 %2 %3").arg(currentPlaneRowVector[0]).arg(currentPlaneRowVector[1]).arg(currentPlaneRowVector[2]));
	//DEBUG_LOG(QString("currentPlaneColumnVector: %1 %2 %3").arg(currentPlaneColumnVector[0]).arg(currentPlaneColumnVector[1]).arg(currentPlaneColumnVector[2]));
	//DEBUG_LOG(QString("stackDirection: %1 %2 %3").arg(stackDirection[0]).arg(stackDirection[1]).arg(stackDirection[2]));

	//vtkMatrix4x4 *projectionMatrix = vtkMatrix4x4::New();
	//projectionMatrix->Identity();

	//if ((   currentPlaneRowVector[0] == 0.0 &&    currentPlaneRowVector[1] == 0.0 &&    currentPlaneRowVector[2] == 0.0) ||
	//	(currentPlaneColumnVector[0] == 0.0 && currentPlaneColumnVector[1] == 0.0 && currentPlaneColumnVector[2] == 0.0) ||
	//	(          stackDirection[0] == 0.0 &&           stackDirection[1] == 0.0 &&           stackDirection[2] == 0.0))
	//{
	//	DEBUG_LOG("One of the vectors is null: setting an identity projection matrix.");
	//}
	//else
	//{
	//	for (int row = 0; row < 3; row++)
	//	{
	//		projectionMatrix->SetElement(row, 0, currentPlaneRowVector[row]);
	//		projectionMatrix->SetElement(row, 1, currentPlaneColumnVector[row]);
	//		projectionMatrix->SetElement(row, 2, stackDirection[row]);
	//	}
	//}

	//m_vtkVolume->SetUserMatrix(projectionMatrix);
	//m_isosurfaceActor->SetUserMatrix(projectionMatrix);

	//projectionMatrix->Delete();
}

void VolumeRenderer::Render()
{
	if (volume_data_ == NULL) {
		CGLogger::Error("VolumeRenderer::Render >> volume data is null.");
		return;
	}

	Timer::begin("VolumeRenderer::Render");

	DoRender(NULL);

	Timer::end("VolumeRenderer::Render");
	CGLogger::Debug(Timer::summery());
}

//此版本从dr.wise客户端修改而来
void VolumeRenderer::DoRender(vtkSmartPointer<vtkImageData> imagedata)
{
	if (render_param_ == NULL){
		CGLogger::Error("VolumeRenderer::DoRender >> no render param exists.");
		throw DataException(RET_STATUS_ENGINE_INPUT_INVALID_PARAM, "no render param exists.");
	}
	VRRenderParam* param_imp = (VRRenderParam *)render_param_;
	// get render parameters
	int width = param_imp->GetWidth();
	int height = param_imp->GetHeight();

	vtk_render_window_->SetSize(width, height);
	// set desired update rate
	vtk_render_window_->SetDesiredUpdateRate(param_imp->GetDesiredUpdateRate());
	
	// The first render after data changed
	if (is_first_render_){
		is_first_render_ = false;
		vtk_renderer_->ResetCamera();

		float clip = param_imp->GetClipping();
		// Set rendering to fit into view port
		FitRenderingIntoViewport(clip, param_imp->GetUpdateViewportToFitTissue());

	}

	try {
		// Update orientation marker
		orientation_marker_->Update();	
		// Needs 5 seconds to render mask data for the first time...
		vtk_render_window_->Render();
	}
	catch(std::bad_alloc &e){
		CGLogger::Error("bad_alloc error occurred in VolumeRenderer::DoRender.");
		throw RenderException(RET_STATUS_ENGINE_EXCEPTION_OUT_OF_MEMORY, e.what());
	}
	catch(...){
		CGLogger::Error("error occurred in VolumeRenderer::DoRender.");
		throw RenderException(RET_STATUS_ENGINE_EXCEPTION_RUNTIME_ERROR, "error occurred in VolumeRenderer::DoRender.");
	}
	
	/// Get screen shot from render window  
	vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
	windowToImageFilter->SetInput(vtk_render_window_);
#if VTK_MAJOR_VERSION < 8
	windowToImageFilter->SetMagnification(1); //set the resolution of the output image (3 times the current resolution of vtk render window)
#endif
	windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
	windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
	try{
		windowToImageFilter->Update();
	}
	catch(std::bad_alloc){
		CGLogger::Error("bad_alloc error occurred in getting image filter.");
		throw RenderException(RET_STATUS_ENGINE_EXCEPTION_OUT_OF_MEMORY, "bad_alloc error occurred in getting image filter.");
	}
	vtkSmartPointer<vtkImageData> tmp_vtk_data = windowToImageFilter->GetOutput();
	

	//////////////////////////////////////////////////////////////////////////
	// Convert to Buffer object
	int *data_ptr = reinterpret_cast<int *>( tmp_vtk_data->GetScalarPointer() );
	int image_width = tmp_vtk_data->GetDimensions()[0];
	int image_height = tmp_vtk_data->GetDimensions()[1];

	int x, y;
	int plane_size = image_width * image_height;
	// 4通道RGBA位图
	try{
		int *raw_data = new int [plane_size];
		for (y=0; y<image_height; ++y){
			for (x=0; x<image_width; ++x){
				/*raw_data[y*image_width + x] = pdata[(image_height-1 - y) * image_width + x];*/
				// 以下输出DICOM方向正确
				raw_data[y*image_width + x] = data_ptr[(image_height-1 - y)* image_width + x];
			}
		}
	
		show_buffer_->SetBufferData(reinterpret_cast<char *>(raw_data), image_width, image_height, 32);
		delete [] raw_data;
	}
	catch(std::bad_alloc &e){
		CGLogger::Error("bad_alloc error occurred in converting to show buffer.");
		throw RenderException(RET_STATUS_ENGINE_EXCEPTION_OUT_OF_MEMORY, e.what());
	}
	catch(std::out_of_range &e){
		CGLogger::Error("out_of_range error occurred in converting to show buffer.");
		throw RenderException(RET_STATUS_ENGINE_EXCEPTION_OUT_OF_RANGE, e.what());
	}
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	// 以下设置输出图像平面信息
	// 图像方向
	double *view_up = camera_->GetVtkCamera()->GetViewUp();
	// This is usually the opposite of the ViewPlaneNormal, 
	// the vector perpendicular to the screen, unless the view is oblique.
	double *projection = camera_->GetVtkCamera()->GetDirectionOfProjection();
	MathTool::Normalize (view_up);
	MathTool::Normalize (projection);
	Vector3d col_vector(view_up[0], view_up[1], view_up[2]);
	MathTool::Normalize(col_vector);
	Vector3d normal_vector(-projection[0], -projection[1], -projection[2]);
	MathTool::Normalize(normal_vector);
	Vector3d row_vector;
	MathTool::Cross(col_vector, normal_vector, row_vector);
	// 
	row_vector[0] *= -1;
	col_vector[1] *= -1;
	ImageOrientation orientation(row_vector, col_vector);
	image_plane_->SetImageOrientation(orientation);
	// 行列间距
	PixelSpacing2D spacing_2d(output_spacings[0], output_spacings[1]);
	image_plane_->SetSpacing(spacing_2d);	
	// 宽高，单位mm
	image_plane_->SetRowLength(width * output_spacings[0]);
	image_plane_->SetColumnLength(height * output_spacings[1]);
	//////////////////////////////////////////////////////////////////////////
	
}

void VolumeRenderer::SetRenderingMode(RenderMode mode)
{
	switch (mode)
	{
	case RenderMode::SMART_RAYCASTING:
		vtk_volume_mapper_->SetRequestedRenderModeToDefault(); 
		break;
	case RenderMode::RAYCASTING: 
		vtk_volume_mapper_->SetRequestedRenderModeToRayCast(); 
		break;
	case RenderMode::TEXTURE_3D_GPU:
		vtk_volume_mapper_->SetRequestedRenderModeToDefault(); 
		break;
	case RenderMode::RAYCASTING_GPU: 
#if VTK_MAJOR_VERSION > 5
		vtk_volume_mapper_->SetRequestedRenderModeToGPU(); 
#else
		// vtk 5以下需要DXD库支持GPU
		vtk_volume_mapper_->SetRequestedRenderMode(vtkSmartVolumeMapper::RayCastRenderMode);
#endif
		break;
	}
}

void VolumeRenderer::SetBlendMode(BlendMode mode)
{
	bool use_window_level = false;
	switch (mode)
	{
	case BlendMode::Composite: 
		vtk_volume_mapper_->SetBlendModeToComposite(); 
		break;
	case BlendMode::MaximumIntensity: 
		vtk_volume_mapper_->SetBlendModeToMaximumIntensity();
		use_window_level = true;
		break;
	case BlendMode::MinimumIntensity: 
		vtk_volume_mapper_->SetBlendModeToMinimumIntensity(); 
		use_window_level = true;
		break;
	case BlendMode::AverageIntensity: 
		//TODO how to deal with it
		vtk_volume_mapper_->SetBlendModeToAverageIntensity(); 
		break;
	case BlendMode::Additive: 
		//TODO similar with AverageIntensity
		vtk_volume_mapper_->SetBlendModeToAdditive(); 
		break;
	default: 
		break;
	}

	if(use_window_level){
		vtk_volume_property_->SetColor(transfer_function_mip_.TovtkColorTransferFunction());
		vtk_volume_property_->SetScalarOpacity(transfer_function_mip_.vtkScalarOpacityTransferFunction());
		//TODO 不能添加梯度
		//vtk_volume_property_->SetGradientOpacity(transfer_function_mip_.vtkGradientOpacityTransferFunction());
		vtk_volume_property_->SetGradientOpacity(NULL);
	}
	else{
		vtk_volume_property_->SetColor(transfer_function_.TovtkColorTransferFunction());
		vtk_volume_property_->SetScalarOpacity(transfer_function_.vtkScalarOpacityTransferFunction());
		vtk_volume_property_->SetGradientOpacity(transfer_function_.vtkGradientOpacityTransferFunction());
	}

}

void VolumeRenderer::SetIndependentComponents(bool independent)
{
	vtk_volume_property_->SetIndependentComponents(independent);
}

void VolumeRenderer::SetOffScreenRendering(bool flag)
{
	is_off_screen_rendering_ = flag;
	vtk_render_window_->SetOffScreenRendering(flag);
}

bool VolumeRenderer::PickByImagePos(const Vector2i& in_point, Vector3d& out_point)
{
	return false;
}

bool VolumeRenderer::PickByImagePos(const int& x, const int& y, Vector3d& out_point)
{
	return false;
}

bool VolumeRenderer::PickBy3DPoint(const Vector3d& in_point, int& x, int& y)
{
	return false;
}

float VolumeRenderer::GetVoxelVolume()
{
	return 0.0;
}

float VolumeRenderer::GetVoxelVolume(BoundingBox*)
{
	return 0.0;
}

float VolumeRenderer::CalculateLength(Vector3d&, Vector3d&)
{
	return 0.0;
}

void VolumeRenderer::GetOrientationTags(Vector3d& left, Vector3d& right, Vector3d& up, Vector3d& down)
{
	ImageOrientation ori = image_plane_->GetImageOrientation();
	Vector3d row_vector = ori.GetRowVector();
	Vector3d colume_vector = ori.GetColumnVector();

	left = -row_vector;
	right = row_vector;
	up = -colume_vector;
	down = colume_vector;

}

//------------------Set rendering parameters---------------------
void VolumeRenderer::SetShading(bool on)
{
	if (on)
	{
		vtk_volume_property_->ShadeOn();
	}
	else
	{
		vtk_volume_property_->ShadeOff();
	}
}

void VolumeRenderer::SetAmbient(double ambient)
{
	vtk_volume_property_->SetAmbient(ambient);
}

void VolumeRenderer::SetDiffuse(double diffuse)
{
	vtk_volume_property_->SetDiffuse(diffuse);
}

void VolumeRenderer::SetSpecular(double specular)
{
	vtk_volume_property_->SetSpecular(specular);
}

void VolumeRenderer::SetSpecularPower(double power)
{
	vtk_volume_property_->SetSpecularPower(power);
}

void VolumeRenderer::SetClippingPlanes(vtkPlaneCollection *clipping)
{
	vtk_clipping_planes_ = clipping;
	vtk_volume_mapper_->SetClippingPlanes(vtk_clipping_planes_);
}

vtkPlaneCollection* VolumeRenderer::GetClippingPlanes() const
{
	return vtk_clipping_planes_;
}

void VolumeRenderer::SetTransferFunction(const TransferFunction &transferFunction)
{
	transfer_function_ = transferFunction;

	vtk_volume_property_->SetScalarOpacity(transfer_function_.vtkScalarOpacityTransferFunction());
	vtk_volume_property_->SetColor(transfer_function_.TovtkColorTransferFunction());
	vtk_volume_property_->SetGradientOpacity(transfer_function_.vtkGradientOpacityTransferFunction());
}

void VolumeRenderer::SetWindowLevel(int ww, int wl)
{
	int mode = vtk_volume_mapper_->GetBlendMode();
	if (vtkVolumeMapper::BlendModes::MAXIMUM_INTENSITY_BLEND == mode ||
		vtkVolumeMapper::BlendModes::MINIMUM_INTENSITY_BLEND == mode){

			window_level_.SetWindowWidth(ww);
			window_level_.SetWindowLevel(wl);

			int min_ctv = window_level_.GetWindowLevel() - 0.5 * window_level_.GetWindowWidth();
			int max_ctv = window_level_.GetWindowLevel() + 0.5 * window_level_.GetWindowWidth();
			transfer_function_mip_.ClearColor();
			transfer_function_mip_.SetColor(min_ctv, 0.0f, 0.0f, 0.0f);
			transfer_function_mip_.SetColor(max_ctv, 1.0f, 1.0f, 1.0f);

			vtk_volume_property_->SetColor(transfer_function_mip_.TovtkColorTransferFunction());
			vtk_volume_property_->SetScalarOpacity(transfer_function_mip_.vtkScalarOpacityTransferFunction());
	}
	else{
		//TODO VR WW/WL...
	}
}
//------------------------- end ---------------------------------

void VolumeRenderer::ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos)
{
	if (vtk_renderer_)
	{
		double display_arr[3] = {0.0};
		vtk_renderer_->SetWorldPoint(world_pos.x, world_pos.y, world_pos.z, 1.0);
		vtk_renderer_->WorldToDisplay();
		vtk_renderer_->GetDisplayPoint(display_arr);

		display_pos.x = display_arr[0];
		display_pos.y = display_arr[1];
		display_pos.z = display_arr[2];
	}
}

void VolumeRenderer::ComputeWorldToDisplay(double x, double y, double z, double display_point[3])
{
	if (vtk_renderer_)
	{
		vtk_renderer_->SetWorldPoint(x, y, z, 1.0);
		vtk_renderer_->WorldToDisplay();
		vtk_renderer_->GetDisplayPoint(display_point);
	}
}

void VolumeRenderer::FitRenderingIntoViewport(float clip, bool update_to_fit_tissue)
{
	// First we get the bounds of the current rendered item in world coordinates
	double bounds[6];
	vtk_volume_->GetBounds(bounds);
	double top_left[3], bottom_right[3];
	double diminsions[3];
	diminsions[0] = bounds[1] - bounds[0];
	diminsions[1] = bounds[3] - bounds[2];
	diminsions[2] = bounds[5] - bounds[4];
	int i;
	for (i=0; i<3; ++i){
		top_left[i] = bounds[i * 2];
		bottom_right[i] = bounds[i * 2 + 1];
	}

	// 需要计算分割结果的BOUNDING BOX，不然宽度方向过大，造成截取后不能放大到合适的比例
	if (update_to_fit_tissue){		
		bounds[5] = bounds[4] + (bounds[5] - bounds[4]) * clip;
		//TODO 因为有截取的比例，需要确定在哪查找截取后组织的包围盒
		if (abs(1.0f - clip) < MathTool::kEpsilon){
			BoundingBox *box = volume_data_->GetMarkBoundingBox();
			box->SetSpacing(voxel_spacing_);
			box->GetCornerPoints(top_left, bottom_right);
		}
		else{
			BoundingBox *box = volume_data_->GetMarkBoundingBoxClipping();
			box->SetSpacing(voxel_spacing_);
			box->GetCornerPoints(top_left, bottom_right);
		}
	}

	double topCorner[3];
	double bottomCorner[3];
	for (i=0; i < 3; ++i)
	{
		if (bounds[i * 2] < top_left[i]){
			topCorner[i] = top_left[i];
		}
		else{
			topCorner[i] = bounds[i * 2];
		}

		if (bounds[i * 2 + 1] > bottom_right[i]){
			bottomCorner[i] = bottom_right[i];
		}
		else{
			bottomCorner[i] = bounds[i * 2 + 1];
		}
	}
	// 计算对角线方向的长度，以便旋转后图像不出窗口，单位mm
	double xx = topCorner[0] - bottomCorner[0];
	double yy = topCorner[1] - bottomCorner[1];
	double zz = topCorner[2] - bottomCorner[2];
	double max_length_in_mm = sqrt(pow(xx, 2) + pow(yy, 2) + pow(zz, 2));
	
	// 先平移再放缩
	if (update_to_fit_tissue){

		double clip_mark_dims[3];
		clip_mark_dims[0] = bottomCorner[0] - topCorner[0];
		clip_mark_dims[1] = bottomCorner[1] - topCorner[1];
		clip_mark_dims[2] = bottomCorner[2] - topCorner[2];
		float offset[3] = { 0.0f };	
		for (i=0; i<3; ++i){
			offset[i] = (float)((topCorner[i] + clip_mark_dims[i] / 2.0) - (bounds[i * 2] + diminsions[i] / 2.0));
		}
		
		camera_->Move(offset);
	}

	// Scaling the viewport to fit the current item bounds
	double marginRate = 0.0;

	// Calculate width and height in display coordinates
	double displayTopLeft[3], displayBottomRight[3];
	this->ComputeWorldToDisplay(topCorner[0], topCorner[1], topCorner[2], displayTopLeft);
	this->ComputeWorldToDisplay(bottomCorner[0], bottomCorner[1], bottomCorner[2], displayBottomRight);

	// We Recalculem taking into account the display
	double width, height;
	width = fabs(displayTopLeft[0] - displayBottomRight[0]);
	height = fabs(displayTopLeft[1] - displayBottomRight[1]);

	double width_world, height_world;
	width_world = fabs(topCorner[0] - bottomCorner[0]);
	height_world = fabs(topCorner[2] - bottomCorner[2]);

	// 得到显示的最大长度，单位像素
	double tmp_spacing = width_world / width;
	double max_display_length = max_length_in_mm / tmp_spacing;

	
	// Adjust the image depending on whether the window is narrower by width or height. If we want to see the whole region that we have chosen, we will adjust
	// Be narrower, if we adjust through wider image to the other side
	int *win_size = vtk_render_window_->GetSize();
	//double ratio = min( (double)win_size[0] / width, (double)win_size[1] / height);
	// 根据最大长度计算合适的放缩比例
	double ratio = min( (double)win_size[0] / max_display_length, (double)win_size[1] / max_display_length);
	double factor = ratio * (1.0 - marginRate);

	vtk_renderer_->GetActiveCamera()->Zoom(factor);

	//TODO 计算VR的图像的间距
	output_spacings[0] = output_spacings[1] = width_world / width / factor;
}