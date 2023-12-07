#pragma once
#include <string>
#include <iostream>
#include "Product.h"
using namespace std;

// default constructor
Product::Product() {
	this->identifier = 0;
	this->quantity = 0;
	this->average_cost = 0;
	this->quantity_sold = 0;
	this->average_sale = 0;
}

// custom constructor
Product::Product(int id, int q, float c) {
	this->identifier = id;
	this->quantity = q;
	this->average_cost = c;
	this->quantity_sold = 0;
	this->average_sale = 0;
}

// calculates new average cost and increments quantity of product
void Product::buy(int q, float c) {
	this->average_cost = (this->quantity * this->average_cost + q * c) / (this->quantity + q);
	this->quantity += q;
}

// increments quantity sold, and calculates average sale price
bool Product::sell(int q, float c) {
	if (this->quantity_sold + q > this->quantity || q == 0)
		return false;
	this->average_sale = (this->quantity_sold * this->average_sale + q * c) / (this->quantity_sold + q);
	this->quantity_sold += q;
	return true;
}

// getter functions
int Product::getIdentifier() {
	return this->identifier;
}

int Product::getQuantity() {
	return this->quantity;
}

float Product::getAverageCost() {
	return this->average_cost;
}

int Product::getQuantitySold() {
	return this->quantity_sold;
}

float Product::getAverageSale() {
	return this->average_sale;
}