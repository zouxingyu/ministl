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
	size_t thesize;

public:
	// define iterator
	class const_iterator {
		class friend List<Object>;
	protected:
		Object* ptr;
		const_iterator(const Object*p):ptr(p){}
	public:
		const Object& operator*()const {
			return *ptr;
		}

	};
	class iterator :public const_iterator {

	};
	//constructor
	List() :head(new Node), tail(new Node),thesize(0) {
		head->next = tail;
		tail->pre = next;
	}
	List(const List& other) :head(new Node), tail(new Node), thesize(0) {
		for (Node* p = other.head->next; p != other.tail; p = p->next) {
			push_back(*p);
			++thesize;
		}
	}
	List(List&& other) :head(other.head), tail(other.tail), thesize(other.thesize) {
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
	//desreuctor
	~List()
	{
		makeEmpty();
		delete head;
		delete tail;
	}
	//element access
	Object& front() {
		return head->next->obj;
	}
	Object& back() {
		return tail->pre->obj;
	}
	//iterators
	iterator begin() {
		return head->next;
	}
	iterator end() {
		return tail;
	}
	//capacity
	bool empty()const noexcept {
		return thesize == 0;
	}
	size_t size()const noexcept {
		return thesize;
	}
	//modifiers
	void  clear() {
		makeEmpty();
	}
	iterator insert(const_iterator pos,const Object& value) {

	}
	iterator erase(const_iterator pos) {

	}
	void push_back(const Object&value) {

	}
	void pop_back() {

	}
	void push_front(const Object& value) {

	}
	void pop_front() {

	}
	void resize(size_t count, const Object& value) {

	}
	void swap(List& other) {

	}
	//operations


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