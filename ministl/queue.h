#pragma once
#ifndef QUEUE_H
#define QUEUE_H
#include<utility>
template<class Object, class Container>class Queue;
template<class Object, class Container>
void swap(Queue<Object, Container>& lhs, Queue<Object, Container>& rhs) {
	swap(lhs.con, rhs.con);
}
template<class Object,class Container>
class Queue {
	friend void swap<>(Queue&, Queue&);
private:
	Container con;
public:
	Queue(){}
	Queue(const Queue&other):con(other.con){}
	Queue(Queue&&other):con(std::move(other.con)){}
	Queue& operator=(const Queue& rhs) {
		Queue copy(rhs);
		swap(copy);
		return *this;
	}
	Queue& operator=(Queue&& rhs) {
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
	void swap(Queue& other) {
		::swap(con,other.con);
	}
};

#endif // !QUEUE_H
