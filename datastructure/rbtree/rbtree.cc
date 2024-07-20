#include <cstdio>
using namespace std;

// ftr father
// 找兄弟节点
#define bro(x) (((x)->ftr->lc == (x)) ? ((x)->ftr->rc) : ((x)->ftr->lc))

typedef bool RB_COLOR;
#define RB_COLOR_RED true
#define RB_COLOR_BLACK false

template <typename T>
class redblacktree {
protected:
    struct RB_Node;

    RB_Node* _root;
    RB_Node* _hot; // 父亲节点

    int _size; // 树大小

    void init(T);

    RB_Node* zig(RB_Node*); // 左旋
    RB_Node* zag(RB_Node*); // 右旋

    void SolveDoubleRed(RB_Node*);
    void SolveLostBlack(RB_Node*);

    RB_Node* find(T);

    void removetree(RB_Node*); // dfs后序遍历

public:
    struct iterator;

    redblacktree() : _root(nullptr), _hot(nullptr), _size(0) {}

    iterator insert(T);
    bool remove(T);
    bool remove(iterator&);

    iterator search(T);
    iterator lower_bound(T);
    iterator upper_bound(T);

    int size();
    void clear(); // dfs入口

    

    bool empty();

    iterator begin();
    static iterator end();

};

template <typename T> 
struct redblacktree<T>::RB_Node {
    T val;
    RB_COLOR RBc;
    RB_Node* ftr;
    RB_Node* lc;
    RB_Node* rc;

    RB_Node(T v = T(), RB_COLOR RB = RB_COLOR_RED, RB_Node* f = nullptr, RB_Node* lchild = nullptr, RB_Node* rchild = nullptr)
    : val(v), RBc(RB), ftr(f), lc(lchild), rc(rchild) {}


    // 真后继，在删除的时候用来覆盖自己
    RB_Node* succ() {
        RB_Node* ptn = rc;
        while (ptn->lc) {
            ptn = ptn->lc;
        }
        return ptn;
    }


    //左孩子找前驱
    RB_Node* left_node() {
        RB_Node* ptn = this;
        if (!lc) { // 左孩子为空，前驱为祖先
            // 自己是父亲左孩子，则要一直往上走，直到自己是父亲的右孩子，再走一步
            // 自己是父亲的右孩子，则前驱为父亲
            while (ptn->ftr && ptn->ftr->lc == ptn) { 
                ptn = ptn->ftr;
            }
            ptn = ptn->ftr;
        } else { // 左孩子不为空，前驱为左子树最右节点
            ptn = ptn->lc;
            while (ptn->rc) {
                ptn = ptn->rc;
            }
        }

        return ptn;
    }

    
    RB_Node* right_node() {
        RB_Node* ptn = this;
        if (!rc) { 
            while (ptn->ftr && ptn->ftr->rc == ptn) { 
                ptn = ptn->ftr;
            }
            ptn = ptn->ftr;
        } else {
            ptn = ptn->rc;
            while (ptn->lc) {
                ptn = ptn->lc;
            }
        }

        return ptn;
    }
};

template <typename T> 
struct redblacktree<T>::iterator {
protected:
    RB_Node* _real_node; // 迭代器指向的节点
public:

    iterator(RB_Node* node_nn = nullptr) : _real_node(node_nn) {}
    // 找到元素的位置
    iterator(T const& val_vv) : _real_node(find(val_vv)) {}
    iterator(const iterator& it) : _real_node(it._real_node) {}

public:
    T operator*() {
        return _real_node->val;
    }

    bool operator==(const iterator& it) {
        return _real_node == it._real_node;
    }

    // 用来判断指向的是否为空节点
    bool operator!() {
        return !_real_node;
    }

    iterator& operator=(iterator const it) {
        _real_node = it._real_node;
        return *this;
    }

    iterator& operator++() {
        _real_node = _real_node->right_node(); //找后继
        return *this;
    }

    iterator& operator--() {
        _real_node = _real_node->left_node(); //找后继
        return *this;
    }
    
};



template <typename T>
redblacktree<T>::RB_Node* redblacktree<T>::find(T v) {
    RB_Node* ptn = _root;
    _hot = nullptr;
    while (ptn && ptn->val != v) {
        _hot = ptn;
        if (ptn->val > v) {
            ptn = ptn->lc;
        } else {
            ptn = ptn->rc;
        }
    }
    return ptn;
}

template <typename T>
redblacktree<T>::iterator redblacktree<T>::search(T v) {
    return iterator(find(v));
}

