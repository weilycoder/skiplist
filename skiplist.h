#include <algorithm>
#include <cstdlib>
#include <cstring>
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

  static int _randLev() {
    int lv = 1;
    while (rand() < PS)
      ++lv;
    return std::min(L, lv);
  }

public:
  typedef iter_t iterator;
  typedef const iterator const_iterator;
  SkipList() : length(0) {
    head = new node_t(L);
    tail = new node_t(L);
    for (int i = L; i >= 0; --i)
      head->forward[i] = tail, tail->back[i] = head;
  }

  iterator before_begin() { return iterator(head); }
  iterator begin() { return iterator(head->forward[0]); }
  iterator end() { return iterator(tail); }

  const_iterator before_begin() const { return iterator(head); }
  const_iterator begin() const { return iterator(head->forward[0]); }
  const_iterator end() const { return iterator(tail); }

  const_iterator cbefore_begin() const { return iterator(head); }
  const_iterator cbegin() const { return iterator(head->forward[0]); }
  const_iterator cend() const { return iterator(tail); }

  void insert(const K &key, const V &value) {
    static node_t *update[L + 1];
    int lv = _randLev();
    node_t *newNode = new node_t(key, value, lv);
    node_t *p = head;

    for (int i = L; i >= 0; --i) {
      while (accessible(p->forward[i]) && p->forward[i]->val->first < key)
        p = p->forward[i];
      update[i] = p;
    }
    p = p->forward[0];
    if (accessible(p) && !(p->val->first > key)) {
      p->val->second = value;
      return;
    }

    for (int i = lv; i >= 0; --i) {
      newNode->back[i] = update[i];
      newNode->forward[i] = update[i]->forward[i];
      update[i]->forward[i] = newNode;
      newNode->forward[i]->back[i] = newNode;
    }
    ++length;
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
  const_iterator find(const K &key) const { return find(key); }

  size_type erase(const K &key) {
    iterator p = find(key);
    if (!accessible(p.nd))
      return 0;
    else
      return erase(p), 1;
  }

  bool empty() const { return !length; }
  size_type size() const { return length; }
  size_type max_size() const {
    return std::numeric_limits<difference_type>::max() / sizeof(node_t);
  }
#undef L
};
