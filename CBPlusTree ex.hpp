#ifndef MAIN_CPP_CBPLUSTREE_HPP
#define MAIN_CPP_CBPLUSTREE_HPP
#include<vector>
#include "MemoryRiver.hpp"
enum NODE_TYPE {
    INTERNAL, LEAF
};        // 结点类型：内结点、叶子结点
enum SIBLING_DIRECTION {
    LEFT, RIGHT
};   // 兄弟结点方向：左兄弟结点、右兄弟结点
const int ORDER = 60;                   // B+树的阶（非根内结点的最小子树个数）
const int MINNUM_KEY = ORDER - 1;        // 最小键值个数
const int MAXNUM_KEY = 2 * ORDER - 1;      // 最大键值个数
const int MINNUM_CHILD = MINNUM_KEY + 1; // 最小子树个数
const int MAXNUM_CHILD = MAXNUM_KEY + 1; // 最大子树个数
const int MINNUM_LEAF = MINNUM_KEY;    // 最小叶子结点键值个数
const int MAXNUM_LEAF = MAXNUM_KEY;    // 最大叶子结点键值个数
// const int CHAR_LENGTH = 42;//key长度

template<class T, int CHAR_LENGTH>
class CBPlusTree;

template<class T, int CHAR_LENGTH>
class CInternalNode;

template<class T, int CHAR_LENGTH>
class CLeafNode;

template<class T, int CHAR_LENGTH>
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
template<class T, int CHAR_LENGTH>
class CInternalNode {
private:
    NODE_TYPE m_Type;
    int m_KeyNum;
    KeyDataType<T, CHAR_LENGTH> m_KeyData[MAXNUM_KEY];
    int storePossession;
    int m_Childs[MAXNUM_CHILD];
    NODE_TYPE childs_type;
public:
    CInternalNode();

    ~CInternalNode();

    int getPossession() const { return storePossession; }

    void setPossession(const int &p) { storePossession = p; }

    NODE_TYPE getType() const { return m_Type; }

    void setType(const NODE_TYPE &type) { m_Type = type; }

    int getKeyNum() const { return m_KeyNum; }

    void setKeyNum(const int &n) { m_KeyNum = n; }

    KeyDataType<T, CHAR_LENGTH> getKeyDataValue(const int &i) const { return m_KeyData[i]; }

    int compareKeyvalue(const int &i, string key) const { return strcmp(key.c_str(), m_KeyData[i].KeyValue); }

    void setKeyData(const int &i, const KeyDataType<T, CHAR_LENGTH> &keyData) { m_KeyData[i] = keyData; }

    int getKeyIndex(string key) const;  // 找到键值在结点中存储的下标(第一个不小于key的下标)

    int getKeyDataIndex(KeyDataType<T, CHAR_LENGTH> keyData) const ;

    int getChildIndex(KeyDataType<T, CHAR_LENGTH> &keyData, const int &keyIndex) const {
        if (keyIndex >= getKeyNum()) return getKeyNum();
        if (keyData < m_KeyData[keyIndex]) return keyIndex;
        else return keyIndex + 1;
    }

    int getChild(const int &i) const { return m_Childs[i]; }

    void getChildInternal(const int &i, CBPlusTree<T, CHAR_LENGTH> *tree, CInternalNode<T, CHAR_LENGTH> &child) const;

    void getChildLeaf(const int &i, CBPlusTree<T, CHAR_LENGTH> *tree, CLeafNode<T, CHAR_LENGTH> &child) const;

    void split(CInternalNode<T, CHAR_LENGTH> parentNode, const int &childIndex, CBPlusTree<T, CHAR_LENGTH> *tree);

    void setChild(const int &i, const int &child, const NODE_TYPE &type) {
        m_Childs[i] = child;
        childs_type = type;
    }

    void insert(const int &keyIndex, const int &childIndex,const KeyDataType<T, CHAR_LENGTH> &keyData, const int &childNode,
                const NODE_TYPE &type);
    //在keyIndex/childIndex位置插入
    // 不修改文件

    NODE_TYPE getChildtype() const { return childs_type; }

    void
    mergeChild(CInternalNode<T, CHAR_LENGTH> parentNode, CInternalNode<T, CHAR_LENGTH> childNode, const int &keyIndex,
               CBPlusTree<T, CHAR_LENGTH> *tree);//将childnode（右）合并到this（左）里

    void removeKey(const int &keyIndex, const int &childIndex);//不修改文件

    void clear(CBPlusTree<T, CHAR_LENGTH> *tree);

    void
    borrowFrom(CInternalNode<T, CHAR_LENGTH> siblingNode, CInternalNode<T, CHAR_LENGTH> parentNode, const int &keyIndex,
               const SIBLING_DIRECTION &d,
               CBPlusTree<T, CHAR_LENGTH> *tree);//尽量从左边借，复杂度小

