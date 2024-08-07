#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <stdexcept>
#include <utility>

template <typename K, typename V> class SkipList {
public:
  typedef K key_type;
  typedef V mapped_type;
  typedef std::pair<const key_type, mapped_type> value_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef std::ptrdiff_t difference_type;
  typedef std::size_t size_type;

private:
#define L 32
  static const size_t ip = 4;
  struct node_t {
    int level;
    value_type *val;
    node_t **forward, **back;
    void init() {
      back = new node_t *[level + 1];
      forward = new node_t *[level + 1];
      memset(back, 0, sizeof(back) * (level + 1));
      memset(forward, 0, sizeof(forward) * (level + 1));
    }
    node_t(int _l) : val(NULL), level(_l) { init(); }
    node_t(const K &_k, const V &_v, int _l) : level(_l) {
      val = new value_type(_k, _v), init();
    }
    ~node_t() { delete[] forward, delete[] back; }
  } *head, *tail;

  size_type length;

  struct iter_t {
    node_t *nd;
    iter_t() : nd(NULL) {}
    iter_t(node_t *ptr) : nd(ptr) {}
    reference operator*() { return *nd->val; }
    value_type *operator->() { return nd->val; }
    iter_t &operator++() { return nd = nd->forward[0], *this; }
    bool operator==(const iter_t &b) const { return nd == b.nd; }
    bool operator!=(const iter_t &b) const { return nd != b.nd; }
  };

  typedef SkipList skip_t;
  static const int PS = RAND_MAX / ip;

  static bool accessible(node_t *ptr) { return ptr && ptr->val; }
  static bool accessible(iter_t it) { return accessible(it.nd); }

  static int _randLev() {
    int lv = 1;
    while (rand() < PS)
      ++lv;
    return std::min(L, lv);
  }

  void _init() {
    head = new node_t(L);
    tail = new node_t(L);
    for (int i = L; i >= 0; --i)
      head->forward[i] = tail, tail->back[i] = head;
  }

  void _destory() {
    node_t *ptr = head;
    while (ptr != tail) {
      node_t *temp = ptr;
      ptr = ptr->forward[0];
      delete temp;
    }
    delete ptr;
  }

  void _push_back(const K &key, const V &value) {
    int lv = _randLev();
    node_t *newNode = new node_t(key, value, lv);
    for (int i = lv; i >= 0; --i) {
      newNode->forward[i] = tail;
      newNode->back[i] = tail->back[i];
      tail->back[i] = newNode;
      newNode->back[i]->forward[i] = newNode;
    }
    ++length;
  }

public:
  typedef iter_t iterator;
  typedef const iterator const_iterator;
  SkipList() : length(0) { _init(); }
  SkipList(const skip_t &other) : length(0) {
    _init();
    for (auto &&[k, v] : other)
      _push_back(k, v);
  }
  SkipList(skip_t &&other) {
    head = other.head;
    tail = other.tail;
    length = other.length;
  }
  SkipList(std::initializer_list<value_type> init) {
    _init();
    if (std::is_sorted(init.begin(), init.end()))
      for (auto &&[k, v] : init)
        _push_back(k, v);
    else
      for (const auto &pair : init)
        insert(pair);
  }
  template <class InputIt> SkipList(InputIt first, InputIt last) {
    _init();
    if (std::is_sorted(first, last))
      for (InputIt it = first; it != last; ++it)
        _push_back(it->first, it->second);
    else
      for (InputIt it = first; it != last; ++it)
        insert(it->first, it->second);
  }
  ~SkipList() { _destory(); }

  skip_t &operator=(const skip_t &other) {
    if (other.head == head)
      return *this;
    _destory(), _init();
    for (auto &&[k, v] : other)
      _push_back(k, v);
    return *this;
  }

  iterator before_begin() { return iterator(head); }
  iterator begin() { return iterator(head->forward[0]); }
  iterator end() { return iterator(tail); }

  const_iterator before_begin() const { return const_iterator(head); }
  const_iterator begin() const { return const_iterator(head->forward[0]); }
  const_iterator end() const { return const_iterator(tail); }

  const_iterator cbefore_begin() const { return const_iterator(head); }
  const_iterator cbegin() const { return const_iterator(head->forward[0]); }
  const_iterator cend() const { return const_iterator(tail); }

  iterator insert(const K &key, const V &value) {
    static node_t *update[L + 1];
    node_t *p = head;

    for (int i = L; i >= 0; --i) {
      while (accessible(p->forward[i]) && p->forward[i]->val->first < key)
        p = p->forward[i];
      update[i] = p;
    }
    p = p->forward[0];
    if (accessible(p) && !(p->val->first > key)) {
      p->val->second = value;
      return iterator(p);
    }

    int lv = _randLev();
    node_t *newNode = new node_t(key, value, lv);
    for (int i = lv; i >= 0; --i) {
      newNode->back[i] = update[i];
      newNode->forward[i] = update[i]->forward[i];
      update[i]->forward[i] = newNode;
      newNode->forward[i]->back[i] = newNode;
    }
    ++length;
    return iterator(newNode);
  }
  iterator insert(const value_type &pair) {
    return insert(pair.first, pair.second);
  }

  iterator erase(const iterator &pos) {
    node_t *ptr = pos.nd;
    for (int i = ptr->level; i >= 0; --i) {
      ptr->back[i]->forward[i] = ptr->forward[i];
      ptr->forward[i]->back[i] = ptr->back[i];
    }
    iterator rt(ptr->forward[0]);
    delete ptr;
    --length;
    return rt;
  }

  iterator find(const K &key) {
    node_t *p = head;
    for (int i = L; i >= 0; --i)
      while (accessible(p->forward[i]) && p->forward[i]->val->first < key)
        p = p->forward[i];
    p = p->forward[0];

    if (accessible(p) && !(p->val->first > key))
      return iterator(p);
    else
      return iterator(tail);
  }

  size_type erase(const K &key) {
    iterator p = find(key);
    if (!accessible(p))
      return 0;
    else
      return erase(p), 1;
  }

  V &at(const K &key) {
    iterator it = find(key);
    if (accessible(it))
      return it->second;
    else
      throw std::out_of_range("skiplist::at");
  }
  V &operator[](const K &key) {
    iterator it = find(key);
    if (accessible(it))
      return it->second;
    else
      return insert(key, V())->second;
  }

  bool empty() const { return !length; }
  size_type size() const { return length; }
  size_type max_size() const {
    return std::numeric_limits<difference_type>::max() / sizeof(node_t);
  }
#undef L
};
