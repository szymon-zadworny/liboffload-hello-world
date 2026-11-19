#include <stdio.h>
#include <offload/OffloadAPI.h>

static const unsigned char binary_image[] = {
    #embed "image.bin"
};

int main(void) {
    olInit(nullptr);

    printf("Image size: %lu bytes\n\n", sizeof(binary_image));

    for (size_t i = 0; i < sizeof(binary_image); ++i) {
        printf("%x ", binary_image[i]);
    }

    printf("\n");

    olShutDown();
}
