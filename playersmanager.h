#ifndef PLAYERS_MANAGER_H_
#define PLAYERS_MANAGER_H_

#include <memory>
#include "ranktree.h"
#include "UF.h"
#include "group.h"
#include "level.h"
#include "player.h"
#include "helper.h"
#include "node.h"

typedef enum {
    PM_SUCCESS = 0,
    PM_FAILURE = -1,
    PM_ALLOCATION_ERROR = -2,
    PM_INVALID_INPUT = -3
} PMStatusType;


class PlayersManager {
    private:
        static int RemoveDuplicates(Helper *helper, int n);
        
    public:
        int k;
        int scale;
        RankTree allLevelsTree;
        HashTable<int, Player> allPlayers;
        UF<Group> groups;
        
        
        PlayersManager(int k, int scale);
        ~PlayersManager() = default;
        PMStatusType AddPlayer(int playerId, int groupId, int score, int levelId = 0);
        PMStatusType RemovePlayer(int playerId);
        PMStatusType MergeGroups(int groupId1, int groupId2);
        PMStatusType IncreasePlayerIDLevel(int playerId, int levelIncrease);
        PMStatusType ChangePlayerIDScore(int playerId, int newScore);
        PMStatusType GetPercentOfPlayersWithScoreInBounds(int groupID, int score, int lowerLevel, int higherLevel, double * players);
        PMStatusType AverageHighestPlayerLevelByGroup(int groupID, int m, double * avgLevel);
        PMStatusType GetPlayersBound(int groupID, int score, int m, int * LowerBoundPlayers, int * HigherBoundPlayers);
};

#endif /* PLAYERS_MANAGER_H_ */