/**
 * @file skiplist.hh
 * @author Ym_cmd (yim45ad@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-19
 * 0. 个人跳表学习，参照 https://github.com/youngyangyang04/Skiplist-CPP
 * 1. 性能测试
 *  跳表树高：18 
 * 采用随机插入数据测试：
 * |插入数据规模（万条） |耗时（秒） | 
 *|---|---|
 *|10 |0.316763 |
 *|50 |1.86778 |
 *|100 |4.10648 |
 *
 *
 *每秒可处理写请求数（QPS）: 24.39w
 *
 *|取数据规模（万条） |耗时（秒） | 
 *|---|---|
 *|10|0.47148 |10|
 *|50|2.56373 |50|
 *|100|5.43204 |100|
 *
 *每秒可处理读请求数（QPS）: 18.41w
 * 2. 线程安全, 锁粒度很大
 * 3. 去重
 * 4. 有顺序
 * 5. 对于自定义类型，重载 >, <, ==, != 即可使用
 * 
 * @copyright Copyright (c) 2024
 * 
 */
//#include "Point.hh"
#include <iostream> 
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>
#include <fstream>


#define STORE_FILE "dumpFile"
std::mutex mtx;     // mutex for critical section
std::string delimiter = ":";

template <typename K, typename V> 
class Node {
    K _key;
    V _value;
public:
    Node() {}
    Node(K k, V v, int);
    ~Node();

    K get_key() const;
    V get_value() const;

    void set_value(V);

    // 节点的层级是从0开始的。因此，如果一个节点的最大层级是 level，
    // 那么它实际上有 level + 1 个前进指针
    // new Node<K, V>*[level + 1] 分配了一个指针数组，这个数组的大小为 level + 1。
    // 数组中的每个元素都是一个指向 Node<K, V> 类型的指针，用于指向相应层级上的下一个节点

    Node<K, V>** _forward; 
    int _node_level; 
};

// Node code
template <typename K, typename V> 
Node<K, V>::Node(const K k, const V v, int level) {
    _key = k;
    _value = v;
    _node_level = level;

    // 如果一个节点的最大层级是 level，那么它实际上有 level + 1 个前进指针。
    
    // Node<K, V> node(5, "value", 2);
    // node_level 是2，表示节点存在于层级0、1和2。
    // forward 指针数组的大小是 2 + 1 = 3，分别对应层级0、1和2。
    // forward[0] 指向层级0的下一个节点。
    // forward[1] 指向层级1的下一个节点。
    //  forward[2] 指向层级2的下一个节点。
    _forward = new Node<K, V>*[level + 1];
    memset(_forward, 0, sizeof(Node<K, V>*) * (level + 1));

}

template <typename K, typename V> 
Node<K, V>::~Node() {
    delete [] _forward;
}

template <typename K, typename V> 
K Node<K, V>::get_key() const {
    return _key;
}

template <typename K, typename V> 
V Node<K, V>::get_value() const {
    return _value;
}

template<typename K, typename V> 
void Node<K, V>::set_value(V value) {
    _value = value;
};

// Class template for Skip list
template <typename K, typename V> 
class SkipList {
    // Maximum level of the skip list
    int _max_level;

    // current level of skip list 
    int _skip_list_level;

    // file operator
    std::ofstream _file_writer;
    std::ifstream _file_reader;

    // pointer to header node 
    // 类似于一个dummy，它的forward指向所有层的第一个结点
    Node<K, V> *_header;

    // skiplist current element count 类似于size(不包括索引
    int _element_count;
public:
    SkipList(int);
    ~SkipList();
    int get_random_level();
    Node<K, V>* create_node(K, V, int);
    int insert_element(K, V);
    void display_list();
    bool search_element(K);
    void delete_element(K);
    void dump_file();
    void load_file();

    void clear(Node<K, V>*);
    int size();
private:
    void get_key_value_from_string(const std::string& str, std::string* key, std::string* value);
    bool is_valid_string(const std::string& str);
};

template <typename K, typename V>
SkipList<K, V>::SkipList(int max_level) {
    _max_level = max_level;
    _skip_list_level = 0;
    _element_count = 0;

    K k;
    V v;
    _header = new Node<K, V>(k, v, _max_level);
}

