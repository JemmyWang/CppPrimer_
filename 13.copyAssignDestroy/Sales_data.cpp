#include"stdafx.h"
#include<iostream>
#include<string>

using namespace std;

class Sales_data {
public:
	Sales_data() : bookNO("111"), units_sold(0), revenue(10) {
		cout << "default constructor |||||||||||||" << endl;
	}

	Sales_data f(Sales_data sd) {
		//Sales_data dd = sd;
		return sd;
	}
	Sales_data(const Sales_data &);
	Sales_data & operator= (const Sales_data &);

	void get() {
		cout << bookNO << " " << units_sold << " " << revenue << endl;
	}
	~Sales_data() {
		cout << "Destructor is called." << endl;
	}
private:
	string bookNO;
	int units_sold;
	double revenue;
};

Sales_data::Sales_data(const Sales_data &orig) :
	bookNO(orig.bookNO), units_sold(orig.units_sold), revenue(orig.revenue) {
	cout << "copy constructor _____" << endl;
}

Sales_data & Sales_data::operator=(const Sales_data & sd) {
	this->bookNO = sd.bookNO + "100";
	this->units_sold = sd.units_sold + 200;
	this->revenue = revenue + 1000;
	cout << "copy assignment +++++++++" << endl;
	return *this;
}

//int main(void) {
//	// d and s point to same memory, can only be destroyed once.
//	Sales_data *d = new Sales_data();
//	Sales_data *s = d;
//	s->f(*d);
//	s->get();
//	delete d;
//	getchar();
//	return 0;
//}