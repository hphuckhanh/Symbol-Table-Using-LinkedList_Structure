#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable {
public:
    SymbolTable() {}
    void run(string filename);
};

struct symbol {
	string	name;
	string	type;
	string	value;
	int		level;
	symbol	*snext;
	symbol() : snext(NULL) {}
	symbol(string s, string t, int & i) : name(s), type(t), level(i), snext(NULL) {}
};


class symbolList {
	symbol *psym;
	int		size;
public:
	symbolList() : psym(NULL) {}
	
	void insert(string name, string type, int & l);
	
	int assign(string name, string value);
	
	int begin_block(int & i);
	
	int end_block(int & k);
	
	int lookup(string name);
	
	void print_table();
	
	void rprint_table();
	
	int find(string name);
	
	
};

#endif