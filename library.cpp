#include "library.h"
#include "playersmanager.h"

void* Init(int k, int scale)
{   
    if (k <= 0 || scale <= 0 || scale > 200)
        return NULL;
    try { 
        PlayersManager *PM = new PlayersManager(k, scale);
        return (void *)PM;     
    } catch (std::bad_alloc& err) {
        return NULL;
    }
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->MergeGroups(GroupID1, GroupID2);
    return res;
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->AddPlayer(PlayerID, GroupID, score); 
    return res;
}

StatusType RemovePlayer(void *DS, int PlayerID)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->RemovePlayer(PlayerID); 
    return res;
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->IncreasePlayerIDLevel(PlayerID, LevelIncrease); 
    return res;
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->ChangePlayerIDScore(PlayerID, NewScore); 
    return res;   
}

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,
                                            double * players)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->GetPercentOfPlayersWithScoreInBounds(GroupID, score, lowerLevel, higherLevel, players); 
    return res;  
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * level)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->AverageHighestPlayerLevelByGroup(GroupID, m, level); 
    return res; 
}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m, int * LowerBoundPlayers, int * HigherBoundPlayers)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->GetPlayersBound(GroupID, score, m, LowerBoundPlayers, HigherBoundPlayers); 
    return res; 
}

void Quit(void** DS)
{
    if (DS == nullptr) return;
    delete (*(PlayersManager **)DS);
    *DS = nullptr;
}



