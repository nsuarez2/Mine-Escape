#ifndef main_h
#define main_h

#include <iostream>
#include <fstream>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <list>
#include <iomanip>
#include <string.h>
#include <sstream>
#include "poorman_priority_queue.h"
#include "sorted_priority_queue.h"
#include "heap_priority_queue.h"
#include "pairing_priority_queue.h"
#include "eecs281_priority_queue.h"
#include "P2random.h"

using namespace std;

static int mapSize;
bool trapped;

class coord {
private:
    int value;
    int location;
    bool been = 0;
    bool tnt = 0;
public:
    int val() const { return this->value;}
    int r() const { return this->location / mapSize;}
    int c() const { return this->location % mapSize;}
    int spot() const { return this->location;}
    bool beenTo() const { return this->been;}
    bool TNT() const { return this->tnt;}
    void markBeenTo() { been = 1;}
    void unsee() { been = 0;}
    void markTNT() { tnt = 1;}
    void setVal(int v) { value = v;}
    void setSpot(int s) { location = s;}
    void operator=(const coord &other) {
        value = other.val();
        location = other.spot();
        been = other.beenTo();
        tnt = other.TNT();
    }
};
ostream& operator<<(ostream& os, const coord& obj) {
    if (obj.val() < 0) {
        os << "TNT";
    }
    else {
        os << obj.val();
    }
    return os << " at [" << obj.r() << "," << obj.c() << "]";
}

struct mapCoord {
    int val;
    bool beenTo;
};

struct coord_comp {
    bool operator() (const coord *a, const coord *b) const {
        if (a->val() > b->val()) {
            return true;
        }
        else if (a->val() < b->val()) {
            return false;
        }
        else {
            if (a->c() > b->c()) {
                return true;
            }
            else if (a->c() < b->c()) {
                return false;
            }
            else {
                if (a->r() > b->r()) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
    }
    bool operator() (const coord a, const coord b) const {
        if (a.val() > b.val()) {
            return true;
        }
        else if (a.val() < b.val()) {
            return false;
        }
        else {
            if (a.c() > b.c()) {
                return true;
            }
            else if (a.c() < b.c()) {
                return false;
            }
            else {
                if (a.r() > b.r()) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
    }
};

struct option longOpts[] = {
    {"container", required_argument, NULL, 'c'},
    {"help", no_argument, NULL, 'h'},
    {"stats", required_argument, NULL, 'o'},
};

// Adds the immegiate neighbors of the current spot, if not seen before
bool addNeighbors(vector<coord> &, int);

// Called when on a TNT tile, adds neighbors to TNTqueue
void TNTBOOM(vector<coord> &, int);

// Clears all tiles from the TNTqueue
void detonate(vector<coord> &);

// Outputs all stats, as specified from the spec
void printStats(ostringstream&, int);


eecs281_priority_queue<coord*, coord_comp> *queue, *TNTqueue;
pairing_priority_queue<coord, coord_comp> easy;
list<coord> first;
bool stats = 0;
long long tilesCleared = 0, rubbleCleared = 0;


#endif