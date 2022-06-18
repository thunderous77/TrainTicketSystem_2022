#ifndef MAIN_CPP_CBPLUSTREE_HPP
#define MAIN_CPP_CBPLUSTREE_HPP

#include "vector.hpp"
#include "MemoryRiver.hpp"

using namespace std;
using namespace sjtu;
#define MINNUM_KEY (ORDER - 1)
#define MAXNUM_KEY (2 * ORDER)
#define MINNUM_CHILD (MINNUM_KEY + 1)
#define MAXNUM_CHILD (MAXNUM_KEY + 1)
#define MINNUM_LEAF (MINNUM_KEY)
#define MAXNUM_LEAF (MAXNUM_KEY)
enum NODE_TYPE {
    INTERNAL, LEAF
};        // 结点类型：内结点、叶子结点
enum SIBLING_DIRECTION {
    LEFT, RIGHT
};   // 兄弟结点方向：左兄弟结点、右兄弟结点
// const int ORDER = 20;                   // B+树的阶（非根内结点的最小子树个数）
// const int MINNUM_KEY = ORDER - 1;        // 最小键值个数
// const int MAXNUM_KEY = 2 * ORDER - 1;      // 最大键值个数
// const int MINNUM_CHILD = MINNUM_KEY + 1; // 最小子树个数
// const int MAXNUM_CHILD = MAXNUM_KEY + 1; // 最大子树个数
// const int MINNUM_LEAF = MINNUM_KEY;    // 最小叶子结点键值个数
// const int MAXNUM_LEAF = MAXNUM_KEY;    // 最大叶子结点键值个数
// const int CHAR_LENGTH = 42;//key长度

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
class CBPlusTree;

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
class CInternalNode;

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
class CLeafNode;

template<class T, int CHAR_LENGTH = 42, int ORDER = 55>
struct KeyDataType {
    char KeyValue[CHAR_LENGTH];
    T DataValue;

    KeyDataType() = default;

    KeyDataType(string key, T data) {
        for (int iter = 0; iter < key.size(); ++iter)
            KeyValue[iter] = key[iter];
        for (int iter = key.size(); iter < CHAR_LENGTH; ++iter)
            KeyValue[iter] = '\0';
        DataValue = data;
    }

    KeyDataType &operator=(const KeyDataType &other) {
        if (this == &other) return *this;
        for (int iter = 0; iter < CHAR_LENGTH; ++iter) {
            KeyValue[iter] = other.KeyValue[iter];
        }
        DataValue = other.DataValue;
        return *this;
    }

    bool operator>(const KeyDataType &other) {
        return (strcmp(KeyValue, other.KeyValue) > 0 ||
                (strcmp(KeyValue, other.KeyValue) == 0 && DataValue > other.DataValue));
    }

    bool operator==(const KeyDataType &other) {
        return (strcmp(KeyValue, other.KeyValue) == 0 && DataValue == other.DataValue);
    }

    bool operator!=(const KeyDataType &other) {
        return !(strcmp(KeyValue, other.KeyValue) == 0 && DataValue == other.DataValue);
    }

    bool operator<(const KeyDataType &other) {
        return (strcmp(KeyValue, other.KeyValue) < 0 ||
                (strcmp(KeyValue, other.KeyValue) == 0 && DataValue < other.DataValue));
    }

    bool operator>=(const KeyDataType &other) {
        return !(*this < other);
    }

    bool operator<=(const KeyDataType &other) {
        return !(*this > other);
    }
};

// 内结点
template<class T1, class T2, int CHAR_LENGTH = 42, int ORDER = 55>
class CInternalNode {
private:
    NODE_TYPE m_Type;
    int m_KeyNum;
    KeyDataType<T1, CHAR_LENGTH, ORDER> m_KeyData[MAXNUM_KEY];
    int storePossession;
    int m_Childs[MAXNUM_CHILD];
    NODE_TYPE childs_type;
public:
    CInternalNode();

    ~CInternalNode();

    int getPossession() const { return storePossession; }

    void setPossession(int p) { storePossession = p; }

    NODE_TYPE getType() const { return m_Type; }

    void setType(NODE_TYPE type) { m_Type = type; }

    int getKeyNum() const { return m_KeyNum; }

    void setKeyNum(int n) { m_KeyNum = n; }

    KeyDataType<T1, CHAR_LENGTH, ORDER> getKeyDataValue(int i) { return m_KeyData[i]; }

    int compareKeyvalue(int i, string key) { return strcmp(key.c_str(), m_KeyData[i].KeyValue); }

    void setKeyData(int i, KeyDataType<T1, CHAR_LENGTH, ORDER> keyData) {
        m_KeyData[i] = keyData;
    }

    int getKeyIndex(string key);  // 找到键值在结点中存储的下标(第一个不小于key的下标)

    int getKeyDataIndex(KeyDataType<T1, CHAR_LENGTH, ORDER> keyData);

    int getChildIndex(KeyDataType<T1, CHAR_LENGTH, ORDER> keyData, int keyIndex) {
        if (keyIndex >= getKeyNum()) return getKeyNum();
        if (keyData < m_KeyData[keyIndex]) return keyIndex;
        else return keyIndex + 1;
    }

    int getChild(int i) { return m_Childs[i]; }

    CInternalNode<T1, T2, CHAR_LENGTH, ORDER> getChildInternal(int i, CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree);

    CLeafNode<T1, T2, CHAR_LENGTH, ORDER> getChildLeaf(int i, CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree);

