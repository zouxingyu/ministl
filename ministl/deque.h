#pragma once
#ifndef DEQUE_H
#define DEQUE_H
template<class Object>
class Deque {
private:
	static constexpr size_t chunk_size = 20;
	static constexpr int up = 0;
	static consteval int down = 1;
	struct node
	{
		Object* chunk;
		sizt_t up,down;
		node(const Object* p = nullptr, size_t u=0,size_t d=0) :chunk(p), up(u),down(d) {}
	};
	node* map;
	size_t mapsize, beg, ed, total;
	void init() {
		mapsize = 10;
		map = new node[mapsize];
		beg = ed = mapsize/ 2;
		map[beg].chunk = new Object[chunk_size];
		total = 0;
	}
	void expandMap(int direction) {
		int offset = 0;
		if (direction = up)
			offset = mapsize;
		node* newmap = new node[mapsize * 2];
		for (size_t sz = beg; sz <= ed; ++sz) {
			newmap[sz+offset] = map[sz];
		}
		delete map;
		map = newmap;
		mapsize *= 2;
		beg = beg + offset;
	}
	void addChunk(int direction) {
		if (direction == up) {
			map[beg].chunk = new Object[chunk_size];
			map[beg].up = map[beg].down =chunk_size-1;
		}
		else{
			map[ed].chunk = new Object[chunk_size];
			map[ed].up =map[ed].down = 0;
		}
	}
public:
	class const_iterator{

	};
	class iterator :public const_iterator {

	};
	Deque() {
		init();
	}
	Deque(const Deque& other);
	Deque(Deque&& other);
	Deque& operator=(const Deque& other);
	Deque& operator=(Deque& other);
	~Deque()
	{

	}
	Object& operator[](size_t pos) {

	}
	Object& front() {

	}
	Object& end() {

	}
	iterator begin() {

	}
	iterator end() {

	}

	size_t size()const noexcept {
		return total;
	}
	bool empty()const noexcept {
		return total == 0;
	}
	void push_back(const Object& value) {
		if (map[ed].down == chunk_size) {
			if (ed == initial_map_size - 1)
				expandMap(down);
			if(map[++ed].chunk==nullptr)
				addChunk(down);
		}
		map[ed].down++ = value;
		++total;
	}
	void push_front(const Object& value) {
		if (map[beg].up == -1){
			if (beg == 0) 
				expandMap(up);
			if(map[--beg].chunk==nullptr)
				addChunk(up);
		}
		map[beg].up-- = value;
		++total;
	}
	void pop_back() {
		if (map[ed].down == 0) {
			if (ed != 0) {
				--ed;
				--map[ed].down;
			}
		}
		else
			--map[ed].down;
		--total;
	}
	void pop_front() {
		if (map[beg].up == chunk_size) {
			if (beg != chunk_size-1) {
				++beg
				++map[beg].up;
			}
		}
		else
			++map[beg].up;
		--total;
	}
};
#endif // !DEQUEUE_H
