#ifndef GROUP_H_
#define GROUP_H_


#include <memory>
#include "level.h"
#include "ranktree.h"
#include "player.h"
#include "hashtable.h"
#include "node.h"

class Group {
    private:
    public:
        int groupId;
        std::shared_ptr<RankTree> levelsTree;
        int numOfPlayers;

        Group(const int groupId) : groupId(groupId), levelsTree(new RankTree()), numOfPlayers(0) {}
        Group() = default;
        ~Group() = default;     
};

#endif /* GROUP_H */