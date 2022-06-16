#ifndef MAIN_CPP_CBPLUSTREE_HPP
#define MAIN_CPP_CBPLUSTREE_HPP
#include<vector>
enum NODE_TYPE {
    INTERNAL, LEAF
};        // 结点类型：内结点、叶子结点
enum SIBLING_DIRECTION {
    LEFT, RIGHT
};   // 兄弟结点方向：左兄弟结点、右兄弟结点
const int ORDER = 50;                   // B+树的阶（非根内结点的最小子树个数）
const int MINNUM_KEY = ORDER - 1;        // 最小键值个数
const int MAXNUM_KEY = 2 * ORDER - 1;      // 最大键值个数
const int MINNUM_CHILD = MINNUM_KEY + 1; // 最小子树个数
const int MAXNUM_CHILD = MAXNUM_KEY + 1; // 最大子树个数
const int MINNUM_LEAF = MINNUM_KEY;    // 最小叶子结点键值个数
const int MAXNUM_LEAF = MAXNUM_KEY;    // 最大叶子结点键值个数
const int CHAR_LENGTH = 46;//key长度

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

    CInternalNode<T> getChildInternal(int i, CBPlusTree<T> *tree);

    CLeafNode<T> getChildLeaf(int i, CBPlusTree<T> *tree);

    void split(CInternalNode<T> parentNode, int childIndex, CBPlusTree<T> *tree);

    void setChild(int i, int child, NODE_TYPE type) {
        m_Childs[i] = child;
        childs_type = type;
    }

    void insert(int keyIndex, int childIndex, KeyDataType<T> keyData, int childNode, NODE_TYPE type);
    //在keyIndex/childIndex位置插入
    // 不修改文件

    NODE_TYPE getChildtype() { return childs_type; }

    void mergeChild(CInternalNode<T> parentNode, CInternalNode<T> childNode, int keyIndex,
                    CBPlusTree<T> *tree);//将childnode（右）合并到this（左）里

    void removeKey(int keyIndex, int childIndex);//不修改文件

    void clear(CBPlusTree<T> *tree);

    void borrowFrom(CInternalNode<T> siblingNode, CInternalNode<T> parentNode, int keyIndex, SIBLING_DIRECTION d,
                    CBPlusTree<T> *tree);//尽量从左边借，复杂度小

    int getChildIndex(string key, int keyIndex);

};

// 叶子结点
template<typename T>
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

    CLeafNode<T> getRightSibling(CBPlusTree<T> *tree) const;

    int getRightPossession();

    void setRightSibling(int possession) { m_RightSibling = possession; }

    T getData(int i) const { return m_KeyData[i].DataValue; }

    void remove(KeyDataType<T> keyData, CBPlusTree<T> *tree);

    void insert(KeyDataType<T> keyData);

    void split(CInternalNode<T> parentNode, int childIndex, CBPlusTree<T> *tree);

    void mergeChild(CInternalNode<T> parentNode, CLeafNode<T> childNode, int keyIndex, CBPlusTree<T> *tree);

    void removeKey(int keyIndex, CBPlusTree<T> *tree);

    void clear(CBPlusTree<T> *tree);

    void
    borrowFrom(CLeafNode<T> siblingNode, CInternalNode<T> parentNode, int keyIndex, SIBLING_DIRECTION d,
               CBPlusTree<T> *tree);
};

template<class T>
class CBPlusTree {
public:
    CBPlusTree(const string &FN);

    ~CBPlusTree();

    void insert(string key, const T &data);

    void Delete(string key, T &dataValue);

    vector<T> FindAll(string compareKey);

    bool Find(string key);

    void clean();

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

