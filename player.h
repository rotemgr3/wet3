#ifndef PLAYER_H_
#define PLAYER_H_


class Player {
    public:
        int playerId;
        int groupId;
        int level;
        Player(const int playerId, const int groupId, const int level) : playerId(playerId), groupId(groupId), level(level) {}
        Player(const Player& player) = default;
        ~Player() = default;
};

#endif /* PLAYER_H */