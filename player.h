#ifndef PLAYER_H_
#define PLAYER_H_


class Player {
    public:
        int playerId;
        int groupId;
        int level;
        int score;
        Player(int playerId, int groupId, int score) : playerId(playerId), groupId(groupId), score(score), level(0) {}
        Player(const Player& player) = default;
        ~Player() = default;
};

#endif /* PLAYER_H */