template <typename T>
redblacktree<T>::iterator redblacktree<T>::lower_bound(T v) {
    RB_Node* ptn = _root;
    _hot = nullptr;
    while (ptn) {
        _hot = ptn;
        if (ptn->val >= v) {
            ptn = ptn->lc;
        } else {
            ptn = ptn->rc;
        }
    } // _hot不为空，ptn为空

    if (_hot->val >= v) { // 找到lower_bound
        ptn = _hot;
    } else { 
        ptn = _hot->right_node();
    }
    return iterator(ptn);
}

template <typename T>
redblacktree<T>::iterator redblacktree<T>::upper_bound(T v) {
    RB_Node* ptn = _root;
    _hot = nullptr;
    while (ptn) {
        _hot = ptn;
        if (ptn->val > v) {
            ptn = ptn->lc;
        } else {
            ptn = ptn->rc;
        }
    } // _hot不为空，ptn为空

    if (_hot->val > v) { // 找到upper_bound
        ptn = _hot;
    } else { 
        ptn = _hot->right_node();
    }
    return iterator(ptn);

}

template <typename T>
void redblacktree<T>::init(T v) {
    _root = new RB_Node(v, RB_COLOR_BLACK, nullptr, nullptr, nullptr);
    _size = 1;
}

//不希望有相同元素
template <typename T>
redblacktree<T>::iterator redblacktree<T>::insert(T v) {
    RB_Node* ptn = find(v);
    if (ptn) {
        return iterator(ptn);
    }
    // 没找到，此时_hot遍历到叶子节点，或者说整棵树一开始就为空

    if (!_hot) {
        // 树size为0, 因为用find必定往下搜，这样_hot只要走一步则必不为空
        init(v);
        return iterator(_root);
    }

    // 插入
    ++size;
    ptn = new RB_Node(v, RB_COLOR_RED, _hot, nullptr, nullptr);
    if (_hot->val < v) {
        _hot->rc = ptn;
    } else (_hot->val > v) {
        _hot->lc = ptn;
    }

    SolveDoubleRed(ptn);
    return iterator(ptn);
}

// 从不平衡的节点旋转
template <typename T>
redblacktree<T>::RB_Node* redblacktree<T>::zig(RB_Node* ptn) {
    ptn->lc->ftr = ptn->ftr; // 将自己的左孩子的父亲指向自己的父亲
    if (ptn->ftr) { // 将自己父亲的孩子指向自己的左孩子
        if (ptn->ftr->lc == ptn) { 
            ptn->ftr->lc = ptn->lc;
        } else {
            ptn->ftr->rc = ptn->lc;
        }
    }

    if (ptn->lc->rc) { // 自己左孩子的右孩子如果存在，那么旋转后就是自己的右孩子
        ptn->lc->rc->ftr = ptn;
    }

    ptn->ftr = ptn->lc; 
    ptn->lc = ptn->lc->rc;
    ptn->ftr->rc = ptn;

    return ptn->ftr; // 最后自己变成自己左孩子的右孩子，所以要返回自己的父亲
} 

template <typename T>
redblacktree<T>::RB_Node* redblacktree<T>::zag(RB_Node* ptn) {
    ptn->rc->ftr = ptn->ftr; // 将自己的右孩子的父亲指向自己的父亲
    if (ptn->ftr) { // 将自己父亲的孩子指向自己的右孩子
        if (ptn->ftr->lc == ptn) { 
            ptn->ftr->lc = ptn->rc;
        } else {
            ptn->ftr->rc = ptn->rc;
        }
    }

    if (ptn->rc->lc) { // 自己右孩子的左孩子如果存在，那么旋转后就是自己的左孩子
        ptn->rc->lc->ftr = ptn;
    }

    ptn->ftr = ptn->rc; 
    ptn->rc = ptn->rc->lc;
    ptn->ftr->lc = ptn;

    return ptn->ftr; // 最后自己变成自己右孩子的左孩子，所以要返回自己的父亲
}


