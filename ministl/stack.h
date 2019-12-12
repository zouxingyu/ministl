#pragma once
#ifndef STACK_H
#define SATCK_H
#include<utility>
namespace zxy {
	template<class Object, class Container>class Stack;
	template<class Object, class Container>
	void swap(Stack<Object, Container>& lhs, Stack<Object, Container>& rhs) {
		swap(lhs.con, rhs.con);
	}
	template<class Object, class Container>
	class Stack {
		friend void swap<>(Stack& lhs, Stack& rhs);
	private:
		Container con;
	public:
		Stack() {}
		Stack(const Stack& other) :con(other.con) {}
		Stack(Stack&& other) :con(std::move(other.con)) {}
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
		void swap(Stack& other) {
			zxy::swap(*this, other);
		}
	};

}

#endif // !STACK_H
