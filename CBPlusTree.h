#ifndef MAIN_CPP_CBPLUSTREE_H
#define MAIN_CPP_CBPLUSTREE_H

#include <cstring>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include "MemoryRiver.hpp"

using namespace std;
enum NODE_TYPE {
    INTERNAL, LEAF
};        // 结点类型：内结点、叶子结点
enum SIBLING_DIRECTION {
    LEFT, RIGHT
};   // 兄弟结点方向：左兄弟结点、右兄弟结点
const int ORDER = 5;                   // B+树的阶（非根内结点的最小子树个数）
const int MINNUM_KEY = ORDER - 1;        // 最小键值个数
const int MAXNUM_KEY = 2 * ORDER - 1;      // 最大键值个数
const int MINNUM_CHILD = MINNUM_KEY + 1; // 最小子树个数
const int MAXNUM_CHILD = MAXNUM_KEY + 1; // 最大子树个数
const int MINNUM_LEAF = MINNUM_KEY;    // 最小叶子结点键值个数
const int MAXNUM_LEAF = MAXNUM_KEY;    // 最大叶子结点键值个数
const int CHAR_LENGTH = 66;//key长度

template<class T>
class CBPlusTree;

template<class T>
class CInternalNode;

template<class T>
class CLeafNode;

template<class T>
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
        if (*this == other) return *this;
        for (int iter = 0; iter < CHAR_LENGTH; ++iter) {
            KeyValue[iter] = other.KeyValue[iter];
            DataValue = other.DataValue;
        }
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
template<class T>
class CInternalNode {
private:
    NODE_TYPE m_Type;
    int m_KeyNum;
    KeyDataType<T> m_KeyData[MAXNUM_KEY];
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

    KeyDataType<T> getKeyDataValue(int i) { return m_KeyData[i]; }

    int compareKeyvalue(int i, string key) { return strcmp(key.c_str(), m_KeyData[i].KeyValue); }

    void setKeyData(int i, KeyDataType<T> keyData) {
        m_KeyData[i] = keyData;
    }

    int getKeyIndex(string key);  // 找到键值在结点中存储的下标(第一个不小于key的下标)

    int getKeyIndexLow(string key); //第一个大于key的下标

    int getKeyDataIndex(KeyDataType<T> keyData);

    int getChildIndex(KeyDataType<T> keyData, int keyIndex) {
        if (keyIndex >= getKeyNum()) return getKeyNum();
        if (keyData < m_KeyData[keyIndex]) return keyIndex;
        else return keyIndex + 1;
    }

    int getChild(int i) { return m_Childs[i]; }

    CInternalNode getChildInternal(int i, CBPlusTree<T> *tree);

    CLeafNode<T> getChildLeaf(int i, CBPlusTree<T> *tree);

    void split(CInternalNode parentNode, int childIndex, CBPlusTree<T> *tree);

    void setChild(int i, int child, NODE_TYPE type) {
        m_Childs[i] = child;
        childs_type = type;
    }

    void insert(int keyIndex, int childIndex, KeyDataType<T> keyData, int childNode, NODE_TYPE type);
    //在keyIndex/childIndex位置插入
    // 不修改文件

    NODE_TYPE getChildtype() { return childs_type; }

    void mergeChild(CInternalNode parentNode, CInternalNode childNode, int keyIndex,
                    CBPlusTree<T> *tree);//将childnode（右）合并到this（左）里

    void removeKey(int keyIndex, int childIndex);//不修改文件

    void clear(CBPlusTree<T> *tree);

    void borrowFrom(CInternalNode siblingNode, CInternalNode parentNode, int keyIndex, SIBLING_DIRECTION d,
                    CBPlusTree<T> *tree);//尽量从左边借，复杂度小

    int getChildIndex(string key, int keyIndex);

};

// 叶子结点
template<typename  T>
class CLeafNode {
private:
    NODE_TYPE m_Type;
    int m_KeyNum;
    KeyDataType<T> m_KeyData[MAXNUM_LEAF];
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

