#pragma once

#include <type_traits>

template <typename T>
class ScopedTransaction
{
	static_assert(std::is_copy_assignable_v<T>);
	static_assert(std::is_copy_constructible_v<T>);

public:
	ScopedTransaction(T &target)
		: m_target(target)
		, m_original(target)
		, m_isCommitted(false)
	{}

	~ScopedTransaction()
	{
		if (!m_isCommitted)
		{
			revert();
		}
	}

	ScopedTransaction(const ScopedTransaction &) = delete;
	ScopedTransaction(ScopedTransaction &&) = delete;
	ScopedTransaction &operator=(const ScopedTransaction &) = delete;
	ScopedTransaction &operator=(ScopedTransaction &&) = delete;

	void commit()
	{
		m_isCommitted = true;
	}

	void revert()
	{
		m_target = m_original;
	}

private:
	T& m_target;
	T m_original;
	bool m_isCommitted;
};
