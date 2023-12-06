#pragma once
#include <vector>
#include "Product.h"
using namespace std;

// Node struct, color set to red by default
struct TreeNode {
	Product* key;
	TreeNode* parent;
	TreeNode* left = nullptr;
	TreeNode* right = nullptr;
	bool color = 1;
	TreeNode(Product* key, TreeNode* parent) {
		this->key = key;
		this->parent = parent;
	}
};

// Tree/Database class
// Deletion is not needed because of how our project is structured, selling an item doesn't "delete" it
class RedBlackTreeDatabase {
	TreeNode* root;

public:
	RedBlackTreeDatabase();
	void buy(int id, int q, float c);
	bool sell(int id, int q, float c);
	bool printItem(int id);
	void printItemHelper(Product* current);
	void printSalesReport();
	void printSalesReportHelper(TreeNode* current, vector<float>& v);
	TreeNode* find(int id);
	void saveSpreadsheet(string filename);
	bool loadSpreadsheet(string filename);
	void saveSpreadsheetHelper(TreeNode* current, vector<TreeNode*>& nodes);
	void rotateLeft(TreeNode* r);
	void rotateRight(TreeNode* r);
};