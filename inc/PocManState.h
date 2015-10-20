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
    Default
};

class PocManState {
public:
    // Constructors
    PocManState();
    PocManState(PocManLevel level);
    PocManState(const PocManState &other) = default;

    // Constructor helpers
    void generateLevel(PocManLevel level); // Sets up wall_, pellet_, loc_;

    // Accessors
    std::array<double, 9> getSenses() const;
    bool isTerminal() const;
    std::string toString() const;

    // Modifiers
    int performAction(Action action);

private:
    // Helper functions
    unsigned int indexAfterAction(unsigned int index, Action action) const;
    bool canSeePellet(Action action) const;
    bool canSmellPellet() const;

private:
    static const unsigned int w_ = 28;
    static const unsigned int h_ = 31;
    std::array<bool, w_ * h_> wall_;
    std::array<bool, w_ * h_> pellet_;
    unsigned int nPellets_;
    unsigned int loc_;
    unsigned int smellDistance_ = 3;
};

#endif //CGP_POCMAN_POCMANSTATE_H
