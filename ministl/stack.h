#pragma once
#ifndef STACK_H
#define SATCK_H
#include <utility>
namespace zxy {
template <class Object, class Container>
class stack;
template <class Object, class Container>
void swap(stack<Object, Container>& lhs, stack<Object, Container>& rhs) {
    swap(lhs.con, rhs.con);
}
template <class Object, class Container>
class stack {
    friend void swap<>(stack& lhs, stack& rhs);

   private:
    Container con;

   public:
    stack() {}
    stack(const stack& other) : con(other.con) {}
    stack(stack&& other) : con(std::move(other.con)) {}
    stack& operator=(const stack& rhs) {
        stack copy(rhs);
        swap(copy);
        return *this;
    }
    stack& operator=(stack&& rhs) {
        swap(rhs);
        return *this;
    }
    Object& top() {
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
        con.pop_back();
    }
    void swap(stack& other) {
        zxy::swap(*this, other);
    }
};

}  // namespace zxy

#endif  // !STACK_H