#pragma once
#ifndef LIST_H
#include <initializer_list>
#include <memory>
#include <utility>

template <class T>
class list {
   private:
    struct Node {
        T data;
        Node *next;
        Node *pre;
        Node(const T &data, Node *next = nullptr, Node *pre = nullptr) : data(data), next(next), pre(pre) {}
        Node(T &&data, Node *next = nullptr, Node *pre = nullptr) : data(std::move(data)), next(next), pre(pre) {}
    };
    Node *head_;
    Node *tail_;
    size_t size_;
    std::allocator<Node> alloc;
    void delete_all_node() {
        while (head_->next != tail_) {
            Node *tmp = head_->next;
            head_->next = tmp->next;
            tmp->next->pre = head_;
            alloc.destroy(tmp);
            alloc.deallocate(tmp, 1);
        }
    }

   public:
    class Iterator;
    class ConstIterator;
    typedef size_t size_type;
    typedef T value_type;
    typedef value_type &reference;
    typedef const value_type const_reference;
    typedef ConstIterator const_iterator;
    typedef Iterator iterator;

    list();
    explicit list(size_type count, const T &value);
    explicit list(size_type count);
    template <class InputIt>
    list(InputIt first, InputIt last);
    list(const list &other);
    list(list &&other) noexcept;
    list(std::initializer_list<T> init);
    ~list() {
        delete_all_node();
        alloc.destroy(head_);
        alloc.deallocate(head_, 1);
        alloc.destroy(tail_);
        alloc.deallocate(tail_, 1);
    }
    list &operator=(const list &other);
    list &operator=(list &&other) noexcept;
    list &operator=(std::initializer_list<T> ilist);
    void assign(size_type count, const T &value);
    template <class InputIt>
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list<T> ilist);
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    T *data() noexcept;
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    bool empty() const noexcept;
    size_type size() const noexcept;
    void clear() noexcept;
    iterator insert(const_iterator pos, const T &value);
    iterator insert(const_iterator pos, size_type count, const T &value);
    iterator insert(const_iterator pos, T &&value);
    template <class InputIt>
    list<T>::iterator insert(iterator pos, InputIt first, InputIt last);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void push_back(const T &value);
    void push_back(T &&value);
    void pop_back();
    void push_front(const T &value);
    void push_front(T &&value);
    void pop_front();
    void resize(size_type count);
    void resize(size_type count, const value_type &value);
    void swap(list &other);
    void merge(list &other);
    void splice(const_iterator pos, list &other);
    void splice(const_iterator pos, list &other,
                const_iterator first, const_iterator last);
    class ConstIterator {
        friend class list<T>;

       protected:
        Node *p_;
        ConstIterator(Node *p) : p_(p) {}

       public:
        ConstIterator &operator++() {
            p_ = p_->next;
            return *this;
        }
        ConstIterator &operator--() {
            p_ = p_->pre;
            return *this;
        }
        ConstIterator operator++(int) {
            T *old = p_;
            ++(*this);
            return old;
        }
        ConstIterator operator--(int) {
            T *old = p_;
            --(*this);
            return old;
        }
        const_reference operator*() {
            return p_->data;
        }
        ConstIterator &operator=(const Iterator &it) {
            p_ = it.p_;
            return *this;
        }
        bool operator==(const ConstIterator &it) const {
            return p_ == it.p_;
        }
        bool operator!=(const ConstIterator &it) const {
            return !(*this == it);
        }
    };
    class Iterator : public ConstIterator {
        friend class list<T>;

       private:
        Iterator(Node *p) : ConstIterator(p) {}

       public:
        Iterator &operator++() {
            this->p_ = this->p_->next;
            return *this;
        }
        Iterator &operator--() {
            this->p_ = this->p_->pre;
            return *this;
        }
        Iterator operator++(int) {
            T *old = this->p_;
            ++(*this);
            return old;
        }
        Iterator operator--(int) {
            T *old = this->p_;
            --(*this);
            return old;
        }
        reference operator*() {
            return this->p_->data;
        }
        const_reference operator*() const {
            return this->p_->data;
        }
        Iterator &operator=(const Iterator &it) {
            this->p_ = it.p_;
            return *this;
        }
    };
};

