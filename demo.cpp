#include "skiplist.h"
#include <iostream>
#include <vector>
using namespace std;

void show() {
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
  for (auto &&[k, v] : copy)
    cout << k << ' ' << v << endl;
  copy = dict;
  cout << dict.size() << endl;
  vector<pair<char, int>> v;
  for (int i = 'a'; i <= 'z'; ++i)
    v.emplace_back(i, i);
  SkipList<char, int> dci(v.begin(), v.begin() + 5);
  for (auto &&[k, v] : dci)
    cout << k << ' ' << v << endl;
}

int main() {
  show();
  clog << "Exit." << endl;
  return 0;
}