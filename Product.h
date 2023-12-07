#pragma once
#include <string>
#include <iostream>
using namespace std;

// product class stores the information about one product ID (integer), representing 4 data points total
class Product {
	int identifier;
	int quantity;
	float average_cost;
	int quantity_sold;
	float average_sale;
	
public:
	Product();
	Product(int id, int q, float c);
	void buy(int q, float c);
	bool sell(int q, float c);
	int getIdentifier();
	int getQuantity();
	float getAverageCost();
	int getQuantitySold();
	float getAverageSale();
};