template <class T>
list<T>::list() : head_(alloc.allocate(1)), tail_(alloc.allocate(1)), size_(0) {
    alloc.construct(head_, T(), tail_, nullptr);
    alloc.construct(tail_, T(), nullptr, head_);
}

template <class T>
list<T>::list(size_type count, const T &value) : list() {
    for (size_t i = 0; i < count; i++) {
        push_back(value);
    }
}

template <class T>
list<T>::list(size_type count) : list() {
    for (size_t i = 0; i < count; i++) {
        push_back(T());
    }
}

template <class T>
template <class InputIt>
list<T>::list(InputIt first, InputIt last) : list() {
    while (first != last) {
        push_back(*first);
        first++;
    }
}

template <class T>
list<T>::list(const list &other) : list(other.begin(), other.end()) {}

template <class T>
list<T>::list(list &&other) noexcept : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ == other.tail_ = nullptr;
}

template <class T>
list<T>::list(std::initializer_list<T> init) : list(init.begin(), init.end()) {}

template <class T>
list<T> &list<T>::operator=(const list &other) {
    if (head_ != other.head_) {
        delete_all_node();
        for (auto it = other.begin(); it != other.end(); it++) {
            push_back(*it);
        }
    }
    return *this;
}

template <class T>
list<T> &list<T>::operator=(list &&other) noexcept {
    if (head_ != other.head_) {
        swap(other);
    }
    return *this;
}

template <class T>
list<T> &list<T>::operator=(std::initializer_list<T> ilist) {
    delete_all_node();
    for (auto it = ilist.begin(); it != ilist.end(); it++) {
        push_back(*it);
    }
    return *this;
}

template <class T>
void list<T>::assign(size_type count, const T &value) {
    delete_all_node();
    *this = list<T>(count, value);
}

template <class T>
void list<T>::assign(std::initializer_list<T> ilist) {
    delete_all_node();
    *this = list<T>(ilist);
}

template <class T>
typename list<T>::reference list<T>::front() {
    return *head_->next_;
}

template <class T>
typename list<T>::const_reference list<T>::front() const {
    return *head_->next_;
}

template <class T>
typename list<T>::reference list<T>::back() {
    return *tail_->pre_;
}

template <class T>
typename list<T>::const_reference list<T>::back() const {
    return *tail_->pre_;
}

template <class T>
typename list<T>::iterator list<T>::begin() noexcept {
    return head_->next;
}

template <class T>
typename list<T>::const_iterator list<T>::begin() const noexcept {
    return head_->next;
}

template <class T>
typename list<T>::iterator list<T>::end() noexcept {
    return tail_;
}

template <class T>
typename list<T>::const_iterator list<T>::end() const noexcept {
    return tail_;
}

template <class T>
bool list<T>::empty() const noexcept {
    return size_ == 0;
}

template <class T>
typename list<T>::size_type list<T>::size() const noexcept {
    return size_;
}

template <class T>
void list<T>::clear() noexcept {
    delete_all_node();
}

template <class T>
typename list<T>::iterator list<T>::insert(const_iterator pos, const T &value) {
    return insert(pos, 1, value);
}

template <class T>
typename list<T>::iterator list<T>::insert(const_iterator pos, size_type count, const T &value) {
    Node *ptr = pos.p_;
    size_ += count;
    while (count--) {
        Node *tmp = alloc.allocate(1);
        alloc.construct(tmp, value, ptr, ptr->pre);
        ptr->pre->next = tmp;
        ptr->pre = tmp;
        ptr = tmp;
    }
    return ptr;
}

template <class T>
typename list<T>::iterator list<T>::insert(const_iterator pos, T &&value) {
    Node *ptr = pos.p_;
    size_++;
    Node *tmp = alloc.allocate(1);
    alloc.construct(tmp, std::move(value), ptr, ptr->pre);
    ptr->pre->next = tmp;
    ptr->pre = tmp;
    return tmp;
}

template <class T>
template <class InputIt>
typename list<T>::iterator list<T>::insert(iterator pos, InputIt first, InputIt last) {
    while (first != last) {
        pos = insert(pos, *first);
        first++;
    }
    return pos;
}

template <class T>
typename list<T>::iterator list<T>::erase(const_iterator pos) {
    return erase(pos, ++pos);
}

