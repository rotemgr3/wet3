#ifndef RankTree_H_
#define RankTree_H_

#include <memory>
#include "node.h"
#include "map.h"
#include "level.h"


class RankTree {
    private:
        static std::shared_ptr<Node> InsertAux(std::shared_ptr<Node> root, 
                                                            std::shared_ptr<Node> toInsert); 
        static std::shared_ptr<Node> LLRotation(std::shared_ptr<Node>& root);
        static std::shared_ptr<Node> LRRotation(std::shared_ptr<Node>& root);
        static std::shared_ptr<Node> RLRotation(std::shared_ptr<Node>& root);
        static std::shared_ptr<Node> RRRotation(std::shared_ptr<Node>& root);
        static std::shared_ptr<Node> RemoveAux(std::shared_ptr<Node>& root, int key);
        static std::shared_ptr<Node> FindNextInOrder(std::shared_ptr<Node> root);
        static void SaveInOrder(const std::shared_ptr<Node> root, std::shared_ptr<int> *keyArr, std::shared_ptr<Level> *dataArr, int *i);
        static void MergeArr(std::shared_ptr<int> *keyArr1, std::shared_ptr<int> *keyArr2, std::shared_ptr<Level> *dataArr1,
                                std::shared_ptr<Level> *dataArr2, std::shared_ptr<int> *keyMergedArr, std::shared_ptr<Level> *dataMergedArr,
                                int size1, int size2);
        static void InsertElements(std::shared_ptr<Node> root, std::shared_ptr<int> *keyArr,
                                   std::shared_ptr<Level> *dataArr, int size, int *i);
        static RankTree BuildEmptyTree(int n);
        static void removeRightLeafs(std::shared_ptr<Node> root, int *removecount, int leafPathLen, int currPathLen);
        static int FindHeightOfComplete(int num);
        static std::shared_ptr<Node> BuildCompleteTree(int h);
        static int ComputeSizeOfComplete(int height);
        static int IntMax(int a, int b);

    public:
        std::shared_ptr<Node> root;
        int size;

        RankTree() : root(nullptr), size(0) {}
        RankTree(std::shared_ptr<Node> root, int size) : root(root), size(size) {}
        ~RankTree() = default;
        std::shared_ptr<Level> Get(int target);
        bool Find(int target);
        void Insert(int key, std::shared_ptr<Level>& data);
        void Remove(int key);
        static RankTree Merge(const RankTree& tree1, const RankTree& tree2);
        Level& GetMax();
        Level& GetMin();
        static Map* MergeToArr(const RankTree& tree1, const RankTree& tree2);
        static RankTree ArrToRankTree(Map* map, int size, int oldSize);  
        RankTree& operator=(const RankTree& copy);   
};


int RankTree::IntMax(int a, int b)
{
    return a > b ? a : b;
}


RankTree& RankTree::operator=(const RankTree& copy)
{
    this->root = copy.root;
    this->size = copy.size;
    return *this;
}     



std::shared_ptr<Level> RankTree::Get(int target)
{
    std::shared_ptr<Node> curr = root;
    while(curr != nullptr)
    {
        if(curr->key == target)
            return curr->data;
        
        if(curr->key < target)
            curr = curr->right;
        else
            curr = curr->left;
    }
    return nullptr;
}


bool RankTree::Find(int target)
{
    if(this->Get(target) == nullptr)
        return false;
    return true;
}


void RankTree::Insert(int key, std::shared_ptr<Level>& dataPtr)
{
    if(this->Find(key))
        return;
    
    std::shared_ptr<Level> copyData = std::shared_ptr<Level>(dataPtr); 
    std::shared_ptr<Node> toInsert = std::shared_ptr<Node>(new Node(key,copyData));

    this->root = RankTree::InsertAux(this->root, toInsert);
    this->size++;
}


std::shared_ptr<Node> RankTree::LLRotation(std::shared_ptr<Node>& root)
{
    std::shared_ptr<Node> B = root;
    std::shared_ptr<Node> A = B->left;
    std::shared_ptr<Node> AL = A->left;
    std::shared_ptr<Node> AR = A->right;

    B->left = AR;
    A->right = B;
    
    B->height = IntMax(Node::GetHeight(B->left), Node::GetHeight(B->right)) + 1;
    A->height = IntMax(Node::GetHeight(A->left), Node::GetHeight(A->right)) + 1;
    B->updateData();
    A->updateData();   
    
    return A;
}


