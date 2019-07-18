#pragma once

namespace perfectpixel { namespace bedrock {

	template <typename T>
	class Singleton
	{
	private:
		static T *instance;

	protected:
		Singleton() {};

		Singleton(const Singleton &) = delete;
		Singleton(const Singleton &&) = delete;
		Singleton &operator=(const Singleton &&) = delete;
		Singleton &operator==(const Singleton &&) = delete;

	public:
		static T* getInstance() {
			if (!instance)
			{
				instance = new T();
			}
			return instance;
		}
	};
	template<typename T>
	T *Singleton<T>::instance = nullptr;


} }
