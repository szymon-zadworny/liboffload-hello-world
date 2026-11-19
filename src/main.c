#include <offload/OffloadAPI.h>
#include <stdio.h>

#include "utils.h"

static const unsigned char binary_image[] = {
#embed "image.bin"
};

int main(void) {
  olInit(nullptr);

  DeviceSelectResult result = findGpuDevice();

  if (result.tag == Ok) {
    printf("Found device: [%s]\n", getTmpDeviceNameStr(result.device));
  }

  olShutDown();
}
