#pragma once
#ifndef STRING_H
#define STRING_H
#include<cstring>
#include<utility>
#include<stdexcept>
class String {
public:
	//constructor
	explicit String(){
		*beg = '\0';
		ed = cap = beg;
	}
	String(size_t count, char ch) :beg(new char[count + 1]{0}), ed(beg + count), cap(ed) {
		memset(beg, count, ch);
	}
	String(const char* s):beg(new char[strlen(s)+1]),ed(beg + strlen(s)), cap(ed) {
		strcpy(beg, s);
	}
	String(const String& other, size_t pos, size_t count) :beg(new char[count + 1]{ 0 }), ed(beg + count), cap(ed) {
		char* tmp = other.beg + pos;
		memcpy(beg, tmp, count);
	}
	String(const String& other):beg(new char[strlen(other.beg)+1]),ed(beg + other.size()),cap(ed) {
		strcpy(beg, other.beg);
	}
	String(String&& other):beg(other.beg), ed(other.ed), cap(other.ed) {
		other.beg = nullptr;
	}
	//dextructor
	~String()
	{
		delete []beg;
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
	char& at(size_t pos) {
		if (pos<0 || pos>size())
			throw std::out_of_range("out of range");
		return beg[pos];
	}
	const char& at(size_t pos)const {
		if (pos<0 || pos>size())
			throw std::out_of_range("out of range");
		return beg[pos];
	}
	char& operator[](size_t pos) {
		return beg[pos];
	}
	const char& operator[](size_t pos)const {
		return beg[pos];
	}
	char& front() {
		return *beg;
	}
	const char& front()const {
		return *beg;
	}
	char& back() {
		return *(ed-1);
	}
	const char& back()const {
		return *(ed-1);
	}
	const char* data() const noexcept {
		return beg;
	}
	const char* c_str()const noexcept {
		return beg;
	}
	//iterators
	iterator begin()noexcept {
		return beg;
	}
	const_iterator begin() const noexcept {
		return beg;
	}
	iterator end()noexcept {
		return ed;
	}
	const_iterator end() const noexcept {
		return ed;
	}
	//capacity
	bool empty()const noexcept {
		return beg==ed;
	}
	size_t size()const noexcept {
		return ed-beg;
	}
	size_t length()const noexcept {
		return  size();
	}
	size_t max_size()const noexcept {

	}
	size_t capacity()const noexcept {
		return cap - beg;
	}
	size_t reserve(size_t newcap) {
		if (newcap > capacity()) {
			realloc_n(newcap);
		}
	}
	//operations
	void clear()noexcept {
		ed = beg;
	}
	String& insert(size_t index, size_t count, char ch) {
		
	}
	void push_back(char ch) {
		if (size() == capacity()) {
			int newsize = size() ? size() * 2 : 1;
			realloc_n(newsize);
		}
		*ed++ = ch;
	}

	void swap(String& other) {
		std::swap(beg, other.beg);
		std::swap(ed, other.ed);
		std::swap(cap, other.cap);
	}

private:
	char* beg, *ed,*cap;
	void realloc_n(size_t newsize) {
		char* newbeg = new char[newsize];
		size_t oldsize = size();
		memcpy(newbeg, beg, sizeof(char) *oldsize);
		delete[]beg;
		beg = newbeg;
		ed = beg + oldsize;
		cap = beg + newsize;
	}

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
		bool operator==(const const_iterator& rhs)const{
			return ptr == rhs.ptr;
		}
		bool operator!=(const const_iterator& rhs)const {
			return !(*this == rhs);
		}
	protected:
		char* ptr;
		const_iterator(char* p) :ptr(p) {}
	};

	struct iterator:public const_iterator {
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
		iterator(char *p):const_iterator(p){}
	};

};
#endif // !STRING_H
