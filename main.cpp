#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>
#include <random>
#include <iomanip>
#include "RedBlackTreeDatabase.h"
#include "MapDatabase.h"
#include "Product.h"

using namespace std;

// command to run test cases
void runTestCases() {
    // initializes rng
    random_device rndm;
    mt19937 generator(rndm());
    uniform_int_distribution<> id_range(0, 999999);

    // initializes temp vars
	int x, y, z, r;
	vector<int> items_to_sell;
    vector<chrono::nanoseconds> tree_times;

    // makes tree to test
    cout << "Testing Red-Black Tree structure..." << endl;
    RedBlackTreeDatabase test_tree;

    // each test takes a time snapshot before and after to high precision, then outputs the difference in ns

	// 30,000 buy commands
    auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 30000; i++) {
        // generates random buy command
        x = id_range(generator);
        y = id_range(generator) % 999;
		z = id_range(generator) % 999;
		r = id_range(generator) % 6;
        // 1/6 chance to save item to sell in test 2
		if (r == 0)
			items_to_sell.push_back(x);

        // executes buy command
		test_tree.buy(x, y, z);
	}
    auto end = chrono::high_resolution_clock::now();
    tree_times.push_back(chrono::duration_cast<chrono::nanoseconds>(end - start));
    cout << "Test 1 - 30,000 purchases (120,000 data points) completed in " << tree_times[0].count() << "ns" << endl;

	// 5,000 sell commands
    start = chrono::high_resolution_clock::now();
	for (int& to_sell : items_to_sell) {
        // generates random q/p for selling previously collected items
        y = id_range(generator) % 50;
        z = id_range(generator) % 1000;
		test_tree.sell(to_sell, y, z);
	}
    end = chrono::high_resolution_clock::now();
    tree_times.push_back(chrono::duration_cast<chrono::nanoseconds>(end - start));
    cout << "Test 2 - 5,000 sales completed in " << tree_times[1].count() << "ns" << endl;

    // Saving Spreadsheet
    start = chrono::high_resolution_clock::now();
    test_tree.saveSpreadsheet("test_database1.csv");
    end = chrono::high_resolution_clock::now();
    tree_times.push_back(chrono::duration_cast<chrono::nanoseconds>(end - start));
    cout << "Test 3 - save spreadsheet completed in " << tree_times[2].count() << "ns" << endl;

    // Loading Spreadsheet
    RedBlackTreeDatabase test_tree2;
    start = chrono::high_resolution_clock::now();
    test_tree2.loadSpreadsheet("test_database1.csv");
    end = chrono::high_resolution_clock::now();
    tree_times.push_back(chrono::duration_cast<chrono::nanoseconds>(end - start));
    cout << "Test 4 - load spreadsheet completed in " << tree_times[3].count() << "ns" << endl;

    // resets testing grounds for hash map test, which are all exactly the same besides using hash map instead
    items_to_sell.clear();
    vector<chrono::nanoseconds> map_times;

    cout << endl << "Testing Hashmap structure..." << endl;
    MapDatabase test_map(100);

    // 30,000 buy commands
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < 30000; i++) {
        x = rand() % 999999;
        y = rand() % 100 + 10;
        z = rand() % 100 + 10;
        r = rand() % 6;
        if (r == 0)
            items_to_sell.push_back(x);
        test_map.buy(x, y, z);
    }
    end = chrono::high_resolution_clock::now();
    map_times.push_back(chrono::duration_cast<chrono::nanoseconds>(end - start));
    cout << "Test 1 - 30,000 purchases (120,000 data points) completed in " << map_times[0].count() << "ns" << endl;

    // 5,000 sell commands
    start = chrono::high_resolution_clock::now();
    for (int& to_sell : items_to_sell) {
        y = rand() % 10;
        z = rand() % 1000;
        test_map.sell(to_sell, y, z);
    }
    end = chrono::high_resolution_clock::now();
    map_times.push_back(chrono::duration_cast<chrono::nanoseconds>(end - start));
    cout << "Test 2 - 5,000 sales completed in " << map_times[1].count() << "ns" << endl;

    // Saving Spreadsheet
    start = chrono::high_resolution_clock::now();
    test_map.saveSpreadsheet("test_database2.csv");
    end = chrono::high_resolution_clock::now();
    map_times.push_back(chrono::duration_cast<chrono::nanoseconds>(end - start));
    cout << "Test 3 - save spreadsheet completed in " << map_times[2].count() << "ns" << endl;

    // Loading Spreadsheet
    MapDatabase test_map2(100);
    start = chrono::high_resolution_clock::now();
    test_map2.loadSpreadsheet("test_database2.csv");
    end = chrono::high_resolution_clock::now();
    map_times.push_back(chrono::duration_cast<chrono::nanoseconds>(end - start));
    cout << "Test 4 - load spreadsheet completed in " << map_times[3].count() << "ns" << endl;

    // prints which was faster iteratively by comparing
    cout << endl << "Results:" << endl;
    for (int i = 0; i < 4; i++) {
        cout << "Test " << i << ": ";
        if (tree_times[i].count() < map_times[i].count())
            cout << "Red-Black Tree is faster by " << (map_times[i]-tree_times[i]).count() << "ns" << endl;
        else
            cout << "Hashmap is faster by " << (tree_times[i]-map_times[i]).count() << "ns" << endl;
    }
}