    void setKeyData(int i, KeyDataType<T> keyData) { m_KeyData[i] = keyData; }

    int getKeyDataIndex(KeyDataType<T> keyData);

    int compareKeyvalue(int i, string key) { return strcmp(key.c_str(), m_KeyData[i].KeyValue); }

    int getKeyIndex(string key);

    KeyDataType<T> getKeyDataValue(int i) { return m_KeyData[i]; }

    void setLeftSibling(int possession) { m_LeftSibling = possession; }

    CLeafNode getRightSibling(CBPlusTree<T> *tree) const;

    int getRightPossession();

    void setRightSibling(int possession) { m_RightSibling = possession; }

    T getData(int i) const { return m_KeyData[i].DataValue; }

    void remove(KeyDataType<T> keyData, CBPlusTree<T> *tree);

    void insert(KeyDataType<T> keyData);

    void split(CInternalNode<T> parentNode, int childIndex, CBPlusTree<T> *tree);

    void mergeChild(CInternalNode<T> parentNode, CLeafNode childNode, int keyIndex, CBPlusTree<T> *tree);

    void removeKey(int keyIndex, CBPlusTree<T> *tree);

    void clear(CBPlusTree<T> *tree);

    void
    borrowFrom(CLeafNode siblingNode, CInternalNode<T> parentNode, int keyIndex, SIBLING_DIRECTION d, CBPlusTree<T> *tree);
};

template<class T>
class CBPlusTree {
public:
    CBPlusTree(const string &FN);

    ~CBPlusTree();

    void insert(string key, const T &data);

    void remove(string key, T &dataValue);

    vector<T> Findall(string compareKey);

    bool search(string key);

    int searchFind(string key);

    void clear();

    void InternalRead(CInternalNode<T> &node, int index) { CInternalNode_store.read(node, index); };

    void InternalDelete(int index) { CInternalNode_store.Delete(index); }

    void InteranlUpdate(CInternalNode<T> &node, int index) { CInternalNode_store.update(node, index); }

    int InternalWrite(CInternalNode<T> &node) { return CInternalNode_store.write(node); }

    void LeafRead(CLeafNode<T> &node, int index) { CLeafNode_store.read(node, index); };

    void LeafDelete(int index) { CLeafNode_store.Delete(index); }

    void LeafUpdate(CLeafNode<T> &node, const int index) { CLeafNode_store.update(node, index); }

    int LeafWrite(CLeafNode<T> &node) { return CLeafNode_store.write(node); }

private:
    MemoryRiver<CInternalNode<T>, 2> CInternalNode_store;
    MemoryRiver<CLeafNode<T>, 3> CLeafNode_store;
    NODE_TYPE rootType;
    int m_DataHead, rootPossession;
private:

    void recursive_insertLeaf(CLeafNode<T> parentNode, KeyDataType<T> keyData);

    void recursive_insertInternal(CInternalNode<T> parentNode, KeyDataType<T> keyData);

    bool recursive_searchInternal(CInternalNode<T> pNode, string key);

    int recursive_searchFindInternal(CInternalNode<T> pNode, string key);

    bool recursive_searchLeaf(CLeafNode<T> pNode, string key);

    void changeKey(CInternalNode<T> pNode, KeyDataType<T> oldKeyData, KeyDataType<T> newKeyData);

    void recursive_removeInternal(CInternalNode<T> parentNode, KeyDataType<T> keyData);

    void recursive_removeLeaf(CLeafNode<T> parentNode, KeyDataType<T> keyData);

    CLeafNode<T> getRootLeaf() {
        CLeafNode<T> root;
        CLeafNode_store.read(root, rootPossession);
        return root;
    }

    CInternalNode<T> getRootInternal() {
        CInternalNode<T> root;
        CInternalNode_store.read(root, rootPossession);
        return root;
    }
};


#endif //MAIN_CPP_CBPLUSTREE_H