std::shared_ptr<Node> RankTree::LRRotation(std::shared_ptr<Node>& root)
{
    std::shared_ptr<Node> A = root->left;
    std::shared_ptr<Node> B = A->right;
    std::shared_ptr<Node> C = root;
    std::shared_ptr<Node> BL = B->left;
    std::shared_ptr<Node> BR = B->right;

    C->left = BR;
    A->right = BL;
    B->left = A;
    B->right = C;

    A->height = IntMax(Node::GetHeight(A->left), Node::GetHeight(A->right)) + 1;
    C->height = IntMax(Node::GetHeight(C->left), Node::GetHeight(C->right)) + 1;
    B->height = IntMax(Node::GetHeight(B->left), Node::GetHeight(B->right)) + 1;
    A->updateData();
    C->updateData();
    B->updateData();

    return B;
}


std::shared_ptr<Node> RankTree::RLRotation(std::shared_ptr<Node>& root)
{
    std::shared_ptr<Node> A = root->right;
    std::shared_ptr<Node> B = A->left;
    std::shared_ptr<Node> C = root;
    std::shared_ptr<Node> BL = B->left;
    std::shared_ptr<Node> BR = B->right;

    C->right = BL;
    A->left = BR;
    B->right = A;
    B->left = C;

    A->height = IntMax(Node::GetHeight(A->left), Node::GetHeight(A->right)) + 1;
    C->height = IntMax(Node::GetHeight(C->left), Node::GetHeight(C->right)) + 1;
    B->height = IntMax(Node::GetHeight(B->left), Node::GetHeight(B->right)) + 1;
    A->updateData();
    C->updateData();
    B->updateData();

    return B;
}


std::shared_ptr<Node> RankTree::RRRotation(std::shared_ptr<Node>& root)
{
    std::shared_ptr<Node> B = root;
    std::shared_ptr<Node> A = B->right;
    std::shared_ptr<Node> AL = A->left;
    std::shared_ptr<Node> AR = A->right;

    B->right = AL;
    A->left = B;
    
    B->height = IntMax(Node::GetHeight(B->left), Node::GetHeight(B->right)) + 1;
    A->height = IntMax(Node::GetHeight(A->left), Node::GetHeight(A->right)) + 1;
    B->updateData();
    A->updateData();
   
    return A;
}



std::shared_ptr<Node> RankTree::InsertAux(std::shared_ptr<Node> root, 
                                                            std::shared_ptr<Node> toInsert)
{
    if (root == nullptr) 
        return toInsert;
    
    if (root->key < toInsert->key)
        root->right = RankTree::InsertAux(root->right, toInsert);
    else
        root->left = RankTree::InsertAux(root->left, toInsert);
    
    root->height = IntMax(Node::GetHeight(root->left), Node::GetHeight(root->right)) + 1;
    int balanceFactor = GetBF(root);

    if (balanceFactor == 2) {
        if (Node::GetBF(root->left) >= 0)
            return RankTree::LLRotation(root);
        else if (Node::GetBF(root->left) == -1)
            return RankTree::LRRotation(root);
    }
    else if (balanceFactor == -2) {
        if (Node::GetBF(root->right) <= 0)
            return RankTree::RRRotation(root);
        else if (Node::GetBF(root->right) == 1)
            return RankTree::RLRotation(root);
    }  
    return root;
}


void RankTree::Remove(const int& key)
{
    if(!this->Find(key) || key == 0)
        return;
    
    this->root = RankTree::RemoveAux(this->root, key);
    this->size--;
}


