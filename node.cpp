#include "node.h"


void Node::UpdateData() {    
    this->numOfPlayers = Node::GetNumPlayers(this->left) + Node::GetNumPlayers(this->right) + this->playersInLevel;
    this->sumOfLevels = Node::GetSumLevels(this->left) + Node::GetSumLevels(this->right) + this->levelId * this->playersInLevel;
    
    for (int i=0; i < 200; i++) {
        int countRight = 0, countLeft = 0;
        if (this->right)
            countRight = this->right->hist[i];
        if (this->left)
            countLeft = this->left->hist[i];
        this->hist[i] = countRight + countLeft + this->histInLevel[i];
    }
}

void Node::UpdateSelfData(int groupId) { 
    this->playersInLevel = this->level->playersTable.size;
    for (int i = 0; i < 200; i++)
        this->histInLevel[i] = 0;

    for (int i = 0; i < this->level->playersTable.m; i++) {
        std::shared_ptr<ListNode<int, Player>> curr = this->level->playersTable.arr[i];
        while (curr) {
            this->histInLevel[((std::shared_ptr<Player>)(curr->data))->score - 1]++;
            if(groupId != -1)
                ((std::shared_ptr<Player>)(curr->data))->groupId = groupId;
            curr = curr->next;
        }
    }
}   


Node::Node(int height) : levelId(), level(nullptr), 
            left(nullptr), right(nullptr), height(height), hist{0}, histInLevel{0},
            numOfPlayers(0), sumOfLevels(0), playersInLevel(0) {

}

Node::Node(int levelId, std::shared_ptr<Level> level) : levelId(levelId), level(level), 
            left(nullptr), right(nullptr), height(0), hist{0}, histInLevel{0},
            numOfPlayers(0), sumOfLevels(0), playersInLevel(0) {
}

Node::Node(int levelId, Level level) : levelId(levelId), level(new Level(level)), 
            left(nullptr), right(nullptr), height(0), hist{0}, histInLevel{0},
            numOfPlayers(0), sumOfLevels(0), playersInLevel(0) {
}


Node::Node(const Node& node) : levelId(node.levelId), level(node.level), left(node.left), 
        right(node.right), height(node.height) { 
    for (int i = 0; i < 200; i++) {
        hist[i] = node.hist[i]; 
        histInLevel[i] = node.histInLevel[i];
    }
    this->numOfPlayers = node.numOfPlayers;
    this->sumOfLevels = node.sumOfLevels;
    this->playersInLevel = node.playersInLevel;
}

Node& Node::operator=(const Node& copy)
{
    this->levelId = copy.levelId;
    this->level = copy.level;
    this->left = copy.left;
    this->right = copy.right;
    for (int i = 0; i < 200; i++) {
        hist[i] = copy.hist[i]; 
        histInLevel[i] = copy.histInLevel[i];
    }
    this->numOfPlayers = copy.numOfPlayers;
    this->sumOfLevels = copy.sumOfLevels;
    this->playersInLevel = copy.playersInLevel;
    
    return *this;
}

int Node::GetHeight(std::shared_ptr<Node> node)
{
    if(node == nullptr)
        return -1;
    return node->height;
}

int Node::GetSumLevels(std::shared_ptr<Node> node)
{
    if(node == nullptr)
        return 0;
    return node->sumOfLevels;
}

int Node::GetNumPlayers(std::shared_ptr<Node> node)
{
    if(node == nullptr)
        return 0;
    return node->numOfPlayers;
}

int Node::GetBF(std::shared_ptr<Node> node)
{
    return Node::GetHeight(node->left) - Node::GetHeight(node->right);
}

int Node::GetHistInScore(std::shared_ptr<Node> node, int score)
{
    if(node == nullptr)
        return 0;
    return node->hist[score - 1];
}