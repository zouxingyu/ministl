#pragma once
#ifndef STRING_H
#define STRING_H
#include<cstring>
#include<utility>
#include<stdexcept>
#include<iostream>

class String {
	friend String operator+(const String& lhs, const String& rhs) {
		String s;
		return s.append(lhs).append(rhs);
	}
	friend bool operator==(const String& lhs, const String& rhs) {
		return strcmp(lhs.p, rhs.p) == 0;
	}
	friend std::ostream& operator<<(std::ostream& os, const String& str) {
		os << str.p;
		return os;
	}
public:
	//iterator
	struct const_iterator {
		friend class String;
	public:
		const char& operator*()const {
			return *ptr;
		}
		const_iterator& operator++() {
			++ptr;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator old(ptr);
			++(*this);
			return old;
		}
		const_iterator& operator--() {
			--ptr;
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator old(ptr);
			--(*this);
			return old;
		}
		bool operator==(const const_iterator& rhs)const {
			return ptr == rhs.ptr;
		}
		bool operator!=(const const_iterator& rhs)const {
			return !(*this == rhs);
		}
	protected:
		char* ptr;
		const_iterator(char* p) :ptr(p) {}
	};

	struct iterator :public const_iterator {
		friend class String;
	public:
		char& operator*() {
			return *ptr;
		}
		const char& operator*()const {
			return *ptr;
		}
		const_iterator& operator++() {
			++ptr;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator old(ptr);
			++(*this);
			return old;
		}
		const_iterator& operator--() {
			--ptr;
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator old(ptr);
			--(*this);
			return old;
		}
	private:
		iterator(char* p) :const_iterator(p) {}
	};
	//constructor
	explicit String() :p(new char[initialcap + 1]), thesize(0), thecapacity(initialcap) {
		*p = '\0';
	}
	String(size_t count, char ch) :p(new char[get_next_cap(count) + 1]), thesize(count), thecapacity(get_next_cap(count)) {
		memset(p, ch, count);
		p[count] = '\0';
	}
	String(const char* s) :p(new char[get_next_cap(strlen(s)) + 1]), thesize(strlen(s)), thecapacity(get_next_cap(strlen(s))) {
		strcpy(p, s);
	}
	String(const String& other, size_t pos, size_t count):p(new char[get_next_cap(count) + 1]), thesize(count), thecapacity(get_next_cap(count)) {
		memcpy(p, other.p + pos, count);
		p[count] = '\0';
	}
	String(const char* s, size_t count) :p(new char[get_next_cap(count) + 1]), thesize(count), thecapacity(get_next_cap(count)) {
		memcpy(p, s, count);
		p[count] = '\0';
	}
	String(const String& other) :p(new char[other.thecapacity + 1]), thesize(other.size()), thecapacity(other.thecapacity) {
		strcpy(p, other.p);
	}
	String(String&& other) :p(other.p), thesize(other.thesize), thecapacity(other.thecapacity) {
		other.p = nullptr;
		thesize = thecapacity = 0;
	}
	template<class Input>
	String(Input first, Input last):p(new char[(last-first)+1]),thesize(last-first+1),thecapacity(get_next_cap(last - first + 1)) {
		size_t pos = 0;
		for (Input it = first; it != last; ++it) {
			p[pos++] = *it;
		}
		p[pos] = '\0';
	}
	//destructor
	~String()
	{
		delete[]p;
	}
	//operator=
	String& operator=(const String& rhs) {
		String copy(rhs);
		swap(copy);
	}
	String& operator=(String&& rhs)noexcept {
		swap(rhs);
	}
	//element access
	char& at(size_t idx) {
		if (idx<0 || idx>size())
			throw std::out_of_range("out of range");
		return p[idx];
	}
	const char& at(size_t idx)const {
		if (idx<0 || idx>size())
			throw std::out_of_range("out of range");
		return p[idx];
	}
	char& operator[](size_t idx) {
		return p[idx];
	}
	const char& operator[](size_t idx)const {
		return p[idx];
	}
	char& front() {
		return p[0];
	}
	const char& front()const {
		return p[0];
	}
	char& back() {
		return p[thesize - 1];
	}
	const char& back()const {
		return p[thesize - 1];
	}
	const char* data() const noexcept {
		return p;
	}
	const char* c_str()const noexcept {
		return p;
	}
	//iterators
	iterator begin()noexcept {
		return p;
	}
	const_iterator begin() const noexcept {
		return p;
	}
	iterator end()noexcept {
		return p + thesize;
	}
	const_iterator end() const noexcept {
		return p + thesize;
	}
	//capacity
	bool empty()const noexcept {
		return thesize == thecapacity;
	}
	size_t size()const noexcept {
		return thesize;
	}
	size_t length()const noexcept {
		return  size();
	}
/*	size_t max_size()const noexcept {

	}
*/
	size_t capacity()const noexcept {
		return thecapacity;
	}
	size_t reserve(size_t newcap) {
		realloc_n(get_next_cap(newcap););
	}
	//operations
	void clear()noexcept {
		*p = '\0';
		thesize = 0;
	}
	String& insert(size_t index, size_t count, char ch) {
		realloc_n_auto(count+thesize);
		char* s = new char[count];
		memset(s, ch, count);
		inserthelper(index, s, count);
		return *this;
	}
	String& insert(size_t index, const char* s) {
		realloc_n_auto(strlen(s)+thesize);
		inserthelper(index, s, strlen(s));
		return *this;
	}
	String& insert(size_t index, const String& str) {
		realloc_n_auto(str.size() + thesize);
		inserthelper(index, str.p, str.size());
		return *this;
	}
	String& insert(size_t index, const char* s, size_t count) {
		inserthelper(index, s, count);
		return *this;
	}
	iterator insert(const_iterator pos, char ch) {
		size_t index = pos.ptr - p;
		insert(index, 1, ch);
		return p + index;
	}
	String& append(size_t count, char ch) {
		return insert(thesize, count, ch);
	}
	String& append(const String& str) {
		return insert(thesize, str);
	}
	String& append(const char* s) {
		return insert(thesize, s);
	}
	String& append(const char* s, size_t count) {
		return insert(thesize, s, count);
	}
	String& operator+=(const String& str) {
		return append(str);
	}
	String& operator+=(char ch) {
		return append(1, ch);
	}
	String& operator+=(const char* s) {
		return append(s);
	}
	void push_back(char ch) {
		if (thesize == thecapacity) {
			realloc_n(thecapacity * 2);
		}
		p[thesize++] = ch;
		p[thesize] = '\0';
	}

