#pragma once

#include "core/Resource.h"
#include "core/ResourceManager.h"

#include "bedrock/vectors.h"

namespace perfectpixel::core {

class Sprite
{
public:
    static ResourceManager::ResourceLoaderFunction CreateSpriteLoader();

public:
    Sprite();
    Sprite(
        const Resource &image,
        bedrock::Vector2 topLeft,
        bedrock::Vector2 size,
        int32_t frames                = 1,
        int32_t framesPerRow          = 0,
        bedrock::Vector2 frameSpacing = bedrock::Vector2());
    virtual ~Sprite();

    Resource getImage() const;
    bedrock::Vector2 getTexCoord(int32_t frame = 0) const;
    bedrock::Vector2 getSize() const;
    int32_t getFrameNumber() const;

public:
    void serialize(serialization::ISerializer &serializer) const;
    void deserialize(serialization::ISerializer &serializer);

private:
    bedrock::Vector2 m_textureTopLeft;
    bedrock::Vector2 m_textureSize;

    int32_t m_frames;
    int32_t m_framesPerRow;
    bedrock::Vector2 m_frameSpacing;

    Resource m_image;

    // bool m_valid;
};

} // namespace perfectpixel::core

PP_TYPE_REFLECTION(::perfectpixel::core::Sprite, Sprite);
namespace perfectpixel { namespace serialization {
ISerializer &
operator<<(ISerializer &serializer, const perfectpixel::core::Sprite &sprite);
ISerializer &
operator>>(ISerializer &serializer, perfectpixel::core::Sprite &sprite);
}} // namespace perfectpixel::serialization