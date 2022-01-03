#ifndef GROUP_H_
#define GROUP_H_


#include <memory>
#include "level.h"
#include "ranktree.h"
#include "player.h"
#include "hashtable.h"

class Group {
    private:
    public:
        int groupId;
        RankTree levelsTree;
        int numOfPlayers;
        HashTable<int, Player> levelZero;

        Group(const int groupId) : groupId(groupId), levelsTree(), numOfPlayers(0), levelZero() {}
        Group() = default;
        ~Group() = default;     
};

#endif /* GROUP_H */