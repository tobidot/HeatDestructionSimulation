#pragma once
#include <inttypes.h>

template<typename T>
struct MyVector : public std::pair<T, T> {
private:
	using SUPER = std::pair<T, T>;
	using SELF = MyVector<T>;
public:
	MyVector() : SUPER() {};
	MyVector(const SELF& cpy) : SUPER(cpy) {};
	MyVector(const SUPER& cpy) : SUPER(cpy) {};
	MyVector(T x, T y) : SUPER(x, y) {};
	MyVector(const std::array<T,2> &list) : SUPER(list) {};
	inline T get_x() const { return this->first; }
	inline T get_y() const { return this->second; }
	inline SELF& set_x(T x) { this->first = x; return *this; }
	inline SELF& set_y(T y) { this->second = y; return *this; }
	const SELF left(T amount = 1) const { return SELF(get_x() - amount, get_y()); }
	const SELF right(T amount = 1) const { return SELF(get_x() + amount, get_y()); }
	const SELF top(T amount = 1) const { return SELF(get_x(), get_y() - amount); }
	const SELF bottom(T amount = 1) const { return SELF(get_x(), get_y() + amount); }

public:
	SELF& operator=(const SELF& other) { return set_x(other.get_x()).set_y(other.get_y()); }
	SELF operator+(const SELF& other) const { return SELF(get_x() + other.get_x(), get_y() + other.get_y()); }
	SELF operator-(const SELF& other) const { return SELF(get_x() - other.get_x(), get_y() - other.get_y()); }
	SELF operator*(const T mul) const { return SELF(get_x() * mul, get_y() * mul); }
	SELF operator/(const T div) const { return SELF(get_x() / div, get_y() / div); }
	template<typename R = T>
	R length2() const { return get_x() * get_x() + get_y() * get_y(); }
	template<typename R = T>
	R length() const { return sqrt(length2()); }
};

using Impuls = MyVector<float>;
using Position = MyVector<int16_t>;

using Pressure = int16_t;
using Mass = int16_t;
using Heat = float;