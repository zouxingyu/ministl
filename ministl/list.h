#pragma once
#ifndef LIST_H
#define LIST_H
#include<utility>
template<class Object>class List;
template<class Object>
void swap(List<Object>& lhs, List<Object>& rhs) {
	lhs.swap(rhs);
}
template<class Object>
class List{
	friend void swap<>(List&, List&);
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
		friend class List<Object>;
	protected:
		Node* ptr;
		const_iterator(Node *p):ptr(p){}
	public:
		const Object& operator*()const {
			return ptr->obj;
		}
		const_iterator& operator++() {
			ptr = ptr->next;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator old(ptr);
			++(*this);
			return old;
		}
		const_iterator& operator--() {
			ptr = ptr->pre;
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator old(ptr);
			--(*this);
			return old;
		}
		bool operator==(const const_iterator& rhs)const {
			return ptr == rhs.ptr;
		}
		bool operator!=(const const_iterator& rhs)const {
			return !(*this == rhs);
		}

	};
	class iterator :public const_iterator {
		friend class List<Object>;
	private:
		iterator(Node *p):const_iterator(p){}
	public:
		Object& operator*() {
			return this->ptr->obj;
		}
		iterator& operator++() {
			this->ptr = this->ptr->next;
			return *this;
		}
		iterator operator++(int) {
			iterator old(this->ptr);
			++(*this);
			return old;
		}
		iterator& operator--() {
			this->ptr = this->ptr->pre;
			return *this;
		}
		iterator operator--(int) {
			iterator old(this->ptr);
			--(*this);
			return old;
		}
	};
	//constructor
	List() :head(new Node), tail(new Node),thesize(0) {
		head->next = tail;
		tail->pre = head;
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
		thesize = 0;
	}
	List& operator=(const List& other) {
		List copy(other);
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
		Node* tmp = new Node{ value };
		tmp->pre = pos.ptr->pre;
		pos.ptr->pre->next = tmp;
		pos.ptr->pre = tmp;
		tmp->next = pos.ptr;
		++thesize;
		return tmp;
	}
	iterator erase(const_iterator pos) {
		iterator tmp = pos.ptr->next;
		pos.ptr->pre->next = pos.ptr->next;
		pos.ptr->next->pre = pos.ptr->pre;
		delete pos.ptr;
		--thesize;
		return tmp;
	}
	iterator erase(const_iterator start, const_iterator end) {
		for (iterator it = start; it != end; ++it)
			erase(it);
		return end.ptr;
	}
	void push_back(const Object&value) {
		insert(tail, value);
	}
	void pop_back() {
		erase(tail->pre);
	}
	void push_front(const Object& value) {
		insert(head->next,value);
	}
	void pop_front() {
		erase(head->next);
	}
	void resize(size_t count, const Object& value) {
		if (count > thesize) {
			for (int i = count - thesize; i > 0; --i) {
				insert(tail, value);
			}
		}
		else if (count < resize) {
			for (int i = 0; i < resize - count; ++i) {
				erase(tail->pre);
			}
		}
	}
	void swap(List& other) {
		std::swap(head, other.head);
		std::swap(tail, other.tail);
		std::swap(thesize, other.thesize);
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