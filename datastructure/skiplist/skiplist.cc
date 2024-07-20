#include <iostream>
#include "skiplist.hh"
#include "Point.hh"
#define FILE_PATH "dumpFile"

int main() {
     SkipList<Point, int> skiplist(10);

    // 插入一些 <Point, int> 键值对
    skiplist.insert_element(Point(1, 2), 10);
    skiplist.insert_element(Point(3, 4), 20);
    skiplist.insert_element(Point(5, 6), 30);
    skiplist.insert_element(Point(7, 8), 40);
    skiplist.insert_element(Point(2, 3), 50);
    skiplist.insert_element(Point(4, 5), 60);
    skiplist.insert_element(Point(6, 7), 70);

    // 打印跳表中的所有元素
    skiplist.display_list();
    skiplist.dump_file();

    // 查找某些元素
    skiplist.search_element(Point(3, 4));
    skiplist.search_element(Point(6, 7));
    skiplist.search_element(Point(8, 9));  // 这个元素不存在

    // 删除某些元素
    skiplist.delete_element(Point(5, 6));
    skiplist.delete_element(Point(7, 8));

    // 打印删除后的跳表
    skiplist.display_list();




    return 0;
}