#pragma once

#include <cstdlib>

template <typename T, size_t N> class Array
{
public:
	using size_type = size_t;
	using reference = T&;
	using const_reference = const T&;

	reference operator [](size_type n);
	const_reference operator [](size_type n) const;

private:
	T m_data[N];
};

template <typename T, size_t N>
typename Array<T, N>::reference Array<T, N>::operator [](size_type n)
{
	return m_data[n];
}

template <typename T, size_t N>
typename Array<T, N>::const_reference Array<T, N>::operator [](size_type n) const
{
	return m_data[n];
}