template <class T>
typename list<T>::iterator list<T>::erase(const_iterator first, const_iterator last) {
    while (first != last) {
        Node *tmp = first.p_;
        ++first;
        tmp->pre->next = tmp->next;
        tmp->next->pre = tmp->pre;
        alloc.destroy(tmp);
        alloc.deallocate(tmp, 1);
        size_--;
    }
    return last;
}
template <class T>
void list<T>::push_back(const T &value) {
    Node *tmp = alloc.allocate(1);
    alloc.construct(tmp, value, tail_, tail_->pre);
    tail_->pre->next = tmp;
    tail_->pre = tmp;
    size_++;
}

template <class T>
void list<T>::push_back(T &&value) {
    Node *tmp = alloc.allocate(1);
    alloc.construct(tmp, std::move(value), tail_, tail_->pre);
    size_++;
}

template <class T>
void list<T>::pop_back() {
    erase(--end());
}

template <class T>
void list<T>::push_front(const T &value) {
    insert(begin(), value);
}

template <class T>
void list<T>::push_front(T &&value) {
    insert(begin(), std::move(value));
}

template <class T>
void list<T>::pop_front() {
    erase(begin());
}

template <class T>
void list<T>::resize(size_type count) {
    if (count > size_) {
        while (size_ < count) {
            push_back(T());
            size_++;
        }
    } else {
        int diff = size_ - count;
        while (diff--) {
            pop_back();
        }
    }
}
template <class T>
void list<T>::resize(size_type count, const T &value) {
    if (count > size_) {
        while (size_ < count) {
            push_back(value);
            size_++;
        }
    } else {
        int diff = size_ - count;
        while (diff--) {
            pop_back();
        }
    }
}

template <class T>
void list<T>::swap(list &other) {
    using std::swap;
    swap(head_, other.head_);
    swap(tail_, other.tail_);
    swap(size_, other.size_);
}

template <class T>
void list<T>::merge(list &other) {
    if (other.head_ != head_) {
        Node *p1 = head_->next, *p2 = other.head_->next;
        Node *p = head_;
        while (p1 != tail_ && p2 != other.tail_) {
            if (p1->data <= p2->data) {
                p->next = p1;
                p1->pre = p;
                p = p1;
                p1 = p1->next;
            } else {
                p->next = p2;
                p2->pre = p;
                p = p2;
                p2 = p2->next;
            }
        }
        if (p1 != tail_) {
            p->next = p1;
            p1->pre = p;
            size_ += other.size_;
            other.head_->next = other.tail_;
            other.tail_->pre = other.head_;
            other.size_ = 0;
        }
        if (p2 != other.tail_) {
            p->next = p2;
            p2->pre = p;
            other.tail_->pre->next = tail_;
            tail_->pre = other.tail_->pre;
            size_ += other.size_;
            other.head_->next = other.tail_;
            other.tail_->pre = other.head_;
            other.size_ = 0;
        }
        
    }
}

template <class T>
void list<T>::splice(const_iterator pos, list &other) {
    splice(pos, other, other.begin(), other.end());
}

template <class T>
void list<T>::splice(const_iterator pos, list &other, const_iterator first, const_iterator last) {
    Node *p = pos.p_;
    Node *h=first.p_->pre,*t=last.p_;
    if (first != last) {
        p->pre->next = h->next;
        h->next->pre = p->pre;
        t->pre->next = p;
        p->pre = t->pre;
        h->next = t;
        t->pre = h;
	}
}

template <class T>
bool operator==(const list<T> &lhs, const list<T> &rhs) {
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
bool operator!=(const list<T> &lhs, const list<T> &rhs) {
    return !(lhs == rhs);
}
template <class T>
bool operator<(const list<T> &lhs, const list<T> &rhs) {
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
bool operator<=(const list<T> &lhs, const list<T> &rhs) {
    return !(lhs > rhs);
}
template <class T>
bool operator>(const list<T> &lhs, const list<T> &rhs) {
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
bool operator>=(const list<T> &lhs, const list<T> &rhs) {
    return !(lhs < rhs);
}

template <class T>
void swap(list<T> &lhs, list<T> &rhs) {
    lhs.swap(rhs);
}
#endif  // !LIST_H
