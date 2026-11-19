#include <offload/OffloadAPI.h>
#include <stdio.h>

#include "utils.h"

#define ALLOC_SZ 1024

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

  ol_context_handle_t context;
  olCreateContext(1, &device, &context);

  void *tmp;

  int *v1;
  olMemAlloc(context, device, OL_ALLOC_TYPE_MANAGED, ALLOC_SZ * sizeof(int),
             &tmp);
  v1 = tmp;

  int *v2;
  olMemAlloc(context, device, OL_ALLOC_TYPE_MANAGED, ALLOC_SZ * sizeof(int),
             &tmp);
  v2 = tmp;

  int *v3;
  olMemAlloc(context, device, OL_ALLOC_TYPE_MANAGED, ALLOC_SZ * sizeof(int),
             &tmp);
  v3 = tmp;

  for (int i = 0; i < ALLOC_SZ; ++i) {
    v1[i] = i;
    v2[i] = ALLOC_SZ - i;
  }

  olShutDown();
}
