#pragma once
#ifndef VECTOR_H
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include<utility>

template <class T>
class vector {
   public:
    class Iterator;
    class ConstIterator;
    typedef size_t size_type;
    typedef T value_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef Iterator iterator;
    typedef ConstIterator const_iterator;
    
	vector();
    explicit vector(size_type count, const T &value);
    explicit vector(size_type count);
    template <class InputIt>
    vector(InputIt first, InputIt last);
    vector(const vector &other);
    vector(vector &&other) noexcept;
    vector(std::initializer_list<T> init);
    ~vector() {
        for (size_t i = 0; i < size_; i++) {
            alloc.destroy(ptr_ + i);
        }
        alloc.deallocate(ptr_,cap_);
    }
    vector &operator=(const vector &other);
    vector &operator=(vector &&other) noexcept;
    vector &operator=(std::initializer_list<T> ilist);
    void assign(size_type count, const T &value);
    template <class InputIt>
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list<T> ilist);
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference operator[](size_type pos);
    const_reference operator[](size_type pos)const;
    T * data() noexcept;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type capacity() const noexcept;
    void reserve(size_type new_cap);
    void shrink_to_fit();
    void clear() noexcept;
    iterator insert(const_iterator pos, const T &value);
    iterator insert(const_iterator pos, size_type count, const T &valur);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void push_back(const T &value);
    void push_back(T &&value);
    void pop_back();
    void resize(size_type count,const value_type&value);
    void resize(size_type count);
    void swap(vector &other);
    class ConstIterator {
        friend class vector<T>;
       protected:
        T *p_;
        ConstIterator(T *p) : p_(p) {}

       public:
        ConstIterator &operator++() {
            ++p_;
            return *this;
        }
        ConstIterator &operator--() {
            --p_;
            return *this;
        }
        ConstIterator operator++(int) {
            T *old = p_;
            ++p_;
            return old;
        }
        ConstIterator operator--(int) {
            T *old = p_;
            --p_;
            return old;
        }
        const_reference operator*() {
            return *p_;
        }
        ConstIterator &operator=(ConstIterator &it) {
            p_ = it.p_;
            return *this;
        }
        ConstIterator operator+(int diff) const {
            return p_ + diff;
        }
        int operator-(const ConstIterator &it) const {
            return p_ - it.p;
        }
        bool operator==(const ConstIterator &it) const {
            return p_ == it.p_;
        }
        bool operator!=(const ConstIterator &it) const {
            return !(*this == it);
        }
    };
    class Iterator : public ConstIterator {
        friend class vector<T>;
       private:
        Iterator(T *p) : ConstIterator(p) {}

       public:
        Iterator &operator++() {
            ++this->p_;
            return *this;
        }
        Iterator &operator--() {
            --this->p_;
            return *this;
        }
        Iterator operator++(int) {
            T *old = this->p_;
            ++this->p_;
            return old;
        }
        Iterator operator--(int) {
            T *old = this->p_;
            --this->p_;
            return old;
        }
        reference operator*() {
            return *this->p_;
        }
        const_reference operator*() const {
            return *this->p_;
        }
        Iterator &operator=(const ConstIterator &it) {
            this->p = it.p_;
            return *this;
        }
    };

   private:
    T *ptr_;
    std::allocator<T> alloc;
    size_t size_, cap_;
    void alloc_n(size_type new_cap) {
        T *newptr = alloc.allocate(new_cap);
        for (size_t i = 0; i < size_; i++) {
            alloc.construct(newptr + i, ptr_[i]);
        }
        for (size_t i = 0; i < size_; i++) {
            alloc.destroy(ptr_ + i);
        }
        alloc.deallocate(ptr_, cap_);
        cap_ = new_cap;
        ptr_ = newptr;
    }
};

template <class T>
vector<T>::vector() : ptr_(nullptr), size_(0), cap_(0) {}

template <class T>
vector<T>::vector(size_type count, const T &value) : ptr_(alloc.allocate(count)), size_(count), cap_(count) {
    for (size_t i = 0; i < count; i++) {
        alloc.construct(ptr_ + i, value);
    }
}

template <class T>
vector<T>::vector(size_type count) : ptr_(alloc.allocate(count)), size_(count), cap_(count) {
    for (size_t i = 0; i < count; i++) {
        alloc.construct(ptr_ + i, T());
    }
}

