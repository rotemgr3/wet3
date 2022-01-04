#include "playersmanager.h"
#include <exception>

PlayersManager::PlayersManager(int k, int scale) : k(k), scale(scale),
    allLevelsTree(),
    allPlayers(),
    levelZero(allLevelsTree.root),
    groups(k) {
}

PMStatusType PlayersManager::MergeGroups(int groupId1, int groupId2) {
    if (groupId1 > this->k || groupId1 <= 0 || groupId2 > this->k || groupId2 <= 0)
        return PM_INVALID_INPUT;
    
    try {
        std::shared_ptr<Group> group1 = this->groups.Find(groupId1);
        std::shared_ptr<Group> group2 = this->groups.Find(groupId2);
        this->groups.Union(group1->groupId, group2->groupId);
        std::shared_ptr<Group> root = this->groups.Find(groupId1);
        root->levelsTree = RankTree::Merge(group1->levelsTree, group2->levelsTree);
        root->levelZeroInGroup = root->levelsTree->GetZeroLevel(0);
        root->numOfPlayers = group1->numOfPlayers + group2->numOfPlayers;
    } catch (std::bad_alloc& error) {
        return PM_ALLOCATION_ERROR;
    }

    return PM_SUCCESS;
}


PMStatusType PlayersManager::AddPlayer(int playerId, int groupId, int score) {
    if (groupId > this->k || groupId <= 0 || playerId <= 0 || score > this->scale || score <= 0)
        return PM_INVALID_INPUT;
    
    if (this->allPlayers.IfExists(playerId))
        return PM_FAILURE;

    try {
        std::shared_ptr<Player> toInsert = std::shared_ptr<Player>(new Player(playerId, groupId, score));
        this->allPlayers.Insert(playerId, toInsert);
        this->levelZero->level->playersTable.Insert(playerId, toInsert);
        this->groups.Find(groupId)->levelZeroInGroup->level->playersTable.Insert(playerId, toInsert);

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
    this->allLevelsTree.Remove(playerId);
    this->groups.Find(toRemove->groupId)->levelsTree->Remove(playerId);

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

        this->allPlayers.Insert(playerId, newPlayer);
        this->allLevelsTree.Insert(playerId, newPlayer);
        this->groups.Find(newPlayer->groupId)->levelsTree->Insert(playerId, newPlayer);
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
        
        if (newPlayer->level == 0)
            this->AddPlayer(newPlayer->playerId, newPlayer->groupId, newPlayer->score);
        else {
            this->allPlayers.Insert(playerId, newPlayer);
            this->allLevelsTree.Insert(playerId, newPlayer);
            this->groups.Find(newPlayer->groupId)->levelsTree->Insert(playerId, newPlayer);
        }
    } catch (std::bad_alloc& error) {
        return PM_ALLOCATION_ERROR;
    }
    
    return PM_SUCCESS;
}

