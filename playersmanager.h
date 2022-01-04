#ifndef PLAYERS_MANAGER_H_
#define PLAYERS_MANAGER_H_

#include <memory>
#include "ranktree.h"
#include "UF.h"
#include "group.h"
#include "level.h"
#include "player.h"
#include "map.h"
#include "node.h"

typedef enum {
    PM_SUCCESS = 0,
    PM_FAILURE = -1,
    PM_ALLOCATION_ERROR = -2,
    PM_INVALID_INPUT = -3
} PMStatusType;


class PlayersManager {
    private:
        static void AddPlayerGroupsTree(Group& group, const int playerId, std::shared_ptr<Player>& newPlayer);
        static void AddPlayerAllLevelsTree(BST<int, Level>& allLevelsTree, const int playerId, std::shared_ptr<Player>& newPlayer);
        static void UpdateMaxLevel(MaxLevel& maxLevel,const int level,const int playerId);
        static void RemovePlayerGroupsTree(Group& group, const Player& playerToRemove);
        static void RemovePlayerNonEmptyGroupsTree(BST<int, Group>& nonEmptyGroupsTree,
                                                                   const int groupId, const int numOfPlayers);
        static void RemovePlayerallLevelsTree(BST<int, Level>& allLevelsTree, const Player& playerToRemove);
        static void SearchAndUpdateMaxLevel(BST<int, Level>& levelsTree, MaxLevel& maxlevel);
        static void InOrderByPlayer(std::shared_ptr<Node<int, Player>> player, int *players, int *i);
        static void InOrderByLevel(std::shared_ptr<Node<int, Level>> level, int **players, int *numOfPlayers, int *i);
        static void GetAllPlayersByLevelAux(BST<int, Level>& levelsTree, int **players, int *numOfPlayers);
        static void InOrderNElements(std::shared_ptr<Node<int, Group>> root, int numOfGroups, int *players, int *i);
        static int RemoveDuplicates(Map *map, int n);
        static void UpdateLevels(std::shared_ptr<Node<int, Level>> tree, std::shared_ptr<Group> replacement, int replacementId);
        static void UpdatePlayers(std::shared_ptr<Node<int, Player>> tree, std::shared_ptr<Group> replacement, int replacementId);
        
    public:
        RankTree allLevelsTree;
        UF<Group> groups;
        HashTable<int, Player> allPlayers;
        std::shared_ptr<Node> levelZero;
        int k;
        int scale;

        PlayersManager(int k, int scale);
        ~PlayersManager() = default;
        PMStatusType AddPlayer(int playerId, int groupId, int score);
        PMStatusType RemovePlayer(int playerId);
        PMStatusType MergeGroups(int groupId1, int groupId2);
        PMStatusType IncreasePlayerIDLevel(int playerId, int levelIncrease);
        PMStatusType ChangePlayerIDScore(int playerId, int newScore);
        PMStatusType GetPercentOfPlayersWithScoreInBounds(int groupID, int score, int lowerLevel, int higherLevel, double * players);
        PMStatusType AverageHighestPlayerLevelByGroup(int groupID, int m, double * avgLevel);
        PMStatusType getPlayersBound(int groupID, int score, int m, int * LowerBoundPlayers, int * HigherBoundPlayers);
};

#endif /* PLAYERS_MANAGER_H_ */