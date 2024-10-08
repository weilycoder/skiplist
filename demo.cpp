#include "skiplist.h"
#include <iostream>
#include <vector>
using namespace std;

void show() {
  cout << __cplusplus << endl;
  SkipList<int, int> dict{{-63, 4}};
  cout << dict.max_size() << endl;

  dict.insert(0, 10);
  cout << dict.size() << endl;
  auto it = dict.find(0);
  cout << it->first << ' ' << it->second << endl;
  dict.insert(2, 100);
  dict.insert(-1, 23);
  dict.insert(6, 12);
  dict.insert(make_pair(10, -20));
  cout << dict.size() << endl;
  for (auto pt : dict)
    cout << pt.first << ' ' << pt.second << endl;
  dict.erase(it);

  SkipList<int, int> copy(dict);
  dict.erase(6);
  dict[10] = -23;
  for (auto pt : dict)
    cout << pt.first << ' ' << pt.second << endl;
  try {
    cout << dict.at(0) << endl;
  } catch (std::out_of_range &err) {
    cerr << "std::out_of_range: " << err.what() << endl;
  }
  cout << copy.size() << endl;
  cout << copy.at(6) << endl;
#if __cplusplus >= 201703L
  for (auto &&[k, v] : copy)
    cout << k << ' ' << v << endl;
#else
    for (auto p : copy)
      cout << p.first << ' ' << p.second << endl;
#endif
  copy = dict;
  cout << copy.size() << ' ' << dict.size() << endl;
  copy[998244353] = -1;
  swap(copy, dict);
  cout << dict.size() << ' ' << dict[998244353] << endl;

  vector<pair<char, int>> vci;
  for (int i = 'a'; i <= 'z'; ++i)
    vci.emplace_back(i, i);
  SkipList<char, int> dci(vci.begin(), vci.begin() + 5);
#if __cplusplus >= 201703L
  for (auto &&[k, v] : dci)
    cout << k << ' ' << v << endl;
#else
    for (auto p : dci)
      cout << p.first << ' ' << p.second << endl;
#endif
  cout << (dci.begin() + 2)->first << endl;
  for (auto it = dci.end() - 1; it != dci.before_begin(); --it)
    cout << it->first << ' ' << it->second << endl;
}

int main() {
  show();
  clog << "Exit." << endl;
  return 0;
}