PMStatusType PlayersManager::GetPercentOfPlayersWithScoreInBounds(int groupID, int score, int lowerLevel, int higherLevel, double * players) {
    if (groupID < 0 || groupID > this->k || players == nullptr)
        return PM_INVALID_INPUT;
    
    if (groupID == 0) {
        ///////check on empty tree!!!!!
        if (this->allLevelsTree.GetMax().levelId < lowerLevel || this->allLevelsTree.GetMin().levelId > higherLevel) 
                return PM_FAILURE;
        
        this->allLevelsTree.GetPercentOfPlayersWithScoreInBounds(score, lowerLevel, higherLevel, players);
    } else {
        std::shared_ptr<Group> group = this->groups.Find(groupID);
        ///////check on empty tree!!!!!
        if (group->levelsTree.GetMax().levelId < lowerLevel || group->levelsTree.GetMin().levelId > higherLevel)
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

PMStatusType PlayersManager::getPlayersBound(int groupID, int score, int m, int * LowerBoundPlayers, int * HigherBoundPlayers) {
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




PMStatusType PlayersManager::AddGroup(const int groupId)
{
    if (groupId <= 0)
        return PM_INVALID_INPUT;
    if (this->groupsTree.Find(groupId))
        return PM_FAILURE;
    
    try {
        std::shared_ptr<Group> newGroup = std::make_shared<Group>(Group(groupId));
        this->groupsTree.Insert(groupId, newGroup);
    } catch (std::bad_alloc& error) {
        return PM_ALLOCATION_ERROR;
    }

    return PM_SUCCESS;
}

void PlayersManager::AddPlayerGroupsTree(Group& group, const int playerId, std::shared_ptr<Player>& newPlayer)
{
    if (!group.levelsTree.Find(newPlayer->level)) {
            std::shared_ptr<Level> newLevel = std::make_shared<Level>(Level(newPlayer->level));
            group.levelsTree.Insert(newPlayer->level, newLevel);
    }
    group.levelsTree.Get(newPlayer->level)->playersTree.Insert(playerId, newPlayer);
    group.numOfPlayers++;

    PlayersManager::UpdateMaxLevel(group.maxLevel, newPlayer->level, newPlayer->playerId);
        
}

void PlayersManager::AddPlayerAllLevelsTree(BST<int, Level>& allLevelsTree, const int playerId, std::shared_ptr<Player>& newPlayer)
{
    if (!allLevelsTree.Find(newPlayer->level)){
            std::shared_ptr<Level> newLevel = std::make_shared<Level>(Level(newPlayer->level));
            allLevelsTree.Insert(newPlayer->level, newLevel);
    }
    allLevelsTree.Get(newPlayer->level)->playersTree.Insert(playerId, newPlayer);
}

void PlayersManager::UpdateMaxLevel(MaxLevel& maxLevel,const int level,const int playerId)
{
     if (maxLevel.level < level) {
            maxLevel.level = level;
            maxLevel.playerId = playerId;
        }
    else if (maxLevel.level == level && maxLevel.playerId > playerId)
            maxLevel.playerId = playerId;
}

PMStatusType PlayersManager::AddPlayer(const int playerId, const int groupId, const int level, std::shared_ptr<Group> groupPtr)
{
    if (playerId <= 0 || groupId <= 0 || level < 0)
        return PM_INVALID_INPUT;

    if (!(this->groupsTree.Find(groupId)) || this->allPlayersTree.Find(playerId))
        return PM_FAILURE;
    
    try {
        std::shared_ptr<Player> newPlayer = std::make_shared<Player>(Player(playerId, groupId, level));
        if (groupPtr == nullptr)
            groupPtr = this->groupsTree.Get(groupId);
        newPlayer->groupPtr = groupPtr;
        
        this->allPlayersTree.Insert(playerId, newPlayer);

        PlayersManager::UpdateMaxLevel(this->maxLevel, level, playerId);

        PlayersManager::AddPlayerGroupsTree(*groupPtr, playerId, newPlayer);

        PlayersManager::AddPlayerAllLevelsTree(this->allLevelsTree, playerId, newPlayer);

        this->nonEmptyGroupsTree.Insert(groupId, groupPtr);

    } catch (std::bad_alloc& error) {
        return PM_ALLOCATION_ERROR;
    }
    
    return PM_SUCCESS;
}


void PlayersManager::RemovePlayerGroupsTree(Group& group, const Player& playerToRemove)
{
    std::shared_ptr<Level> playerLevel = group.levelsTree.Get(playerToRemove.level);
    playerLevel->playersTree.Remove(playerToRemove.playerId);
    if (playerLevel->playersTree.size == 0)
        group.levelsTree.Remove(playerLevel->levelId);
    group.numOfPlayers--;
}

void PlayersManager::RemovePlayerNonEmptyGroupsTree(BST<int, Group>& nonEmptyGroupsTree, const int groupId, const int numOfPlayers)
{
    if (numOfPlayers == 0)
        nonEmptyGroupsTree.Remove(groupId);
}

void PlayersManager::RemovePlayerallLevelsTree(BST<int, Level>& allLevelsTree, const Player& playerToRemove)
{
    std::shared_ptr<Level> level = allLevelsTree.Get(playerToRemove.level);
    level->playersTree.Remove(playerToRemove.playerId);
    if (level->playersTree.size == 0) {
        allLevelsTree.Remove(level->levelId);
    }

}

void PlayersManager::SearchAndUpdateMaxLevel(BST<int, Level>& levelsTree, MaxLevel& maxlevel)
{
    if (levelsTree.size == 0) {
        maxlevel.level = -1;
        maxlevel.playerId = -1;
    }
    else {
        Level maxLevel = levelsTree.GetMax();
        int newMaxLevel = maxLevel.levelId;
        int newMaxPlayerId = maxLevel.playersTree.GetMin().playerId;
        maxlevel.level = newMaxLevel;
        maxlevel.playerId = newMaxPlayerId;
    }
}

PMStatusType PlayersManager::RemovePlayer(const int playerId)
{
    if (playerId <= 0)
        return PM_INVALID_INPUT;

    std::shared_ptr<Player> playerToRemovePtr = this->allPlayersTree.Get(playerId);
    if (playerToRemovePtr == nullptr)
        return PM_FAILURE;

    int playerToRemoveId = playerToRemovePtr->playerId;
    this->allPlayersTree.Remove(playerToRemoveId);
    
    std::shared_ptr<Group> group = playerToRemovePtr->groupPtr.lock();
    PlayersManager::RemovePlayerGroupsTree(*group, *playerToRemovePtr);
    PlayersManager::RemovePlayerNonEmptyGroupsTree(this->nonEmptyGroupsTree, playerToRemovePtr->groupId, group->numOfPlayers);
    PlayersManager::RemovePlayerallLevelsTree(this->allLevelsTree, *playerToRemovePtr);

    PlayersManager::SearchAndUpdateMaxLevel(this->allLevelsTree, this->maxLevel);
    PlayersManager::SearchAndUpdateMaxLevel(group->levelsTree, group->maxLevel);
    
    return PM_SUCCESS;
}

PMStatusType PlayersManager::ReplaceGroup(const int groupId, const int replacementId)
{
    if (groupId <= 0 || replacementId <= 0 || groupId == replacementId)
        return PM_INVALID_INPUT;

    Map* map = nullptr;
    Group *group = nullptr;
    try {    
        group = this->groupsTree.Get(groupId).get();
        std::shared_ptr<Group> replacement = this->groupsTree.Get(replacementId);
        if (group == nullptr || replacement == nullptr)
            return PM_FAILURE;
    
        if (group->numOfPlayers == 0) {
            this->groupsTree.Remove(groupId);
            return PM_SUCCESS;
        }
        map = BST<int, Level>::MergeToArr(group->levelsTree, replacement->levelsTree);
        int newSize = PlayersManager::RemoveDuplicates(map, group->levelsTree.size + replacement->levelsTree.size);
        BST<int, Level> merged = BST<int, Level>::ArrToBST(map, newSize, group->levelsTree.size + replacement->levelsTree.size);
        PlayersManager::UpdateLevels(merged.root, replacement, replacementId);

        replacement->numOfPlayers += group->numOfPlayers;
        replacement->levelsTree.root = merged.root;
        replacement->levelsTree.size = merged.size;
        PlayersManager::UpdateMaxLevel(replacement->maxLevel, group->maxLevel.level,  group->maxLevel.playerId);

        this->nonEmptyGroupsTree.Insert(replacementId, replacement);
        this->nonEmptyGroupsTree.Remove(groupId);
        this->groupsTree.Remove(groupId);
        MapDestroy(map);


    } catch(std::bad_alloc& err) {
        MapDestroy(map);
        return PM_ALLOCATION_ERROR;
    }
    return PM_SUCCESS;
}

void PlayersManager::UpdateLevels(std::shared_ptr<Node<int, Level>> root, std::shared_ptr<Group> replacement, int replacementId)
{
    if(root == nullptr)
        return;
    PlayersManager::UpdateLevels(root->left, replacement, replacementId);
    PlayersManager::UpdatePlayers(root->data->playersTree.root, replacement, replacementId);
    PlayersManager::UpdateLevels(root->right, replacement, replacementId);

}

void PlayersManager::UpdatePlayers(std::shared_ptr<Node<int, Player>> root, std::shared_ptr<Group> replacement, int replacementId)
{
     if(root == nullptr)
        return;
    PlayersManager::UpdatePlayers(root->left, replacement, replacementId);
    root->data->groupId = replacementId;
    root->data->groupPtr = replacement;
    PlayersManager::UpdatePlayers(root->right, replacement, replacementId);  
}

int PlayersManager::RemoveDuplicates(Map *map, int n)
{
    int newsize = n;
    std::shared_ptr<int> *keyArr = (std::shared_ptr<int> *)(map->key);
    std::shared_ptr<Level> *dataArr = (std::shared_ptr<Level> *)(map->data);
    for (int i = 0; i < n - 1; i++) {
        if (dataArr[i] == nullptr)
            continue;
        if (*(keyArr[i]) == *(keyArr[i + 1])) {
            dataArr[i]->playersTree = BST<int, Player>::Merge(dataArr[i]->playersTree, dataArr[i + 1]->playersTree);
            dataArr[i + 1] = nullptr;
            newsize--;
        }
    }
    return newsize;
}


PMStatusType PlayersManager::IncreaseLevel(const int playerId, const int levelIncrease)
{
    if (playerId <= 0 || levelIncrease <= 0)
        return PM_INVALID_INPUT;
        
    std::shared_ptr<Player> playerToIncreasePtr = this->allPlayersTree.Get(playerId);
    if (playerToIncreasePtr == nullptr)
        return PM_FAILURE;

    Player newPlayer = Player(*playerToIncreasePtr);
    newPlayer.level += levelIncrease;
    
    PlayersManager::RemovePlayer(playerToIncreasePtr->playerId);
    PMStatusType insertRes =  PlayersManager::AddPlayer(newPlayer.playerId, newPlayer.groupId, newPlayer.level, newPlayer.groupPtr.lock());
    if (insertRes == PM_ALLOCATION_ERROR)   
        return PM_ALLOCATION_ERROR;
    
    
    return PM_SUCCESS;
}

PMStatusType PlayersManager::GetHighestLevel(const int groupId, int* playerId)
{
    if (groupId == 0 || playerId == nullptr)
        return PM_INVALID_INPUT;
        
    if (groupId < 0) {
        *playerId = this->maxLevel.playerId;
        return PM_SUCCESS;
    }
    
    std::shared_ptr<Group> group = this->groupsTree.Get(groupId);
    if (group == nullptr)
        return PM_FAILURE;

    *playerId = group->maxLevel.playerId;
    return PM_SUCCESS;
}

void PlayersManager::InOrderByLevel(std::shared_ptr<Node<int, Level>> level, int **players, int *numOfPlayers, int *i)
{
    if (level == nullptr)
        return;

    PlayersManager::InOrderByLevel(level->right, players, numOfPlayers, i);
    
    int j = *i;
    PlayersManager::InOrderByPlayer(level->data->playersTree.root, *players, &j);
    int right = *i + level->data->playersTree.size - 1;
    (*i) = right + 1;
    PlayersManager::InOrderByLevel(level->left, players, numOfPlayers, i);
    return;
}

void PlayersManager::InOrderByPlayer(std::shared_ptr<Node<int, Player>> player, int *players, int *i)
{
    if (player == nullptr)
        return;

    PlayersManager::InOrderByPlayer(player->left, players, i);
    players[*i] = player->data->playerId;
    (*i)++;
    PlayersManager::InOrderByPlayer(player->right, players, i);
    return;
}

void PlayersManager::GetAllPlayersByLevelAux(BST<int, Level>& levelsTree, int **players, int *numOfPlayers)
{
    int i = 0;
    PlayersManager::InOrderByLevel(levelsTree.root, players, numOfPlayers, &i);
}

PMStatusType PlayersManager::GetAllPlayersByLevel(const int groupId, int **players, int *numOfPlayers)
{
    if (groupId == 0 || players == nullptr || numOfPlayers == nullptr)
        return PM_INVALID_INPUT;
    
    if (groupId < 0) {
        try { 
            *numOfPlayers = this->allPlayersTree.size;
            if(*numOfPlayers == 0){
                *players = nullptr;    
                return PM_SUCCESS;
            }
            *players = (int *)malloc(sizeof(int) * (*numOfPlayers));
            if (*players == nullptr)
                 return PM_ALLOCATION_ERROR;
            PlayersManager::GetAllPlayersByLevelAux(this->allLevelsTree, players, numOfPlayers);
        } catch (std::bad_alloc& error) {
            return PM_ALLOCATION_ERROR;
        }
        return PM_SUCCESS;
    }  
        
    std::shared_ptr<Group> group = this->groupsTree.Get(groupId);
    if (group == nullptr)
        return PM_FAILURE;

    try { 
        *numOfPlayers = group->numOfPlayers;
        if(*numOfPlayers == 0){
            *players = nullptr;    
            return PM_SUCCESS;
        }
        *players = (int *)malloc(sizeof(int) * (*numOfPlayers));
        if (*players == nullptr)
            return PM_ALLOCATION_ERROR;
        PlayersManager::GetAllPlayersByLevelAux(group->levelsTree, players, numOfPlayers);
    } catch (std::bad_alloc& error) {
        return PM_ALLOCATION_ERROR;
    }
    return PM_SUCCESS; 
}

void PlayersManager::InOrderNElements(std::shared_ptr<Node<int, Group>> root, int numOfGroups, int *players, int *i)
{
    if (root == nullptr)
        return;

    PlayersManager::InOrderNElements(root->left, numOfGroups, players, i);

    if (numOfGroups == *i)
        return;
    players[*i] = root->data->maxLevel.playerId;
    (*i)++;
    PlayersManager::InOrderNElements(root->right, numOfGroups, players, i);

    return;
}

PMStatusType PlayersManager::GetGroupsHighestLevel(int numOfGroups, int **players)
{
    if (numOfGroups < 1 || players == nullptr)
        return PM_INVALID_INPUT;
    
    if (numOfGroups > this->nonEmptyGroupsTree.size)
        return PM_FAILURE;

    try {
        *players = (int *)malloc(sizeof(int) * (numOfGroups)); 
        if (*players == nullptr)
            return PM_ALLOCATION_ERROR;
        int i = 0;
        PlayersManager::InOrderNElements(this->nonEmptyGroupsTree.root, numOfGroups, *players, &i);
    } catch (std::bad_alloc& error) {
        return PM_ALLOCATION_ERROR;
    }
    return PM_SUCCESS; 
}