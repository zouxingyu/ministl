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
		Node(const Comparable&e,Node*l=nullptr,Node*r=nullptr,Node*parent=nullptr):
	};
	Node* root;
	size_t thesize;
public:
	class const_iterator
	{
		friend class set<Comparable>;
	protected:
		Node* ptr;
		const_iterator(Node* p):ptr(p){}
	public:
		const Comparable& operator*() {
			return ptr->element;
		}
	};

	class iterator:public const_iterator
	{
		friend class Set<Comparable>
	private:
		iterator(Node*p):const_iterator(p){}
	public:

	};

	Set():root(nullptr),thesize(0){}
	Set(const Set& other);
	Set(Set&& other);
	Set& operator=(const Set& other);
	Set& operator=(Set&& other);
	~Set()
	{

	}

	iterator begin() {
		return get_smallest(root);
	}
	const_iterator begin()const {
		return get_smallest(root);
	}
	iterator end() {
		return nullptr;
	}
	const_iterator end()const {
		return nullptr
	}
	size_t size()const noexcept {
		return thesize;
	}
	bool empty()const noexcept {
		return thesize == 0;
	}
	std::pair<iterator, bool> insert(const Comparable& value) {
		Node* k;
		bool inserted = false;
		inserthelper(root, root->parent, k, inserted, value);
		return std::make_pair(iterator(k), inserted);
	}
	iterator find(const Comparable& value) {
		return find(root, value);
	}
	size_t count(const Comparable& value) {
		return (find(root, value) == nullptr) ? 0 : 1;
	}
private:
	node* get_smallest(Node*t){
		if (t->left == nullptr)
			return t;
		return get_smallest(t->left);
	}
	void inserthelper(Node* t,Node*p, Node*&k,bool inserted,const Comparable& value) {
		if (t == nullptr) {
			t = new Node{ value,nullptr,nullptr,p };
			k = t;
			inserted = true;
			++thesize;
		}
		else if (value < t->element) {
			inserthelper(t->left, t, k, inserted, value);
		}
		else if (t->element < value) {
			inserthelper(t->right, t, k, inserted, value)
		}
		else{
			k = t;
		}
	}
	Node* find(Node* t, const Comparable& value) {
		if (t == nulltpr || t->element == value)
			return t;
		else if (value < t->element)
			return find(t->left, value);
		else
			return find(t->right, value);
	}
};
#endif // !SET_H