    void
    split(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> &parentNode, int childIndex,
          CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree);
    //文件中不更新parentNode

    void setChild(int i, int child, NODE_TYPE type) {
        m_Childs[i] = child;
        childs_type = type;
    }

    void
    insert(int keyIndex, int childIndex, KeyDataType<T1, CHAR_LENGTH, ORDER> keyData, int childNode, NODE_TYPE type);
    //在keyIndex/childIndex位置插入
    // 不修改文件

    NODE_TYPE getChildtype() { return childs_type; }

    void mergeChild(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> parentNode,
                    CInternalNode<T1, T2, CHAR_LENGTH, ORDER> childNode,
                    int keyIndex,
                    CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree);//将childnode（右）合并到this（左）里

    void removeKey(int keyIndex, int childIndex);//不修改文件

    void clear(CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree);

    void borrowFrom(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> siblingNode,
                    CInternalNode<T1, T2, CHAR_LENGTH, ORDER> parentNode,
                    int keyIndex, SIBLING_DIRECTION d,
                    CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree);//尽量从左边借，复杂度小

    int getChildIndex(string key, int keyIndex);

};

// 叶子结点
template<class T1, class T2, int CHAR_LENGTH = 42, int ORDER = 55>
class CLeafNode {
private:
    NODE_TYPE m_Type;
    int m_KeyNum;
    KeyDataType<T1, CHAR_LENGTH, ORDER> m_KeyData[MAXNUM_LEAF];
    T2 m_Datas[MAXNUM_LEAF];
    int storePossession;
    int m_LeftSibling;
    int m_RightSibling;
public:
    CLeafNode();

    ~CLeafNode();

    int getPossession() const { return storePossession; }

    void setPossession(int p) { storePossession = p; }

    NODE_TYPE getType() const { return m_Type; }

    void setType(NODE_TYPE type) { m_Type = type; }

    int getKeyNum() const { return m_KeyNum; }

    void setKeyNum(int n) { m_KeyNum = n; }

    void setKeyData(int i, KeyDataType<T1, CHAR_LENGTH, ORDER> keyData) { m_KeyData[i] = keyData; }

    void setData(int i, const T2 &data) { m_Datas[i] = data; }

    int getKeyDataIndex(KeyDataType<T1, CHAR_LENGTH, ORDER> keyData);

    int compareKeyvalue(int i, string key) { return strcmp(key.c_str(), m_KeyData[i].KeyValue); }

    int getKeyIndex(string key);

    KeyDataType<T1, CHAR_LENGTH, ORDER> getKeyDataValue(int i) { return m_KeyData[i]; }

    void setLeftSibling(int possession) { m_LeftSibling = possession; }

    CLeafNode<T1, T2, CHAR_LENGTH, ORDER> getRightSibling(CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree) const;

    int getRightPossession();

    void setRightSibling(int possession) { m_RightSibling = possession; }

    T2 getDataValue(int i) const { return m_Datas[i]; }

    void remove(KeyDataType<T1, CHAR_LENGTH, ORDER> keyData, CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree);

    void insert(KeyDataType<T1, CHAR_LENGTH, ORDER> keyData, const T2 &data2);

    void
    split(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> &parentNode, int childIndex,
          CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree);
    //文件中不更新parentNode

    void
    mergeChild(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> parentNode, CLeafNode<T1, T2, CHAR_LENGTH, ORDER> childNode,
               int keyIndex, CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree);

    void removeKey(int keyIndex, CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree);

    void clear(CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree);

    void
    borrowFrom(CLeafNode<T1, T2, CHAR_LENGTH, ORDER> siblingNode, CInternalNode<T1, T2, CHAR_LENGTH, ORDER> parentNode,
               int keyIndex, SIBLING_DIRECTION d,
               CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree);
};

template<class T1, class T2, int CHAR_LENGTH = 42, int ORDER = 55>
class CBPlusTree {
public:
    CBPlusTree(const string &FN);

    ~CBPlusTree();

    void insert(string key, const T1 &data1, const T2 &data2);

    void Delete(string key, const T1 &data);

    Vector<T2> FindAll(string compareKey);

    bool Find(string key);

    void clean();

    void InternalRead(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> &node, int index) {
        CInternalNode_store.read(node, index);
    };

    void InternalDelete(int index) { CInternalNode_store.Delete(index); }

    void InteranlUpdate(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> &node, int index) {
        CInternalNode_store.update(node, index);
    }

    int InternalWrite(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> &node) { return CInternalNode_store.write(node); }

    void LeafRead(CLeafNode<T1, T2, CHAR_LENGTH, ORDER> &node, int index) { CLeafNode_store.read(node, index); };

    void LeafDelete(int index) { CLeafNode_store.Delete(index); }

    void LeafUpdate(CLeafNode<T1, T2, CHAR_LENGTH, ORDER> &node, const int index) {
        CLeafNode_store.update(node, index);
    }

    int LeafWrite(CLeafNode<T1, T2, CHAR_LENGTH, ORDER> &node) { return CLeafNode_store.write(node); }

private:
    MemoryRiver<CInternalNode<T1, T2, CHAR_LENGTH, ORDER>, 2> CInternalNode_store;
    MemoryRiver<CLeafNode<T1, T2, CHAR_LENGTH, ORDER>, 3> CLeafNode_store;
    NODE_TYPE rootType;
    int m_DataHead, rootPossession;
private:

    void recursive_insertLeaf(CLeafNode<T1, T2, CHAR_LENGTH, ORDER> parentNode,
                              KeyDataType<T1, CHAR_LENGTH, ORDER> keyData, const T2 &data2);

