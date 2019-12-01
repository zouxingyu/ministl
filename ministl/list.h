#pragma once
#ifndef LIST_H
#define LIST_H

template<class Object>
class List{
private:
	struct Node
	{
		Object obj;
		Node* pre;
		Node* next;
		Node(const Object& o = Object{}, Node* p = nullptr, Node* n = nullptr) :obj(o), pre(p), next(n) {}
	};
	Node* head;
	Node* tail;

public:
	List() :head(new Node), tail(new Node) {
		head->next = tail;
		tail->pre = next;
	}
	List(const List& other) :head(new Node), tail(new Node) {
		for (Node* p = other.head->next; p != other.tail; p = p->next) {
			push_back(*p);
		}
	}
	List(List&& other) :head(other.head), tail(other.tail) {
		other.head = nullptr;
		other.tail = nullptr;
	}
	List& operator=(const List& other) {
		Listcopy(other);
		swap(copy);
		return *this;
	}
	List& operator=(List&& other) {
		swap(other);
		return *this;
	}
	~List()
	{
		makeEmpty();
		delete head;
		delete tail;
	}

	void push_back()
private:
	void makeEmpty() {
		for (Node* p = head->next; p != tail;) {
			Node* tmp = p->next;
			p->pre->next = tmp;
			tmp->pre = p->pre;
			delete p;
			p = tmp;
		}
	}
};

#endif // !LIST_H