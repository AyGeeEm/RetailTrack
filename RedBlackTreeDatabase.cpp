#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "Product.h"
#include "RedBlackTreeDatabase.h"
using namespace std;

// Constructor sets root to nullptr
RedBlackTreeDatabase::RedBlackTreeDatabase() {
	this->root = nullptr;
}

// Buy method is a specialized insertion
// Adapted from Aman's lectures and pseudocode from GeeksForGeeks
void RedBlackTreeDatabase::buy(int id, int q, float c) {
	// Iterates to find proper location while also keeping parent stored
	TreeNode* current = this->root;
	TreeNode* p = nullptr;
	while (current != nullptr) {
		p = current;
		if (id > current->key->getIdentifier())
			current = current->right;
		else if (id < current->key->getIdentifier())
			current = current->left;
		else if (id == current->key->getIdentifier()) {
			// Instead of failing when equal, the program initiates a buy function
			current->key->buy(q, c);
			return;
		}
	}

	// Red/Black insertion assuming the product is unique
	// New TreeNode containing product as key is created, parent is passed in
	current = new TreeNode(new Product(id, q, c), p);

	if (p == nullptr) {
		// If it's the current root, that is set here (and the root is always black so that is ensured)
		current->color = 0;
		this->root = current;
		return;
	}

	// Otherwise it checks if it is its parent's right or left child
	else if (id > p->key->getIdentifier())
		p->right = current;
	else
		p->left = current;

	// If the grandparent is nullptr then no color swap or rotation is necessary
	if (p->parent == nullptr)
		return;

	// In all other cases, color swap/rotation needs to be checked for, so uncle node is initialized
	TreeNode* uncle;
	// This while loop iterates up the tree to perform necessary permutations
	while (current != this->root && current->parent->color == 1) {
		// If parent is left child case
		if (current->parent == current->parent->parent->left) {
			// The uncle is the right child if the parent is the left
			uncle = current->parent->parent->right;
			// If uncle is red, then color swap
			if (uncle != nullptr && uncle->color == 1) {
				current->parent->color = 0;
				uncle->color = 0;
				current->parent->parent->color = 1;
				// Color swap may require a swap or rotation of grandparent so the while loop iterates up
				current = current->parent->parent;
			}
			// If the uncle is black
			else {
				// If the current node is the right child of it's parent, then a left right rotation of parent is needed
				// otherwise a right rotation of grandparent is needed after a color swap
				if (current == current->parent->right) {
					// Left/Right case with color swap
					current = current->parent;
					current->right->color = 0;
					current->parent->color = 1;
					this->rotateLeft(current);
					this->rotateRight(current->parent->parent);
				}
				else {
					// Left/Left case with color swap
					current->parent->color = 0;
					current->parent->parent->color = 1;
					this->rotateRight(current->parent->parent);
				}
			}
		}
		// Parent is right child, mirror case
		else {
			uncle = current->parent->parent->left;
			if (uncle != nullptr && uncle->color == 1) {
				current->parent->color = 0;
				uncle->color = 0;
				current->parent->parent->color = 1;
				current = current->parent->parent;
			}
			else {
				if (current == current->parent->left) {
					// Right/Left case with color swap
					current = current->parent;
					current->left->color = 0;
					current->parent->color = 1;
					this->rotateRight(current);
					this->rotateLeft(current->parent->parent);
				}
				else {
					// Right/Right case with color swap
					current->parent->color = 0;
					current->parent->parent->color = 1;
					this->rotateLeft(current->parent->parent);
				}
			}
		}
	}
	// The algorithm given above guarantees a balanced tree, but the root must be changed to black afterwards in the case that it's a red node
	this->root->color = 0;
}


// rotations (different than AVL since parent pointers must also be fixed)
void RedBlackTreeDatabase::rotateLeft(TreeNode* current) {
	TreeNode* child = current->right;
	current->right = child->left;
	child->parent = current->parent;

	// if childs left child exists, sets its parent to current
	if (child->left != nullptr)
		child->left->parent = current;
	// sets the root if necessary
	if (current->parent == nullptr)
		this->root = child;

	// checks if left or right child and assigns proper position to replacing child
	else if (current == current->parent->right)
		current->parent->right = child;
	else
		current->parent->left = child;

	child->left = current;
	current->parent = child;
}

