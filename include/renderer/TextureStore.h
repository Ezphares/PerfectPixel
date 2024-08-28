#pragma once

#include "Texture.h"

#include "bedrock/Hash.h"

#include <unordered_map>

namespace perfectpixel::renderer {
class TextureStore
{
public:
    using TextureID     = bedrock::ID;
    using TextureBundle = std::unordered_map<TextureID, Texture>;

    std::unordered_map<ImageResourceBundleID, TextureBundle> m_data;

    const Texture *
    getTexture(ImageResourceBundleID bundleID, TextureID id) const
    {
        auto bundleIt = m_data.find(bundleID);

        if (bundleIt == m_data.end())
        {
            return nullptr;
        }

        auto textureIt = bundleIt->second.find(id);

        return textureIt != bundleIt->second.end() ? &(textureIt->second)
                                                   : nullptr;
    }

    template <class... TArgs>
    const Texture &
    createTexture(ImageResourceBundleID bundleID, TextureID id, TArgs &&...args)
    {
        return getBundle(bundleID)
            .insert_or_assign(id, Texture(std::forward<TArgs>(args)...))
            .first->second;
    }

    void destroyBundle(ImageResourceBundleID bundleID)
    {
        m_data.erase(bundleID);
    }

private:
    inline TextureBundle &getBundle(ImageResourceBundleID bundleID)
    {
        return m_data.insert({bundleID, TextureBundle{}}).first->second;
    }
};
} // namespace perfectpixel::renderer