// create new node 
template<typename K, typename V>
Node<K, V>* SkipList<K, V>::create_node(const K k, const V v, int level) {
    Node<K, V> *n = new Node<K, V>(k, v, level);
    return n;
}

// Search for element in skip list 
/*
                           +------------+
                           |  select 60 |
       (_header)           +------------+
level 4     +-->1+                                                      100
                 |
                 |
level 3         1+-------->10+------------------>50+           70       100
                                                   |
                                                   |
level 2         1          10         30         50|           70       100
                                                   |
                                                   |
level 1         1    4     10         30         50|           70       100
                                                   |
                                                   |
level 0         1    4   9 10         30   40    50+-->60      70       100
*/

template <typename K, typename V>
bool SkipList<K, V>::search_element(K key) {
    // 定义一个指针current，初始化为跳表的头节点_header
    Node<K, V>* current = _header;

    // 从跳表的最高层开始搜索
    for (int i = _skip_list_level; i >= 0; --i) {
        // 遍历当前层级，直到下一个节点的键值大于或等于待查找的键值
        while (current->_forward[i] && current->_forward[i]->get_key() < key) {
            // 移动到当前层的下一个节点
            current = current->_forward[i];
        }
        // 当前节点的下一个节点的键值大于待查找的键值时，进行下称到下一层
        // 下沉操作通过--i实现
    }

    // 检查当前层（最底层）的下一个节点的键值是否为待查找的键值
    current = current->_forward[0];
    if (current && current->get_key() == key) {
        return true;
    }

    // 没找到
    return false;
}


// Insert given key and value in skip list 
// return 1 means element exists  
// return 0 means insert successfully
/* 
                           +------------+
                           |  insert 50 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |                      insert +----+
level 3         1+-------->10+---------------> | 50 |          70       100
                                               |    |
                                               |    |
level 2         1          10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 1         1    4     10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 0         1    4   9 10         30   40  | 50 |  60      70       100
                                               +----+

*/

template <typename K, typename V>
int SkipList<K, V>::insert_element(const K key, const V value) {
    mtx.lock();

    Node<K, V>* current = _header;

    // 用于在各层更新指针的数组
    // 用于存储在每一层需要更新的前驱结点，_max_level + 1是数组的大小，确保可以存储所有层的前驱节点
    Node<K, V>* update[_max_level + 1]; // 用于记录各层中待更新的节点
    memset(update, 0, sizeof(Node<K, V>*) * (_max_level + 1));

    // 从最高层向下搜索插入位置
    for (int i = _skip_list_level; i >= 0; --i) {
        // 寻找当前层中最接近且小于key的节点
        while (current->_forward[i] && current->_forward[i]->get_key() < key) {
            current = current->_forward[i]; 
        }
        // 保存每层中该节点，以便后续插入时更新指针
        update[i] = current;
    }


    // 移动到最底层的下一节点，准备插入操作
    current = current->_forward[0];
    // 检查待插入的节点的键是否已存在
    if (current && current->get_key() == key) {
        mtx.unlock();
        return 1;
    }

    // 检查待插入的节点是否已经存在于跳表中
    if (current == NULL || current->get_key() != key) {
        // 通过随机函数决定新节点的层次高度
        int random_level = get_random_level();
        
        // 如果新节点的层级超出了跳表当前的最高层级
        if (random_level > _skip_list_level) {
            // 对所有新的最高层级，将头节点(_header(或者叫dummy))设置为它们的前驱节点
            for (int i = _skip_list_level + 1; i <= random_level; ++i) {
                update[i] = _header; 
                // 第i层(这里都是新加的层，所以前驱为_header, 要将_header->_forward[i]更新为新节点)
            }

            _skip_list_level = random_level;
        }

        Node<K, V>* inserted_node = create_node(key, value, random_level);
        
        // 在各层插入新节点，同时更新前驱节点的forward指针
        for (int i = 0; i <= random_level; ++i) {
            // 新节点指向当前节点的下一个节点
            inserted_node->_forward[i] = update[i]->_forward[i];

            // 当前节点的下一个节点更新为新节点
            update[i]->_forward[i] = inserted_node;
        }

        _element_count++;
    }
    mtx.unlock();
    return 0;
}