    int searchFind(string key);

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


template<class T>
int CInternalNode<T>::getKeyIndex(string key) {
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

template<class T>
int CInternalNode<T>::getKeyIndexLow(string key) {
    int left = 0;
    int right = getKeyNum() - 1;
    int mid;
    while (left <= right) {
        mid = (left + right) >> 1;
        if (compareKeyvalue(mid, key) >= 0)
            left = mid + 1;
        else right = mid - 1;
    }
    return left;
}

template<class T>
int CInternalNode<T>::getKeyDataIndex(KeyDataType<T> keyData) {
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

template<class T>
int CLeafNode<T>::getKeyDataIndex(KeyDataType<T> keyData) {
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

template<class T>
int CLeafNode<T>::getKeyIndex(string key) {
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
template<class T>
CInternalNode<T>::CInternalNode() {
    setType(INTERNAL);
    setKeyNum(0);
    for (int i = 0; i < MAXNUM_CHILD; ++i)
        m_Childs[i] = 0;
    for (int i = 0; i < MAXNUM_KEY; ++i)
        fill(m_KeyData[i].KeyValue, m_KeyData[i].KeyValue + CHAR_LENGTH, '\0');
}

template<class T>
CInternalNode<T>::~CInternalNode() {}

template<class T>
CInternalNode<T> CInternalNode<T>::getChildInternal(int i, CBPlusTree<T> *tree) {
    CInternalNode<T> tmp;
    tree->InternalRead(tmp, m_Childs[i]);
    return tmp;
}

template<class T>
CLeafNode<T> CInternalNode<T>::getChildLeaf(int i, CBPlusTree<T> *tree) {
    CLeafNode<T> tmp;
    tree->LeafRead(tmp, m_Childs[i]);
    return tmp;
}

template<class T>
void CInternalNode<T>::clear(CBPlusTree<T> *tree) {
    for (int i = 0; i <= m_KeyNum; ++i) {
        if (m_Childs[i] == 0) continue;
        if (childs_type == INTERNAL) {
            CInternalNode<T> tmp = getChildInternal(i, tree);
            tmp.clear(tree);
            tree->InternalDelete(m_Childs[i]);
        } else {
            CLeafNode<T> tmp;
            tree->LeafRead(tmp, m_Childs[i]);
            tmp.clear(tree);
        }
        m_Childs[i] = 0;
    }
}

template<class T>
void CInternalNode<T>::split(CInternalNode<T> parentNode, int childIndex, CBPlusTree<T> *tree) {
    CInternalNode<T> nNode;//分裂后的右节点
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
    CInternalNode<T> tmp = *this;
    tree->InteranlUpdate(tmp, storePossession);
}

template<class T>
void CInternalNode<T>::insert(int keyIndex, int childIndex, KeyDataType<T> keyData, int childNode, NODE_TYPE type) {
    int i;
    for (i = getKeyNum(); i > keyIndex; --i)//将父节点中的childIndex后的所有关键字的值和子树指针向后移一位
        setKeyData(i, m_KeyData[i - 1]);
    for (i = getKeyNum() + 1; i > childIndex; --i)
        setChild(i, m_Childs[i - 1], childs_type);
    setChild(childIndex, childNode, type);
    setKeyData(keyIndex, keyData);
    setKeyNum(m_KeyNum + 1);
}

template<class T>
void CInternalNode<T>::mergeChild(CInternalNode<T> parentNode, CInternalNode<T> childNode, int keyIndex,
                                  CBPlusTree<T> *tree) {
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
    CInternalNode<T> tmp = *this;
    tree->InteranlUpdate(tmp, storePossession);
}

template<class T>
void CInternalNode<T>::removeKey(int keyIndex, int childIndex) {
    for (int i = 0; i < getKeyNum() - keyIndex - 1; ++i)
        setKeyData(keyIndex + i, m_KeyData[keyIndex + i + 1]);
    for (int i = 0; i < getKeyNum() - childIndex; ++i)
        setChild(childIndex + i, m_Childs[childIndex + i + 1], childs_type);
    setKeyNum(getKeyNum() - 1);
}

template<class T>
void CInternalNode<T>::borrowFrom(CInternalNode<T> siblingNode, CInternalNode<T> parentNode, int keyIndex,
                                  SIBLING_DIRECTION d,
                                  CBPlusTree<T> *tree) {
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
    CInternalNode<T> tmp = *this;
    tree->InteranlUpdate(tmp, storePossession);
}

template<class T>
int CInternalNode<T>::getChildIndex(string key, int keyIndex) {
    if (compareKeyvalue(keyIndex, key) == 0)
        return keyIndex + 1;
    else return keyIndex;
}

// CLeafNode
template<class T>
CLeafNode<T>::CLeafNode() {
    setType(LEAF);
    m_LeftSibling = m_RightSibling = 0;
    setKeyNum(0);
    for (int i = 0; i < MAXNUM_KEY; ++i)
        fill(m_KeyData[i].KeyValue, m_KeyData[i].KeyValue + CHAR_LENGTH, '\0');
}

template<class T>
CLeafNode<T>::~CLeafNode() = default;;

template<class T>
void CLeafNode<T>::clear(CBPlusTree<T> *tree) {
    tree->LeafDelete(storePossession);
}

template<class T>
CLeafNode<T> CLeafNode<T>::getRightSibling(CBPlusTree<T> *tree) const {
    CLeafNode<T> tmp;
    tree->LeafRead(tmp, m_RightSibling);
    return tmp;
}

template<class T>
int CLeafNode<T>::getRightPossession() {
    return m_RightSibling;
}

template<class T>
void CLeafNode<T>::insert(KeyDataType<T> keyData) {
    int i;
    for (i = m_KeyNum; i > 0 && m_KeyData[i - 1] > keyData; --i)
        setKeyData(i, m_KeyData[i - 1]);
    setKeyData(i, keyData);
    setKeyNum(m_KeyNum + 1);
}

template<class T>
void CLeafNode<T>::split(CInternalNode<T> parentNode, int childIndex, CBPlusTree<T> *tree) {
    CLeafNode<T> nNode;//分裂后的右节点
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
    CLeafNode<T> tmp1 = *this;
    tree->LeafUpdate(tmp1, this->getPossession());
    parentNode.insert(childIndex, childIndex + 1, m_KeyData[MINNUM_LEAF], nNode.storePossession, LEAF);
    tree->InteranlUpdate(parentNode, parentNode.getPossession());
}

template<class T>
void CLeafNode<T>::mergeChild(CInternalNode<T> parentNode, CLeafNode<T> childNode, int keyIndex, CBPlusTree<T> *tree) {
    // 合并数据
    for (int i = 0; i < childNode.getKeyNum(); ++i)
        setKeyData(getKeyNum() + i, childNode.getKeyDataValue(i));
    setKeyNum(m_KeyNum + childNode.getKeyNum());
    setRightSibling(childNode.m_RightSibling);
    //父节点删除index的key，
    parentNode.removeKey(keyIndex, keyIndex + 1);
    CLeafNode<T> tmp = *this;
    tree->LeafUpdate(tmp, getPossession());
    tree->InteranlUpdate(parentNode, parentNode.getPossession());
}

template<class T>
void CLeafNode<T>::removeKey(int keyIndex, CBPlusTree<T> *tree) {
    for (int i = keyIndex; i < getKeyNum() - 1; ++i)
        setKeyData(i, m_KeyData[i + 1]);
    setKeyNum(getKeyNum() - 1);
    CLeafNode<T> tmp = *this;
    tree->LeafUpdate(tmp, getPossession());
}

template<class T>
void CLeafNode<T>::remove(KeyDataType<T> keyData, CBPlusTree<T> *tree) {
    int keyDataIndex = getKeyDataIndex(keyData);
    if (keyDataIndex >= getKeyNum()) return;
    if (getKeyDataValue(keyDataIndex) != keyData) return;
    for (int i = keyDataIndex; i < getKeyNum() - 1; ++i)
        setKeyData(i, getKeyDataValue(i + 1));
    setKeyNum(getKeyNum() - 1);
    CLeafNode<T> *tmp = this;
    tree->LeafUpdate(*tmp, getPossession());
}

template<class T>
void CLeafNode<T>::borrowFrom(CLeafNode<T> siblingNode, CInternalNode<T> parentNode, int keyIndex, SIBLING_DIRECTION d,
                              CBPlusTree<T> *tree) {
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
    CLeafNode<T> *tmp1 = this;
    tree->LeafUpdate(*tmp1, getPossession());
    tree->InteranlUpdate(parentNode, parentNode.getPossession());
}

template<class T>
CBPlusTree<T>::CBPlusTree(const string &FN) : CLeafNode_store(FN + "LeafStore", 1, 0),
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

template<class T>
CBPlusTree<T>::~CBPlusTree() {
    CLeafNode_store.write_info(m_DataHead, 3);
    if (rootType == LEAF) {
        CLeafNode_store.write_info(rootPossession, 2);
        CInternalNode_store.write_info(0, 2);
    } else {
        CInternalNode_store.write_info(rootPossession, 2);
        CLeafNode_store.write_info(0, 2);
    }
}

template<class T>
void CBPlusTree<T>::insert(string key, const T &data) {
    // cout<<"!!!"<<key<<endl;
    KeyDataType<T> keyData(key, data);
    if (rootPossession == 0) {
        CLeafNode<T> newRoot;
        rootType = LEAF;
        newRoot.setPossession(CLeafNode_store.write(newRoot));
        m_DataHead = rootPossession = newRoot.getPossession();
        CLeafNode_store.update(newRoot, newRoot.getPossession());
    }
    if (rootType == LEAF) {
        CLeafNode<T> root = getRootLeaf();
        if (root.getKeyNum() >= MAXNUM_KEY) { // 根结点已满，分裂
            CInternalNode<T> newNode;  //创建新的根节点
            newNode.setPossession(CInternalNode_store.write(newNode));
            newNode.setChild(0, root.getPossession(), root.getType());
            root.split(newNode, 0, this);    // 叶子结点分裂
            rootPossession = newNode.getPossession();
            rootType = newNode.getType();//更新根节点指针
            CInternalNode<T> nRoot = getRootInternal();
            recursive_insertInternal(nRoot, keyData);
        } else
            recursive_insertLeaf(root, keyData);
    } else {
        CInternalNode<T> root = getRootInternal();
        if (root.getKeyNum() >= MAXNUM_KEY) { // 根结点已满，分裂
            CInternalNode<T> newNode;  //创建新的根节点
            newNode.setPossession(CInternalNode_store.write(newNode));
            newNode.setChild(0, root.getPossession(), root.getType());
            root.split(newNode, 0, this);    // 叶子结点分裂
            rootPossession = newNode.getPossession();
            rootType = newNode.getType();//更新根节点指针
            root = getRootInternal();
        }
        recursive_insertInternal(root, keyData);
    }
}

template<class T>
void CBPlusTree<T>::recursive_insertLeaf(CLeafNode<T> parentNode, KeyDataType<T> keyData) {
    parentNode.insert(keyData);
    CLeafNode_store.update(parentNode, parentNode.getPossession());
}

template<class T>
void CBPlusTree<T>::recursive_insertInternal(CInternalNode<T> parentNode, KeyDataType<T> keyData) {
    int keyIndex = parentNode.getKeyDataIndex(keyData);
    int childIndex = parentNode.getChildIndex(keyData, keyIndex);
    if (parentNode.getChildtype() == INTERNAL) {
        CInternalNode<T> childNode = parentNode.getChildInternal(childIndex, this);
        if (childNode.getKeyNum() >= MAXNUM_LEAF) {
            childNode.split(parentNode, childIndex, this);
            CInternalNode_store.read(parentNode, parentNode.getPossession());
            if (keyData >= parentNode.getKeyDataValue(childIndex))   // 确定目标子结点
                childNode = parentNode.getChildInternal(childIndex + 1, this);
        }
        recursive_insertInternal(childNode, keyData);
    } else {
        CLeafNode<T> childNode = parentNode.getChildLeaf(childIndex, this);
        if (childNode.getKeyNum() >= MAXNUM_LEAF) {  // 子结点已满，需进行分裂
            childNode.split(parentNode, childIndex, this);
            CInternalNode_store.read(parentNode, parentNode.getPossession());
            if (keyData >= parentNode.getKeyDataValue(childIndex))// 确定目标子结点
                childNode = parentNode.getChildLeaf(childIndex + 1, this);
        }
        recursive_insertLeaf(childNode, keyData);
    }
}

template<class T>
void CBPlusTree<T>::clean() {
    if (rootPossession != 0) {
        if (rootType == INTERNAL) {
            CInternalNode<T> root = getRootInternal();
            root.clear(this);
        } else {
            CLeafNode<T> root = getRootLeaf();
            root.clear(this);
        }
        rootPossession = m_DataHead = 0;
    }
}

template<class T>
bool CBPlusTree<T>::Find(string key) {
    // cout<<"Find "<<key<<endl;
    if (rootPossession == 0) return false;
    if (rootType == INTERNAL) return recursive_searchInternal(getRootInternal(), key);
    else return recursive_searchLeaf(getRootLeaf(), key);
}

template<class T>
bool CBPlusTree<T>::recursive_searchInternal(CInternalNode<T> pNode, string key) {
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

template<class T>
bool CBPlusTree<T>::recursive_searchLeaf(CLeafNode<T> pNode, string key) {
    int keyIndex = pNode.getKeyIndex(key);
    if (keyIndex < pNode.getKeyNum() && pNode.compareKeyvalue(keyIndex, key) == 0)
        return true;
    else return false;
}

template<class T>
void CBPlusTree<T>::Delete(string key, T &dataValue) {
    // cout<<"@@@"<<key<<endl;
    if (rootPossession == 0) return;
    KeyDataType<T> keyData(key, dataValue);
    if (rootType == LEAF) {
        CLeafNode<T> root = getRootLeaf();
        if (root.getKeyNum() == 1) {
            if (root.getKeyDataValue(0) == keyData)
                clean();
            return;
        } else recursive_removeLeaf(root, keyData);
    } else {
        CInternalNode<T> root = getRootInternal();
//        cout << dataValue << " :";
//        for (int i = 0; i < root.getKeyNum(); ++i)
//            cout << root.getKeyDataValue(i).DataValue << " ";
//        cout << "\n";
        if (root.getKeyNum() == 1) {
            if (root.getChildtype() == INTERNAL) {
                CInternalNode<T> pChild1, pChild2;
                pChild1 = root.getChildInternal(0, this);
                pChild2 = root.getChildInternal(1, this);
                if (pChild1.getKeyNum() == MINNUM_KEY && pChild2.getKeyNum() == MINNUM_KEY) {
                    pChild1.mergeChild(root, pChild2, 0, this);
                    CInternalNode_store.Delete(rootPossession);
                    rootPossession = pChild1.getPossession();
                    recursive_removeInternal(pChild1, keyData);
                } else recursive_removeInternal(root, keyData);
            } else {
                CLeafNode<T> pChild1, pChild2;
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

template<class T>
void CBPlusTree<T>::recursive_removeInternal(CInternalNode<T> parentNode, KeyDataType<T> keyData) {
    int keyIndex = parentNode.getKeyDataIndex(keyData);
    int childIndex = parentNode.getChildIndex(keyData, keyIndex); // 孩子结点指针索引
    if (parentNode.getChildtype() == LEAF) {
        CLeafNode<T> pChildNode = parentNode.getChildLeaf(childIndex, this);
        if (pChildNode.getKeyNum() == MINNUM_KEY) {
            CLeafNode<T> pLeft, pRight;
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
        CInternalNode<T> pChildNode = parentNode.getChildInternal(childIndex, this);
        if (pChildNode.getKeyNum() == MINNUM_KEY) {
            CInternalNode<T> pLeft, pRight;
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

template<class T>
void CBPlusTree<T>::recursive_removeLeaf(CLeafNode<T> parentNode, KeyDataType<T> keyData) {
    int keyIndex = parentNode.getKeyDataIndex(keyData);
    parentNode.remove(keyData, this);  // 直接删除
    // 如果键值在内部结点中存在，也要相应的替换内部结点
    if (keyIndex == 0 && rootType != LEAF && parentNode.getPossession() != m_DataHead) {
        CInternalNode<T> root = getRootInternal();
        changeKey(root, keyData, parentNode.getKeyDataValue(0));
    }
}

template<class T>
void CBPlusTree<T>::changeKey(CInternalNode<T> pNode, KeyDataType<T> oldKeyData, KeyDataType<T> newKeyData) {
    int keyIndex = pNode.getKeyDataIndex(oldKeyData);
    if (keyIndex < pNode.getKeyNum() && pNode.getKeyDataValue(keyIndex) == oldKeyData) {  // 找到
        pNode.setKeyData(keyIndex, newKeyData);
        CInternalNode_store.update(pNode, pNode.getPossession());
    } else {  // 继续找
        if (pNode.getChildtype() == INTERNAL) {
            CInternalNode<T> pChildnode = pNode.getChildInternal(keyIndex, this);
            changeKey(pChildnode, oldKeyData, newKeyData);
        }
    }
}

template<class T>
vector<T> CBPlusTree<T>::FindAll(string compareKey) {
    // cout<<"FindAll "<<compareKey<<endl;
    vector<T> ans;
    if (m_DataHead == 0) {
        // cout << "null\n";
        return ans;
    }
    int start = searchFind(compareKey);
    CLeafNode<T> it;
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

template<class T>
int CBPlusTree<T>::searchFind(string key) {
    if (rootType == INTERNAL) return recursive_searchFindInternal(getRootInternal(), key);
    else return rootPossession;
}

template<class T>
int CBPlusTree<T>::recursive_searchFindInternal(CInternalNode<T> pNode, string key) {
    int keyIndex = pNode.getKeyIndex(key);
    if (keyIndex < pNode.getKeyNum() && pNode.compareKeyvalue(keyIndex, key) > 0) keyIndex++;
    if (pNode.getChildtype() == INTERNAL)
        return recursive_searchFindInternal(pNode.getChildInternal(keyIndex, this), key);
    else return pNode.getChild(keyIndex);
}
#endif //MAIN_CPP_CBPLUSTREE_HPP