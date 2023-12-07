#include <iostream>
#include <vector>
#include <utility>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cfloat>
#include "MapDatabase.h"

using namespace std;

MapDatabase::MapDatabase(int size) : capacity(size), size(0) {
    hashTable.resize(capacity, nullptr);
}

int MapDatabase::hashFunction(int key) {
    return key % capacity;
}

void MapDatabase::resizeHashtable() {
    capacity *= 2;
    vector<Node*> newHashtable(capacity, nullptr);

    for (auto oldNode : hashTable) {
        Node* currentNode = oldNode;
        while (currentNode) {
            Node* temp = currentNode->next;
            int index = hashFunction(currentNode->key);

            currentNode->next = newHashtable[index];
            newHashtable[index] = currentNode;

            currentNode = temp;
        }
    }

    //___________________ CHECK IF WORKS
    hashTable = newHashtable;
}

void MapDatabase::buy(int id, int q, float c) {
    int index = hashFunction(id);

    //Checks if product exists
    if (!hashTable[index]) {
        hashTable[index] = new Node(id, Product(id, q, c));
    }
    else {
        //Check for collision in linked list, and traverse if so
        Node* currentNode = hashTable[index];
        while (currentNode) {
            //Product exists, call buy function from Product class
            if (currentNode->key == id) {
                currentNode->value.buy(q, c);
                return;
            }

            //Go to next node in list
            if (currentNode->next) {
                currentNode = currentNode->next;
            }
            else {
                break;
            }
        }

        //Add product to end of linked list
        currentNode->next = new Node(id, Product(id, q, c));
    }

    size++;

    //Check if resizing is needed
    float currLoadFactor = size / capacity;
    if (currLoadFactor >= loadFactor) {
        resizeHashtable();
    }
}

bool MapDatabase::sell(int id, int q, float c) {
    int index = hashFunction(id);

    //Check if product does not exist
    if (!hashTable[index]) {
        return false;
    }
    else {
        Node* currentNode = hashTable[index];
        while (currentNode) {
            //Check if product found
            if (currentNode->key == id) {
                //Check if quantity is enough for sell through Product buy function
                if (currentNode->value.sell(q, c)) {
                    return true;
                }
                else {
                    return false;
                }
            }
            //Go to next node in linked list
            currentNode = currentNode->next;
        }
    }
    return false;
}

bool MapDatabase::printItem(int id) {
    int index = hashFunction(id);

    //Check if product does not exist
    if (!hashTable[index]) {
        return false;
    }
    else {
        Node* currentNode = hashTable[index];
        while (currentNode) {
            if (currentNode->key == id) {
                cout << left << setw(30) << "ID: " << currentNode->key << '\n';
                cout << left << setw(30) << "Current Stock: " << currentNode->value.getQuantity() - currentNode->value.getQuantitySold() << '\n';
                cout << left << setw(30) << "Amount Bought: " << currentNode->value.getQuantity() << '\n';
                cout << left << setw(30) << "Amount Sold: " << currentNode->value.getQuantitySold() << '\n';
                cout << left << setw(30) << "Average Cost: " << "$" << fixed << setprecision(2) << currentNode->value.getAverageCost() << '\n';
                cout << left << setw(30) << "Average Sale: " << "$" << fixed << setprecision(2) << currentNode->value.getAverageSale() << '\n';
                cout << left << setw(30) << "Sold Item Profit: " << "$" << fixed << setprecision(2) << currentNode->value.getAverageSale() * currentNode->value.getQuantitySold() - currentNode->value.getAverageCost() * currentNode->value.getQuantity() << '\n';
                cout << left << setw(30) << "Unsold Inventory Cost: " << "$" << fixed <<setprecision(2) << currentNode->value.getAverageCost() * (currentNode->value.getQuantity() - currentNode->value.getQuantitySold()) << '\n' << '\n';

                return true;
            }
            currentNode = currentNode->next;
        }
    }

    //Return false if product was not found
    return false;
}

