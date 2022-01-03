#ifndef NODE_H_
#define NODE_H_

#include <memory>
#include "level.h"

class Node {
    public:
        int levelId;
        std::shared_ptr<Level> level;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right; 
        int height; 
        int hist[200];
        int histInLevel[200];
        int numOfPlayers;
        int sumOfLevels;
        int playersInLevel;

        Node(int height);
        Node(const int levelId, std::shared_ptr<Level>& level);
        ~Node() = default;
        Node(const Node& node);
        static int GetBF(std::shared_ptr<Node> node);
        static int GetHeight(std::shared_ptr<Node> node);
        static int GetSumLevels(std::shared_ptr<Node> node);
        static int GetNumPlayers(std::shared_ptr<Node> node);
        void UpdateData();
        Node& operator=(const Node& copy);
};

void Node::UpdateData() {
    this->numOfPlayers = Node::GetNumPlayers(this->left) + Node::GetNumPlayers(this->left) + this->playersInLevel + this->playersInLevel;
    this->sumOfLevels = Node::GetSumLevels(this->left) + Node::GetSumLevels(this->left) + this->levelId * this->playersInLevel;
    
    for (int i=0; i < 200; i++) {
        int countRight = 0, countLeft = 0;
        if (this->right)
            countRight = this->right->hist[i];
        if (this->left)
            countLeft = this->left->hist[i];
        this->hist[i] = countRight + countLeft + this->histInLevel[i];
    }
}

Node::Node(int height) : levelId(), level(nullptr), 
            left(nullptr), right(nullptr), height(height), hist({0}), histInLevel({0}),
            numOfPlayers(0), sumOfLevels(0), playersInLevel(0) {

}

Node::Node(const int levelId, std::shared_ptr<Level>& level) : levelId(levelId), level(level), 
            left(nullptr), right(nullptr), height(0), hist({0}), histInLevel({0}),
            numOfPlayers(0), sumOfLevels(0), playersInLevel(0) {
}

Node(const Node& node) : levelId(node.levelId), level(node.level), left(node.left), 
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
    return Node::NodeGetHeight(node->left) - Node::NodeGetHeight(node->right);
}

#endif /* NODE_H */