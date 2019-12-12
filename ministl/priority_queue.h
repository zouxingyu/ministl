#pragma once
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#include<utility>
template<class Object,class Container,class Compare>
class Priority_queue {
private:
	Container con;
	size_t thesize;
	void buildHeap() {
		for (int i = thesize / 2; i > 0; --i) {
			down(i);
		}
	}
	void down(int pos) {
		int child;
		Object tmp = std::move(con[pos]);
		for (; pos * 2 <= thesize; pos = child) {
			child = pos * 2;
			if ((child + 1) <= thesize && Compare()(con[child], con[child + 1]))
				child += 1;
			if (Compare()(tmp, con[child]))
				con[pos] = std::move(con[child]);
			else
				break;
		}
		con[pos] = std::move(tmp);
	}
public:
	Priority_queue():thesize(0){}
	Priority_queue(const Container& c) :con(c.size() + 10), thesize(c.size()) {
		for (size_t i = 0; i < c.size(); ++i) {
			con[i + 1] = c[i];
		}
		buildHeap();
	}
	void push(const Object& value) {
		++thesize;
		int parent, pos;
		for (pos = thesize; pos > 1; pos = parent) {
			parent = pos / 2;
			if (Compare()(con[parent], con[pos]))
				con[pos] = std::move(con[parent]);
			else
				break;
		}
		con[pos] = value;
	}
	const Object& top() {
		return con[1];
	}
	void pop() {
		con[1] = std::move(con[thesize--]);
	}
	bool isempty() {
		return thesize == 0;
	}
	size_t size() {
		return thesize;
	}

};
#endif // !PRIORITY_QUEUE_H
