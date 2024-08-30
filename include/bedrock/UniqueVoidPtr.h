#pragma once

#include <cstddef>
#include <cstring>
#include <utility>

namespace perfectpixel { namespace bedrock {

class UniqueVoidPtr
{
private:
    static constexpr size_t opaqueInlineBytes = 16;

public:
    UniqueVoidPtr();
    ~UniqueVoidPtr();
    UniqueVoidPtr(const UniqueVoidPtr &)            = delete;
    UniqueVoidPtr &operator=(const UniqueVoidPtr &) = delete;
    UniqueVoidPtr(UniqueVoidPtr &&rhs);
    UniqueVoidPtr &operator=(UniqueVoidPtr &&rhs);

    template <typename T, typename... ArgT>
    static UniqueVoidPtr create(ArgT &&...args)
    {
        if constexpr (
            sizeof(T) > opaqueInlineBytes || alignof(T) > alignof(void *))
        {
            return UniqueVoidPtr(
                new T(std::forward<ArgT>(args)...),
                [](void *raw) { delete reinterpret_cast<T *>(raw); });
        }
        else
        {
            UniqueVoidPtr opaque;
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
    UniqueVoidPtr(void *data, void (*deleter)(void *));

    void destroy();

    void *m_data;
    void (*m_deleter)(void *);
    char m_inlineData[opaqueInlineBytes];
};

}} // namespace perfectpixel::bedrock
