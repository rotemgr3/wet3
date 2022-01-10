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
        Node(int levelId, std::shared_ptr<Level> level);
        Node(int levelId, Level level);
        ~Node() = default;
        Node(const Node& node);
        static int GetBF(std::shared_ptr<Node> node);
        static int GetHeight(std::shared_ptr<Node> node);
        static int GetSumLevels(std::shared_ptr<Node> node);
        static int GetNumPlayers(std::shared_ptr<Node> node);
        static int GetHistInScore(std::shared_ptr<Node> node, int score);
        void UpdateData();
        void UpdateSelfData(int groupId = -1);
        Node& operator=(const Node& copy);
};

#endif /* NODE_H */