void MapDatabase::printSalesReport() {
    //Define a vector of pairs to store the collected information for sales report calculation
    vector<pair<string, float>> salesInfo = {
        {"[0] = Costs", 0.0f},
        {"[1] = Revenue", 0.0f},
        {"[2] = How Many items bought", 0},
        {"[3] = How many items sold", 0},
        {"[4] = product ID with the most sales", -1},
        {"[5] = sale amount of 4", 0.0f},
        {"[6] = product ID with the highest '%' over retail", -1},
        {"[7] = '%' over retail of 6", 0.0f},
        {"[8] = min remaining inventory revenue", FLT_MAX}
    };

    cout << "\nInventory:\n" << endl;
    //Iterating through the hashtable
    for (auto& currentNode : hashTable) {
        Node* current = currentNode;
        while (current) {
            //Calculate and accumulate total costs and revenue

            cout << left << setw(30) << "ID: " << currentNode->key << '\n';
            cout << left << setw(30) << "Current Stock: " << currentNode->value.getQuantity() - currentNode->value.getQuantitySold() << '\n';
            cout << left << setw(30) << "Amount Bought: " << currentNode->value.getQuantity() << '\n';
            cout << left << setw(30) << "Amount Sold: " << currentNode->value.getQuantitySold() << '\n';
            cout << left << setw(30) << "Average Cost: " << "$" << fixed << setprecision(2) << currentNode->value.getAverageCost() << '\n';
            cout << left << setw(30) << "Average Sale: " << "$" << fixed << setprecision(2) << currentNode->value.getAverageSale() << '\n';
            cout << left << setw(30) << "Sold Item Profit: " << "$" << fixed << setprecision(2) << currentNode->value.getAverageSale() * currentNode->value.getQuantitySold() - currentNode->value.getAverageCost() * currentNode->value.getQuantity() << '\n';
            cout << left << setw(30) << "Unsold Inventory Cost: " << "$" << fixed << setprecision(2) << currentNode->value.getAverageCost() * (currentNode->value.getQuantity() - currentNode->value.getQuantitySold()) << '\n' << '\n';

            salesInfo[0].second += current->value.getAverageCost() * current->value.getQuantity();
            salesInfo[1].second += current->value.getAverageSale() * current->value.getQuantitySold();
            salesInfo[2].second += current->value.getQuantity();
            salesInfo[3].second += current->value.getQuantitySold();

            //Check for product with most sales
            if (current->value.getQuantitySold() > salesInfo[5].second) {
                salesInfo[4].second = current->key;
                salesInfo[5].second = current->value.getQuantitySold();
            }

            //Calculate % over retail and check for product with the highest %
            //float percentOverRetail = ((current->value.getAverageSale() - current->value.getAverageCost()) / current->value.getAverageCost()) * 100;
            float percentOverRetail = ((current->value.getAverageSale() / current->value.getAverageCost())) * 100;
            if (percentOverRetail > salesInfo[7].second) {
                salesInfo[6].second = current->key;
                salesInfo[7].second = percentOverRetail;
            }

            //Calculate revenue from remaining inventory and check for minimum
            float remainingInventoryRevenue = current->value.getAverageCost() * (current->value.getQuantity() - current->value.getQuantitySold());
            if (remainingInventoryRevenue < salesInfo[8].second) {
                salesInfo[8].second = remainingInventoryRevenue;
            }

            current = current->next;
        }
    }

    /*// Testing if collected information from the vector of pairs is right
    for (const auto& info : salesInfo) {
        cout << left << setw(30) << info.first << ": $" << fixed << setprecision(2) << info.second << endl;
    }*/

    //Printing out sales report
    cout << "Cumulative Sales Report:\n" << '\n';
    cout << left << setw(30) << "Total Cost:" << '$' << fixed << setprecision(2) << salesInfo[0].second << '\n';
    cout << left << setw(30) << "Total Revenue:" << '$' << fixed << setprecision(2) << salesInfo[1].second << '\n';
    cout << left << setw(30) << "Total Profit:" << '$' << fixed << setprecision(2) << salesInfo[1].second - salesInfo[0].second << '\n';
    cout << left << setw(30) << "Unsold Inventory Cost:" << '$' << fixed << setprecision(2) << salesInfo[8].second << '\n';
    cout << left << setw(30) << "Inventory Size:" << int(salesInfo[2].second) - int(salesInfo[3].second) << '\n';
    cout << left << setw(30) << "Total Bought:" << int(salesInfo[2].second) << '\n';
    cout << left << setw(30) << "Total Sold:" << int(salesInfo[3].second) << '\n';
    cout << left << setw(30) << "Highest Sales:" << int(salesInfo[4].second) << " - " << int(salesInfo[5].second) << '\n';
    cout << left << setw(30) << "Highest % Profit:" << int(salesInfo[6].second) << " - %" << fixed << setprecision(2) << salesInfo[7].second * 100 << '\n';
    cout << '\n';
}

void MapDatabase::saveSpreadsheet(string filename) {

    ofstream csvFile(filename); // creates file
    if (!csvFile.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }
    else {
        csvFile << "Identifier, Stock, Historical Quantity, Quantity Sold, Average Cost, Average Sale" << endl; // first line of file always
        for (auto& currentNode : hashTable) {
            Node* current = currentNode;
            while (current) {
                csvFile << current->key << ", "  << current->value.getQuantity() - current->value.getQuantitySold() << ", " 
                    << current->value.getQuantity() << ", " << current->value.getQuantitySold() << ", " <<
                    current->value.getAverageCost() << ", " << current->value.getAverageCost() << endl;
                current = current->next;
            }

        }

    }


}

bool MapDatabase::loadSpreadsheet(string filename) {
    ifstream csvFile(filename); // accesses the file,, should have ".csv" in string
    if (csvFile.is_open()) {
        string line;
        getline(csvFile, line); // reads first line

         // all start as a string
        string not_needed;
        string identifier;
        string quantity;
        string quantity_sold;
        string average_cost;
        string average_sale;


        // to stoi/stof the strings
        int id;
        int q;
        int q_sold;
        float avg_cost;
        float avg_sale;

        while (getline(csvFile, line)) {  // will stop after it goes through every line of the file
            istringstream stream(line); // a stream of data

            // separates the stream of data into the string variable when it finds the comma
            getline(stream, identifier, ',');
            getline(stream, not_needed, ',');
            getline(stream, quantity, ',');
            getline(stream, quantity_sold, ',');
            getline(stream, average_cost, ',');
            getline(stream, average_sale, ',');

            // changes these all into correct data type
            id = stoi(identifier);
            q = stoi(quantity);
            q_sold = stoi(quantity_sold);
            avg_cost = stof(average_cost);
            avg_sale = stof(average_sale);

            buy(id, q, avg_cost);
            sell(id, q_sold, avg_sale);
        }
        return true;
    }
    return false;
}
