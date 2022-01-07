#include "ranktree.h"

RankTree::RankTree() : root(nullptr), levelZero(new Node(0, std::shared_ptr<Level>(new Level(0)))), size(0) {
}

RankTree::RankTree(std::shared_ptr<Node> root, int size) : root(root), levelZero(new Node(0, std::shared_ptr<Level>(new Level(0)))), size(0)  {
}


RankTree& RankTree::operator=(const RankTree& copy)
{
    this->root = copy.root;
    this->levelZero = copy.levelZero;
    this->size = copy.size;
    return *this;
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
    B->UpdateData();
    A->UpdateData();   
    
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
    A->UpdateData();
    C->UpdateData();
    B->UpdateData();

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
    A->UpdateData();
    C->UpdateData();
    B->UpdateData();

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
    B->UpdateData();
    A->UpdateData();
   
    return A;
}

void RankTree::Remove(std::shared_ptr<Player> player) {
    if (player->level == 0) {
        this->levelZero->level->playersTable.Remove(player->playerId);
        this->levelZero->histInLevel[player->score - 1]--;
        this->levelZero->playersInLevel--;
        return;
    }

    std::shared_ptr<Node> curr = root;
    while(curr != nullptr)
    {
        curr->hist[player->score - 1]--;
        curr->numOfPlayers--;
        curr->sumOfLevels -= player->level;

        if(curr->levelId == player->level)
            break;
        
        if(curr->levelId < player->level)
            curr = curr->right;
        else
            curr = curr->left;
    }
    if (!curr)
        return;
    
    curr->histInLevel[player->score - 1]--;
    curr->playersInLevel--;
    curr->level->playersTable.Remove(player->playerId);
    
    if (curr->playersInLevel == 0)
        this->RemoveLevel(curr->levelId);
}

bool RankTree::Find(int levelId)
{
    if(this->GetLevel(levelId) == nullptr)
        return false;
    return true;
}

void RankTree::RemoveLevel(int levelId)
{
    if(!this->Find(levelId))
        return;
    
    this->root = RankTree::RemoveAux(this->root, levelId);
    this->size--;
}


