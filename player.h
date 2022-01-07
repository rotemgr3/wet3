#ifndef PLAYER_H_
#define PLAYER_H_


class Player {
    public:
        int playerId;
        int groupId;
        int score;
        int level;
        

        Player(int playerId, int groupId, int score) : playerId(playerId), groupId(groupId), score(score), level(0) {}
        Player(int playerId, int groupId, int score, int level) : playerId(playerId), groupId(groupId), score(score), level(level) {}
        Player(const Player& player) = default;
        ~Player() = default;
};

#endif /* PLAYER_H */