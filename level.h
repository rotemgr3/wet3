#ifndef LEVEL_H_
#define LEVEL_H_

#include "player.h"
#include "hashtable.h"

class Level {
    private:
    public:
        int levelId;
        HashTable<int, Player> playersTable;
        
        Level(const int level) : levelId(level), playersTable() {}
        ~Level() = default;
        bool operator<(const Level& level) { return this->levelId < level.levelId; }

        
};

#endif /* LEVEL_H */