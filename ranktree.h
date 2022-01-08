#ifndef RankTree_H_
#define RankTree_H_

#include <memory>
#include "node.h"
#include "helper.h"
#include "level.h"
#include <stdbool.h>


class RankTree {
    private:
        void InsertLevel(int levelId, std::shared_ptr<Level>& level);
        void RemoveLevel(int levelId);
        std::shared_ptr<Level> GetLevel(int levelId) const;
        static std::shared_ptr<Node> InsertAux(std::shared_ptr<Node> root, 
                                                            std::shared_ptr<Node> toInsert); 
        static std::shared_ptr<Node> LLRotation(std::shared_ptr<Node>& root);
        static std::shared_ptr<Node> LRRotation(std::shared_ptr<Node>& root);
        static std::shared_ptr<Node> RLRotation(std::shared_ptr<Node>& root);
        static std::shared_ptr<Node> RRRotation(std::shared_ptr<Node>& root);
        static std::shared_ptr<Node> RemoveAux(std::shared_ptr<Node>& root, int key);
        static std::shared_ptr<Node> FindNextInOrder(std::shared_ptr<Node> root);
        static void SaveInOrder(const std::shared_ptr<Node> root, int * keyArr, std::shared_ptr<Level> *dataArr, int *i);
        static void MergeArr(int * keyArr1, int * keyArr2, std::shared_ptr<Level> *dataArr1,
                                std::shared_ptr<Level> *dataArr2, int * keyMergedArr, std::shared_ptr<Level> *dataMergedArr,
                                int size1, int size2);
        static void InsertElements(std::shared_ptr<Node> root, int * keyArr,
                                   std::shared_ptr<Level> *dataArr, int size, int *i);
        static std::shared_ptr<RankTree> BuildEmptyTree(int n);
        static void removeRightLeafs(std::shared_ptr<Node> root, int *removecount, int leafPathLen, int currPathLen);
        static int FindHeightOfComplete(int num);
        static std::shared_ptr<Node> BuildCompleteTree(int h);
        static int ComputeSizeOfComplete(int height);
        static int IntMax(int a, int b);
        static int IntMin(int a, int b);
        static int RemoveDuplicates(Helper *helper, int n);
        static void ComputeExtra(std::shared_ptr<Node> root, int groupId);
        static void GetNumOfPlayersWithScoreUnderLevel(std::shared_ptr<Node> root, int score, int level, int* total, int* playerWithScore);
        static void SumOfLevels(std::shared_ptr<Node> root, int m, int * sum);
        static Helper* MergeToArr(const RankTree& tree1, const RankTree& tree2);
        static int getPlayersBoundHelper(std::shared_ptr<Node> root, int score, int m, int * LowerBoundPlayers,int * HigherBoundPlayers);
        static std::shared_ptr<RankTree> ArrToRankTree(Helper* helper, int size, int oldSize);
        static void testmerge1(std::shared_ptr<Node> root, const RankTree& tree1, const RankTree& tree2);
        static void testmerge2(std::shared_ptr<Node> root, std::shared_ptr<RankTree> merged);


    public:
        std::shared_ptr<Node> root;
        std::shared_ptr<Node> levelZero;
        int size;

        RankTree();
        RankTree(std::shared_ptr<Node> root, int size);
        ~RankTree() = default;
        bool Find(int levelId) const;
        void Remove(std::shared_ptr<Player> player);
        void Insert(std::shared_ptr<Player> player);
        static std::shared_ptr<RankTree> Merge(const RankTree& tree1, const RankTree& tree2, int groupId);
        int GetMaxLevel();
        int GetMinLevel();
        bool IsEmpty();
        void GetPercentOfPlayersWithScoreInBounds(int score, int lowerLevel, int higherLevel, double * players);
        void AverageHighestPlayerLevelByGroup(int m, double * avgLevel);
        void getPlayersBound(int score, int m, int * LowerBoundPlayers,int * HigherBoundPlayers);  
        RankTree& operator=(const RankTree& copy);   
};


#endif /* RankTree_H */