int main() {
    int option;
    bool exit = false;

    // choose data structure or run test
    cout << "Enter 1 for Red-Black Tree, 2 for Hash Map, or 3 to run comparison test:" << endl;
    cin >> option;

    // runs test cases if selected
    if (option == 3) {
        runTestCases();
        return 0;
    }

    // otherwise, makes both data structures either way, but only uses the selected one
    RedBlackTreeDatabase tree;
    MapDatabase hash(100);
    cout << fixed << setprecision(2) << "Data structure created successfully. Enter a command or \"help\" for menu options." << endl;

    // text based menu
    while (!exit) {
        // collects input
        string line;
        getline(cin >> ws, line);
        vector<string> input;
        istringstream stream(line);

        while (getline(stream, line, ' '))
            input.push_back(line);

        // buy [id] [q] [p]
        if (input[0] == "buy") {
            if (input.size() == 4) {
                int identifier = stoi(input[1]);
                int quantity = stoi(input[2]);
                float price = stof(input[3]);
                // runs the buy function of database
                if (option == 1)
                    tree.buy(identifier, quantity, price);
                else
                    hash.buy(identifier, quantity, price);
                cout << identifier << " (" << quantity << ") bought for $" << price << " per unit successfully." << endl;
            }
        }

        // sell [id] [q] [p]
        else if (input[0] == "sell") {
            if (input.size() == 4) {
                int identifier = stoi(input[1]);
                int quantity = stoi(input[2]);
                float price = stof(input[3]);
                // runs the sell function of database
                if (option == 1)
                    if (tree.sell(identifier, quantity, price))
                        cout << identifier << " (" << quantity << ") sold for $" << price << " per unit successfully." << endl;
                    else
                        cout << "Sale failed! Item not found or quantity exceeds stock." << endl;
                else
                    if (hash.sell(identifier, quantity, price))
                        cout << identifier << " (" << quantity << ") sold for $" << price << " per unit successfully." << endl;
                    else
                        cout << "Sale failed! Item not found or quantity exceeds stock." << endl;
            }
        }
        
        // print [id]
        else if (input[0] == "print") {
            int identifier = stoi(input[1]);
            if (option == 1) {
                if (!tree.printItem(identifier))
                    cout << "Item not found.\n" << endl;
            }
            else {
                if (!hash.printItem(identifier))
                    cout << "Item not found.\n" << endl;
            }
        }

        // report
        else if (input[0] == "report") {
            if (option == 1) {
                tree.printSalesReport();
            }
            else {
                hash.printSalesReport();
            }
        }

        // save [filename]
        else if (input[0] == "save") {
            if (input.size() == 2) {
                if (option == 1)
                    tree.saveSpreadsheet(input[1]);
                else
                    hash.saveSpreadsheet(input[1]);
                cout << "Data saved to " << input[1] << " successfully!" << endl;
            }
        }

        // load [filename]
        else if (input[0] == "load") {
            if (input.size() == 2) {
                if (option == 1) {
                    if (tree.loadSpreadsheet(input[1]))
                        cout << "Spreadsheet loaded successfully!" << endl;
                    else
                        cout << "File not found." << endl;
                }
                else {
                    if (hash.loadSpreadsheet(input[1]))
                        cout << "Spreadsheet loaded successfully!" << endl;
                    else
                        cout << "File not found." << endl;
                }
            }
        }

        // exit
        else if (input[0] == "exit") {
            cout << "Exiting program..." << endl;
            exit = true;
        }
        
        // help
        else if (input[0] == "help") {
            cout << "Command List:" << endl;
            cout << "buy (id) (quantity) (unit price)" << endl;
            cout << "sell (id) (quantity) (unit price)" << endl;
            cout << "print (id)" << endl;
            cout << "report" << endl;
            cout << "save (file)" << endl;
            cout << "load (file)" << endl;
            cout << "exit" << endl;
        }
    }
    return 0;
}
