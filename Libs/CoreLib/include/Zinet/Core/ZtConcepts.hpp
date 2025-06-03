#pragma once

#include <iterator>

template<typename T>
concept STDContainer = requires(T t) {
	typename T::value_type;
	std::begin(t);
	std::end(t);
	t.begin();
	t.end();
};

template<typename T>
concept NotSTDContainer = !STDContainer<T>;