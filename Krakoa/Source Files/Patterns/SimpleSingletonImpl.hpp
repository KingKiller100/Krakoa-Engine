#pragma once
#include <Patterns/SimpleSingleton.hpp>

template<class T>
T* pattern::SimpleSingleton<T>::instance = nullptr;