	void swap(String& other) {
		std::swap(p, other.p);
		std::swap(thesize, other.thesize);
		std::swap(thecapacity, other.thecapacity);
	}
	int compare(const String& str)const noexcept {
		return strcmp(p, str.p);
	}
	
	String& replace(size_t pos, size_t count, const String& str) {
		realloc_n_auto(str.size() + thesize - count);
		replacehelper(pos, count, str.p, str.size());
		return *this;
	}
	String substr(size_t pos = 0, size_t count = npos)const {
		size_t newsize = (count == npos) ? thesize : count;
		return String(p + pos, p + pos + count);
	}
	//search
	size_t find(const String& str, size_t pos = 0)const noexcept {
		const char* c = strstr(p + pos, str.p);
		return (c == nullptr) ? npos : c - p;
	}

	static constexpr int npos = -1;

private:
	static constexpr size_t initialcap = 15;
	char* p;
	size_t thesize, thecapacity;
	void realloc_n_auto(size_t need) {
		if (need > thecapacity)
			realloc_n(get_next_cap(need));
	}
	void realloc_n(size_t newcap) {
		char* newp = new char[newcap+1];
		//add '\0'
		memcpy(newp, p, thesize+1);
		delete[]p;
		p = newp;
		thecapacity = newcap;
	}
	size_t get_next_cap(size_t k) {
		while (thecapacity < k) {
			thecapacity *= 2;
		}
		return thecapacity;
	}
	void inserthelper(size_t index, const char* s, size_t sz) {
		for (size_t i = thesize; i >= index; --i) {
			p[i + sz] = p[i];
		}
		memcpy(p + index, s, sz);
	}
	void replacehelper(size_t index, size_t count, const char* s, size_t sz) {
		if (sz < count) {
			int offset = sz - count;
			for (size_t i = index + count; i < thesize; ++i) {
				p[i + offset] = p[i];
			}
		}
		else {
			int offset = sz - count;
			for (size_t i = thesize; i >=index; --i) {
				p[i + offset] = p[i];
			}
		}
		memcpy(p + index, s, sz);
	}
	
};
#endif // !STRING_H
