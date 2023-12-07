#pragma once
#include <vector>
#include "Product.h"
using namespace std;

class MapDatabase {
    struct Node {
        int key;
        Product value;
        Node* next;

        Node(int k, const Product& val) : key(k), value(val), next(nullptr) {}
    };

    vector<Node*> hashTable;
    int capacity;
    int size;
    const float loadFactor = 0.75;

    int hashFunction(int key);
    void resizeHashtable();

    public:
    MapDatabase(int size);
    void buy(int id, int q, float c);
    bool sell(int id, int q, float c);
    bool printItem(int id);
    void printSalesReport();
    void saveSpreadsheet(string filename);
    bool loadSpreadsheet(string filename);
};

