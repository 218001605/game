#ifndef AGENT_H
#define AGENT_H

#include "board.h"
#include "cell.h"
#include <set>
#include <algorithm>
#include <cmath>
#include <cstdlib>

#ifndef _SLP_CL_EG
#define _SLP_CL_EG
#define C(x) #x
#define CL C(cls)
#endif
#ifdef _WIN32
#define SLP(t) Sleep(t*1000)
#include <windows.h>
#ifdef unix
#define SLP(t) sleep(t)
#include <unistd.h>
#define CL C(clear)
#endif
#endif


struct movement {
    string l;//location
    int n;//number of current player's marbles moving
    int fd;//direction from leading marbles to following marbles
    int md;//direction in which n marbles seek to move
    int d2e;//distance from leading marble to the edge of the board in move direction
    int mtype;//-1 broad-side 0-2 for in-line depending on how many opponent marbles are to be pushed
    bool scoreMove;//opponent marble to be pushed off the board?
    movement(const string&, const int&, const int&, const int&, const int&, const int&, const bool&);
    movement(const movement&);
};
double score(const movement&);
struct moveTNDPriority {//priority based on move types and distance to edge
    inline bool operator()(const movement& a, const movement& b) {
        return score(a) < score(b);
    }
};
class agent {
protected:
    char symbol;
    vector<vector<int> > pmd;
    vector<movement> findMoves(const board&) const;
public:
    virtual ~agent();
    explicit agent(const char&);
    char getSymbol() const { return symbol; }
    virtual movement move(movement const* , board) = 0;
};

class randAgent : public agent {
public:
    explicit randAgent(const char&);
    movement move(movement const* , board) override;
};
class distAgent : public agent {
public:
    explicit distAgent(const char&);
    movement move(movement const* , board) override;
};
class Human : public agent {
public:
    explicit Human(const char&);
    movement move(movement const* , board) override;
};
#endif /* AGENT_H */