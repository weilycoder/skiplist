#include "skiplist.h"
#include <iostream>
using namespace std;

int main() {
  SkipList<int, int> mp;
  mp.insert(0, 10);
  cout << mp.size() << endl;
  auto it = mp.find(0);
  cout << it->first << ' ' << it->second << endl;
  mp.insert(2, 100);
  mp.insert(-1, 23);
  mp.insert(6, 12);
  mp.insert(10, -20);
  cout << mp.size() << endl << endl;
  for (auto pt: mp)
    cout << pt.first << ' ' << pt.second << endl;
  cout << endl;
  mp.erase(it);
  mp.erase(6);
  for (auto pt: mp)
    cout << pt.first << ' ' << pt.second << endl;
  return 0;
}