std::shared_ptr<Node> RankTree::RemoveAux(std::shared_ptr<Node>& root, int levelId)
{
    if (root == nullptr)
        return nullptr;
    
    if (root->levelId < levelId)
        root->right = RankTree::RemoveAux(root->right, levelId);
    else if (root->levelId > levelId)
        root->left = RankTree::RemoveAux(root->left, levelId);
    
    else {
        if (!root->left && !root->right)
            root = nullptr;
        
        else if (root->left && root->right) {
            std::shared_ptr<Node> leaf = RankTree::FindNextInOrder(root);
            root->levelId = leaf->levelId;
            root->level = leaf->level;
            root->right = RankTree::RemoveAux(root->right, leaf->levelId);
        }

        else if (root->left)
            root = root->left;
        else
            root = root->right;   
    }
    
    if (root == nullptr)
        return nullptr;
    
    root->height = IntMax(Node::GetHeight(root->left), Node::GetHeight(root->right)) + 1;
    root->UpdateData();
    int balanceFactor = Node::GetBF(root);

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

std::shared_ptr<Level> RankTree::GetLevel(int levelId)
{
    std::shared_ptr<Node> curr = root;
    while(curr != nullptr)
    {
        if(curr->levelId == levelId)
            return curr->level;
        
        if(curr->levelId < levelId)
            curr = curr->right;
        else
            curr = curr->left;
    }
    return nullptr;
}

void RankTree::Insert(std::shared_ptr<Player> player) {
    if (player->level != 0 && !this->Find(player->level)) {
        std::shared_ptr<Level> level = std::shared_ptr<Level>(new Level(player->level));
        this->InsertLevel(player->level, level);
    }
    
    if (player->level == 0) {
        this->levelZero->level->playersTable.Insert(player->playerId, player);
        this->levelZero->histInLevel[player->score - 1]++;
        this->levelZero->playersInLevel++;
        return;
    }

    std::shared_ptr<Node> curr = root;
    while(curr != nullptr)
    {
        curr->hist[player->score - 1]++;
        curr->numOfPlayers++;
        curr->sumOfLevels += player->level;

        if(curr->levelId == player->level)
            break;
        
        if(curr->levelId < player->level)
            curr = curr->right;
        else
            curr = curr->left;
    }
    if (!curr)
        return;
    
    curr->histInLevel[player->score - 1]++;
    curr->playersInLevel++;
    curr->level->playersTable.Insert(player->playerId, player);
}


void RankTree::InsertLevel(int levelId, std::shared_ptr<Level>& level)
{
    if(this->Find(levelId))
        return;
    
    std::shared_ptr<Level> copyData = std::shared_ptr<Level>(level); 
    std::shared_ptr<Node> toInsert = std::shared_ptr<Node>(new Node(levelId,copyData));

    this->root = RankTree::InsertAux(this->root, toInsert);
    this->size++;
}


std::shared_ptr<Node> RankTree::InsertAux(std::shared_ptr<Node> root, 
                                                            std::shared_ptr<Node> toInsert)
{
    if (root == nullptr) 
        return toInsert;
    
    if (root->levelId < toInsert->levelId)
        root->right = RankTree::InsertAux(root->right, toInsert);
    else
        root->left = RankTree::InsertAux(root->left, toInsert);
    
    root->height = IntMax(Node::GetHeight(root->left), Node::GetHeight(root->right)) + 1;
    root->UpdateData();
    int balanceFactor = Node::GetBF(root);

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

bool RankTree::IsEmpty() {
    if (this->size == 0 && this->levelZero->playersInLevel == 0)
        return true;
    return false;
}

int RankTree::GetMaxLevel() //assumes that the ranktree isn't empty
{
    std::shared_ptr<Node> curr = this->root;
    if (curr == nullptr)
        return 0;

    while(curr->right != nullptr)
        curr = curr->right;

    return curr->levelId;
}


int RankTree::GetMinLevel() //assumes that the ranktree isn't empty
{   
    if (this->levelZero->numOfPlayers > 0)
        return 0;

    std::shared_ptr<Node> curr = this->root;
    while(curr->left != nullptr)
        curr = curr->left;

    return curr->levelId;
}

void RankTree::GetPercentOfPlayersWithScoreInBounds(int score, int lowerLevel, int higherLevel, double * players) {
    int playersInBoundHigh = 0, playersInBoundWithScoreHigh = 0;
    RankTree::GetNumOfPlayersWithScoreUnderLevel(this->root, score, higherLevel, &playersInBoundHigh, &playersInBoundWithScoreHigh);
    int playersInBoundLow = 0, playersInBoundWithScoreLow = 0;
    RankTree::GetNumOfPlayersWithScoreUnderLevel(this->root, score, lowerLevel - 1, &playersInBoundLow, &playersInBoundWithScoreLow);
    int playersInBoundzero = 0, playersInBoundWithScorezero = 0;
    if (0 >= lowerLevel && 0 <= higherLevel) {
        playersInBoundzero = this->levelZero->playersInLevel;
        playersInBoundWithScorezero = this->levelZero->histInLevel[score - 1];
    }
    *players = (double)((playersInBoundWithScorezero + playersInBoundWithScoreHigh - playersInBoundWithScoreLow) / (playersInBoundzero + playersInBoundHigh - playersInBoundLow));
}

void RankTree::GetNumOfPlayersWithScoreUnderLevel(std::shared_ptr<Node> root, int score, int level, int* total, int* playerWithScore) {
    while (root != nullptr) {
        if (root->levelId > level)
            root = root->left;
        else {
            *total += root->numOfPlayers - Node::GetNumPlayers(root->right);
            *playerWithScore += root->hist[score - 1] - Node::GetHistInScore(root->right, score);
            if (root->levelId == level)
                return;
            root = root->right;
        }   
    }
}

void RankTree::AverageHighestPlayerLevelByGroup(int m, double * avgLevel) {
    int sumOfLevels = 0;
    RankTree::SumOfLevels(this->root, m, &sumOfLevels);
    *avgLevel = (double)(sumOfLevels / m);
}

int RankTree::IntMax(int a, int b)
{
    return a > b ? a : b;
}

int RankTree::IntMin(int a, int b)
{
    return a > b ? b : a;
}

void RankTree::SumOfLevels(std::shared_ptr<Node> root, int m, int * sum) {
    if (m == 0)
        return;

    while (root != nullptr) {
        if (Node::GetNumPlayers(root->right) >= m)
            root = root->right;
        else {
            m -= Node::GetNumPlayers(root->right);
            *sum += Node::GetSumLevels(root->right) + RankTree::IntMin(m, root->playersInLevel) * root->levelId;
            RankTree::SumOfLevels(root->left, m - RankTree::IntMin(m, root->playersInLevel), sum);
        }
    }
}

void RankTree::getPlayersBound(int score, int m, int * LowerBoundPlayers,int * HigherBoundPlayers) {
    *HigherBoundPlayers = 0;
    *LowerBoundPlayers = 0;
    m = RankTree::getPlayersBoundHelper(this->root, score, m, LowerBoundPlayers, HigherBoundPlayers);
    if (m > 0) {
        *HigherBoundPlayers += RankTree::IntMin(m, this->levelZero->histInLevel[score - 1]);
        *LowerBoundPlayers += RankTree::IntMin(0, (m - (this->levelZero->playersInLevel - this->levelZero->histInLevel[score - 1])));
    }
}

int RankTree::getPlayersBoundHelper(std::shared_ptr<Node> root, int score, int m, int * LowerBoundPlayers,int * HigherBoundPlayers) {
    if (m == 0)
        return 0;

    while (root != nullptr) {
        if (Node::GetNumPlayers(root->right) >= m)
            root = root->right;
        else {
            m -= Node::GetNumPlayers(root->right);
            *HigherBoundPlayers += Node::GetHistInScore(root->right, score) + RankTree::IntMin(m, root->histInLevel[score - 1]);
            *LowerBoundPlayers += Node::GetHistInScore(root->right, score);
            if (m < root->playersInLevel)
                *LowerBoundPlayers += RankTree::IntMin(0, (m - (root->playersInLevel - root->histInLevel[score - 1])));
            else
                *LowerBoundPlayers = *HigherBoundPlayers;
            m -= RankTree::IntMin(m, root->playersInLevel);
            m = RankTree::getPlayersBoundHelper(root->left, score, m, LowerBoundPlayers, HigherBoundPlayers);
        }
    }
    return m;
} 
     
void RankTree::ComputeExtra(std::shared_ptr<Node> root, int groupId) {
    if (root == nullptr)
        return;
    
    RankTree::ComputeExtra(root->left, groupId);
    RankTree::ComputeExtra(root->right, groupId);
    root->UpdateSelfData(groupId);
    root->UpdateData();
    return;
}

std::shared_ptr<RankTree> RankTree::Merge(const RankTree& tree1, const RankTree& tree2, int groupId)
{
    Helper* helper = RankTree::MergeToArr(tree1, tree2);
    int newSize = RankTree::RemoveDuplicates(helper, tree1.size + tree2.size);
    std::shared_ptr<RankTree> mergedRankTree = RankTree::ArrToRankTree(helper, newSize, tree1.size + tree2.size);
    RankTree::ComputeExtra(mergedRankTree->root, groupId);
    delete[] helper->key;
    delete[] helper->data;
    HelperDestroy(helper);
    mergedRankTree->levelZero->level->playersTable = *HashTable<int, Player>::Merge(tree1.levelZero->level->playersTable, tree2.levelZero->level->playersTable);
    mergedRankTree->levelZero->playersInLevel = tree1.levelZero->playersInLevel + tree2.levelZero->playersInLevel;
    for (int i = 0; i < 200; i++)
        mergedRankTree->levelZero->histInLevel[i] = tree1.levelZero->histInLevel[i] + tree2.levelZero->histInLevel[i];

    return mergedRankTree; 
}

Helper* RankTree::MergeToArr(const RankTree& tree1, const RankTree& tree2)
{
    std::shared_ptr<Level> *dataArr1 = new std::shared_ptr<Level>[tree1.size];
    std::shared_ptr<Level> *dataArr2 = new std::shared_ptr<Level>[tree2.size];
    std::shared_ptr<Level> *dataMergedArr = new std::shared_ptr<Level>[tree1.size + tree2.size];
    int * keyArr1 = new int[tree1.size];
    int * keyArr2 = new int[tree2.size];
    int * keyMergedArr = new int[tree1.size + tree2.size];
    int i = 0;
    SaveInOrder(tree1.root, keyArr1, dataArr1, &i);
    i = 0;
    SaveInOrder(tree2.root, keyArr2, dataArr2, &i);
    MergeArr(keyArr1, keyArr2, dataArr1, dataArr2, keyMergedArr, dataMergedArr, tree1.size, tree2.size);
    delete[] dataArr1;
    delete[] dataArr2;
    delete[] keyArr1;
    delete[] keyArr2;
    Helper* helper = HelperCreate(keyMergedArr, dataMergedArr);
    return helper;  
}


void RankTree::SaveInOrder(const std::shared_ptr<Node> root, int * keyArr, std::shared_ptr<Level> *dataArr, int *i)
{
    if (root == nullptr)
        return;
    RankTree::SaveInOrder(root->left, keyArr, dataArr, i);
    keyArr[*i] = root->levelId;
    dataArr[*i] = root->level;
    (*i)++;
    RankTree::SaveInOrder(root->right, keyArr, dataArr, i);
}


void RankTree::MergeArr(int * keyArr1, int * keyArr2, std::shared_ptr<Level> *dataArr1,
                                std::shared_ptr<Level> *dataArr2, int * keyMergedArr, std::shared_ptr<Level> *dataMergedArr,
                                int size1, int size2)
{
    while (size1 > 0 && size2 > 0) {
        if (*keyArr1 < *keyArr2) {
            *keyMergedArr = *keyArr1;
            *dataMergedArr = *dataArr1;
            keyArr1++;
            dataArr1++;
            size1--;
        }
        else {
            *keyMergedArr = *keyArr2;
            *dataMergedArr = *dataArr2;
            keyArr2++;
            dataArr2++;
            size2--;
        }
        keyMergedArr++;
        dataMergedArr++;
    }

    while (size1 > 0) {    
        *keyMergedArr = *keyArr1;
        *dataMergedArr = *dataArr1;
        keyArr1++;
        dataArr1++;
        keyMergedArr++;
        dataMergedArr++;
        size1--;
    }
    
    while (size2 > 0) {    
        *keyMergedArr = *keyArr2;
        *dataMergedArr = *dataArr2;
        keyArr2++;
        dataArr2++;
        keyMergedArr++;
        dataMergedArr++;
        size2--;
    }
}

int RankTree::RemoveDuplicates(Helper *helper, int n)
{
    int newsize = n;
    for (int i = 0; i < n - 1; i++) {
        if (helper->data[i] == nullptr)
            continue;
        if (helper->key[i] == helper->key[i + 1]) {
            helper->data[i]->playersTable = *HashTable<int, Player>::Merge(helper->data[i]->playersTable, helper->data[i + 1]->playersTable);
            helper->data[i + 1] = nullptr;
            newsize--;
        }
    }
    return newsize;
}

std::shared_ptr<RankTree> RankTree::BuildEmptyTree(int n)
{
    int completeHeight = RankTree::FindHeightOfComplete(n + 1);
    int completeSize = RankTree::ComputeSizeOfComplete(completeHeight);
    std::shared_ptr<RankTree> res(new RankTree(RankTree::BuildCompleteTree(completeHeight), completeSize));
    int removeCount = completeSize - n;
    RankTree::removeRightLeafs(res->root, &removeCount, completeHeight, 0);

    res->size = n;
    return res;
}

std::shared_ptr<RankTree> RankTree::ArrToRankTree(Helper* helper, int size, int oldSize)
{
    std::shared_ptr<RankTree> mergedRankTree = RankTree::BuildEmptyTree(size);
    int i = 0;
    RankTree::InsertElements(mergedRankTree->root, helper->key, helper->data, oldSize, &i);

    return mergedRankTree;   
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


std::shared_ptr<Node> RankTree::BuildCompleteTree(int h)
{
    if (h == -1)
        return nullptr;
    std::shared_ptr<Node> root = std::shared_ptr<Node>(new Node(h));
    root->right = RankTree::BuildCompleteTree(h - 1);
    root->left = RankTree::BuildCompleteTree(h - 1);
    return root;
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


void RankTree::InsertElements(std::shared_ptr<Node> root, int * keyArr, std::shared_ptr<Level> *dataArr, int size, int *i)
{
    if(root == nullptr)
        return;

    RankTree::InsertElements(root->left, keyArr, dataArr, size, i);
    
    while (*i < size && dataArr[*i] == nullptr) {
        (*i)++;
    }
    root->level = dataArr[*i];
    root->levelId = keyArr[*i];
    (*i)++;
    RankTree::InsertElements(root->right, keyArr, dataArr, size, i);

    return;
}