template <class T>
template <class InputIt>
vector<T>::vector(InputIt first, InputIt last) {
    size_t sz = 0;
    for (InputIt it = first; it != last; it++) {
        sz++;
    }
    ptr_ = alloc.allocate(sz);
    for (size_t i = 0; first != last; i++, first++) {
        alloc.construct(ptr_ + i, *first);
    }
    size_ = cap_ = sz;
}

template <class T>
vector<T>::vector(const vector &other) : ptr_(alloc.allocate(other.size_)), size_(other.size_), cap_(other.cap_) {
    for (size_t i = 0; i < other.size_; i++) {
        alloc.construct(ptr_ + i, other.ptr_[i]);
    }
}

template <class T>
vector<T>::vector(vector &&other)noexcept : ptr_(other.ptr_), size_(other.size_), cap_(other.cap_) {
    other.ptr_ = nullptr;
}

template <class T>
vector<T>::vector(std::initializer_list<T> init) : vector(init.begin(), init.end()) {}

template <class T>
vector<T> &vector<T>::operator=(const vector &other) {
    if (ptr_!=other.ptr_) {
        if (cap_ < other.size_) {
            alloc_n(other.size_);
        }
        size_t i;
        for ( i = 0; i < size_; i++) {
            ptr_[i] = other.ptr_[i];
        }
        for (; i < other.size_; i++) {
            alloc.construct(ptr_ + i, other.ptr_[i]);
		}
	}
    size_ = other.size_;
    return *this;
}

template <class T>
vector<T> &vector<T>::operator=(vector &&other)noexcept {
    if (ptr_!=other.ptr_) {
        swap(other);
    }
    return *this;
}

template <class T>
vector<T> &vector<T>::operator=(std::initializer_list<T> ilist) {
    *this = vector<T>(ilist.begin(), ilist.end());
}

template <class T>
void vector<T>::assign(size_type count, const T &value) {
    *this = vector<T>(count, value);
}

template <class T>
template <class InputIt>
void vector<T>::assign(InputIt first, InputIt last) {
    *this = vector<T>(first, last);
}

template <class T>
void vector<T>::assign(std::initializer_list<T> ilist) {
    *this = ilist;
}

template <class T>
typename vector<T>::reference vector<T>::at(size_type pos) {
    if (pos >= size_)
        throw std::out_of_range("pos out of range");
    return ptr_[pos];
}

template <class T>
typename vector<T>::const_reference vector<T>::at(size_type pos) const {
    if (pos >= size_)
        throw std::out_of_range("pos out of range");
    return ptr_[pos];
}

template <class T>
typename vector<T>::reference vector<T>::operator[](size_type pos) {
    return ptr_[pos];
}

template <class T>
typename vector<T>::const_reference vector<T>::operator[](size_type pos) const {
    return ptr_[pos];
}

template <class T>
typename vector<T>::reference vector<T>::front() {
    return ptr_[0];
}

template <class T>
typename vector<T>::const_reference vector<T>::front()const {
    return ptr_[0];
}

template <class T>
T *vector<T>::data() noexcept {
    return ptr_;
}

template <class T>
typename vector<T>::iterator vector<T>::begin() noexcept {
    return ptr_;
}

template <class T>
typename vector<T>::const_iterator vector<T>::begin() const noexcept {
    return ptr_;
}

template <class T>
typename vector<T>::iterator vector<T>::end() noexcept {
	return ptr_ + size_;
}

template <class T>
typename vector<T>::const_iterator vector<T>::end() const noexcept {
	return ptr_ + size_;
}

template <class T>
bool vector<T>::empty() const noexcept {
	return size_ == 0;
}

template <class T>
typename vector<T>::size_type vector<T>::size() const noexcept {
	return size_;
}

template <class T>
typename vector<T>::size_type vector<T>::capacity() const noexcept {
	return cap_;
}

template <class T>
void vector<T>::reserve(size_type new_cap) {
	if (new_cap > cap_) {
		alloc_n(new_cap);
	}
}

template <class T>
void vector<T>::clear() noexcept {
	for (size_t i = 0; i < size_; i++) {
		alloc.destroy(ptr_ + i);
	}
	size_ = 0;
}

template <class T>
typename vector<T>::iterator vector<T>::insert(const_iterator pos, const T &value) {
    return insert(pos, 1, value);
}

