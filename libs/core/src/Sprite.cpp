#include "core/Sprite.h"

#include "renderer/ImageResource.h"

namespace perfectpixel::core {

namespace {
auto KEY_IMAGE     = PP_KEY(Image);
auto KEY_TOPLEFT   = PP_KEY(UVTopLeft);
auto KEY_FRAMESIZE = PP_KEY(UVFrameSize);
auto KEY_FRAMES    = PP_KEY(Frames);
auto KEY_PERROW    = PP_KEY(FramesPerRow);
auto KEY_SPACING   = PP_KEY(UVFrameSpacing);
} // namespace

perfectpixel::core::ResourceManager::ResourceLoaderFunction
Sprite::CreateSpriteLoader()
{
    return [](char *data,
              size_t dataSize,
              void **target,
              const bedrock::UniqueVoidPtr &,
              void *loaderUserData) {
        serialization::SerializerFactory factory
            = reinterpret_cast<serialization::SerializerFactory>(
                loaderUserData);
        serialization::ISerializer *serializer = factory();
        serializer->loadBuffer(data, dataSize);

        Sprite *res = new Sprite();
        *serializer >> (*res);
        *target = res;
        delete serializer;
    };
}

Sprite::Sprite()
    : m_image(bedrock::typeID<renderer::ImageResource>())
{}

Sprite::Sprite(
    const Resource &image,
    bedrock::Vector2 topLeft,
    bedrock::Vector2 size,
    int32_t frames /* = 1 */,
    int32_t framesPerRow /* = 0 */,
    bedrock::Vector2 frameSpacing /* = bedrock::Vector2() */)
    : m_textureTopLeft(topLeft)
    , m_textureSize(size)
    , m_frames(frames)
    , m_framesPerRow(framesPerRow)
    , m_frameSpacing(frameSpacing)
    , m_image(bedrock::typeID<renderer::ImageResource>())
{
    m_image = image;
    if (m_framesPerRow <= 0)
        m_framesPerRow = m_frames;
}

Sprite::~Sprite()
{}

Resource Sprite::getImage() const
{
    return m_image;
}

bedrock::Vector2 Sprite::getTexCoord(int32_t frame /*= 0*/) const
{
    frame %= m_frames;
    bedrock::Vector2 step = m_textureSize + m_frameSpacing;

    return bedrock::Vector2{
        m_textureTopLeft.x + (frame % m_framesPerRow) * step.x,
        m_textureTopLeft.y + (frame / m_framesPerRow) * step.y};
}

bedrock::Vector2 Sprite::getSize() const
{
    return m_textureSize;
}

int32_t Sprite::getFrameNumber() const
{
    return m_frames;
}

void Sprite::serialize(serialization::ISerializer &serializer) const
{
    serializer.writeMapStart();

    serializer.writeMapKey(KEY_IMAGE);
    serializer << m_image;

    if (m_image.isValid())
    {
        serializer.writeMapKey(KEY_TOPLEFT);
        serializer << m_textureTopLeft;

        serializer.writeMapKey(KEY_FRAMESIZE);
        serializer << m_textureSize;

        serializer.writeMapKey(KEY_SPACING);
        serializer << m_frameSpacing;

        serializer.writeMapKey(KEY_FRAMES);
        serializer << m_frames;

        serializer.writeMapKey(KEY_PERROW);
        serializer << m_framesPerRow;
    }

    serializer.writeMapEnd();
}

void core::Sprite::deserialize(serialization::ISerializer &serializer)
{
    serializer.readMapBegin();
    int32_t key;

    while (serializer.readMapKey(&key))
    {
        if (PP_KEY_EQUAL(KEY_IMAGE, key))
        {
            serializer >> m_image;
        }
        else if (PP_KEY_EQUAL(KEY_TOPLEFT, key))
        {
            serializer >> m_textureTopLeft;
        }
        else if (PP_KEY_EQUAL(KEY_FRAMESIZE, key))
        {
            serializer >> m_textureSize;
        }
        else if (PP_KEY_EQUAL(KEY_SPACING, key))
        {
            serializer >> m_frameSpacing;
        }
        else if (PP_KEY_EQUAL(KEY_FRAMES, key))
        {
            serializer >> m_frames;
        }
        else if (PP_KEY_EQUAL(KEY_PERROW, key))
        {
            serializer >> m_framesPerRow;
        }
    }
}

} // namespace perfectpixel::core

namespace perfectpixel { namespace serialization {
ISerializer &
operator<<(ISerializer &serializer, const perfectpixel::core::Sprite &sprite)
{
    sprite.serialize(serializer);
    return serializer;
}

ISerializer &
operator>>(ISerializer &serializer, perfectpixel::core::Sprite &sprite)
{
    sprite.deserialize(serializer);
    return serializer;
}
}} // namespace perfectpixel::serialization