    int getChildIndex(string key, const int &keyIndex) const;

};

// 叶子结点
template<class T, int CHAR_LENGTH>
class CLeafNode {
private:
    NODE_TYPE m_Type;
    int m_KeyNum;
    KeyDataType<T, CHAR_LENGTH> m_KeyData[MAXNUM_LEAF];
    int storePossession;
    int m_LeftSibling;
    int m_RightSibling;
public:
    CLeafNode();

    ~CLeafNode();

    int getPossession() const { return storePossession; }

    void setPossession(const int &p) { storePossession = p; }

    NODE_TYPE getType() const { return m_Type; }

    void setType(const NODE_TYPE &type) { m_Type = type; }

    int getKeyNum() const { return m_KeyNum; }

    void setKeyNum(const int &n) { m_KeyNum = n; }

    void setKeyData(const int &i, const KeyDataType<T, CHAR_LENGTH> &keyData) { m_KeyData[i] = keyData; }

    int getKeyDataIndex(KeyDataType<T, CHAR_LENGTH> keyData) const;

    int compareKeyvalue(const int &i, string key) const { return strcmp(key.c_str(), m_KeyData[i].KeyValue); }

    int getKeyIndex(string key) const;

    KeyDataType<T, CHAR_LENGTH> getKeyDataValue(const int &i)const { return m_KeyData[i]; }

    void setLeftSibling(const int &possession) { m_LeftSibling = possession; }

    CLeafNode<T, CHAR_LENGTH> getRightSibling(CBPlusTree<T, CHAR_LENGTH> *tree) const;

    int getRightPossession() const;

    void setRightSibling(const int &possession) { m_RightSibling = possession; }

    T getData(const int &i) const { return m_KeyData[i].DataValue; }

    void remove(KeyDataType<T, CHAR_LENGTH> keyData, CBPlusTree<T, CHAR_LENGTH> *tree);

    void insert(KeyDataType<T, CHAR_LENGTH> keyData);

    void split(CInternalNode<T, CHAR_LENGTH> parentNode, const int &childIndex, CBPlusTree<T, CHAR_LENGTH> *tree);

    void mergeChild(CInternalNode<T, CHAR_LENGTH> parentNode, CLeafNode<T, CHAR_LENGTH> childNode, const int &keyIndex,
                    CBPlusTree<T, CHAR_LENGTH> *tree);

    void removeKey(const int &keyIndex, CBPlusTree<T, CHAR_LENGTH> *tree);

    void clear(CBPlusTree<T, CHAR_LENGTH> *tree);

    void
    borrowFrom(CLeafNode<T, CHAR_LENGTH> siblingNode, CInternalNode<T, CHAR_LENGTH> parentNode, const int &keyIndex,
               const SIBLING_DIRECTION &d,
               CBPlusTree<T, CHAR_LENGTH> *tree);
};

template<class T, int CHAR_LENGTH>
class CBPlusTree {
public:
    CBPlusTree(const string &FN);

    ~CBPlusTree();

    void insert(string key, const T &data);

    void Delete(string key, T &dataValue);

    vector<T> FindAll(string compareKey);

    bool Find(string key);

    void clean();

    void InternalRead(CInternalNode<T, CHAR_LENGTH> &node, const int &index) { CInternalNode_store.read(node, index); };

    void InternalDelete(const int &index) { CInternalNode_store.Delete(index); }

    void InteranlUpdate(CInternalNode<T, CHAR_LENGTH> &node, const int &index) {
        CInternalNode_store.update(node, index);
    }

    int InternalWrite(CInternalNode<T, CHAR_LENGTH> &node) { return CInternalNode_store.write(node); }

    void LeafRead(CLeafNode<T, CHAR_LENGTH> &node, const int &index) { CLeafNode_store.read(node, index); };

    void LeafDelete(const int &index) { CLeafNode_store.Delete(index); }

    void LeafUpdate(CLeafNode<T, CHAR_LENGTH> &node, const int &index) { CLeafNode_store.update(node, index); }

    int LeafWrite(CLeafNode<T, CHAR_LENGTH> &node) { return CLeafNode_store.write(node); }

private:
    MemoryRiver<CInternalNode<T, CHAR_LENGTH>, 2> CInternalNode_store;
    MemoryRiver<CLeafNode<T, CHAR_LENGTH>, 3> CLeafNode_store;
    NODE_TYPE rootType;
    int m_DataHead, rootPossession;
private:

    void recursive_insertLeaf(CLeafNode<T, CHAR_LENGTH> parentNode, KeyDataType<T, CHAR_LENGTH> keyData);

    void recursive_insertInternal(CInternalNode<T, CHAR_LENGTH> parentNode, KeyDataType<T, CHAR_LENGTH> keyData);

