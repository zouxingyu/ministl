#pragma once
#ifndef QUEUE_H
#define QUEUE_H
#include <utility>
template <class Object, class Container>
class queue;
template <class Object, class Container>
void swap(queue<Object, Container>& lhs, queue<Object, Container>& rhs) {
    swap(lhs.con, rhs.con);
}
template <class Object, class Container>
class queue {
    friend void swap<>(queue&, queue&);

   private:
    Container con;

   public:
    queue() {}
    queue(const queue& other) : con(other.con) {}
    queue(queue&& other) : con(std::move(other.con)) {}
    queue& operator=(const queue& rhs) {
        queue copy(rhs);
        swap(copy);
        return *this;
    }
    queue& operator=(queue&& rhs) {
        swap(rhs);
        return *this;
    }
    Object& front() {
        return con.front();
    }
    Object& back() {
        return con.back();
    }
    bool empty() {
        return con.empty();
    }
    size_t size() {
        return con.size();
    }
    void push(const Object& value) {
        con.push_back(value);
    }
    void pop() {
        con.pop_front();
    }
    void swap(queue& other) {
        ::swap(con, other.con);
    }
};

#endif  // !QUEUE_H