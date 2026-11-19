#pragma once

#include <offload/OffloadAPI.h>

typedef struct {
  enum {
    Ok,
    NotFound,
  } tag;

  ol_device_handle_t device;
} DeviceSelectResult;

DeviceSelectResult findGpuDevice();
char const *getTmpDeviceNameStr(ol_device_handle_t);
