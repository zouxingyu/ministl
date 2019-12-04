#pragma once
#ifndef VECTOR_H
#define VECTOR_H
#include<stdexcept>
#include<utility>
template<class Object>class Vector;
template<class Object>
bool operator==(const Vector<Object>& lhs, const Vector<Object>& rhs) {
	if (lhs.size() != rhs.size())
		return false;
	for (size_t p = 0; p != lhs.size(); ++p) {
		if (lhs.ptr[p] != rhs.ptr[p])
			return false;
	}
	return true;
}
template<class Object>
bool operator!=(const Vector<Object>& lhs, const Vector<Object>& rhs) {
	return !(lhs == rhs);
}
template<class Object>
bool operator<(const Vector<Object>& lhs, const Vector<Object>& rhs) {
	size_t p1 = 0, p2 = 0;
	while (p1 < lhs.size() && p2 < rhs.size()) {
		if (lhs.ptr[p1] < rhs.ptr[p2])
			return true;
		else if (lhs.ptr[p1] > rhs.ptr[p2])
			return false;
		++p1;
		++p2;
	}
	if (p2 < rhs.size())
		return true;
	else
		return false;
}
template<class Object>
bool operator>(const Vector<Object>& lhs, const Vector<Object>& rhs) {
	return (rhs < lhs);
}
template<class Object>
void swap(Vector<Object>& lhs, Vector<Object>& rhs) {
	lhs.swap(rhs);
}
template<class Object>
class Vector {
	friend bool operator==<>(const Vector& lhs, const Vector& rhs);
	friend bool operator!=<>(const Vector& lhs, const Vector& rhs);
	friend bool operator< <>(const Vector& lhs, const Vector& rhs);
	friend bool operator> <>(const Vector& lhs, const Vector& rhs);
	friend void swap<>(Vector<Object>& lhs, Vector<Object>& rhs);
public:
	class const_iterator {
		friend class Vector<Object>;
	protected:
		Object* p;
		const_iterator(Object*pp):p(pp){}
	public:
		const Object& operator*()const {
			return *p;
		}
		const_iterator& operator++() {
			++p;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator old = *this;
			++(*this);
			return old;
		}
		const_iterator& operator--() {
			--p;
			return *this;
		}
		const_iterator& operator--(int) {
			const_iterator old = *this;
			--(*this);
			return old;
		}
		int operator-(const const_iterator& rhs) {
			return p - rhs.p;
		}
		bool operator==(const const_iterator& rhs) {
			return p == rhs.p;
		}
		bool operator!=(const const_iterator& rhs) {
			return !(*this == rhs);
		}
		bool operator<(const const_iterator& rhs) {
			return p < rhs.p;
		}
		bool operator>(const const_iterator& rhs) {
			return rhs < *this;
		}
	};
	class iterator :public const_iterator {
		friend class Vector<Object>;
	private:
		iterator(Object*pp):const_iterator(pp){}
	public:
		Object& operator*()const {
			return *this->p;
		}
		iterator& operator++() {
			++this->p;
			return *this;
		}
		iterator operator++(int) {
			iterator old = *this;
			++(*this);
			return old;
		}
		iterator& operator--() {
			--this->p;
			return *this;
		}
		iterator& operator--(int) {
			iterator old = *this;
			--(*this);
			return old;
		}
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
		thesize = 0;
		thecapacity = 0;
	}
	Vector& operator=(const Vector& other) {
		Vector copy(other);
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
		return ptr+thesize;
	}
	const_iterator end()const {
		return ptr+thesize;
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
			realloc_n(count);
		}	
	}
	void makeEmpty() {
		delete[]ptr;
	}
	iterator insert(const_iterator pos, const Object& value) {
		int idx = pos.p - ptr;
		if (thesize == thecapacity)
			realloc_n(get_next_cap(thesize + 1));
		for (int it = thesize; it > idx; --it) {
			ptr[it] = std::move(ptr[it - 1]);
		}
		ptr[idx] = value;
		++thesize;
		return &ptr[idx];
	}
	void push_back(const Object& value) {
		if(thesize==thecapacity)
			realloc_n(get_next_cap(thesize + 1));
		ptr[thesize++] = value;
	}
	void pop_back() {
		--thesize;
	}
	void swap(Vector& rhs) {
		std::swap(ptr, rhs.ptr);
		std::swap(thesize, rhs.thesize);
		std::swap(thecapacity, rhs.thecapacity);
	}
	void resize(size_t count) {
		if (count > thecapacity)
			realloc_n(count);
		if (count < thesize)
			thesize = count;
		else if (count > thesize) {
			for (size_t it = thesize; it < count; ++it)
				ptr[it] = Object{};
			thesize = count;
		}
	}
private:
	Object* ptr;
	size_t thesize;
	size_t thecapacity;
	size_t get_next_cap(size_t need) {
		if (thecapacity == 0)return 1;
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
