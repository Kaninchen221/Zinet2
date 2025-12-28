#pragma once

#include <iterator>

namespace zt::core
{
	template<typename T>
	concept STDContainer = requires(T t) {
		typename T::value_type;
		std::begin(t);
		std::end(t);
		t.begin();
		t.end();
	};

	// TODO: Rename to NonSTDContainer
	template<typename T>
	concept NotSTDContainer = !STDContainer<T>;

	template<class T>
	concept LikeSmartPointer = requires(T & t) {
		{ t.operator ->() };
		{ t.operator bool() };
	};

	template<class T>
	concept NotLikeSmartPointer = !LikeSmartPointer<T>;

	template<class T>
	concept LikePair = requires(T & t) {
		t.first;
		t.second;
	};
	template<class T>
	concept NotLikePair = !LikePair<T>;
}