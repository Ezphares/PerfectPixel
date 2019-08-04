#pragma once

#include <Resources/Resource.h>
#include <Resources/ResourceManager.h>

#include <Bedrock/vectors.h>

namespace perfectpixel { namespace resources {

	class Sprite
	{
	public:
		static ResourceManager::ResourceLoaderFunction CreateSpriteLoader(std::function<serialization::ISerializer*()> provider);

	public:
		Sprite();
		Sprite(
			const Resource &image,
			bedrock::Vector2 topLeft,
			bedrock::Vector2 size,
			int32_t frames = 1,
			int32_t framesPerRow = 0,
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

		bool m_valid;
	};




} }

PP_TYPE_REFLECTION(::perfectpixel::resources::Sprite, Sprite);

perfectpixel::serialization::ISerializer &operator<<(perfectpixel::serialization::ISerializer &serializer, const perfectpixel::resources::Sprite &sprite);
perfectpixel::serialization::ISerializer &operator>>(perfectpixel::serialization::ISerializer &serializer, perfectpixel::resources::Sprite &sprite);
