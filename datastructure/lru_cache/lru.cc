/**
 * @file lru.cc
 * @author Ym_cmd (yim45ad@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <unordered_map>
using namespace std;

struct Node {
    int key, value;
    Node* prev, *next;
    Node(int k = 0, int v = 0) : key(k), value(v) {}
};

class LRUCache {
private:
    int _capacity;
    unordered_map<int, Node*> _key_to_node;
    Node* _dummy;
public:
    LRUCache(int capacity) : _capacity(capacity), _dummy(new Node()) { 
        _dummy->prev = _dummy;
        _dummy->next = _dummy;
    }
    
    int get(int key) {
        auto node = get_node(key);
        return node ? node->value : -1;
    }
    
    void put(int key, int value) {
        // 存在
        auto node = get_node(key);
        if (node) {
            node->value = value;
            return;
        }

        // 不存在
        if (_key_to_node.size() == _capacity) {
            // 删最久未使用的关键字
            auto back_node = _dummy->prev;
            _key_to_node.erase(back_node->key);
            remove(back_node);
            delete back_node;
        }

        _key_to_node[key] = node = new Node(key, value);
        push_front(node);
    }

private:
    Node* get_node(int key) {
        auto it = _key_to_node.find(key);
        if (it == _key_to_node.end()) {
            return nullptr;
            
        }
        auto node = it->second;
        remove(node);
        push_front(node);
        return node;
    }

    void remove(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void push_front(Node* node) {
        node->prev = _dummy;
        node->next = _dummy->next;
        node->next->prev = node;
        node->prev->next = node;
    }
};