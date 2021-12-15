#ifndef CACHE_H
#define CACHE_H
#include "globals.h"
#include "nodeDataManager.h"
//#include "cache_wipe.h"

using namespace std;

enum CacheType {
    CacheTrie, CacheHash, CacheHashCover
};

enum WipeType {
    All, Subnodes, Recall
};

/*This struct is used to represent a node in the tree search algorithm*/
struct Node {
    NodeData *data; // data is the information kept by a node during the tree search
    bool is_used;
    Node() {
        data = nullptr;
        is_used = false;
    }
    virtual ~Node() { delete data; }
};


/*This class represents the cache structure saved during the tree search algorithm*/
class Cache {
public:
    Cache(Depth maxdepth, WipeType wipe_type, Size maxcachesize);
    virtual ~Cache() {}

    Node *root; // the root node of the tree
    Size cachesize; // the size (number of nodes) of the cache
    Size maxcachesize; // the maximum size allowed by the cache system
    Depth maxdepth;
    WipeType wipe_type;

//    virtual pair<Node*, bool> insert ( Array<Item> itemset ) = 0; // add a node to the tree
    virtual pair<Node*, bool> insert ( Itemset &itemset ) {return {nullptr, false}; } // add a node to the tree
    virtual pair<Node*, bool> insert ( NodeDataManager*, int depth = 0, bool rootnode = false ) { return {nullptr, false}; }

    virtual Node* get ( const Itemset &itemset ){return nullptr;} // get a node in the tree based on its corresponding itemset
    virtual Node *get ( NodeDataManager*, int depth) { return nullptr; }

    virtual int getCacheSize(){return cachesize;}

    virtual void wipe(){}

    virtual void updateParents(Node* best, Node* left, Node* right, Itemset = Itemset()){}
//    virtual void updateParents(Node* best, Node* left, Node* right){}
};

#endif
