#include "core/ResourceLoaders.h"

#include "renderer/ImageResource.h"

#include "stb_image.h"

namespace perfectpixel::core {

void imageLoaderFunc(
    char *data,
    size_t dataSize,
    void **target,
    const bedrock::UniqueVoidPtr &userData,
    void *loaderUserData)
{
    (void)userData;
    (void)loaderUserData;

    renderer::ImageResource *image = new renderer::ImageResource();

    stbi_set_flip_vertically_on_load(true);
    image->buffer = (char *)stbi_load_from_memory(
        (stbi_uc *)data,
        (int)dataSize,
        &image->x,
        &image->y,
        &image->channels,
        0);

    *target = image;
}

void imageUnloaderFunc(void **target)
{
    renderer::ImageResource *image = (renderer::ImageResource *)*target;

    stbi_image_free(image->buffer);

    delete image;
    *target = nullptr;
}

} // namespace perfectpixel::core
