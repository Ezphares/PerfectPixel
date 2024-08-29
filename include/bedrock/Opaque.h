#pragma once

#include <cstddef>
#include <cstring>
#include <utility>

namespace perfectpixel { namespace bedrock {

class Opaque
{
private:
    static constexpr size_t opaqueInlineBytes = 16;

public:
    Opaque();
    ~Opaque();
    Opaque(const Opaque &)            = delete;
    Opaque &operator=(const Opaque &) = delete;
    Opaque(Opaque &&rhs);
    Opaque &operator=(Opaque &&rhs);

    template <typename T, typename... ArgT>
    static Opaque create(ArgT &&...args)
    {
        if constexpr (
            sizeof(T) > opaqueInlineBytes || alignof(T) > alignof(void *))
        {
            return Opaque(new T(std::forward<ArgT>(args)...), [](void *raw) {
                delete reinterpret_cast<T *>(raw);
            });
        }
        else
        {
            Opaque opaque;
            opaque.m_data
                = new (opaque.m_inlineData) T(std::forward<ArgT>(args)...);
            opaque.m_deleter
                = [](void *raw) { delete reinterpret_cast<T *>(raw); };
            return opaque;
        }
    }
    template <typename T>
    const T &get() const
    {
        return *reinterpret_cast<T *>(m_data);
    }

    template <typename T>
    T &get()
    {
        return *reinterpret_cast<T *>(m_data);
    }

    operator bool() const;

private:
    Opaque(void *data, void (*deleter)(void *));

    void destroy();

    void *m_data;
    void (*m_deleter)(void *);
    char m_inlineData[opaqueInlineBytes];
};

template <typename T>
const T &getOpaque(const Opaque &opaque)
{
    return *reinterpret_cast<const T *>(opaque->get());
}
}} // namespace perfectpixel::bedrock