std::shared_ptr<Node> RankTree::RemoveAux(std::shared_ptr<Node>& root, const int& key)
{
    if (root == nullptr)
        return nullptr;
    
    if (root->key < key)
        root->right = RankTree::RemoveAux(root->right, key);
    else if (root->key > key)
        root->left = RankTree::RemoveAux(root->left, key);
    
    else {
        if (!root->left && !root->right)
            root = nullptr;
        
        else if (root->left && root->right) {
            std::shared_ptr<Node> leaf = RankTree::FindNextInOrder(root);
            root->key = leaf->key;
            root->data = leaf->data;
            root->right = RankTree::RemoveAux(root->right, leaf->key);
        }

        else if (root->left)
            root = root->left;
        else
            root = root->right;   
    }
    
    if (root == nullptr)
        return nullptr;
    
    root->height = IntMax(Node::GetHeight(root->left), Node::GetHeight(root->right)) + 1;
    int balanceFactor = GetBF(root);

    if (balanceFactor == 2) {
        if (Node::GetBF(root->left) >= 0)
            return RankTree::LLRotation(root);
        else if (Node::GetBF(root->left) == -1)
            return RankTree::LRRotation(root);
    }
    else if (balanceFactor == -2) {
        if (Node::GetBF(root->right) <= 0)
            return RankTree::RRRotation(root);
        else if (Node::GetBF(root->right) == 1)
            return RankTree::RLRotation(root);
    }  
    return root;
}


std::shared_ptr<Node> RankTree::FindNextInOrder(std::shared_ptr<Node> root)
{
    root = root->right;
    while(root->left != nullptr)
        root = root->left;

    return root;
}


void RankTree::MergeArr(std::shared_ptr<int> *keyArr1, std::shared_ptr<int> *keyArr2, std::shared_ptr<Level> *dataArr1,
                                std::shared_ptr<Level> *dataArr2, std::shared_ptr<int> *keyMergedArr, std::shared_ptr<Level> *dataMergedArr,
                                int size1, int size2)
{
    while (size1 > 0 && size2 > 0) {
        if (**keyArr1 < **keyArr2) {
            *keyMergedArr = std::shared_ptr<int>(*keyArr1);
            *dataMergedArr = std::shared_ptr<Level>(*dataArr1);
            keyArr1++;
            dataArr1++;
            size1--;
        }
        else {
            *keyMergedArr = std::shared_ptr<int>(*keyArr2);
            *dataMergedArr = std::shared_ptr<Level>(*dataArr2);
            keyArr2++;
            dataArr2++;
            size2--;
        }
        keyMergedArr++;
        dataMergedArr++;
    }

    while (size1 > 0) {    
        *keyMergedArr = std::shared_ptr<int>(*keyArr1);
        *dataMergedArr = std::shared_ptr<Level>(*dataArr1);
        keyArr1++;
        dataArr1++;
        keyMergedArr++;
        dataMergedArr++;
        size1--;
    }
    
    while (size2 > 0) {    
        *keyMergedArr = std::shared_ptr<int>(*keyArr2);
        *dataMergedArr = std::shared_ptr<Level>(*dataArr2);
        keyArr2++;
        dataArr2++;
        keyMergedArr++;
        dataMergedArr++;
        size2--;
    }
}


void RankTree::SaveInOrder(const std::shared_ptr<Node> root,
                                     std::shared_ptr<int> *keyArr, std::shared_ptr<Level> *dataArr, int *i)
{
    if(root == nullptr)
        return;
    RankTree::SaveInOrder(root->left, keyArr, dataArr, i);
    keyArr[*i] = std::make_shared<int>(int(root->key));
    dataArr[*i] = std::shared_ptr<Level>(root->data);
    (*i)++;
    RankTree::SaveInOrder(root->right, keyArr, dataArr, i);
}


RankTree RankTree::Merge(const RankTree& tree1, const RankTree& tree2)
{
    Map* map = RankTree::MergeToArr(tree1, tree2);
    RankTree mergedRankTree = RankTree::BuildEmptyTree(tree1.size + tree2.size);
    int i = 0;
    RankTree::InsertElements(mergedRankTree.root, (std::shared_ptr<int> *)(map->key), (std::shared_ptr<Level> *)(map->data), tree1.size + tree2.size, &i);
    delete[] (std::shared_ptr<int> *)(map->key);
    delete[] (std::shared_ptr<Level> *)(map->data);
    MapDestroy(map);
    return mergedRankTree;   
}