    bool recursive_searchInternal(const CInternalNode<T, CHAR_LENGTH> &pNode, string key);

    int recursive_searchFindInternal(const CInternalNode<T, CHAR_LENGTH> &pNode, string key);

    bool recursive_searchLeaf(const CLeafNode<T, CHAR_LENGTH> &pNode, string key);

    int searchFind(string key);

    void changeKey(CInternalNode<T, CHAR_LENGTH> pNode, KeyDataType<T, CHAR_LENGTH> oldKeyData,
                   KeyDataType<T, CHAR_LENGTH> newKeyData);

    void recursive_removeInternal(CInternalNode<T, CHAR_LENGTH> parentNode, KeyDataType<T, CHAR_LENGTH> keyData);

    void recursive_removeLeaf(CLeafNode<T, CHAR_LENGTH> parentNode, KeyDataType<T, CHAR_LENGTH> keyData);

    void getRootLeaf(CLeafNode<T, CHAR_LENGTH> &root) {
        CLeafNode_store.read(root, rootPossession);
    }

    void getRootInternal(CInternalNode<T, CHAR_LENGTH> &root) {
        CInternalNode_store.read(root, rootPossession);
    }
};


template<class T, int CHAR_LENGTH>
int CInternalNode<T, CHAR_LENGTH>::getKeyIndex(string key) const {
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

template<class T, int CHAR_LENGTH>
int CInternalNode<T, CHAR_LENGTH>::getKeyDataIndex(KeyDataType<T, CHAR_LENGTH> keyData) const{
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

template<class T, int CHAR_LENGTH>
int CLeafNode<T, CHAR_LENGTH>::getKeyDataIndex(KeyDataType<T, CHAR_LENGTH> keyData) const{
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

template<class T, int CHAR_LENGTH>
int CLeafNode<T, CHAR_LENGTH>::getKeyIndex(string key) const{
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
template<class T, int CHAR_LENGTH>
CInternalNode<T, CHAR_LENGTH>::CInternalNode() {
    setType(INTERNAL);
    setKeyNum(0);
    for (int i = 0; i < MAXNUM_CHILD; ++i)
        m_Childs[i] = 0;
    for (int i = 0; i < MAXNUM_KEY; ++i)
        fill(m_KeyData[i].KeyValue, m_KeyData[i].KeyValue + CHAR_LENGTH, '\0');
}

template<class T, int CHAR_LENGTH>
CInternalNode<T, CHAR_LENGTH>::~CInternalNode() {}

template<class T, int CHAR_LENGTH>
void CInternalNode<T, CHAR_LENGTH>::getChildInternal(const int &i, CBPlusTree<T, CHAR_LENGTH> *tree,
                                                     CInternalNode<T, CHAR_LENGTH> &tmp) const {
    tree->InternalRead(tmp, m_Childs[i]);
}

template<class T, int CHAR_LENGTH>
void CInternalNode<T, CHAR_LENGTH>::getChildLeaf(const int &i, CBPlusTree<T, CHAR_LENGTH> *tree,
                                                 CLeafNode<T, CHAR_LENGTH> &tmp) const {
    tree->LeafRead(tmp, m_Childs[i]);
}

template<class T, int CHAR_LENGTH>
void CInternalNode<T, CHAR_LENGTH>::clear(CBPlusTree<T, CHAR_LENGTH> *tree) {
    for (int i = 0; i <= m_KeyNum; ++i) {
        if (m_Childs[i] == 0) continue;
        if (childs_type == INTERNAL) {
            CInternalNode<T, CHAR_LENGTH> tmp;
            getChildInternal(i, tree, tmp);
            tmp.clear(tree);
//            tree->InternalDelete(m_Childs[i]);
        } else {
            CLeafNode<T, CHAR_LENGTH> tmp;
            tree->LeafRead(tmp, m_Childs[i]);
            tmp.clear(tree);
        }
        m_Childs[i] = 0;
    }
    tree->InternalDelete(storePossession);
}

template<class T, int CHAR_LENGTH>
void CInternalNode<T, CHAR_LENGTH>::split(CInternalNode<T, CHAR_LENGTH> parentNode, const int &childIndex,
                                          CBPlusTree<T, CHAR_LENGTH> *tree) {
    CInternalNode<T, CHAR_LENGTH> nNode;//分裂后的右节点
    nNode.storePossession = tree->InternalWrite(nNode);
    nNode.setKeyNum(MINNUM_KEY);
    int i;
    for (i = 0; i < MINNUM_KEY; ++i) // 拷贝关键字的值
        nNode.setKeyData(i, m_KeyData[i + MINNUM_CHILD]);
    for (i = 0; i < MINNUM_CHILD; ++i) // 拷贝孩子节点指针
        nNode.setChild(i, m_Childs[i + MINNUM_CHILD], childs_type);
    setKeyNum(MINNUM_KEY);  //更新左子树的关键字个数
    tree->InteranlUpdate(nNode, nNode.storePossession);
    parentNode.insert(childIndex, childIndex + 1, m_KeyData[MINNUM_KEY],
                      nNode.storePossession, INTERNAL);
    tree->InteranlUpdate(parentNode, parentNode.getPossession());
    CInternalNode<T, CHAR_LENGTH> tmp = *this;
    tree->InteranlUpdate(tmp, storePossession);
}

template<class T, int CHAR_LENGTH>
void
CInternalNode<T, CHAR_LENGTH>::insert(const int &keyIndex, const int &childIndex,const KeyDataType<T, CHAR_LENGTH> &keyData,
                                      const int &childNode,
                                      const NODE_TYPE &type) {
    int i;
    for (i = getKeyNum(); i > keyIndex; --i)//将父节点中的childIndex后的所有关键字的值和子树指针向后移一位
        setKeyData(i, m_KeyData[i - 1]);
    for (i = getKeyNum() + 1; i > childIndex; --i)
        setChild(i, m_Childs[i - 1], childs_type);
    setChild(childIndex, childNode, type);
    setKeyData(keyIndex, keyData);
    setKeyNum(m_KeyNum + 1);
}

template<class T, int CHAR_LENGTH>
void CInternalNode<T, CHAR_LENGTH>::mergeChild(CInternalNode<T, CHAR_LENGTH> parentNode,
                                               CInternalNode<T, CHAR_LENGTH> childNode, const int &keyIndex,
                                               CBPlusTree<T, CHAR_LENGTH> *tree) {
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
    CInternalNode<T, CHAR_LENGTH> tmp = *this;
    tree->InteranlUpdate(tmp, storePossession);
}

template<class T, int CHAR_LENGTH>
void CInternalNode<T, CHAR_LENGTH>::removeKey(const int &keyIndex, const int &childIndex) {
    for (int i = 0; i < getKeyNum() - keyIndex - 1; ++i)
        setKeyData(keyIndex + i, m_KeyData[keyIndex + i + 1]);
    for (int i = 0; i < getKeyNum() - childIndex; ++i)
        setChild(childIndex + i, m_Childs[childIndex + i + 1], childs_type);
    setKeyNum(getKeyNum() - 1);
}

template<class T, int CHAR_LENGTH>
void CInternalNode<T, CHAR_LENGTH>::borrowFrom(CInternalNode<T, CHAR_LENGTH> siblingNode,
                                               CInternalNode<T, CHAR_LENGTH> parentNode, const int &keyIndex,
                                               const SIBLING_DIRECTION &d,
                                               CBPlusTree<T, CHAR_LENGTH> *tree) {
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
    CInternalNode<T, CHAR_LENGTH> tmp = *this;
    tree->InteranlUpdate(tmp, storePossession);
}

template<class T, int CHAR_LENGTH>
int CInternalNode<T, CHAR_LENGTH>::getChildIndex(string key, const int &keyIndex) const {
    if (compareKeyvalue(keyIndex, key) == 0)
        return keyIndex + 1;
    else return keyIndex;
}

// CLeafNode
template<class T, int CHAR_LENGTH>
CLeafNode<T, CHAR_LENGTH>::CLeafNode() {
    setType(LEAF);
    m_LeftSibling = m_RightSibling = 0;
    setKeyNum(0);
    for (int i = 0; i < MAXNUM_KEY; ++i)
        fill(m_KeyData[i].KeyValue, m_KeyData[i].KeyValue + CHAR_LENGTH, '\0');
}

template<class T, int CHAR_LENGTH>
CLeafNode<T, CHAR_LENGTH>::~CLeafNode() = default;;

template<class T, int CHAR_LENGTH>
void CLeafNode<T, CHAR_LENGTH>::clear(CBPlusTree<T, CHAR_LENGTH> *tree) {
    tree->LeafDelete(storePossession);
}

template<class T, int CHAR_LENGTH>
CLeafNode<T, CHAR_LENGTH> CLeafNode<T, CHAR_LENGTH>::getRightSibling(CBPlusTree<T, CHAR_LENGTH> *tree) const {
    CLeafNode<T, CHAR_LENGTH> tmp;
    tree->LeafRead(tmp, m_RightSibling);
    return tmp;
}

template<class T, int CHAR_LENGTH>
int CLeafNode<T, CHAR_LENGTH>::getRightPossession() const{
    return m_RightSibling;
}

template<class T, int CHAR_LENGTH>
void CLeafNode<T, CHAR_LENGTH>::insert(KeyDataType<T, CHAR_LENGTH> keyData) {
    int i;
    int index = getKeyDataIndex(keyData);
    for (i = m_KeyNum; i > index; --i)
        setKeyData(i, m_KeyData[i - 1]);
    setKeyData(i, keyData);
    setKeyNum(m_KeyNum + 1);
}

template<class T, int CHAR_LENGTH>
void CLeafNode<T, CHAR_LENGTH>::split(CInternalNode<T, CHAR_LENGTH> parentNode, const int &childIndex,
                                      CBPlusTree<T, CHAR_LENGTH> *tree) {
    CLeafNode<T, CHAR_LENGTH> nNode;//分裂后的右节点
    nNode.setPossession(tree->LeafWrite(nNode));
    setKeyNum(MINNUM_LEAF);
    nNode.setKeyNum(MINNUM_LEAF + 1);
    nNode.setRightSibling(m_RightSibling);
    nNode.setLeftSibling(this->getPossession());
    int i;
    for (i = 0; i < MINNUM_LEAF + 1; ++i)// 拷贝数据
        nNode.setKeyData(i, m_KeyData[i + MINNUM_LEAF]);
    tree->LeafUpdate(nNode, nNode.getPossession());
    setRightSibling(nNode.getPossession());
    CLeafNode<T, CHAR_LENGTH> tmp1 = *this;
    tree->LeafUpdate(tmp1, this->getPossession());
    parentNode.insert(childIndex, childIndex + 1, m_KeyData[MINNUM_LEAF], nNode.storePossession, LEAF);
    tree->InteranlUpdate(parentNode, parentNode.getPossession());
}

template<class T, int CHAR_LENGTH>
void
CLeafNode<T, CHAR_LENGTH>::mergeChild(CInternalNode<T, CHAR_LENGTH> parentNode, CLeafNode<T, CHAR_LENGTH> childNode,
                                      const int &keyIndex,
                                      CBPlusTree<T, CHAR_LENGTH> *tree) {
    // 合并数据
    for (int i = 0; i < childNode.getKeyNum(); ++i)
        setKeyData(getKeyNum() + i, childNode.getKeyDataValue(i));
    setKeyNum(m_KeyNum + childNode.getKeyNum());
    setRightSibling(childNode.m_RightSibling);
    //父节点删除index的key，
    parentNode.removeKey(keyIndex, keyIndex + 1);
    CLeafNode<T, CHAR_LENGTH> tmp = *this;
    tree->LeafUpdate(tmp, getPossession());
    tree->InteranlUpdate(parentNode, parentNode.getPossession());
}

template<class T, int CHAR_LENGTH>
void CLeafNode<T, CHAR_LENGTH>::removeKey(const int &keyIndex, CBPlusTree<T, CHAR_LENGTH> *tree) {
    for (int i = keyIndex; i < getKeyNum() - 1; ++i)
        setKeyData(i, m_KeyData[i + 1]);
    setKeyNum(getKeyNum() - 1);
    CLeafNode<T, CHAR_LENGTH> tmp = *this;
    tree->LeafUpdate(tmp, getPossession());
}

template<class T, int CHAR_LENGTH>
void CLeafNode<T, CHAR_LENGTH>::remove(KeyDataType<T, CHAR_LENGTH> keyData, CBPlusTree<T, CHAR_LENGTH> *tree) {
    int keyDataIndex = getKeyDataIndex(keyData);
    if (keyDataIndex >= getKeyNum()) return;
    if (getKeyDataValue(keyDataIndex) != keyData) return;
    for (int i = keyDataIndex; i < getKeyNum() - 1; ++i)
        setKeyData(i, getKeyDataValue(i + 1));
    setKeyNum(getKeyNum() - 1);
    CLeafNode<T, CHAR_LENGTH> *tmp = this;
    tree->LeafUpdate(*tmp, getPossession());
}

template<class T, int CHAR_LENGTH>
void
CLeafNode<T, CHAR_LENGTH>::borrowFrom(CLeafNode<T, CHAR_LENGTH> siblingNode, CInternalNode<T, CHAR_LENGTH> parentNode,
                                      const int &keyIndex,
                                      const SIBLING_DIRECTION &d,
                                      CBPlusTree<T, CHAR_LENGTH> *tree) {
    switch (d) {
        case LEFT:  // 从左兄弟结点借
        {
            insert(siblingNode.getKeyDataValue(siblingNode.getKeyNum() - 1));
            siblingNode.removeKey(siblingNode.getKeyNum() - 1, tree);
            parentNode.setKeyData(keyIndex, getKeyDataValue(0));
        }
            break;
        case RIGHT:  // 从右兄弟结点借
        {
            insert(siblingNode.getKeyDataValue(0));
            siblingNode.removeKey(0, tree);
            parentNode.setKeyData(keyIndex, siblingNode.getKeyDataValue(0));
        }
            break;
    }
    CLeafNode<T, CHAR_LENGTH> *tmp1 = this;
    tree->LeafUpdate(*tmp1, getPossession());
    tree->InteranlUpdate(parentNode, parentNode.getPossession());
}

template<class T, int CHAR_LENGTH>
CBPlusTree<T, CHAR_LENGTH>::CBPlusTree(const string &FN) : CLeafNode_store(FN + "LeafStore", 1, 0),
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

template<class T, int CHAR_LENGTH>
CBPlusTree<T, CHAR_LENGTH>::~CBPlusTree() {
    CLeafNode_store.write_info(m_DataHead, 3);
    if (rootType == LEAF) {
        CLeafNode_store.write_info(rootPossession, 2);
        CInternalNode_store.write_info(0, 2);
    } else {
        CInternalNode_store.write_info(rootPossession, 2);
        CLeafNode_store.write_info(0, 2);
    }
}

template<class T, int CHAR_LENGTH>
void CBPlusTree<T, CHAR_LENGTH>::insert(string key, const T &data) {
    KeyDataType<T, CHAR_LENGTH> keyData(key, data);
    if (rootPossession == 0) {
        CLeafNode<T, CHAR_LENGTH> newRoot;
        rootType = LEAF;
        newRoot.setPossession(CLeafNode_store.write(newRoot));
        m_DataHead = rootPossession = newRoot.getPossession();
        CLeafNode_store.update(newRoot, newRoot.getPossession());
    }
    if (rootType == LEAF) {
        CLeafNode<T, CHAR_LENGTH> root;
        getRootLeaf(root);
        if (root.getKeyNum() >= MAXNUM_KEY) { // 根结点已满，分裂
            CInternalNode<T, CHAR_LENGTH> newNode;  //创建新的根节点
            newNode.setPossession(CInternalNode_store.write(newNode));
            newNode.setChild(0, root.getPossession(), root.getType());
            root.split(newNode, 0, this);    // 叶子结点分裂
            rootPossession = newNode.getPossession();
            rootType = newNode.getType();//更新根节点指针
            CInternalNode<T, CHAR_LENGTH> nRoot;
            getRootInternal(nRoot);
            recursive_insertInternal(nRoot, keyData);
        } else
            recursive_insertLeaf(root, keyData);
    } else {
        CInternalNode<T, CHAR_LENGTH> root;
        getRootInternal(root);
        if (root.getKeyNum() >= MAXNUM_KEY) { // 根结点已满，分裂
            CInternalNode<T, CHAR_LENGTH> newNode;  //创建新的根节点
            newNode.setPossession(CInternalNode_store.write(newNode));
            newNode.setChild(0, root.getPossession(), root.getType());
            root.split(newNode, 0, this);    // 叶子结点分裂
            rootPossession = newNode.getPossession();
            rootType = newNode.getType();//更新根节点指针
            getRootInternal(root);
        }
        recursive_insertInternal(root, keyData);
    }
}

template<class T, int CHAR_LENGTH>
void CBPlusTree<T, CHAR_LENGTH>::recursive_insertLeaf(CLeafNode<T, CHAR_LENGTH> parentNode,
                                                      KeyDataType<T, CHAR_LENGTH> keyData) {
    parentNode.insert(keyData);
    CLeafNode_store.update(parentNode, parentNode.getPossession());
}

template<class T, int CHAR_LENGTH>
void CBPlusTree<T, CHAR_LENGTH>::recursive_insertInternal(CInternalNode<T, CHAR_LENGTH> parentNode,
                                                          KeyDataType<T, CHAR_LENGTH> keyData) {
    int keyIndex = parentNode.getKeyDataIndex(keyData);
    int childIndex = parentNode.getChildIndex(keyData, keyIndex);
    if (parentNode.getChildtype() == INTERNAL) {
        CInternalNode<T, CHAR_LENGTH> childNode;
        parentNode.getChildInternal(childIndex, this, childNode);
        if (childNode.getKeyNum() >= MAXNUM_LEAF) {
            childNode.split(parentNode, childIndex, this);
            CInternalNode_store.read(parentNode, parentNode.getPossession());
            if (keyData >= parentNode.getKeyDataValue(childIndex))   // 确定目标子结点
                parentNode.getChildInternal(childIndex + 1, this, childNode);
        }
        recursive_insertInternal(childNode, keyData);
    } else {
        CLeafNode<T, CHAR_LENGTH> childNode;
        parentNode.getChildLeaf(childIndex, this, childNode);
        if (childNode.getKeyNum() >= MAXNUM_LEAF) {  // 子结点已满，需进行分裂
            childNode.split(parentNode, childIndex, this);
            CInternalNode_store.read(parentNode, parentNode.getPossession());
            if (keyData >= parentNode.getKeyDataValue(childIndex))// 确定目标子结点
                parentNode.getChildLeaf(childIndex + 1, this, childNode);
        }
        recursive_insertLeaf(childNode, keyData);
    }
}

template<class T, int CHAR_LENGTH>
void CBPlusTree<T, CHAR_LENGTH>::clean() {
    if (rootPossession != 0) {
        if (rootType == INTERNAL) {
            CInternalNode<T, CHAR_LENGTH> root;
            getRootInternal(root);
            root.clear(this);
        } else {
            CLeafNode<T, CHAR_LENGTH> root;
            getRootLeaf(root);
            root.clear(this);
        }
        rootPossession = m_DataHead = 0;
    }
}

template<class T, int CHAR_LENGTH>
bool CBPlusTree<T, CHAR_LENGTH>::Find(string key) {
    if (rootPossession == 0) return false;
    if (rootType == INTERNAL) {
        CInternalNode<T, CHAR_LENGTH> root;
        getRootInternal(root);
        return recursive_searchInternal(root, key);
    } else {
        CLeafNode<T, CHAR_LENGTH> root;
        getRootLeaf(root);
        return recursive_searchLeaf(root, key);
    }
}

template<class T, int CHAR_LENGTH>
bool CBPlusTree<T, CHAR_LENGTH>::recursive_searchInternal(const CInternalNode<T, CHAR_LENGTH> &pNode, string key) {
    int keyIndex = pNode.getKeyIndex(key);
    int childIndex = pNode.getChildIndex(key, keyIndex); // 孩子结点指针索引
    if (keyIndex < pNode.getKeyNum() && pNode.compareKeyvalue(keyIndex, key) == 0) {
        return true;
    } else {
        if (pNode.getChildtype() == INTERNAL) {
            CInternalNode<T, CHAR_LENGTH> tmp;
            pNode.getChildInternal(childIndex, this, tmp);
            return recursive_searchInternal(tmp, key);
        } else {
            CLeafNode<T, CHAR_LENGTH> tmp;
            pNode.getChildLeaf(childIndex, this, tmp);
            return recursive_searchLeaf(tmp, key);
        }
    }
}

template<class T, int CHAR_LENGTH>
bool CBPlusTree<T, CHAR_LENGTH>::recursive_searchLeaf(const CLeafNode<T, CHAR_LENGTH> &pNode, string key) {
    int keyIndex = pNode.getKeyIndex(key);
    if (keyIndex < pNode.getKeyNum() && pNode.compareKeyvalue(keyIndex, key) == 0)
        return true;
    else return false;
}

template<class T, int CHAR_LENGTH>
void CBPlusTree<T, CHAR_LENGTH>::Delete(string key, T &dataValue) {
    if (rootPossession == 0) return;
    KeyDataType<T, CHAR_LENGTH> keyData(key, dataValue);
    if (rootType == LEAF) {
        CLeafNode<T, CHAR_LENGTH> root;
        getRootLeaf(root);
        if (root.getKeyNum() == 1) {
            if (root.getKeyDataValue(0) == keyData)
                clean();
            return;
        } else recursive_removeLeaf(root, keyData);
    } else {
        CInternalNode<T, CHAR_LENGTH> root;
        getRootInternal(root);
//        cout << dataValue << " :";
//        for (int i = 0; i < root.getKeyNum(); ++i)
//            cout << root.getKeyDataValue(i).DataValue << " ";
//        cout << "\n";
        if (root.getKeyNum() == 1) {
            if (root.getChildtype() == INTERNAL) {
                CInternalNode<T, CHAR_LENGTH> pChild1, pChild2;
                root.getChildInternal(0, this, pChild1);
                root.getChildInternal(1, this, pChild2);
                if (pChild1.getKeyNum() == MINNUM_KEY && pChild2.getKeyNum() == MINNUM_KEY) {
                    pChild1.mergeChild(root, pChild2, 0, this);
                    CInternalNode_store.Delete(rootPossession);
                    rootPossession = pChild1.getPossession();
                    recursive_removeInternal(pChild1, keyData);
                } else recursive_removeInternal(root, keyData);
            } else {
                CLeafNode<T, CHAR_LENGTH> pChild1, pChild2;
                root.getChildLeaf(0, this, pChild1);
                root.getChildLeaf(1, this, pChild2);
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

template<class T, int CHAR_LENGTH>
void CBPlusTree<T, CHAR_LENGTH>::recursive_removeInternal(CInternalNode<T, CHAR_LENGTH> parentNode,
                                                          KeyDataType<T, CHAR_LENGTH> keyData) {
    int keyIndex = parentNode.getKeyDataIndex(keyData);
    int childIndex = parentNode.getChildIndex(keyData, keyIndex); // 孩子结点指针索引
    if (parentNode.getChildtype() == LEAF) {
        CLeafNode<T, CHAR_LENGTH> pChildNode;
        parentNode.getChildLeaf(childIndex, this, pChildNode);
        if (pChildNode.getKeyNum() == MINNUM_KEY) {
            CLeafNode<T, CHAR_LENGTH> pLeft, pRight;
            if (childIndex > 0) parentNode.getChildLeaf(childIndex - 1, this, pLeft);
            if (childIndex < parentNode.getKeyNum()) parentNode.getChildLeaf(childIndex + 1, this, pRight);
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
        CInternalNode<T, CHAR_LENGTH> pChildNode;
        parentNode.getChildInternal(childIndex, this, pChildNode);
        if (pChildNode.getKeyNum() == MINNUM_KEY) {
            CInternalNode<T, CHAR_LENGTH> pLeft, pRight;
            if (childIndex > 0) parentNode.getChildInternal(childIndex - 1, this, pLeft);
            if (childIndex < parentNode.getKeyNum()) parentNode.getChildInternal(childIndex + 1, this, pRight);
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

template<class T, int CHAR_LENGTH>
void CBPlusTree<T, CHAR_LENGTH>::recursive_removeLeaf(CLeafNode<T, CHAR_LENGTH> parentNode,
                                                      KeyDataType<T, CHAR_LENGTH> keyData) {
    int keyIndex = parentNode.getKeyDataIndex(keyData);
    parentNode.remove(keyData, this);  // 直接删除
    // 如果键值在内部结点中存在，也要相应的替换内部结点
    if (keyIndex == 0 && rootType != LEAF && parentNode.getPossession() != m_DataHead) {
        CInternalNode<T, CHAR_LENGTH> root;
        getRootInternal(root);
        changeKey(root, keyData, parentNode.getKeyDataValue(0));
    }
}

template<class T, int CHAR_LENGTH>
void CBPlusTree<T, CHAR_LENGTH>::changeKey(CInternalNode<T, CHAR_LENGTH> pNode, KeyDataType<T, CHAR_LENGTH> oldKeyData,
                                           KeyDataType<T, CHAR_LENGTH> newKeyData) {
    int keyIndex = pNode.getKeyDataIndex(oldKeyData);
    if (keyIndex < pNode.getKeyNum() && pNode.getKeyDataValue(keyIndex) == oldKeyData) {  // 找到
        pNode.setKeyData(keyIndex, newKeyData);
        CInternalNode_store.update(pNode, pNode.getPossession());
    } else {  // 继续找
        if (pNode.getChildtype() == INTERNAL) {
            CInternalNode<T, CHAR_LENGTH> pChildnode;
            pNode.getChildInternal(keyIndex, this, pChildnode);
            changeKey(pChildnode, oldKeyData, newKeyData);
        }
    }
}

template<class T, int CHAR_LENGTH>
vector<T> CBPlusTree<T, CHAR_LENGTH>::FindAll(string compareKey) {
    vector<T> ans;
    if (m_DataHead == 0) {
        // cout << "null\n";
        return ans;
    }
    int start = searchFind(compareKey);
    CLeafNode<T, CHAR_LENGTH> it;
    CLeafNode_store.read(it, start);
    for (;; it = it.getRightSibling(this)) {
        int keyIndex = it.getKeyIndex(compareKey);
        int i;
        for (i = keyIndex; i < it.getKeyNum(); ++i) {
            if (it.compareKeyvalue(i, compareKey) == 0)
                ans.push_back(it.getData(i));
            else break;
        }
        if (i != it.getKeyNum()) break;
        if (it.getRightPossession() == 0) break;
    }
    // if (ans.empty()) cout << "null\n";
    // else {
    //     for (auto iter: ans) {
    //         cout << iter << " ";
    //     }
    //     cout << "\n";
    // }
    return ans;
}

template<class T, int CHAR_LENGTH>
int CBPlusTree<T, CHAR_LENGTH>::searchFind(string key) {
    if (rootType == INTERNAL) {
        CInternalNode<T, CHAR_LENGTH> root;
        getRootInternal(root);
        return recursive_searchFindInternal(root, key);
    } else return rootPossession;
}

template<class T, int CHAR_LENGTH>
int CBPlusTree<T, CHAR_LENGTH>::recursive_searchFindInternal(const CInternalNode<T, CHAR_LENGTH> &pNode, string key) {
    int keyIndex = pNode.getKeyIndex(key);
    if (keyIndex < pNode.getKeyNum() && pNode.compareKeyvalue(keyIndex, key) > 0) keyIndex++;
    if (pNode.getChildtype() == INTERNAL) {
        CInternalNode<T, CHAR_LENGTH> childNode;
        pNode.getChildInternal(keyIndex, this, childNode);
        return recursive_searchFindInternal(childNode, key);
    } else return pNode.getChild(keyIndex);
}
#endif //MAIN_CPP_CBPLUSTREE_HPP