void RedBlackTreeDatabase::rotateRight(TreeNode* current) {
	TreeNode* child = current->left;
	current->left = child->right;
	child->parent = current->parent;

	// if childs left child exists, sets its parent to current
	if (child->right != nullptr)
		child->right->parent = current;
	// sets the root if necessary
	if (current->parent == nullptr)
		this->root = child;

	// checks if left or right child and assigns proper position to replacing child
	else if (current == current->parent->right)
		current->parent->right = child;
	else
		current->parent->left = child;

	child->right = current;
	current->parent = child;
}

// uses BST search to find a key
TreeNode* RedBlackTreeDatabase::find(int id) {
	TreeNode* current = this->root;
	while (current != nullptr) {
		if (id > current->key->getIdentifier())
			current = current->right;
		else if (id < current->key->getIdentifier())
			current = current->left;
		else if (id == current->key->getIdentifier()) {
			break;
		}
	}
	return current;
}

// If the key is found, attempts to sell the item. If not found or sale fails than returns 0. Otherwise returns 1
bool RedBlackTreeDatabase::sell(int id, int q, float c) {
	TreeNode* search = this->find(id);
	if (search != nullptr) {
		if (search->key->sell(q, c))
			return 1;
	}
	return 0;
}

// Similar to above but prints item instead
bool RedBlackTreeDatabase::printItem(int id) {
	TreeNode* current = this->find(id);
	if (current != nullptr) {
		this->printItemHelper(current->key);
		return 1;
	}
	return 0;

}

// Prints all stats of item.
void RedBlackTreeDatabase::printItemHelper(Product* current) {
	cout << '\n';
	cout << left << setw(30) << "ID:" << current->getIdentifier() << '\n';
	cout << left << setw(30) << "Current Stock:" << current->getQuantity() - current->getQuantitySold() << '\n';
	cout << left << setw(30) << "Amount Bought:" << current->getQuantity() << '\n';
	cout << left << setw(30) << "Amount Sold:" << current->getQuantitySold() << '\n';
	cout << left << setw(30) << "Average Cost:" << '$' << fixed << setprecision(2) << current->getAverageCost() << '\n';
	cout << left << setw(30) << "Average Sale:" << '$' << fixed << setprecision(2) << current->getAverageSale() << '\n';
	cout << left << setw(30) << "Sold Item Profit:" << '$' << fixed << setprecision(2) << current->getAverageSale() * current->getQuantitySold() - current->getAverageCost() * current->getQuantitySold() << '\n';
	cout << left << setw(30) << "Unsold Inventory Cost:" << '$' << fixed << setprecision(2) << current->getAverageCost() * (current->getQuantity() - current->getQuantitySold()) << '\n';

}

void RedBlackTreeDatabase::printSalesReport() {
	// prints each item in order and collects cumulative info
	// [0] = Costs
	// [1] = Revenue
	// [2] = How Many items bought
	// [3] = How many items sold
	// [4] = product ID with the most sales
	// [5] = sale amount of 4
	// [6] = product ID with the highest % over retail
	// [7] = % over retail of 6
	// [8] = min remaining inventory revenue

	vector<float> cumulative_info(10, 0.0f); // initialize vector for storing cumulative data
	cumulative_info[4] = -1;
	cumulative_info[6] = -1;
	cout << '\n' << "Inventory:" << '\n';
	this->printSalesReportHelper(this->root, cumulative_info); // traversal to collect data and print item by item
	cout << "\nCumulative Sales Report:" << '\n';
	cout << left << setw(30) << "Total Cost:" << '$' << fixed << setprecision(2) << cumulative_info[0] << '\n';
	cout << left << setw(30) << "Total Revenue:" << '$' << fixed << setprecision(2) << cumulative_info[1] << '\n';
	cout << left << setw(30) << "Total Profit:" << '$' << fixed << setprecision(2) << cumulative_info[1] - cumulative_info[0] << '\n';
	cout << left << setw(30) << "Unsold Inventory Cost:" << '$' << fixed << setprecision(2) << cumulative_info[8] << '\n';
	cout << left << setw(30) << "Inventory Size:" << int(cumulative_info[2]) - int(cumulative_info[3]) << '\n';
	cout << left << setw(30) << "Total Bought:" << int(cumulative_info[2]) << '\n';
	cout << left << setw(30) << "Total Sold:" << int(cumulative_info[3]) << '\n';
	cout << left << setw(30) << "Highest Sales:" << int(cumulative_info[4]) << " - " << int(cumulative_info[5]) << '\n';
	cout << left << setw(30) << "Highest % Profit:" << int(cumulative_info[6]) << " - %" << fixed << setprecision(2) << cumulative_info[7] * 100 << '\n';
	cout << '\n';

}