template <typename T>
void redblacktree<T>::SolveDoubleRed(RB_Node* nn) { // 迭代
    while(nn->ftr && nn->ftr->RBc == RB_COLOR_RED) { // 排除了递归到根和RR-0情况
        RB_Node* pftr = nn->ftr;
        RB_Node* uncle = bro(pftr);
        RB_Node* grdftr = pftr->ftr;

        if (uncle && uncle->RBc == RB_COLOR_RED) { // RR-2 红色上溢两格
            grdftr->RBc = RB_COLOR_RED;
            uncle->RBc = RB_COLOR_BLACK;
            pftr->RBc = RB_COLOR_BLACK;

            nn = grdftr; 
        } else { // 不存在就是黑色（uncle） RR-1

            if (grdftr->lc == pftr) {
                if (pftr->lc == nn) { // 同向，同向只用一次旋转
                    if (grdftr == _root) {
                        _root = pftr;
                    }
                    zig(grdftr);
                    pftr->RBc = RB_COLOR_BLACK;
                    grdftr->RBc = RB_COLOR_RED;
                } else { // 非同向
                    if (grdftr == _root) {
                        _root == nn;
                    }
                    zag(pftr); zig(grdftr);
                    nn->RBc = RB_COLOR_BLACK;
                    grdftr->RBc = RB_COLOR_RED;
                }
            } else { // 另外一边，与上面对称
				if(pftr->lc == nn) {
					if(grdftr == _root) {
						_root = nn;
					}
					zig(pftr); zag(grdftr);
					nn->RBc = RB_COLOR_BLACK;
					grdftr->RBc = RB_COLOR_RED;
				} else {
					if(grdftr == _root) {
						_root = pftr;
					}
					zag(grdftr);
					pftr->RBc = RB_COLOR_BLACK;
					grdftr->RBc = RB_COLOR_RED;
				}
			}

            return;
        }
    }

    if (nn == _root) {
        nn->RBc = RB_COLOR_BLACK;
    }
}

template <typename T>
redblacktree<T>::iterator redblacktree<T>::begin() {
    RB_Node* ptn = _root;
    while (ptn->lc) {
        ptn = ptn->lc;
    }
    return iterator(ptn);
}

template <typename T>
redblacktree<T>::iterator redblacktree<T>::end() {
    return iterator(nullptr);
}

template <typename T>
int redblacktree<T>::size() {
	return _size;
}

template <typename T>
bool redblacktree<T>::empty() {
	return !_size;
}

template <typename T>
void redblacktree<T>::clear() {
    removetree(_root);
    _size = 0;
    _root = nullptr;
}

template <typename T>
void redblacktree<T>::removetree(RB_Node* ptn) {
    if (!ptn) return;
    remove(ptn->lc);
    remove(ptn->rc);
    delete ptn;
}

template <typename T>
bool redblacktree<T>::remove(T v) {
    RB_Node* ptn = find(v);
    RB_Node* node_suc; // 后继
    if (!ptn) return false; // 没有这个元素，删除失败
    
    --_size;
    // 找真后继
    while (ptn->lc || ptn->rc) { // 左右孩子至少有一个存在
        if (!ptn->lc) { // 只有右儿子(分析可得只能为黑色，且儿子是叶子节点)
            node_suc = ptn->rc; 
        } else if (!ptn->rc) { // 只有左孩子
            node_suc = ptn->lc;
        } else {
            node_suc = ptn->succ();
        }
        ptn->val = node_suc->val;
        ptn = node_suc;
    } // 直到没有真后继

    // 将ptn删除
    if (ptn->RBc == RB_COLOR_BLACK) {
        SolveLostBlack(ptn);
    }
    if (ptn->ftr) {
        if (ptn->ftr->lc == ptn) {
            ptn->ftr->lc = nullptr;
        } else {
            ptn->ftr->rc = nullptr;
        }
    }

    if (ptn == _root) {
        // 没有左右孩子，只有根了
        _root = nullptr;
    }
    delete ptn;
    return;
}

template <typename T>
bool redblacktree<T>::remove(iterator& it) {
    RB_Node* ptn = it._real__node;
    
    // 防止迭代器失效
	it._real__node = it._real__node->right_node();
    if(!it._real__node) {
		it._real__node = ptn->left_node();
	}

    RB_Node* node_suc;
	--_size;
	while(ptn->lc || ptn->rc) {
		if(!(ptn->lc)) {
			node_suc = ptn->rc;
		} else if(!(ptn->rc)) {
			node_suc = ptn->lc;
		} else {
			node_suc = ptn->succ();
		}
		ptn->val = node_suc->val;
		ptn = node_suc;
	}
	if(ptn->RBc == RB_COLOR_BLACK) {
		SolveLostBlack(ptn);
	}
	if(ptn->ftr) {
		if(ptn->ftr->lc == ptn) {
			ptn->ftr->lc = NULL;
		} else {
			ptn->ftr->rc = NULL;
		}
	}
	if(_root == ptn) {
		_root = NULL;
	}
	delete ptn;
	return true;
}


template <typename T>
void redblacktree<T>::SolveLostBlack(RB_Node* ptn) {
    
}