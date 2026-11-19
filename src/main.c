#include <offload/OffloadAPI.h>
#include <stdio.h>

#include "utils.h"

static const unsigned char binary_image[] = {
#embed "image.bin"
};

int main(void) {
  olInit(nullptr);

  DeviceSelectResult result = findGpuDevice();

  if (result.tag == NotFound) {
    fprintf(stderr, "Couldn't find a Level Zero GPU device.\n");
    return 1;
  }

  ol_device_handle_t device = result.device;
  bool isValid;
  olIsValidBinary(device, binary_image, sizeof(binary_image), &isValid);

  if (!isValid) {
    fprintf(stderr, "The provided binary cannot be loaded onto the device.\n");
    return 2;
  }

  olShutDown();
}