// Navigates in order and updates vector
void RedBlackTreeDatabase::printSalesReportHelper(TreeNode* current, vector<float>& v) {
	if (current == nullptr)
		return;

	// in order traversal

	this->printSalesReportHelper(current->left, v);
	this->printItemHelper(current->key);

	// updates cumulative statistics by incrementing and comparing
	v[0] += current->key->getAverageCost() * current->key->getQuantity();
	v[1] += current->key->getAverageSale() * current->key->getQuantitySold();
	v[2] += current->key->getQuantity();
	v[3] += current->key->getQuantitySold();
	if (current->key->getQuantitySold() > v[5]) {
		v[4] = current->key->getIdentifier();
		v[5] = current->key->getQuantitySold();
	}
	if (current->key->getAverageSale() / current->key->getAverageCost() > v[7]) {
		v[6] = current->key->getIdentifier();
		v[7] = current->key->getAverageSale() / current->key->getAverageCost();
	}
	v[8] += current->key->getAverageCost() * (current->key->getQuantity() - current->key->getQuantitySold());

	this->printSalesReportHelper(current->right, v);
}

void RedBlackTreeDatabase::saveSpreadsheet(string filename) {

	ofstream csvFile(filename); // creates file
	if (!csvFile.is_open()) {
		cout << "Error opening file!" << endl;
		return;
	}
	else {
		csvFile << "Identifier, Stock, Historical Quantity, Quantity Sold, Average Cost, Average Sale" << endl; // first line of file always
		vector<TreeNode*> nodes; // each node will be added as a vector from inorder traversal
		saveSpreadsheetHelper(root, nodes);
		for (int i = 0; i < nodes.size(); i++) { // inserts line in file with node data based on 1st line's format
			csvFile << nodes[i]->key->getIdentifier() << ", " << nodes[i]->key->getQuantity() - nodes[i]->key->getQuantitySold() << ", " <<
				nodes[i]->key->getQuantity() << ", " << nodes[i]->key->getQuantitySold() << ", " << nodes[i]->key->getAverageCost() << ", " <<
				nodes[i]->key->getAverageSale() << endl;

		}
	}
}

void RedBlackTreeDatabase::saveSpreadsheetHelper(TreeNode* current, vector<TreeNode*>& nodes) {
	// inorder traversal
	if (current == nullptr) {
		return;
	}
	saveSpreadsheetHelper(current->left, nodes);
	nodes.push_back(current); // adds each node into vector
	saveSpreadsheetHelper(current->right, nodes);
	return;
}


bool RedBlackTreeDatabase::loadSpreadsheet(string filename) {
	ifstream csvFile(filename); // accesses the file,, should have ".csv" in string
	if (csvFile.is_open()) {
		string line;
		getline(csvFile, line); // reads first line

		string not_needed;
		string identifier;
		string quantity;
		string quantity_sold;
		string average_cost;
		string average_sale;
		// all start as a string


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

			this->buy(id, q, avg_cost);
			this->sell(id, q_sold, avg_sale);

			//need to create tree nodes with all this product data^

		}
		return 1;
	}
	return 0;
}