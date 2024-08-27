#pragma once

#include <utility>

namespace perfectpixel { namespace bedrock {

class Opaque
{
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
        return Opaque(new T(std::forward<ArgT>(args)...), [](void *raw) {
            delete reinterpret_cast<T *>(raw);
        });
    }

    template <typename T>
    const T &get() const
    {
        return *reinterpret_cast<T *>(m_data);
    }

    operator bool() const;

private:
    Opaque(void *data, void (*deleter)(void *));

    void destroy();

    void *m_data;
    void (*m_deleter)(void *);
};

template <typename T>
const T &getOpaque(const Opaque &opaque)
{
    return *reinterpret_cast<const T *>(opaque->get());
}

}} // namespace perfectpixel::bedrock
