/****************************************************************************************************

 * \file global_define.h
 * \date 2019/06/05 11:18
 *
 * \author jiayf
 * Contact: @deepwise.com
 *
 * \brief  

 Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once
#define USE_GOOGLE_BREAKPAD_LIB (0)   //dump log

#define USE_JSONCPP_LIB   0

#define JSON_KEY_DICOM_PATH				"dicom_path"
#define JSON_KEY_MASK_PATH				"mask_path"
#define JSON_KEY_CURVE_PATH				"curve_path"
#define JSON_KEY_PATIENT_ID				"patient_id"
#define JSON_KEY_STUDY_UID				"study_uid"
#define JSON_KEY_SERIRES_UID			"series_uid"

#define JSON_KEY_IMAGE_TYPE 			"image_type"
#define JSON_KEY_VESSEL_NAME			"vessel_name"
#define JSON_KEY_BLEND_MODE				"blend_mode"
#define JSON_KEY_GENERATE_RULE			"generate_rule"
#define JSON_KEY_INIT_ORIENTATION		"init_orientation"
#define JSON_KEY_CLIP_PERCENT			"clip_percent"
#define JSON_KEY_THICKNESS				"thickness"
#define JSON_KEY_SPACING_BETWEEN_SLICES	"spacing_between_slices"
#define JSON_KEY_OUTPUT_IMAGE_NUMBER	"output_image_number"
#define JSON_KEY_OUTPUT_PATH			"output_path"
#define JSON_KEY_WINDOW_WIDTH			"window_width"
#define JSON_KEY_WINDOW_LEVEL			"window_level"
#define JSON_KEY_ROTATION_DIRECTION		"rotation_direction"

#define IMAGE_WINDOW_NAME_VR            "vr"
#define IMAGE_WINDOW_NAME_MPR           "mpr1"
#define IMAGE_WINDOW_NAME_CPR           "cpr"

#define RET_STATUS_SUCCESS                          (1)
// Error       ( -1~ -99)
#define RET_STATUS_ENGINE_INPUT_INVALID_PARAM       (-1)
#define RET_STATUS_ENGINE_RENDER_INVALID_SIZE       (-2)
#define RET_STATUS_ENGINE_EXCEPTION_OUT_OF_RANGE    (-3)
#define RET_STATUS_ENGINE_EXCEPTION_OVERFLOW        (-4)
#define RET_STATUS_ENGINE_EXCEPTION_UNLOAD_DATA     (-5)
#define RET_STATUS_ENGINE_EXCEPTION_RUNTIME_ERROR   (-6)
#define RET_STATUS_ENGINE_EXCEPTION_UNKNOWN         (-7)
#define RET_STATUS_ACQUIRE_VOLUME_DATA_ERROR        (-8)   
#define RET_STATUS_GENERATOR_DICOM_PART_ERROR       (-9)
#define RET_STATUS_POINTER_IS_NULL                  (-10)
#define RET_STATUS_PARAS_ERROR                      (-11)
#define RET_STATUS_FAILURE                          (-12)

// Retry error (-100~-199)
#define RET_STATUS_ENGINE_EXCEPTION_OUT_OF_MEMORY   (-100)

// Fatal error (-200~-299)
#define RET_STATUS_DICOM_NOT_FOUND_FILE             (-200)      
#define RET_STATUS_DICOM_NOT_SLICE                  (-201)      
#define RET_STATUS_DICOM_NOT_TAGS                   (-202)
#define RET_STATUS_ENGINE_DATA_PIXELDATA_NULL       (-203)
#define RET_STATUS_JSON_PARSE_FAIL                  (-204)
#define RET_STATUS_JSON_VALUE_TYPE_ERROR            (-205)
#define RET_STATUS_JSON_NAME_NOT_FOUND              (-206)
#define RET_STATUS_DICOM_NOT_FIND_PIXELDATA         (-207)
#define RET_STATUS_DICOM_FORMAT_ERROR               (-208)
#define RET_STATUS_DICOM_LOAD_SERIES                (-209)


//  第一级 "请求类型"    的枚举
enum JSON_VALUE_REQUEST_TYPE{
    JSON_VALUE_REQUEST_TYPE_VR = 0,
    JSON_VALUE_REQUEST_TYPE_MPR,
    JSON_VALUE_REQUEST_TYPE_CPR,

    JSON_VALUE_REQUEST_TYPE_MAX
};

// #define MASK_FILE_NAME                      "/maks_data.nii"
#define CURVE_FILE_PAHT                     "/vesselpoint.txt"

#define VESSEL_NAME_L_ICA   "L_ICA"
#define VESSEL_NAME_R_ICA   "R_ICA"
#define VESSEL_NAME_L_VA    "L_VA"
#define VESSEL_NAME_R_VA    "R_VA"
