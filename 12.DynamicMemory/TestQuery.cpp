// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<vector>
#include<string>
#include<set>
#include<map>
#include<sstream>
#include<memory>
#include<fstream>

using namespace std;

using line_no = vector<string>::size_type;

class QueryResult;

class TextQuery {
public:
	TextQuery(ifstream &);
	QueryResult query(const string &) const;
private:
	shared_ptr<vector<string>> file;
	map<string, shared_ptr<set<line_no>>> vm;
};

TextQuery::TextQuery(ifstream &is) : file(new vector<string>) {
	string text;
	while (getline(is, text)) {
		file->push_back(text);
		int n = file->size() - 1;
		istringstream line(text);
		string word;
		while (line >> word) {
			//shared_ptr<set<line_no>> lines = vm[word];
			auto &lines = vm[word];
			if (!lines) {
				lines.reset(new set<line_no>);
			}
			lines->insert(n);
		}
	}
}



class QueryResult {
public:
	friend ostream &print(ostream &, const QueryResult &);
	QueryResult(string s, shared_ptr<set<line_no>> p, shared_ptr<vector<string>> f) :
		sought(s), lines(p), file(f) {}
private:
	string sought;
	shared_ptr<set<line_no>> lines;
	shared_ptr<vector<string>> file;
};

QueryResult TextQuery::query(const string &str) const {
	static shared_ptr <set<line_no >> noData(new set<line_no>);
	auto loc = vm.find(str);
	if (loc == vm.end()) {
		return QueryResult(str, noData, file);
	} else {
		return QueryResult(str, loc->second, file);
	}
}

ostream &print(ostream &os, const QueryResult &qr) {
	os << qr.sought << " occurs " << qr.lines->size() << "tiems" << endl;
	for (auto num : *qr.lines) {
		os << "  ( line: " << num + 1 << ")  " <<
			*(qr.file->begin() + num) << endl << endl;
	}
	return os;
}

int main()
{
	ifstream is;
	is.open("D:\\cppworkspace\\cpppriemr\\cpppriemr\\textquery.txt");
	if (is) {
		string sought;
		TextQuery tq(is);
		while (sought != "b" && sought != "B") {
			cout << "Input word to search, b or B to stop." << endl;
			cin >> sought;
			QueryResult qr = tq.query(sought);
			print(cout, qr);
		}
		
	}


	return 0;
}

