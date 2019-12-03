#pragma once
#ifndef VECTOR_H
#define VECTOR_H
#include<stdexcept>
#include<utility>
template<class Object>
class Vector {
public:
	class const_iterator {

	};
	class iterator :public const_iterator {

	};
	Vector() :ptr(nullptr), thesize(0), thecapacity(0) {}
	explicit Vector(size_t count, const Object& value) :ptr(new Object[count]),thesize(count),thecapacity(count) {
		for (size_t i = 0; i < count; ++i)
			ptr[i] = value;
	}
	Vector(const Vector& other) :ptr(new Object[other.thesize]{}), thesize(other.thesize), thecapacity(other.thesize) {
		for (int i = 0; i < other.thesize; ++i)
			ptr[i] = other.ptr[i];
	}
	Vector(Vector&& other) :ptr(other.ptr), thesize(other.thesize), thecapacity(other.thecapacity) {
		other.ptr = nullptr;
	}
	Vector& operator=(const Vector& other) {
		vector copy(other);
		swap(copy);
		return *this;
	}
	Vector& operator=(Vector&& other) {
		swap(other);
		return *this;
	}
	~Vector()
	{
		delete[]ptr;
	}
	Object& at(size_t pos) {
		if (pos < 0 || pos >= thesize)
			throw std::out_of_range("access failed");
		return ptr[pos];
	}
	Object& operator[](size_t pos) {
		return ptr[pos];
	}
	Object& front() {
		return ptr[0];
	}
	Object& back() {
		return ptr[thesize - 1];
	}
	iterator begin() {
		return ptr;
	}
	const_iterator begin()const {
		return ptr;
	}
	iterator end() {
		return ptr;
	}
	const_iterator end()const {
		return ptr;
	}
	bool empty() const noexcept{
		return thesize == 0;
	}
	size_t size() const noexcept{
		return thesize;
	}
	size_t capacity()const noexcept {
		return thecapacity;
	}
	void reserve(size_t count) {
		if (count > thecapacity) {
			realloc_n(get_next_cap(count));
		}	
	}
	void makeEmpty() {
		delete[]ptr;
	}

private:
	Object* ptr;
	size_t thesize;
	size_t thecapacity;
	size_t get_next_cap(size_t need) {
		while (thecapacity < need) {
			thecapacity *= 2;
		}
		return thecapacity;
	}
	void realloc_n(size_t newcap) {
		Object* tmp = new Object[newcap];
		for (size_t i = 0; i < thesize; ++i) {
			tmp[i] = std::move(ptr[i]);
		}
		delete[]ptr;
		ptr = tmp;
		thecapacity = newcap;
	}
};
#endif // !VECTOR_H