    bool recursive_insertInternal(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> &parentNode,
                                  KeyDataType<T1, CHAR_LENGTH, ORDER> keyData, const T2 &data2);

    bool recursive_searchInternal(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> pNode, string key);

    int recursive_searchFindInternal(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> pNode, string key);

    bool recursive_searchLeaf(CLeafNode<T1, T2, CHAR_LENGTH, ORDER> pNode, string key);

    int searchFind(string key);

    void changeKey(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> pNode, KeyDataType<T1, CHAR_LENGTH, ORDER> oldKeyData,
                   KeyDataType<T1, CHAR_LENGTH, ORDER> newKeyData);

    void recursive_removeInternal(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> parentNode,
                                  KeyDataType<T1, CHAR_LENGTH, ORDER> keyData);

    void recursive_removeLeaf(CLeafNode<T1, T2, CHAR_LENGTH, ORDER> parentNode,
                              KeyDataType<T1, CHAR_LENGTH, ORDER> keyData);

    CLeafNode<T1, T2, CHAR_LENGTH, ORDER> getRootLeaf() {
        CLeafNode<T1, T2, CHAR_LENGTH, ORDER> root;
        CLeafNode_store.read(root, rootPossession);
        return root;
    }

    CInternalNode<T1, T2, CHAR_LENGTH, ORDER> getRootInternal() {
        CInternalNode<T1, T2, CHAR_LENGTH, ORDER> root;
        CInternalNode_store.read(root, rootPossession);
        return root;
    }
};


