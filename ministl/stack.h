#pragma once
#ifndef STACK_H
#define SATCK_H
#include<utility>
#include"vector.h"
template<class Object, class Container>
class Stack;
template<class Object, class Container>
void swap(Stack<Object, Container>& lhs, Stack<Object, Container>& rhs) {
	swap(lhs.container, rhs.container);
}
template<class Object,class Container>
class Stack {
	friend void swap<>(Stack& lhs, Stack& rhs);
private:
	Container container;
public:
	Stack(){}
	Stack(const Stack&other):container(other.container){}
	Stack(Stack&&other):container(std::move(other.container)){}
	Stack& operator=(const Stack& rhs) {
		Stack copy(rhs);
		swap(copy);
		return *this;
	}
	Stack& operator=(Stack&& rhs) {
		swap(rhs);
		return *this;
	}
	Object& top() {
		return container.back();
	}
	bool empty() {
		return container.empty();
	}
	size_t size() {
		return container.size();
	}
	void push(const Object& value) {
		container.push_back(value);
	}
	void pop() {
		container.pop_back();
	}
	void swap(Stack &other) {
		::swap(container, other.container);
	}
};
#endif // !STACK_H
