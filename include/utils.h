#pragma once

#include <offload/OffloadAPI.h>

typedef struct {
  enum {
    OK,
    NOT_FOUND,
  } tag;

  ol_device_handle_t device;
} DeviceSelectResult;

DeviceSelectResult findGpuDevice();
char const *getTmpDeviceNameStr(ol_device_handle_t);