template <class T>
typename vector<T>::iterator vector<T>::insert(const_iterator pos, size_type count, const T &value) {
    int diff = pos.p_ - ptr_;
	if (size_ == cap_) {
        alloc_n(size_ == 0 ? 10 : size_ * 2);
	}
	
        size_t j = 0;
	for (int idx = static_cast<int>(size_) - 1; idx >= diff; idx--) {
            if (j++ < count) {
                alloc.construct(ptr_ + idx + count, std::move(ptr_[idx]));
            } else {
                ptr_[idx + count] = std::move(ptr_[idx]);
			}
		
	}
	for (size_t idx = diff, i = 0; i < count; i++) {
		ptr_[idx + i] = value;
	}
	size_ += count;
	return ptr_ + diff;
}

template <class T>
typename vector<T>::iterator vector<T>::erase(const_iterator pos) {
	return erase(pos, pos + 1);
}

template <class T>
typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last) {
	T* after = first.p_;
	while (last != end()) {
		*first.p_ = std::move(last.p_);
		first++, last++;
	}
	size -= (last - first);
	return after;
}

template <class T>
void vector<T>::push_back(const T& value) {
	if (size_ == cap_) {
        alloc_n(size_ == 0 ? 10 : size_ * 2);
	}
        alloc.construct(ptr_ + size_, value);
	size_++;
}

template <class T>
void vector<T>::push_back(T &&value) {
    if (size_ == cap_) {
        alloc_n(size_ == 0 ? 10 : size_ * 2);
    }
    alloc.construct(ptr_ + size_, std::move(value));
    size_++;
}

template <class T>
void vector<T>::pop_back() {
	alloc.destroy(ptr_ + size_ - 1);
	size_--;
}
template<class T>
void vector<T>::resize(size_type count, const value_type& value){
    if (count < size_) {
        for (size_t idx = count; idx<size_; idx++) {
            alloc.destroy(ptr_ + idx);
		}
    } else {
        alloc_n(count);
        for (size_t idx = size_; idx < count; idx++) {
            alloc.construct(ptr_+idx,value);
        }
	}
    size_ = count;
}

template <class T>
void vector<T>::resize(size_type count) {
    if (count < size_) {
        for (size_t idx = count; idx < size_; idx++) {
            alloc.destroy(ptr_ + idx);
        }
    } else {
        alloc_n(count);
        for (size_t idx = size_; idx < count; idx++) {
            alloc.construct(ptr_ + idx, T());
        }
    }
    size_ = count;
}

template <class T>
void vector<T>::swap(vector& other) {
	using std::swap;
    swap(ptr_, other.ptr_);
    swap(size_, other.size_);
    swap(cap_, other.cap_);
    swap(alloc, other.alloc);
}

template <class T>
bool operator==(const vector<T>& lhs, const vector<T>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    } else {
        auto p1 = lhs.begin(), p2 = rhs.begin();
        while (p1 != lhs.end() && p2 != rhs.end()) {
            if (*p1 != *p2) {
                return false;
            }
            p1++, p2++;
        }
        return true;
    }
}
template <class T>
bool operator!=(const vector<T>& lhs, const vector<T>& rhs) {
    return !(lhs == rhs);
}
template <class T>
bool operator<(const vector<T>& lhs, const vector<T>& rhs) {
    auto p1 = lhs.begin(), p2 = rhs.begin();
    while (p1 != lhs.end() && p2 != rhs.end()) {
        if (*p1 < *p2) {
            return false;
        }
        p1++, p2++;
    }
    return p1.size() < p2.size();
}
template <class T>
bool operator<=(const vector<T>& lhs, const vector<T>& rhs) {
    return !(lhs > rhs);
}
template <class T>
bool operator>(const vector<T>& lhs, const vector<T>& rhs) {
    auto p1 = lhs.begin(), p2 = rhs.begin();
    while (p1 != lhs.end() && p2 != rhs.end()) {
        if (*p1 > *p2) {
            return false;
        }
        p1++, p2++;
    }
    return p1.size() > p2.size();
}
template <class T>
bool operator>=(const vector<T>& lhs, const vector<T>& rhs) {
    return !(lhs < rhs);
}


template <class T>
void swap(vector<T> &lhs, vector<T> &rhs) {
    lhs.swap(rhs);
}
#endif  // ! VECTOR_H
