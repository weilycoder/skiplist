用跳表实现的 map，暂时命名为 `skiplist`。

你觉得奇怪是因为我在写八股文（实现容器），也可能是因为八股文没写完。

~~命名应该是见名知意的，所以暂时没有文档。~~

下面的文档已经有部分内容过时，未来可能会迁移到 Wiki 页面。

已经实现的接口和（期望）效果，以及部分函数原型：

1. `before_begin`, `begin`, `end`：与其他容器的语义一致，提供迭代器。

2. $(1)$ 中函数加前缀 `c`：常量版。

3. `iterator`, `const_iterator`：迭代器接口，实现了自增、解引用、和相等运算符。自增运算是 $O(1)$ 的。

4. `insert`：语义与 `map` 的 `insert_or_assign` 类似，插入元素，若存在则赋值。
   ```cpp
   iterator insert(const K &key, const V &value);
   iterator insert(const value_type &pair);
   ```

   复杂度期望 $O(\log n)$。

5. `erase`：语义与 `map` 的 `erase` 类似，删除元素。
   ```cpp
   iterator erase(const iterator &pos);
   size_type erase(const K &key);
   ```

   + 对于第一个原型，返回 `pos` 的下一个迭代器，复杂度 $O(1)$；
   + 对于第二个原型，返回删除的元素数量，复杂度期望 $O(\log n)$。

6. `find`：语义与 `map` 的 `find` 类似，查询元素。

   ```cpp
   iterator find(const K &key);
   ```

   复杂度期望 $O(\log n)$，若元素不存在，返回尾后迭代器。

7. `at`：语义与 `map` 的 `at` 类似，返回元素的值，若不存在则报错。

   ```cpp
   V &at(const K &key);
   ```

   期望复杂度 $O(\log n)$。

8. `operator[]`：语义与 `map` 的 `operator[]` 类似，返回元素的值，若不存在则创建一个，值为 `V()`。

9. `empty`, `size`, `max_size`：语义与其他容器一致，其中 `max_size` 实现为

   ```cpp
   size_type max_size() const {
     return std::numeric_limits<difference_type>::max() / sizeof(node_t);
   }
   ```

10. `(constructor)`：接受以下构造方式

   ```cpp
   SkipList();
   SkipList(const skip_t &other);
   SkipList(skip_t &&other);
   SkipList(std::initializer_list<value_type> init);
   template <class InputIt> SkipList(InputIt first, InputIt last);
   ```

   第二种构造是 $O(n)$ 的，第四、五种构造是 $O(n\log n)$ 的（有序则 $O(n)$），其余是 $O(1)$ 的。

11. `(destructor)`：销毁容器，释放空间，复杂度 $O(n)$。
12. `operator=`：效果和复杂度与第二个构造函数类似。
13. `swap`：对 `swap` 特化。


计划使 `SkipList` 满足容器、关联容器、可逆容器，并使迭代器支持随机访问。