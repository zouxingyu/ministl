#pragma once
#ifndef SET_H
#define SET_H
#include<utility>

template<class Comparable>
class Set {
private:
	struct Node {
		Comparable element;
		Node* left;
		Node* right;
		Node* parent;
		Node(const Comparable& e, Node* l = nullptr, Node* r = nullptr, Node* p = nullptr) :element(e),left(l),right(r),parent(p){}
	};
	Node* root;
	size_t thesize;
public:
	class const_iterator
	{
		friend class Set<Comparable>;
	protected:
		Node* ptr;
		const_iterator(Node* p) :ptr(p) {}
	public:
		const Comparable& operator*() {
			return ptr->element;
		}
		const_iterator& operator++() {
			Node* t;
			if (ptr->right) {
				t = ptr->right;
				while (t->left) {
					t = t->left;
				}
			}
			else {
				t = ptr->parent;
				while (t && t->element < ptr->element) {
					t = t->parent;
				}
			}
			ptr = t;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator old = *this;
			++(*this);
			return old;
		}
		bool operator==(const const_iterator& rhs) {
			return ptr == rhs.ptr;
		}
		bool operator!=(const const_iterator& rhs) {
			return !(*this == rhs);
		}
	};

	class iterator :public const_iterator
	{
		friend class Set<Comparable>;
	private:
		iterator(Node* p) :const_iterator(p) {}
	public:
		Comparable& operator*() {
			return this->ptr->element;
		}
		iterator& operator++() {
			Node* t;
			if (this->ptr->right) {
				t = this->ptr->right;
				while (t->left) {
					t = t->left;
				}
			}
			else {
				t = this->ptr->parent;
				while (t && t->element < this->ptr->element) {
					t = t->parent;
				}
			}
			this->ptr = t;
			return *this;
		}
		iterator operator++(int) {
			const_iterator old = *this;
			++(*this);
			return old;
		}
	};
	Set():root(nullptr),thesize(0){}
	Set(const Set& other) :root(nullptr), thesize(0) {
		root = clone(root);
	}
	Set(Set&& other) :root(other.root) {
		other.root = nullptr;
		thesize = 0;
	}
	Set& operator=(const Set& other) {
		Set copy(other);
		swap(copy);
		return *this;
	}
	Set& operator=(Set&& other) {
		swap(other);
		return *this;
	}
	~Set()
	{
		makeEmpty(root);
	}

	iterator begin() {
		Node* t=root;
		if (t == nullptr)return t;
		while (t->left != nullptr) {
			t = t->left;
		}
		return t;
	}
	const_iterator begin()const {
		Node* t;
		while (t->left != nullptr) {
			t = t->left;
		}
		return t;
	}
	iterator end() {
		return nullptr;
	}
	const_iterator end()const {
		return nullptr;
	}
	size_t size()const noexcept {
		return thesize;
	}
	bool empty()const noexcept {
		return thesize == 0;
	}
	iterator insert(const Comparable& value) {
		++thesize;
		return inserthelper(root,nullptr,value);
	}
	iterator find(const Comparable& value) {
		return find(root, value);
	}
	size_t count(const Comparable& value) {
		return (find(root, value) == nullptr) ? 0 : 1;
	}
	void swap(const Set& other) {
		std::swap(root, other.root);
		std::swap(thesize, other.thesize);
	}
	size_t erase(const Comparable& value) {
		return remove(root, value) == nullptr ? 0 : 1;
	}
	
private:
	iterator inserthelper(Node* &t,Node*p,const Comparable& value) {
		if (t == nullptr) {
			t = new Node{value,nullptr,nullptr,p};
		}
		else if (value < t->element) {
			return inserthelper(t->left, t, value);
		}
		else if (t->element < value) {
			return inserthelper(t->right, t, value);
		}
		return t;
	}
	Node* find(Node* t, const Comparable& value) {
		if (t == nullptr || t->element == value)
			return t;
		else if (value < t->element)
			return find(t->left, value);
		else
			return find(t->right, value);
	}
	Node* clone(Node* t) {
		if (t == nullptr)return t;
		return clone(new Node{ t->element,clone(t->left),clone(t->right),t->parent });
	}
	Node* remove(Node* t, const Comparable& value) {
		if (t == nullptr)
			return t;
	}
	void makeEmpty(Node*t) {
		if (t == nullptr)return;
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
		t = nullptr;
	}
};
#endif // !SET_H
