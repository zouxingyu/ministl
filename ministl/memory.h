#pragma once
#ifndef MEMORY_H
#include <functional>
namespace ministl {
template <class T>
class shared_ptr {
 private:
  size_t *cnt;
  T *ptr;
  using deleter_type = std::function<void(T *)>;
  struct DefaultDeleter {
    void operator()(T *p) { 
		if(!--*cnt)
			delete p;
	}
  };
  deleter_type deleter;

 public:
  shared_ptr() noexcept : ptr(nullptr), cnt(nullptr), deleter(DefaultDeleter) {}
  explicit shared_ptr(T *p)
      : ptr(p), cnt(new size_t(1)), deleter(DefaultDeleter) {}
  template <class Deleter>
  shared_ptr(T *p, Deleter del) : ptr(p), cnt(new size_t(1)), deleter(del) {}
  shared_ptr(const shared_ptr &r) noexcept
      : ptr(r.ptr), cnt(r.cnt), deleter(DefaultDeleter) {
    ++(*cnt);
  }
  shared_ptr(shared_ptr &&r) noexcept : ptr(r.ptr), cnt(r.cnt) {
    r.ptr = r.cnt = nullptr;
  }
  shared_ptr &operator=(const shared_ptr &r) noexcept {
    if (r != *this) {
      if (!--*cnt) {
        deleter(ptr);
      }
      cnt = r.cnt;
      ++*cnt;
      ptr = r.ptr;
    }
    return *this;
  }
  shared_ptr &operator=(shared_ptr &&r) noexcept {
    if (r != *this) {
      if (!--*cnt) {
        deleter(ptr);
      }
      cnt = r.cnt;
      ptr = r.ptr;
      r.cnt = r.ptr = nullptr;
    }
    return *this;
  }
  void reset() noexcept { shared_ptr().swap(*this); }
  void reset(T *ptr) { shared_ptr(ptr).swap(*this); }
  template <class Deleter>
  void reset(T *ptr,Deleter d) {
    shared_ptr<T>(ptr, d).swap(*this);
  }
  void swap(shared_ptr &r) noexcept {
    std::swap(cnt, r.cnt);
    std::swap(ptr, r.ptr);
    std::swap(deleter, r.deleter);
  }
  T *get() const noexcept { return ptr; }
  T &operator*() const noexcept { return *ptr;
  }
  T *operator->() const noexcept { return ptr;
  }
  size_t use_count() const noexcept { return *cnt;
  }
  bool unique() { return use_count() == 1;
  }
  explicit operator bool() const noexcept { return ptr != nullptr;
  }
};
}  // namespace ministl
#endif  // !MEMORY_H
