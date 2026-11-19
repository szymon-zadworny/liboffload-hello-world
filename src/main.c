#include <offload/OffloadAPI.h>
#include <stdio.h>

#include "utils.h"

#define ALLOC_SZ 1024
#define GROUP_SZ 16

static const unsigned char binary_image[] = {
#embed "image.bin"
};

int main(void) {
  olInit(nullptr);

  DeviceSelectResult result = findGpuDevice();

  if (result.tag == NOT_FOUND) {
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

  struct {
    int *sum;
    int *v1;
    int *v2;
  } args;

  olMemAlloc(context, device, OL_ALLOC_TYPE_MANAGED, ALLOC_SZ * sizeof(int),
             (void **)&args.sum);

  olMemAlloc(context, device, OL_ALLOC_TYPE_MANAGED, ALLOC_SZ * sizeof(int),
             (void **)&args.v1);

  olMemAlloc(context, device, OL_ALLOC_TYPE_MANAGED, ALLOC_SZ * sizeof(int),
             (void **)&args.v2);

  for (int i = 0; i < ALLOC_SZ; ++i) {
    args.v1[i] = i;
    args.v2[i] = ALLOC_SZ - i;
  }

  ol_program_handle_t program;
  olCreateProgram(context, device, binary_image, sizeof(binary_image),
                  &program);

  ol_symbol_handle_t kernel;
  // Kernel signature:
  // void VectorAdd(struct { int *sum, int *v1, int *v })
  //
  // The arguments are wrapped in a structure because this is how upstream SYCL
  // passes lambda captures to resulting SPIR-V.
  olGetSymbol(program, "VectorAdd", OL_SYMBOL_KIND_KERNEL, &kernel);

  ol_kernel_launch_size_args_t launch_size_args = {
      .Dimensions = 1,
      .NumGroups = {.x = ALLOC_SZ / GROUP_SZ, .y = 1, .z = 1},
      .GroupSize = {.x = GROUP_SZ, .y = 1, .z = 1},
      .DynSharedMemory = 0};

  void *arg_ptrs[] = {&args};
  size_t arg_sizes[] = {sizeof(args)};

  // First nullptr specifies the queue handle. Setting the queue to nullptr
  // means that the kernel launch will happen synchronously.
  //
  // The second nullptr specifies optional launch properties.
  olLaunchKernel(nullptr, device, kernel, &launch_size_args, nullptr, 1,
                 arg_ptrs, arg_sizes);

  for (int i = 0; i < ALLOC_SZ; ++i) {
    printf("%d ", args.sum[i]);
  }

  printf("\n");

  olMemFree(context, args.sum);
  olMemFree(context, args.v1);
  olMemFree(context, args.v2);

  olDestroyProgram(program);
  olDestroyContext(context);
  olShutDown();
}
