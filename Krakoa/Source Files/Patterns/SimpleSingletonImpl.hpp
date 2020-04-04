#pragma once
#include "SimpleSingleton.hpp"

template<class T>
T* pattern::SimpleSingleton<T>::instance = nullptr;
