#ifndef __NuiDataType_h_
#define __NuiDataType_h_

typedef enum _NuiManagerState{
	NUI_MANAGER_OK,
	NUI_MANAGER_UNINITIATED,
	NUI_MANAGER_API_ERROR,
	NUI_MANAGER_NO_SENSOR,
	NUI_MANAGER_NO_READY_SENSOR
} NuiManagerState;

#endif