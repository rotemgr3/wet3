#include "playersmanager.h"
#include <exception>

PlayersManager::PlayersManager(int k, int scale) : k(k), scale(scale),
    allLevelsTree(),
    allPlayers(),
    groups(k) {
}

PMStatusType PlayersManager::MergeGroups(int groupId1, int groupId2) {
    if (groupId1 > this->k || groupId1 <= 0 || groupId2 > this->k || groupId2 <= 0)
        return PM_INVALID_INPUT;
    
    try {
        std::shared_ptr<Group> group1 = this->groups.Find(groupId1);
        std::shared_ptr<Group> group2 = this->groups.Find(groupId2);
        this->groups.Union(group1->groupId, group2->groupId);
        group1->levelsTree = std::shared_ptr<RankTree>(RankTree::Merge(*group1->levelsTree, *group2->levelsTree, this->groups.Find(groupId1)->groupId));
        group1->numOfPlayers = group1->numOfPlayers + group2->numOfPlayers;
        group2->levelsTree = std::shared_ptr<RankTree>(group1->levelsTree);
        group2->numOfPlayers = group1->numOfPlayers;
    } catch (std::bad_alloc& error) {
        return PM_ALLOCATION_ERROR;
    }

    return PM_SUCCESS;
}


PMStatusType PlayersManager::AddPlayer(int playerId, int groupId, int score, int levelId) {
    if (groupId > this->k || groupId <= 0 || playerId <= 0 || score > this->scale || score <= 0)
        return PM_INVALID_INPUT;
    
    if (this->allPlayers.IfExists(playerId))
        return PM_FAILURE;

    try {
        std::shared_ptr<Player> toInsert = std::shared_ptr<Player>(new Player(playerId, groupId, score, levelId));
        this->allPlayers.Insert(playerId, toInsert);
        this->allLevelsTree.Insert(toInsert);
        this->groups.Find(groupId)->levelsTree->Insert(toInsert);

    } catch (std::bad_alloc& error) {
        return PM_ALLOCATION_ERROR;
    }
    
    return PM_SUCCESS;
}

PMStatusType PlayersManager::RemovePlayer(int playerId) {
    if (playerId <= 0)
        return PM_INVALID_INPUT;
    
    std::shared_ptr<Player> toRemove = this->allPlayers.Get(playerId);
    if (!toRemove)
        return PM_FAILURE;

    
    this->allPlayers.Remove(playerId);
    this->allLevelsTree.Remove(toRemove);
    this->groups.Find(toRemove->groupId)->levelsTree->Remove(toRemove);
    this->groups.Find(toRemove->groupId)->numOfPlayers--;


    return PM_SUCCESS;
}

PMStatusType PlayersManager::IncreasePlayerIDLevel(int playerId, int levelIncrease) {
    if (playerId <= 0 || levelIncrease <= 0)
        return PM_INVALID_INPUT;

    std::shared_ptr<Player> toRemove = this->allPlayers.Get(playerId);
    if (!toRemove)
        return PM_FAILURE;
    
    try {
        std::shared_ptr<Player> newPlayer = std::shared_ptr<Player>(new Player(*toRemove));
        newPlayer->level += levelIncrease;

        this->RemovePlayer(playerId);
        this->AddPlayer(newPlayer->playerId, newPlayer->groupId, newPlayer->score, newPlayer->level);
    } catch (std::bad_alloc& error) {
        return PM_ALLOCATION_ERROR;
    }
    
    return PM_SUCCESS;
}

PMStatusType PlayersManager::ChangePlayerIDScore(int playerId, int newScore) {
    if (playerId <= 0 || newScore <= 0 || newScore > this->scale)
        return PM_INVALID_INPUT;
    
    std::shared_ptr<Player> player = this->allPlayers.Get(playerId);
    if (!player)
        return PM_FAILURE;
    
    try {
        std::shared_ptr<Player> newPlayer = std::shared_ptr<Player>(new Player(*player));
        newPlayer->score = newScore;

        this->RemovePlayer(playerId);
        this->AddPlayer(newPlayer->playerId, newPlayer->groupId, newPlayer->score, newPlayer->level);
    } catch (std::bad_alloc& error) {
        return PM_ALLOCATION_ERROR;
    }
    
    return PM_SUCCESS;
}

PMStatusType PlayersManager::GetPercentOfPlayersWithScoreInBounds(int groupID, int score, int lowerLevel, int higherLevel, double * players) {
    if (groupID < 0 || groupID > this->k || players == nullptr)
        return PM_INVALID_INPUT;
    
    if (groupID == 0) {
        if (this->allLevelsTree.IsEmpty())
            return PM_FAILURE;
        if (this->allLevelsTree.GetMaxLevel() < lowerLevel || this->allLevelsTree.GetMinLevel() > higherLevel) 
                return PM_FAILURE;
        
        this->allLevelsTree.GetPercentOfPlayersWithScoreInBounds(score, lowerLevel, higherLevel, players);
    } else {
        std::shared_ptr<Group> group = this->groups.Find(groupID);
        if (group->levelsTree->IsEmpty())
            return PM_FAILURE;
        if (group->levelsTree->GetMaxLevel() < lowerLevel || group->levelsTree->GetMinLevel() > higherLevel)
                return PM_FAILURE;

        group->levelsTree->GetPercentOfPlayersWithScoreInBounds(score, lowerLevel, higherLevel, players);
    }
    return PM_SUCCESS;
}

PMStatusType PlayersManager::AverageHighestPlayerLevelByGroup(int groupID, int m, double * avgLevel) {
    if (groupID < 0 || groupID > this->k || avgLevel == nullptr || m <= 0)
        return PM_INVALID_INPUT;
    
    if (groupID == 0) {
        if (this->allPlayers.size < m)
            return PM_FAILURE;

        this->allLevelsTree.AverageHighestPlayerLevelByGroup(m, avgLevel);
    } else {
        std::shared_ptr<Group> group = this->groups.Find(groupID);
        if (group->numOfPlayers < m)
            return PM_FAILURE;

        group->levelsTree->AverageHighestPlayerLevelByGroup(m, avgLevel);
    }
    
    return PM_SUCCESS;
}

PMStatusType PlayersManager::GetPlayersBound(int groupID, int score, int m, int * LowerBoundPlayers, int * HigherBoundPlayers) {
    if (groupID < 0 || groupID > this->k || LowerBoundPlayers == nullptr 
        || HigherBoundPlayers == nullptr || m < 0 || score <= 0 || score > this->scale)
        return PM_INVALID_INPUT;
    
    if (groupID == 0) {
        if (this->allPlayers.size < m)
            return PM_FAILURE;

        this->allLevelsTree.getPlayersBound(score, m, LowerBoundPlayers, HigherBoundPlayers);
    } else {
        std::shared_ptr<Group> group = this->groups.Find(groupID);
        if (group->numOfPlayers < m)
            return PM_FAILURE;

        group->levelsTree->getPlayersBound(score, m, LowerBoundPlayers, HigherBoundPlayers);
    }
    
    return PM_SUCCESS;
}