Map* RankTree::MergeToArr(const RankTree& tree1, const RankTree& tree2)
{
    std::shared_ptr<Level> *dataArr1 = new std::shared_ptr<Level>[tree1.size];
    std::shared_ptr<Level> *dataArr2 = new std::shared_ptr<Level>[tree2.size];
    std::shared_ptr<Level> *dataMergedArr = new std::shared_ptr<Level>[tree1.size + tree2.size];
    std::shared_ptr<int> *keyArr1 = new std::shared_ptr<int>[tree1.size];
    std::shared_ptr<int> *keyArr2 = new std::shared_ptr<int>[tree2.size];
    std::shared_ptr<int> *keyMergedArr = new std::shared_ptr<int>[tree1.size + tree2.size];
    int i = 0;
    SaveInOrder(tree1.root, keyArr1, dataArr1, &i);
    i = 0;
    SaveInOrder(tree2.root, keyArr2, dataArr2, &i);
    MergeArr(keyArr1, keyArr2, dataArr1, dataArr2, keyMergedArr, dataMergedArr, tree1.size, tree2.size);
    delete[] dataArr1;
    delete[] dataArr2;
    delete[] keyArr1;
    delete[] keyArr2;
    Map* map = MapCreate((void*)keyMergedArr, (void*)dataMergedArr);
    return map;  
}


RankTree RankTree::ArrToRankTree(Map* map, int size, int oldSize)
{
    RankTree mergedRankTree = RankTree::BuildEmptyTree(size);
    int i = 0;
    RankTree::InsertElements(mergedRankTree.root, (std::shared_ptr<int> *)(map->key), (std::shared_ptr<Level> *)(map->data), oldSize, &i);
    delete[] (std::shared_ptr<int> *)(map->key);
    delete[] (std::shared_ptr<Level> *)(map->data);

    return mergedRankTree;   
}



void RankTree::InsertElements(std::shared_ptr<Node> root, std::shared_ptr<int> *keyArr,
                                      std::shared_ptr<Level> *dataArr, int size, int *i)
{
    if(root == nullptr)
        return;

    RankTree::InsertElements(root->left, keyArr, dataArr, size, i);
    
    while (*i < size && dataArr[*i] == nullptr) {
        (*i)++;
    }
    root->data = dataArr[*i];
    root->key = *(keyArr[*i]);
    (*i)++;
    RankTree::InsertElements(root->right, keyArr, dataArr, size, i);

    return;
}


RankTree RankTree::BuildEmptyTree(int n)
{
    int completeHeight = RankTree::FindHeightOfComplete(n + 1);
    int completeSize = RankTree::ComputeSizeOfComplete(completeHeight);
    RankTree res = RankTree(RankTree::BuildCompleteTree(completeHeight), completeSize);
    int removeCount = completeSize - n;
    removeRightLeafs(res.root, &removeCount, completeHeight, 0);

    res.size = n;
    return res;
}


void RankTree::removeRightLeafs(std::shared_ptr<Node> root, int* removecount, int leafPathLen, int currPathLen)
{
    if(root == nullptr || *removecount == 0)
        return;

    RankTree::removeRightLeafs(root->right, removecount, leafPathLen, currPathLen + 1);

    if (currPathLen + 1 == leafPathLen) {
        root->right = nullptr;
        (*removecount)--;
    }
    if (currPathLen + 1 == leafPathLen && *removecount > 0) {
        root->left = nullptr;
        (*removecount)--;
        root->height--;
    }

    RankTree::removeRightLeafs(root->left, removecount, leafPathLen, currPathLen + 1);
    return;
}



std::shared_ptr<Node> RankTree::BuildCompleteTree(int h)
{
    if (h == -1)
        return nullptr;
    std::shared_ptr<Node> root = std::shared_ptr<Node>(new Node(h));
    root->right = RankTree::BuildCompleteTree(h - 1);
    root->left = RankTree::BuildCompleteTree(h - 1);
    return root;
}


int RankTree::FindHeightOfComplete(int num)
{
    int twoPow = 1;
    int height = -1;
    while(twoPow < num)
    {
        twoPow *= 2;
        height++;
    }
    return height;
}


int RankTree::ComputeSizeOfComplete(int height)
{
    int n = 1;
    int count = 1;

    while(count <= height + 1) {
        n *= 2;
        count++;
    }

    return n - 1;
}


Level& RankTree::GetMax()
{
    std::shared_ptr<Node> curr = this->root;
    while(curr->right != nullptr)
        curr = curr->right;

    return *(curr->data);
}


Level& RankTree::GetMin()
{
    std::shared_ptr<Node> curr = this->root;
    while(curr->left != nullptr)
        curr = curr->left;

    return *(curr->data);
}

#endif /* RankTree_H */