template<class T1, class T2, int CHAR_LENGTH, int ORDER>
int CInternalNode<T1, T2, CHAR_LENGTH, ORDER>::getKeyIndex(string key) {
    int left = 0;
    int right = getKeyNum() - 1;
    int mid;
    while (left <= right) {
        mid = (left + right) >> 1;
        if (compareKeyvalue(mid, key) > 0)
            left = mid + 1;
        else right = mid - 1;
    }
    return left;
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
int CInternalNode<T1, T2, CHAR_LENGTH, ORDER>::getKeyDataIndex(KeyDataType<T1, CHAR_LENGTH, ORDER> keyData) {
    int left = 0;
    int right = getKeyNum() - 1;
    int mid;
    while (left <= right) {
        mid = (left + right) >> 1;
        if (keyData > getKeyDataValue(mid))
            left = mid + 1;
        else right = mid - 1;
    }
    return left;
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
int CLeafNode<T1, T2, CHAR_LENGTH, ORDER>::getKeyDataIndex(KeyDataType<T1, CHAR_LENGTH, ORDER> keyData) {
    int left = 0;
    int right = getKeyNum() - 1;
    int mid;
    while (left <= right) {
        mid = (left + right) >> 1;
        if (keyData > getKeyDataValue(mid))
            left = mid + 1;
        else right = mid - 1;
    }
    return left;
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
int CLeafNode<T1, T2, CHAR_LENGTH, ORDER>::getKeyIndex(string key) {
    int left = 0;
    int right = getKeyNum() - 1;
    int mid;
    while (left <= right) {
        mid = (left + right) >> 1;
        if (compareKeyvalue(mid, key) > 0)
            left = mid + 1;
        else right = mid - 1;
    }
    return left;
}

// CInternalNode
template<class T1, class T2, int CHAR_LENGTH, int ORDER>
CInternalNode<T1, T2, CHAR_LENGTH, ORDER>::CInternalNode() {
    setType(INTERNAL);
    setKeyNum(0);
    for (int i = 0; i < MAXNUM_CHILD; ++i)
        m_Childs[i] = 0;
    for (int i = 0; i < MAXNUM_KEY; ++i)
        fill(m_KeyData[i].KeyValue, m_KeyData[i].KeyValue + CHAR_LENGTH, '\0');
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
CInternalNode<T1, T2, CHAR_LENGTH, ORDER>::~CInternalNode() {}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
CInternalNode<T1, T2, CHAR_LENGTH, ORDER>
CInternalNode<T1, T2, CHAR_LENGTH, ORDER>::getChildInternal(int i, CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree) {
    CInternalNode<T1, T2, CHAR_LENGTH, ORDER> tmp;
    tree->InternalRead(tmp, m_Childs[i]);
    return tmp;
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
CLeafNode<T1, T2, CHAR_LENGTH, ORDER>
CInternalNode<T1, T2, CHAR_LENGTH, ORDER>::getChildLeaf(int i, CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree) {
    CLeafNode<T1, T2, CHAR_LENGTH, ORDER> tmp;
    tree->LeafRead(tmp, m_Childs[i]);
    return tmp;
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CInternalNode<T1, T2, CHAR_LENGTH, ORDER>::clear(CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree) {
    for (int i = 0; i <= m_KeyNum; ++i) {
        if (m_Childs[i] == 0) continue;
        if (childs_type == INTERNAL) {
            CInternalNode<T1, T2, CHAR_LENGTH, ORDER> tmp = getChildInternal(i, tree);
            tmp.clear(tree);
            // tree->InternalDelete(m_Childs[i]);
        } else {
            CLeafNode<T1, T2, CHAR_LENGTH, ORDER> tmp;
            tree->LeafRead(tmp, m_Childs[i]);
            tmp.clear(tree);
        }
        m_Childs[i] = 0;
    }
    tree->InternalDelete(storePossession);
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void
CInternalNode<T1, T2, CHAR_LENGTH, ORDER>::split(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> &parentNode, int childIndex,
                                                 CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree) {
    CInternalNode<T1, T2, CHAR_LENGTH, ORDER> nNode;//分裂后的右节点
    nNode.storePossession = tree->InternalWrite(nNode);
    nNode.setKeyNum(MINNUM_KEY + 1);
    int i;
    for (i = 0; i < MINNUM_KEY + 1; ++i) // 拷贝关键字的值
        nNode.setKeyData(i, m_KeyData[i + MINNUM_CHILD]);
    for (i = 0; i < MINNUM_CHILD + 1; ++i) // 拷贝孩子节点指针
        nNode.setChild(i, m_Childs[i + MINNUM_CHILD], childs_type);
    setKeyNum(MINNUM_KEY);  //更新左子树的关键字个数
    tree->InteranlUpdate(nNode, nNode.storePossession);
    parentNode.insert(childIndex, childIndex + 1, m_KeyData[MINNUM_KEY],
                      nNode.storePossession, INTERNAL);
    tree->InteranlUpdate(*this, storePossession);
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void
CInternalNode<T1, T2, CHAR_LENGTH, ORDER>::insert(int keyIndex, int childIndex,
                                                  KeyDataType<T1, CHAR_LENGTH, ORDER> keyData,
                                                  int childNode, NODE_TYPE type) {
    int i;
    for (i = getKeyNum(); i > keyIndex; --i)//将父节点中的childIndex后的所有关键字的值和子树指针向后移一位
        setKeyData(i, m_KeyData[i - 1]);
    for (i = getKeyNum() + 1; i > childIndex; --i)
        setChild(i, m_Childs[i - 1], childs_type);
    setChild(childIndex, childNode, type);
    setKeyData(keyIndex, keyData);
    setKeyNum(m_KeyNum + 1);
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CInternalNode<T1, T2, CHAR_LENGTH, ORDER>::mergeChild(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> parentNode,
                                                           CInternalNode<T1, T2, CHAR_LENGTH, ORDER> childNode,
                                                           int keyIndex,
                                                           CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree) {
    // 合并数据
    setKeyData(MINNUM_KEY, parentNode.getKeyDataValue(keyIndex));
    setChild(MINNUM_KEY + 1, childNode.getChild(0), getChildtype());
    int i;
    for (i = 1; i <= childNode.getKeyNum(); ++i) {
        setKeyData(MINNUM_KEY + i, childNode.getKeyDataValue(i - 1));
        setChild(MINNUM_KEY + i + 1, childNode.getChild(i), getChildtype());
    }
    setKeyNum(getKeyNum() + childNode.getKeyNum() + 1);
    //父节点删除index的key
    tree->InternalDelete(childNode.getPossession());
    parentNode.removeKey(keyIndex, keyIndex + 1);
    tree->InteranlUpdate(parentNode, parentNode.getPossession());
    tree->InteranlUpdate(*this, storePossession);
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CInternalNode<T1, T2, CHAR_LENGTH, ORDER>::removeKey(int keyIndex, int childIndex) {
    for (int i = 0; i < getKeyNum() - keyIndex - 1; ++i)
        setKeyData(keyIndex + i, m_KeyData[keyIndex + i + 1]);
    for (int i = 0; i < getKeyNum() - childIndex; ++i)
        setChild(childIndex + i, m_Childs[childIndex + i + 1], childs_type);
    setKeyNum(getKeyNum() - 1);
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CInternalNode<T1, T2, CHAR_LENGTH, ORDER>::borrowFrom(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> siblingNode,
                                                           CInternalNode<T1, T2, CHAR_LENGTH, ORDER> parentNode,
                                                           int keyIndex,
                                                           SIBLING_DIRECTION d,
                                                           CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree) {
    switch (d) {
        case LEFT:  // 从左兄弟结点借
        {
            insert(0, 0, parentNode.getKeyDataValue(keyIndex), siblingNode.getChild(siblingNode.getKeyNum()),
                   getChildtype());
            parentNode.setKeyData(keyIndex, siblingNode.getKeyDataValue(siblingNode.getKeyNum() - 1));
            siblingNode.removeKey(siblingNode.getKeyNum() - 1, siblingNode.getKeyNum());
        }
            break;
        case RIGHT:  // 从右兄弟结点借
        {
            insert(getKeyNum(), getKeyNum() + 1, parentNode.getKeyDataValue(keyIndex), siblingNode.getChild(0),
                   getChildtype());
            parentNode.setKeyData(keyIndex, siblingNode.getKeyDataValue(0));
            siblingNode.removeKey(0, 0);
        }
            break;
    }
    tree->InteranlUpdate(siblingNode, siblingNode.getPossession());
    tree->InteranlUpdate(parentNode, parentNode.getPossession());
    tree->InteranlUpdate(*this, storePossession);
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
int CInternalNode<T1, T2, CHAR_LENGTH, ORDER>::getChildIndex(string key, int keyIndex) {
    if (compareKeyvalue(keyIndex, key) == 0)
        return keyIndex + 1;
    else return keyIndex;
}

// CLeafNode
template<class T1, class T2, int CHAR_LENGTH, int ORDER>
CLeafNode<T1, T2, CHAR_LENGTH, ORDER>::CLeafNode() {
    setType(LEAF);
    m_LeftSibling = m_RightSibling = 0;
    setKeyNum(0);
    for (int i = 0; i < MAXNUM_KEY; ++i)
        fill(m_KeyData[i].KeyValue, m_KeyData[i].KeyValue + CHAR_LENGTH, '\0');
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
CLeafNode<T1, T2, CHAR_LENGTH, ORDER>::~CLeafNode() = default;;

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CLeafNode<T1, T2, CHAR_LENGTH, ORDER>::clear(CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree) {
    tree->LeafDelete(storePossession);
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
CLeafNode<T1, T2, CHAR_LENGTH, ORDER>
CLeafNode<T1, T2, CHAR_LENGTH, ORDER>::getRightSibling(CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree) const {
    CLeafNode<T1, T2, CHAR_LENGTH, ORDER> tmp;
    tree->LeafRead(tmp, m_RightSibling);
    return tmp;
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
int CLeafNode<T1, T2, CHAR_LENGTH, ORDER>::getRightPossession() {
    return m_RightSibling;
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CLeafNode<T1, T2, CHAR_LENGTH, ORDER>::insert(KeyDataType<T1, CHAR_LENGTH, ORDER> keyData, const T2 &data2) {
    int i;
    for (i = m_KeyNum; i > 0 && m_KeyData[i - 1] > keyData; --i) {
        setKeyData(i, m_KeyData[i - 1]);
        setData(i, m_Datas[i - 1]);
    }
    setKeyData(i, keyData);
    setData(i, data2);
    setKeyNum(m_KeyNum + 1);
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CLeafNode<T1, T2, CHAR_LENGTH, ORDER>::split(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> &parentNode, int childIndex,
                                                  CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree) {
    CLeafNode<T1, T2, CHAR_LENGTH, ORDER> nNode;//分裂后的右节点
    nNode.setPossession(tree->LeafWrite(nNode));
    setKeyNum(MINNUM_LEAF + 1);
    nNode.setKeyNum(MINNUM_LEAF + 1);
    nNode.setRightSibling(m_RightSibling);
    nNode.setLeftSibling(this->getPossession());
    int i;
    for (i = 0; i < MINNUM_LEAF + 1; ++i) {// 拷贝数据
        nNode.setKeyData(i, m_KeyData[i + MINNUM_LEAF + 1]);
        nNode.setData(i, m_Datas[i + MINNUM_LEAF + 1]);
    }
    tree->LeafUpdate(nNode, nNode.getPossession());
    setRightSibling(nNode.getPossession());
    tree->LeafUpdate(*this, this->getPossession());
    parentNode.insert(childIndex, childIndex + 1, m_KeyData[MINNUM_LEAF + 1], nNode.storePossession, LEAF);
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CLeafNode<T1, T2, CHAR_LENGTH, ORDER>::mergeChild(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> parentNode,
                                                       CLeafNode<T1, T2, CHAR_LENGTH, ORDER> childNode, int keyIndex,
                                                       CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree) {
    // 合并数据
    for (int i = 0; i < childNode.getKeyNum(); ++i) {
        setKeyData(getKeyNum() + i, childNode.getKeyDataValue(i));
        setData(getKeyNum() + i, childNode.getDataValue(i));
    }
    setKeyNum(m_KeyNum + childNode.getKeyNum());
    setRightSibling(childNode.m_RightSibling);
    //父节点删除index的key，
    parentNode.removeKey(keyIndex, keyIndex + 1);
    tree->LeafUpdate(*this, getPossession());
    tree->InteranlUpdate(parentNode, parentNode.getPossession());
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CLeafNode<T1, T2, CHAR_LENGTH, ORDER>::removeKey(int keyIndex, CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree) {
    for (int i = keyIndex; i < getKeyNum() - 1; ++i) {
        setKeyData(i, m_KeyData[i + 1]);
        setData(i, m_Datas[i + 1]);
    }
    setKeyNum(getKeyNum() - 1);
    tree->LeafUpdate(*this, getPossession());
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CLeafNode<T1, T2, CHAR_LENGTH, ORDER>::remove(KeyDataType<T1, CHAR_LENGTH, ORDER> keyData,
                                                   CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree) {
    int keyDataIndex = getKeyDataIndex(keyData);
    if (keyDataIndex >= getKeyNum()) return;
    if (getKeyDataValue(keyDataIndex) != keyData) return;
    for (int i = keyDataIndex; i < getKeyNum() - 1; ++i) {
        setKeyData(i, getKeyDataValue(i + 1));
        setData(i, getDataValue(i + 1));
    }
    setKeyNum(getKeyNum() - 1);
    CLeafNode<T1, T2, CHAR_LENGTH, ORDER> *tmp = this;
    tree->LeafUpdate(*tmp, getPossession());
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CLeafNode<T1, T2, CHAR_LENGTH, ORDER>::borrowFrom(CLeafNode<T1, T2, CHAR_LENGTH, ORDER> siblingNode,
                                                       CInternalNode<T1, T2, CHAR_LENGTH, ORDER> parentNode,
                                                       int keyIndex,
                                                       SIBLING_DIRECTION d,
                                                       CBPlusTree<T1, T2, CHAR_LENGTH, ORDER> *tree) {
    switch (d) {
        case LEFT:  // 从左兄弟结点借
        {
            insert(siblingNode.getKeyDataValue(siblingNode.getKeyNum() - 1),
                   siblingNode.getDataValue(siblingNode.getKeyNum() - 1));
            siblingNode.removeKey(siblingNode.getKeyNum() - 1, tree);
            parentNode.setKeyData(keyIndex, getKeyDataValue(0));
        }
            break;
        case RIGHT:  // 从右兄弟结点借
        {
            insert(siblingNode.getKeyDataValue(0), siblingNode.getDataValue(0));
            siblingNode.removeKey(0, tree);
            parentNode.setKeyData(keyIndex, siblingNode.getKeyDataValue(0));
        }
            break;
    }
    CLeafNode<T1, T2, CHAR_LENGTH, ORDER> *tmp1 = this;
    tree->LeafUpdate(*tmp1, getPossession());
    tree->InteranlUpdate(parentNode, parentNode.getPossession());
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::CBPlusTree(const string &FN) : CLeafNode_store(FN + "LeafStore", 1, 0),
                                                                       CInternalNode_store(FN + "InternalStore", 1, 0) {
    int tmpLeaf, tmpInternal;
    CLeafNode_store.read_info(tmpLeaf, 2);
    CInternalNode_store.read_info(tmpInternal, 2);
    CLeafNode_store.read_info(m_DataHead, 3);
    if (tmpLeaf > 0) {
        rootType = LEAF;
        rootPossession = tmpLeaf;
    } else {
        rootType = INTERNAL;
        rootPossession = tmpInternal;
    }
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::~CBPlusTree() {
    CLeafNode_store.write_info(m_DataHead, 3);
    if (rootType == LEAF) {
        CLeafNode_store.write_info(rootPossession, 2);
        CInternalNode_store.write_info(0, 2);
    } else {
        CInternalNode_store.write_info(rootPossession, 2);
        CLeafNode_store.write_info(0, 2);
    }
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::insert(string key, const T1 &data1, const T2 &data2) {
    // cout<<"!!!"<<key<<endl;
    KeyDataType<T1, CHAR_LENGTH, ORDER> keyData(key, data1);
    if (rootPossession == 0) {
        CLeafNode<T1, T2, CHAR_LENGTH, ORDER> newRoot;
        rootType = LEAF;
        newRoot.setPossession(CLeafNode_store.write(newRoot));
        m_DataHead = rootPossession = newRoot.getPossession();
        CLeafNode_store.update(newRoot, newRoot.getPossession());
    }
    if (rootType == LEAF) {
        CLeafNode<T1, T2, CHAR_LENGTH, ORDER> root = getRootLeaf();
        root.insert(keyData, data2);
        if (root.getKeyNum() == MAXNUM_KEY) { // 根结点已满，分裂
            CInternalNode<T1, T2, CHAR_LENGTH, ORDER> newNode;  //创建新的根节点
            newNode.setPossession(CInternalNode_store.write(newNode));
            newNode.setChild(0, root.getPossession(), root.getType());
            root.split(newNode, 0, this);    // 叶子结点分裂
            rootPossession = newNode.getPossession();
            rootType = newNode.getType();//更新根节点指针
            CInternalNode_store.update(newNode, newNode.getPossession());
        } else CLeafNode_store.update(root, root.getPossession());
    } else {
        CInternalNode<T1, T2, CHAR_LENGTH, ORDER> root = getRootInternal();
        if (recursive_insertInternal(root, keyData, data2)) {
            CInternalNode<T1, T2, CHAR_LENGTH, ORDER> newNode;  //创建新的根节点
            newNode.setPossession(CInternalNode_store.write(newNode));
            newNode.setChild(0, root.getPossession(), root.getType());
            root.split(newNode, 0, this);    // 叶子结点分裂
            rootPossession = newNode.getPossession();
            rootType = newNode.getType();//更新根节点指针
            CInternalNode_store.update(newNode, newNode.getPossession());
        }
    }
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::recursive_insertLeaf(CLeafNode<T1, T2, CHAR_LENGTH, ORDER> parentNode,
                                                                  KeyDataType<T1, CHAR_LENGTH, ORDER> keyData,
                                                                  const T2 &data2) {
    parentNode.insert(keyData, data2);
    CLeafNode_store.update(parentNode, parentNode.getPossession());
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
bool
CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::recursive_insertInternal(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> &parentNode,
                                                                 KeyDataType<T1, CHAR_LENGTH, ORDER> keyData,
                                                                 const T2 &data2) {
    int keyIndex = parentNode.getKeyDataIndex(keyData);
    int childIndex = parentNode.getChildIndex(keyData, keyIndex);
    if (parentNode.getChildtype() == INTERNAL) {
        CInternalNode<T1, T2, CHAR_LENGTH, ORDER> childNode = parentNode.getChildInternal(childIndex, this);
        if (recursive_insertInternal(childNode, keyData, data2)) {
            childNode.split(parentNode, childIndex, this);
            if (parentNode.getKeyNum() == MAXNUM_LEAF) return true;
            else CInternalNode_store.update(parentNode, parentNode.getPossession());
        }
        return false;
    } else {
        CLeafNode<T1, T2, CHAR_LENGTH, ORDER> childNode = parentNode.getChildLeaf(childIndex, this);
        childNode.insert(keyData, data2);
        if (childNode.getKeyNum() == MAXNUM_LEAF) {  // 子结点已满，需进行分裂
            childNode.split(parentNode, childIndex, this);
            if (parentNode.getKeyNum() == MAXNUM_LEAF) return true;
            else CInternalNode_store.update(parentNode, parentNode.getPossession());
        } else CLeafNode_store.update(childNode, childNode.getPossession());
        return false;
    }
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::clean() {
    if (rootPossession != 0) {
        if (rootType == INTERNAL) {
            CInternalNode<T1, T2, CHAR_LENGTH, ORDER> root = getRootInternal();
            root.clear(this);
        } else {
            CLeafNode<T1, T2, CHAR_LENGTH, ORDER> root = getRootLeaf();
            root.clear(this);
        }
        rootPossession = m_DataHead = 0;
    }
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
bool CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::Find(string key) {
    if (rootPossession == 0) return false;
    if (rootType == INTERNAL) return recursive_searchInternal(getRootInternal(), key);
    else return recursive_searchLeaf(getRootLeaf(), key);
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
bool
CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::recursive_searchInternal(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> pNode,
                                                                 string key) {
    int keyIndex = pNode.getKeyIndex(key);
    int childIndex = pNode.getChildIndex(key, keyIndex); // 孩子结点指针索引
    if (keyIndex < pNode.getKeyNum() && pNode.compareKeyvalue(keyIndex, key) == 0) {
        return true;
    } else {
        if (pNode.getChildtype() == INTERNAL)
            return recursive_searchInternal(pNode.getChildInternal(childIndex, this), key);
        else return recursive_searchLeaf(pNode.getChildLeaf(childIndex, this), key);
    }
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
bool
CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::recursive_searchLeaf(CLeafNode<T1, T2, CHAR_LENGTH, ORDER> pNode, string key) {
    int keyIndex = pNode.getKeyIndex(key);
    if (keyIndex < pNode.getKeyNum() && pNode.compareKeyvalue(keyIndex, key) == 0)
        return true;
    else return false;
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::Delete(string key, const T1 &dataValue) {
    // cout<<"@@@"<<key<<endl;
    if (rootPossession == 0) return;
    KeyDataType<T1, CHAR_LENGTH, ORDER> keyData(key, dataValue);
    if (rootType == LEAF) {
        CLeafNode<T1, T2, CHAR_LENGTH, ORDER> root = getRootLeaf();
        if (root.getKeyNum() == 1) {
            if (root.getKeyDataValue(0) == keyData)
                clean();
            return;
        } else recursive_removeLeaf(root, keyData);
    } else {
        CInternalNode<T1, T2, CHAR_LENGTH, ORDER> root = getRootInternal();
        if (root.getKeyNum() == 1) {
            if (root.getChildtype() == INTERNAL) {
                CInternalNode<T1, T2, CHAR_LENGTH, ORDER> pChild1, pChild2;
                pChild1 = root.getChildInternal(0, this);
                pChild2 = root.getChildInternal(1, this);
                if (pChild1.getKeyNum() == MINNUM_KEY && pChild2.getKeyNum() == MINNUM_KEY) {
                    pChild1.mergeChild(root, pChild2, 0, this);
                    CInternalNode_store.Delete(rootPossession);
                    rootPossession = pChild1.getPossession();
                    recursive_removeInternal(pChild1, keyData);
                } else recursive_removeInternal(root, keyData);
            } else {
                CLeafNode<T1, T2, CHAR_LENGTH, ORDER> pChild1, pChild2;
                pChild1 = root.getChildLeaf(0, this);
                pChild2 = root.getChildLeaf(1, this);
                if (pChild1.getKeyNum() == MINNUM_KEY && pChild2.getKeyNum() == MINNUM_KEY) {
                    pChild1.mergeChild(root, pChild2, 0, this);
                    CInternalNode_store.Delete(rootPossession);
                    rootPossession = pChild1.getPossession();
                    rootType = LEAF;
                    recursive_removeLeaf(pChild1, keyData);
                } else recursive_removeInternal(root, keyData);
            }
        } else recursive_removeInternal(root, keyData);
    }
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void
CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::recursive_removeInternal(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> parentNode,
                                                                 KeyDataType<T1, CHAR_LENGTH, ORDER> keyData) {
    int keyIndex = parentNode.getKeyDataIndex(keyData);
    int childIndex = parentNode.getChildIndex(keyData, keyIndex); // 孩子结点指针索引
    if (parentNode.getChildtype() == LEAF) {
        CLeafNode<T1, T2, CHAR_LENGTH, ORDER> pChildNode = parentNode.getChildLeaf(childIndex, this);
        if (pChildNode.getKeyNum() == MINNUM_KEY) {
            CLeafNode<T1, T2, CHAR_LENGTH, ORDER> pLeft, pRight;
            if (childIndex > 0) pLeft = parentNode.getChildLeaf(childIndex - 1, this);
            if (childIndex < parentNode.getKeyNum()) pRight = parentNode.getChildLeaf(childIndex + 1, this);
            // 先考虑从兄弟结点中借
            if (childIndex > 0 && pLeft.getKeyNum() > MINNUM_KEY) {  // 左兄弟结点可借
                pChildNode.borrowFrom(pLeft, parentNode, childIndex - 1, LEFT, this);
            } else if (childIndex < parentNode.getKeyNum() && pRight.getKeyNum() > MINNUM_KEY) {  //右兄弟结点可借
                pChildNode.borrowFrom(pRight, parentNode, childIndex, RIGHT, this);
            }        //左右兄弟节点都不可借，考虑合并
            else if (childIndex > 0) {                 //与左兄弟合并
                pLeft.mergeChild(parentNode, pChildNode, childIndex - 1, this);
                pChildNode = pLeft;
            } else if (childIndex < parentNode.getKeyNum())                   //与右兄弟合并
                pChildNode.mergeChild(parentNode, pRight, childIndex, this);
        }
        recursive_removeLeaf(pChildNode, keyData);
    } else {
        CInternalNode<T1, T2, CHAR_LENGTH, ORDER> pChildNode = parentNode.getChildInternal(childIndex, this);
        if (pChildNode.getKeyNum() == MINNUM_KEY) {
            CInternalNode<T1, T2, CHAR_LENGTH, ORDER> pLeft, pRight;
            if (childIndex > 0) pLeft = parentNode.getChildInternal(childIndex - 1, this);
            if (childIndex < parentNode.getKeyNum()) pRight = parentNode.getChildInternal(childIndex + 1, this);
            // 先考虑从兄弟结点中借
            if (childIndex > 0 && pLeft.getKeyNum() > MINNUM_KEY) {  // 左兄弟结点可借
                pChildNode.borrowFrom(pLeft, parentNode, childIndex - 1, LEFT, this);
            } else if (childIndex < parentNode.getKeyNum() && pRight.getKeyNum() > MINNUM_KEY) {  //右兄弟结点可借
                pChildNode.borrowFrom(pRight, parentNode, childIndex, RIGHT, this);
            }        //左右兄弟节点都不可借，考虑合并
            else if (childIndex > 0) {                 //与左兄弟合并
                pLeft.mergeChild(parentNode, pChildNode, childIndex - 1, this);
                pChildNode = pLeft;
            } else if (childIndex < parentNode.getKeyNum())                   //与右兄弟合并
                pChildNode.mergeChild(parentNode, pRight, childIndex, this);
        }
        recursive_removeInternal(pChildNode, keyData);
    }
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::recursive_removeLeaf(CLeafNode<T1, T2, CHAR_LENGTH, ORDER> parentNode,
                                                                  KeyDataType<T1, CHAR_LENGTH, ORDER> keyData) {
    int keyIndex = parentNode.getKeyDataIndex(keyData);
    parentNode.remove(keyData, this);  // 直接删除
    // 如果键值在内部结点中存在，也要相应的替换内部结点
    if (keyIndex == 0 && rootType != LEAF && parentNode.getPossession() != m_DataHead) {
        CInternalNode<T1, T2, CHAR_LENGTH, ORDER> root = getRootInternal();
        changeKey(root, keyData, parentNode.getKeyDataValue(0));
    }
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
void CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::changeKey(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> pNode,
                                                       KeyDataType<T1, CHAR_LENGTH, ORDER> oldKeyData,
                                                       KeyDataType<T1, CHAR_LENGTH, ORDER> newKeyData) {
    int keyIndex = pNode.getKeyDataIndex(oldKeyData);
    if (keyIndex < pNode.getKeyNum() && pNode.getKeyDataValue(keyIndex) == oldKeyData) {  // 找到
        pNode.setKeyData(keyIndex, newKeyData);
        CInternalNode_store.update(pNode, pNode.getPossession());
    } else {  // 继续找
        if (pNode.getChildtype() == INTERNAL) {
            CInternalNode<T1, T2, CHAR_LENGTH, ORDER> pChildnode = pNode.getChildInternal(keyIndex, this);
            changeKey(pChildnode, oldKeyData, newKeyData);
        }
    }
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
Vector<T2> CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::FindAll(string compareKey) {
    Vector<T2> ans;
    if (m_DataHead == 0)
        return ans;
    int start = searchFind(compareKey);
    CLeafNode<T1, T2, CHAR_LENGTH, ORDER> it;
    CLeafNode_store.read(it, start);
    while (1) {
        int keyIndex = it.getKeyIndex(compareKey);
        if (keyIndex != it.getKeyNum()) break;
        if (it.getRightPossession() != 0)
            it = it.getRightSibling(this);
        else return ans;
    }
    int keyIndex = it.getKeyIndex(compareKey);
    int i;
    if (it.compareKeyvalue(it.getKeyNum() - 1, compareKey) == 0) {
        for (i = keyIndex; i < it.getKeyNum(); ++i)
            ans.push_back(it.getDataValue(i));
    } else {
        for (i = keyIndex; i < it.getKeyNum(); ++i) {
            if (it.compareKeyvalue(i, compareKey) == 0)
                ans.push_back(it.getDataValue(i));
            else break;
        }
        return ans;
    }
    if (it.getRightPossession() != 0)
        it = it.getRightSibling(this);
    else return ans;
    for (;; it = it.getRightSibling(this)) {
        if (it.compareKeyvalue(it.getKeyNum() - 1, compareKey) == 0) {
            for (i = 0; i < it.getKeyNum(); ++i)
                ans.push_back(it.getDataValue(i));
        } else {
            for (i = 0; i < it.getKeyNum(); ++i) {
                if (it.compareKeyvalue(i, compareKey) == 0)
                    ans.push_back(it.getDataValue(i));
                else break;
            }
            break;
        }
        if (it.getRightPossession() == 0) break;
    }
    return ans;
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
int CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::searchFind(string key) {
    if (rootType == INTERNAL) return recursive_searchFindInternal(getRootInternal(), key);
    else return rootPossession;
}

template<class T1, class T2, int CHAR_LENGTH, int ORDER>
int
CBPlusTree<T1, T2, CHAR_LENGTH, ORDER>::recursive_searchFindInternal(CInternalNode<T1, T2, CHAR_LENGTH, ORDER> pNode,
                                                                     string key) {
    int keyIndex = pNode.getKeyIndex(key);
    if (keyIndex < pNode.getKeyNum() && pNode.compareKeyvalue(keyIndex, key) > 0) keyIndex++;
    if (pNode.getChildtype() == INTERNAL)
        return recursive_searchFindInternal(pNode.getChildInternal(keyIndex, this), key);
    else return pNode.getChild(keyIndex);
}

#endif //MAIN_CPP_CBPLUSTREE_HPP