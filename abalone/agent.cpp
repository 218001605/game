
#include "agent.h"

movement::movement(const string& ll, const int& nn, const int& f, const int& m, const int& d, const int& mt, const bool& sm) :
    l(ll), n(nn), fd(f), md(m), d2e(d), mtype(mt), scoreMove(sm) {}
movement::movement(const movement& copy) : l(copy.l), n(copy.n), fd(copy.fd), md(copy.md), d2e(copy.d2e), mtype(copy.mtype), scoreMove(copy.scoreMove) {}

agent::agent(const char& c) : symbol(c) {
    for (int n = 1;n < 4;n++) {
        for (int fd = 0;fd < 6;fd++) {//all 6 directions
            if (n == 1) {//will create 6 moves for n=1
                vector<int> tv;
                tv.push_back(n);
                tv.push_back(fd);
                tv.push_back(getAdjacentIndex(fd));
                pmd.push_back(tv);
            }
            else {//will create a total of 60 moves for n=2 and n=3
                for (int md = 0;md < 6;md++) {
                    if (md != fd) {
                        vector<int> tv;
                        tv.push_back(n);
                        tv.push_back(fd);
                        tv.push_back(md);
                        pmd.push_back(tv);
                    }
                }
            }
        }
    }
}
vector<movement> agent::findMoves(const board& bd) const {
    vector<movement> mv;
    map<string, cell*> cells = bd.getCells();
    for (auto & it : cells) {
        string l = it.first;
        cell* cp = it.second;
        if (cp != nullptr && cp->getMarble() == symbol) {
            set<int> mdSet;
            for (int md = 0;md < 6;md++) {
                cell* np = cp->getAdjacent(md);
                if (np != nullptr && np->getMarble() != symbol) {
                    mdSet.insert(md);
                }
            }
            for (vector<int> tv : pmd) {
                int md = tv[2];
                if (mdSet.find(md) != mdSet.end()) {
                    int mtype = 0;//default to in-line with no following marbles
                    bool scoreMove = false;
                    if (bd.validateMove(symbol, l, tv[0], tv[1], md, mtype, scoreMove))
                        mv.emplace_back(l, tv[0], tv[1], md, cp->getDistanceToEdge(md), mtype, scoreMove);
                }
            }
        }
    }
    return mv;
}

agent::~agent() = default;

double score(const movement& a) {
    double maxScore = 0;
    if (a.mtype < 0)
        maxScore = 0.6;
    else if (a.mtype > 0)
        maxScore = 0.2;
    else
        maxScore = 0.8;
    if (a.mtype > 0) {
        return maxScore * (1 + (1 - a.d2e) / 9.0);
    }
    else {
        return maxScore * (1 - abs(5 - a.d2e) / 5.0);
    }
}
double scoreB(const movement& a) {
    double maxScore = 0;
    if (a.mtype == 3 && a.scoreMove) {
        maxScore = 20;
        return maxScore;
    }
    else if (a.mtype == 2 && a.scoreMove) {
        maxScore = 15;
        return maxScore;
    }
    return maxScore;
}


randAgent::randAgent(const char& c) :agent(c) {}
movement randAgent::move(const movement* const om, board bd) {
    vector<movement> mv = findMoves(bd);
    if (mv.empty()) {
        throw string("Error! There are no available moves!");
    }
    int i = rand() % mv.size();
    return mv[i];
}
distAgent::distAgent(const char& c) :agent(c) {}

movement distAgent::move(movement const* const om, board bd) {
    vector<movement> mv = findMoves(bd);
    sort(mv.begin(), mv.end(), moveTNDPriority());
    const int bm2c = 3;//consider only these top best moves.
    if (mv.empty()) {
        throw string("Error! There are no available moves!");
    }
    if (mv.size() < bm2c)
        return mv[0];
    else {
        int i = rand() % bm2c;
        return mv[i];
    }
}

int myMap(board bd, string l, int m) {
    map<string, cell*> cellsMap = bd.getCells();
    cell* cp = cellsMap.at(l);
    int edgeDist = cp->getDistanceToEdge(m);
    return edgeDist;
}
Human::Human(const char& c) : agent(c) {}

movement Human::move(movement const* const om, board bd) {
    vector<movement> availableMoves = findMoves(bd);
    if (availableMoves.empty()) {
        throw string("Error! There are no available moves!");
    }
    // Variables for move input
    string marbleLocation;
    int marbleAmount, leadDir, moveDir;
    bool scoreMove, valid;
    int moveType = 0;
    cout << "Enter a move: ";
    cin >> marbleLocation >> marbleAmount >> leadDir >> moveDir;

    // Validate the move
    valid = bd.validateMove(symbol, marbleLocation, marbleAmount, leadDir, moveDir, moveType, scoreMove);
    while (!valid) {
        if (cin.fail()) {
            cout << "Failed to register the input." << endl;
            exit(-1);
        }
        cout << "The move you entered cannot be executed in this board state." << endl;
        cout << "Enter move again: ";
        cin >> marbleLocation >> marbleAmount >> leadDir >> moveDir;
        valid = bd.validateMove(symbol, marbleLocation, marbleAmount, leadDir, moveDir, moveType, scoreMove);
    }
    // Calculate the edge distance value for the move
    int edgeDistVal = myMap(bd, marbleLocation, moveDir);
    // Return the validated movement
    return movement(marbleLocation, marbleAmount, leadDir, moveDir, edgeDistVal, moveType, scoreMove);
}