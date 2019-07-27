#pragma once

#include <memory>

namespace perfectpixel { namespace bedrock {

	class Opaque
	{
	public:
		Opaque()
			: m_raw(nullptr)
		{}

		template<typename T>
		static Opaque Create(const T &data)
		{
			return Opaque(new T(data), [](void *raw){ delete reinterpret_cast<T*>(raw); });
		}

		template<typename T>
		T *get()
		{
			return m_raw ? reinterpret_cast<T *>(m_raw.get()) : nullptr;
		}

		template<typename T>
		const T *get() const
		{
			return m_raw ? reinterpret_cast<T *>(m_raw.get()) : nullptr;
		}

	private:
		Opaque(void *data, void(*deleter)(void*))
			: m_raw(data, deleter)
		{
		}
		std::shared_ptr<void> m_raw;
	};
} }
