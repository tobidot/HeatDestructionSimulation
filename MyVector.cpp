#include "MyVector.h"




template<typename T>
const MyVector<T> MyVector<T>::UP = MyVector<T>(T(0), T(-1));
template<typename T>
const MyVector<T> MyVector<T>::DOWN = MyVector<T>(T(0), T(1));
template<typename T>
const MyVector<T> MyVector<T>::LEFT = MyVector<T>(T(-1), T(0));
template<typename T>
const MyVector<T> MyVector<T>::RIGHT = MyVector<T>(T(1), T(0));
