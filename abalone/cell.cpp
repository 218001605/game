#include "cell.h"
#include <iostream>
int getAdjacentIndex(const int& i) {
    if (i < 3)
        return i + 3;
    else
        return i - 3;
}
char cell::getMarble()const {
    return marble;
}
void cell::setMarble(const char& m) {
    if (m == 'O' || m == '@' || m == '+') {
        marble = m;
    }
}

cell* cell::getAdjacent(const int& i)const {
    return adjacent.at(i);
}
void cell::setAdjacent(cell* a, const int& i) {
    if (adjacent.at(i) == nullptr) {
        adjacent.at(i) = a;
        a->setAdjacent(this, getAdjacentIndex(i));
    }
}
string cell::getLocation() const {
    return location;
}

bool cell::validateLocation(const string& l) {
    bool validPosition = false;
    // Check if the string has the correct length.
    if (l.size() == 2) {
        // Check if the first character is an uppercase letter from A to I.
        if (l[0] >= 'A' && l[0] <= 'M') {
            // Check if the second and third characters are digits from 5 to 14.
            if (l[1] >= '1' && l[1] <= '9') {
                validPosition = true;
            }
        }
    }
    return validPosition;
}
void cell::setLocation(const string& l) {
    if (validateLocation(l))
        location = l;
    else {
        throw "Invalid Location!";
    }
}

cell::cell() {
    for (int i = 0;i < 6;i++) {
        adjacent.push_back(nullptr);
        distanceToEdge.push_back(0);
    }
}

void cell::setDistanceToEdge(const int& i) {
    if (i > 5 || i < 0)
        return;//invalid direction
    if (distanceToEdge[i] == 0) {
        cell* cp = this;
        cell* np = adjacent[i];//neighbour in the same direction
        cell* nq = adjacent[getAdjacentIndex(i)];//neighbour in the opposite direction
        if (np != nullptr && np->getDistanceToEdge(i) != 0) {
            distanceToEdge[i] = np->getDistanceToEdge(i) + 1;
        }
        else if (nq != nullptr && nq->getDistanceToEdge(i) != 0) {
            distanceToEdge[i] = nq->getDistanceToEdge(i) - 1;
        }
        else
            while (cp != nullptr) {
                cp = cp->adjacent[i];
                distanceToEdge[i]++;
            }
    }
}

int cell::getDistanceToEdge(const int& i) const {
    return distanceToEdge.at(i);
}