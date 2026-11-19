#include "utils.h"

bool deviceSelector(ol_device_handle_t device, void *user_data) {
  DeviceSelectResult *result = user_data;

  ol_device_type_t device_type;
  olGetDeviceInfo(device, OL_DEVICE_INFO_TYPE, sizeof(device_type),
                  &device_type);

  ol_platform_handle_t platform_handle;
  olGetDeviceInfo(device, OL_DEVICE_INFO_PLATFORM, sizeof(platform_handle),
                  &platform_handle);

  ol_platform_backend_t backend;
  olGetPlatformInfo(platform_handle, OL_PLATFORM_INFO_BACKEND, sizeof(backend),
                    &backend);

  if (device_type == OL_DEVICE_TYPE_GPU &&
      backend == OL_PLATFORM_BACKEND_LEVEL_ZERO) {
    result->tag = OK;
    result->device = device;

    return false;
  }

  return true;
}

DeviceSelectResult findGpuDevice() {
  DeviceSelectResult result = {.tag = NOT_FOUND};

  olIterateDevices(deviceSelector, &result);

  return result;
}

char const *getTmpDeviceNameStr(ol_device_handle_t device) {
  static char name[1024];
  size_t name_length;
  olGetDeviceInfoSize(device, OL_DEVICE_INFO_NAME, &name_length);
  olGetDeviceInfo(device, OL_DEVICE_INFO_NAME, name_length, name);

  return name;
}
