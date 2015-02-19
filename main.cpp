#include "main.h"

using namespace std;

int main(int argc, char **argv) {
    std::ios_base::sync_with_stdio(false);
    
    ostringstream os;
    string file;
    char queueType = 'e';
    
    int opt = 0, index = 0, N = 0;
    while((opt = getopt_long (argc, argv, "c:hs:", longOpts, &index)) != -1) {
        switch(opt) {
            case 'c':
                if (!strcmp(optarg, "BINARY")) {
                    queueType = 'a';
                }
                else if (!strcmp(optarg, "POOR_MAN")) {
                    queueType = 'b';
                }
                else if (!strcmp(optarg, "SORTED")) {
                    queueType = 'c';
                }
                else if (!strcmp(optarg, "PAIRING")) {
                    queueType = 'd';
                }
                break;
            case 's':
                stats = 1;
                N = atoi(optarg);
                break;
            case 'h':
                cout << "Choose the container type scheme by adding the flag"
                << " '-c', followed by either BINARY POOR_MAN SORTED or PAIRING"
                << ".\nTo print the stats, add the flag '-s', followed by an int"
                << "iger N.\nThe last arg should by the filename of the map."
                << endl;
                exit(0);
            case '?':
                cerr << "I didn't recognize one of your flags" << endl;
                exit(1);
        }
    }
    if (!(optind < argc)) {
        cerr << "No minefile\n";
        exit(1);
    }
    else {
        file = argv[optind];
    }
    
    char inMode;
    int startRow, startCol, seed, max_rubble, tnt;
    string trash;
    int value;
    coord here;
    trapped = 1;
    
    switch (queueType) {
        case 'a': {
            queue = new heap_priority_queue<coord*, coord_comp>;
            TNTqueue = new heap_priority_queue<coord*, coord_comp>;
            break;
        }
        case 'b': {
            queue = new poorman_priority_queue<coord*, coord_comp>;
            TNTqueue = new poorman_priority_queue<coord*, coord_comp>;
            break;
        }
        case 'c': {
            queue = new sorted_priority_queue<coord*, coord_comp>;
            TNTqueue = new sorted_priority_queue<coord*, coord_comp>;
            break;
        }
        case 'd': {
            queue = new pairing_priority_queue<coord*, coord_comp>;
            TNTqueue = new pairing_priority_queue<coord*, coord_comp>;
            break;
        }
        default: {
            queue = new heap_priority_queue<coord*, coord_comp>;
            TNTqueue = new heap_priority_queue<coord*, coord_comp>;
            cerr << "DECLARE THE CONTAINER TYPE UGH" << endl;
            delete queue;
            delete TNTqueue;
            exit(1);
            break;
        }
    }
    
    ifstream in(file);
    
    in >> inMode >> trash >> mapSize >> trash >> startRow >> startCol;
    
    vector<coord> map;
    
    // Create map for map mode
    if (inMode == 'M') {
        for (int i = 0; i < mapSize * mapSize; i++) {
            in >> value;
            here.setVal(value);
            here.setSpot(i);
            map.resize(map.size() + 1, here);
        }
    }
    // Create map for random mode
    else {
        in >> trash >> seed >> trash >> max_rubble >> trash >> tnt;
        P2random::seed_mt(seed);
        for (int i = 0; i < mapSize * mapSize; i++) {
            here.setVal(P2random::generate_tile(max_rubble, tnt));
            here.setSpot(i);
            map.resize(map.size() + 1, here);
        }
    }
    
    // initialize starting location
    here.setSpot((startRow * mapSize) + startCol);
    here.setVal(map[here.spot()].val());
    
    queue->push(&here);
    
    while (trapped) {
        
        here = *queue->top();
        queue->pop();
        
        if (here.val() == 0)  {
            trapped = addNeighbors(map, here.spot());
            map[here.spot()].markBeenTo();
        }
        
        else {
            
            map[here.spot()].setVal(0);
            if (here.val() > 0) {
                tilesCleared++;
                rubbleCleared += here.val();
            }
            if (stats) {
                first.push_back(here);
                easy.push(here);
            }
            
            if (here.val() < 0) {
                TNTBOOM(map, here.spot());
                detonate(map);
                queue->fix();
            }
            trapped = addNeighbors(map, here.spot());
            map[here.spot()].markBeenTo();
            
        }
        
        if ((here.r() == 0) || (here.r() == mapSize) ||
            (here.c() == 0) || (here.c() == mapSize)) {
            
            trapped = 0;
            break;
        }
        
        
        
    }
    
    os << "Cleared " << tilesCleared << " tiles containing " << rubbleCleared
    << " rubble and escaped." << endl;
    
    if (stats) printStats(os, N);
    cout << os.str();
    
    in.close();
    delete queue;
    delete TNTqueue;
    return 0;
}

