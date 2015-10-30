//
// Created by bedder on 15/10/2015.
//

#ifndef CGP_POCMAN_POCMANSTATE_H
#define CGP_POCMAN_POCMANSTATE_H

#include <array>
#include <sstream>
#include <stdexcept>

typedef enum {
    North = 0, East = 1, South = 2, West = 3
} Action;
enum class PocManLevel {
    Default,
    Level1,
    Level2
};

static const int PLT_REWARD = 10;   // Reward for eating a pellet
static const int CLR_REWARD = 100;  // Reward for clearing the stage
static const int STP_PUNISH = -1;   // Punishment for stepping
static const int INV_PUNISH = -20;  // Punishment for walking into a wall
static const int DTH_PUNISH = -100; // Punishment for dying

class PocManState {
struct Ghost {
  unsigned int loc_;
  Action heading_;
};

public:
    // Constructors
    PocManState();
    PocManState(PocManLevel level);
    PocManState(const PocManState &other) = default;

    // Constructor helpers
    void generateLevel(PocManLevel level); // Sets up wall_, pellet_, loc_;

    // Accessors
    std::array<double, 14> getSenses() const;
    unsigned int nPellets() const { return nPellets_; }
    bool isTerminal() const;
    std::string toString() const;

    // Modifiers
    int performAction(Action action);

private:
    // Helper functions
    unsigned int indexAfterAction(unsigned int index, Action action) const;
    inline bool canSeeGhost(Action action) const;
    inline bool canHearGhost() const;
    inline bool canSmellPellet() const;

private:
    static const unsigned int w_ = 28;
    static const unsigned int h_ = 31;

    bool dead_ = false;
    std::array<bool, w_ * h_> wall_;
    std::array<bool, w_ * h_> pellet_;
    unsigned int nPellets_;
    unsigned int hearingDistance_ = 2;
    unsigned int smellDistance_ = 1;
    Ghost pocman_;
    std::array<Ghost, 2> ghosts_;
};

#endif //CGP_POCMAN_POCMANSTATE_H