template<typename K, typename V> 
int SkipList<K, V>::get_random_level() {
    int k = 1;
    while (rand() % 2) {
        k++; 
    }
    k = (k < _max_level) ? k : _max_level;
    return k;
}

template <typename K, typename V> 
void SkipList<K, V>::display_list() {

    std::cout << "\n*****Skip List*****"<<"\n"; 
    for (int i = 0; i <= _skip_list_level; ++i) {
        Node<K, V>* node = _header->_forward[i];
        std::cout << "Level" << i << ":";
        while (node != nullptr) {
            std::cout << "(" << node->get_key() << " : " << node->get_value() << ")";
            node = node->_forward[i];
        }
        std::cout << std::endl;
    }
}

template <typename K, typename V> 
int SkipList<K, V>::size() {
    return _element_count;
}

// Dump data in memory to file 
template<typename K, typename V> 
void SkipList<K, V>::dump_file() {
    std::cout << "dump_file-----------------" << std::endl;
    _file_writer.open(STORE_FILE);
    Node<K, V>* node = _header->_forward[0];

    while (node != nullptr) {
        _file_writer << node->get_key() << " : " << node->get_value() << '\n';
        std::cout << node->get_key() << " : " << node->get_value() << ";\n";
        node = node->_forward[0];
    }

    _file_writer.flush();
    _file_writer.close();
    return;
}

// Load data from disk
template<typename K, typename V> 
void SkipList<K, V>::load_file() {
    _file_reader.open(STORE_FILE);
    std::cout << "load_file-----------------" << std::endl;
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();

    while (getline(_file_reader, line)) {
        get_key_value_from_string(line, key, value);
        if (key->empty() || value->empty()) {
            continue;
        }

        // Define key as int type
        insert_element(stoi(*key), stoi(*value));
        std::cout << "key: " << *key << " value: " << *value << std::endl; 
    }
    delete key;
    delete value;
    _file_reader.close();
}

template<typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string& str, std::string* key, std::string* value) {
    if (!is_valid_string(str)) {
        return;
    }
    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter) + 1, str.length());
}

template<typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string& str) {

    if (str.empty()) {
        return false;
    }
    if (str.find(delimiter) == std::string::npos) {
        return false;
    }
    return true;
}

// Delete element from skip list 
template<typename K, typename V> 
void SkipList<K, V>::delete_element(K key) {
    mtx.lock();

    Node<K, V>* current = _header;
    Node<K, V>* update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V>*)*(_max_level+1));


    // start from highest level of skip list
    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->_forward[i] !=NULL && current->_forward[i]->get_key() < key) {
            current = current->_forward[i];
        }
        update[i] = current; // 把需要更新的前驱保存起来
    }

    current = current->_forward[0]; 
    if (current && current->get_key() == key) {
        // start for lowest level and delete the current node of each level
        for (int i = 0; i <= _skip_list_level; ++i) {
            // if at level i, next node is not target node, break the loop.
            if (update[i]->_forward[i] != current) {
                break;
            }

            update[i]->_forward[i] = current->_forward[i];
        }

        // Remove levels which have no elements
        while (_skip_list_level > 0 && _header->_forward[_skip_list_level] == nullptr) {
            _skip_list_level--;
        }

        std::cout << "Successfully deleted key "<< key << std::endl;
        delete current;
        _element_count --;

    }
    mtx.unlock();
    return;

}

template<typename K, typename V> 
SkipList<K, V>::~SkipList() {
    if (_file_writer.is_open()) {
        _file_writer.close();
    }
    if (_file_reader.is_open()) {
        _file_reader.close();
    }

    //递归删除跳表链条
    if(_header->_forward[0]!=nullptr){
        clear(_header->_forward[0]);
    }
    delete(_header);
    
}

template <typename K, typename V>
void SkipList<K, V>::clear(Node<K, V> * cur)
{
    if(cur->_forward[0]!=nullptr){
        clear(cur->_forward[0]);
    }
    delete(cur);
}