bool addNeighbors(vector<coord> &map, int here) {
    
    // ------------------------WEST--------------------------
    if ((here % mapSize) != 0) {
        if (!map.at(here - 1).beenTo()) {
            map.at(here - 1).markBeenTo();
            queue->push(&map.at(here - 1));
        }
    }
    else {
        return false;
    }
    
    // ------------------------NORTH--------------------------
    if ((here / mapSize) > 0) {
        if (!map.at(here - mapSize).beenTo()) {
            map.at(here - mapSize).markBeenTo();
            queue->push(&map.at(here - mapSize));
        }
    }
    else {
        return false;
    }
    
    // ------------------------EAST--------------------------
    if (((here + 1) % mapSize) != 0) {
        if (!map.at(here + 1).beenTo()) {
            map.at(here + 1).markBeenTo();
            queue->push(&map.at(here + 1));
        }
    }
    else {
        return false;
    }
    
    // ------------------------SOUTH--------------------------
    if ((here + mapSize) < (mapSize * mapSize)) {
        if (!map.at(here + mapSize).beenTo()) {
            map.at(here + mapSize).markBeenTo();
            queue->push(&map.at(here + mapSize));
        }
    }
    else {
        return false;
    }
    
    return true;
    
}


void TNTBOOM(vector<coord> &map, int here) {
    coord TNT;
    
    // WEST
    if ((here % mapSize) != 0) {
        if (map.at(here - 1).val() && !(map.at(here - 1).TNT())) {
            map.at(here - 1).markTNT();
            TNTqueue->push(&map.at(here - 1));
        }
    }
    // NORTH
    if (here / mapSize > 0) {
        if (map.at(here - mapSize).val() && !(map.at(here - mapSize).TNT())) {
            map.at(here - mapSize).markTNT();
            TNTqueue->push(&map.at(here - mapSize));
        }
        
    }
    // EAST
    if (((here + 1) % mapSize) != 0) {
        if (map.at(here + 1).val() && !(map.at(here + 1).TNT())) {
            map.at(here + 1).markTNT();
            TNTqueue->push(&map.at(here + 1));
        }
    }
    // SOUTH
    if ((here + mapSize) < (mapSize * mapSize)) {
        if (map.at(here + mapSize).val() && !(map.at(here + mapSize).TNT())) {
            map.at(here + mapSize).markTNT();
            TNTqueue->push(&map.at(here + mapSize));
        }
    }
}

void detonate(vector<coord>& map) {
    
    coord here;
    
    while (!TNTqueue->empty()) {
        
        here = *TNTqueue->top();
        TNTqueue->pop();
        if (here.val() < 0) {
            map.at(here.spot()).setVal(0);
            TNTBOOM(map, here.spot());
        }
        map.at(here.spot()).setVal(0);
        map.at(here.spot()).unsee();
        if (here.val() > 0) {
            tilesCleared++;
            rubbleCleared += here.val();
        }
        if (stats) {
            first.push_back(here);
            easy.push(here);
        }
    }
}

void printStats(ostringstream& os, int N){
    int j = 0;
    coord here;
    list<coord> hard;
    os << "First tiles cleared:\n";
    for (auto i: first) {
        if (j < N) {
            os << i << "\n";
        }
        j++;
    }
    j = 0;
    os << "Last tiles cleared:\n";
    for (list<coord>::const_reverse_iterator iter(first.rbegin()),
                iter_end(first.rend()); iter != iter_end; ++iter) {
        if (j < N) {
            os << *iter << "\n";
        }
        j++;
    }
    j = 0;
    os << "Easiest tiles cleared:\n";
    while (!easy.empty()) {
        
        here = easy.top();
        easy.pop();
        if (j < N) {
            os << here << "\n";
        }
        hard.push_front(here);
        j++;
    }
    j = 0;
    os << "Hardest tiles cleared:\n";
    for (auto i: hard) {
        if (j < N) {
            os << i << "\n";
        }
        else break;
        j++;
    }
}