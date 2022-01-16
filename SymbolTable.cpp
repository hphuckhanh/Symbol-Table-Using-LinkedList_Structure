#include "SymbolTable.h"

void SymbolTable::run(string filename) {
	int code = 0; 
	int i=0;
	ifstream ifs;
	ifs.open(filename, ifstream::in);
	
	symbolList *symlist = new symbolList();
	
	string lenh, name, type, value;
	while(ifs.good()){
		ifs >> lenh;
		if (lenh == "INSERT")	code=1;
		else if (lenh == "ASSIGN")	{
			code=2;
			ifs >> name;
			string s;
			getline(ifs, s);
			int start = s.find(" ") + 1;
			value = s.substr(start, s.length());
		}
		else if (lenh == "BEGIN")	code=3;
		else if (lenh == "END")		code=4;
		else if (lenh == "LOOKUP")	code=5;
		else if (lenh == "PRINT")	code=6;
		else if (lenh == "RPRINT")	code=7;
		else {
			string s1, s2;
			s1 = getline(ifs, s1);
			s2 = lenh + s1;
			throw InvalidInstruction(s2);
		}
		switch (code) {
			case 1: 
				ifs >> name; ifs >> type;
				symlist->insert(name, type, i);
				break;
			case 2:
				symlist->assign(name, value);
				break;
			case 3:
				i = i+1;
				break;				
			case 4:
				i = i-1;
				break;
			case 5:
				ifs >> name;
				symlist->lookup(name);
				break;
			case 6:
				symlist->print_table();
				break;
			case 7:
				symlist->rprint_table();
				break;
		}
		if(ifs.eof() != 1 && code != 3 && code != 4) cout << endl;
	}
	if (i != 0) {
		cout << endl;
		throw UnclosedBlock(i);
	}
		
	delete symlist;
	ifs.close();
}

bool isNumber(const string& str)
{
    for (char const &c : str) {
        if (isdigit(c) == 0) return false;
    }
    return true;
}

bool isString(string str)
{
	//const regex pattern("[@_!#$%^&*()<>?/|}{~:]");
	if(str.find("'") == 0){
		if(str.find("'", 1) == (str.length()-1)){
			 if (str.empty())
			 {
				return true;
			 }
			 else if(str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890") != std::string::npos)
			 {
				return true;
			 }
			 else
			 {
				return false;
			 }
		} else return true;
	} else return true;
} 

int symbolList::find(string name){
	symbol *look = new symbol();
	look = this->psym;
	if(look == NULL){
		return -1;
	}
	do {
		if (look->name == name){
			return look->level;
			break;
		} else {
			return -1;
		}
		look = look->snext;
	} while (look->snext == NULL);
	return look->level;
	delete look;
}

void symbolList::insert(string name, string type, int & l){
	symbol *nsymbol = new symbol(name, type, l);
	int j = this->find(name);
	if (j == l) {
		stringstream s;
		s << "INSERT " << name << " " << type;
		string err = s.str();
		throw Redeclared(err);
	}else {
		nsymbol->snext = psym;
		psym = nsymbol;
		this->size +=1;
		cout << "success";
	}
}

int symbolList::assign(string name, string value){
	symbol *find = new symbol();
	find = this->psym;
	if(find == NULL){
		throw Undeclared(name);
	}
	for(int i=0; i < this->size; i++){
		if(find->name == name){
			if(find->type == "number" && isNumber(value) == 1){
				find->value = value;
				cout << "success";
			} else if(find->type == "number" && isNumber(value) != 1){
				stringstream s;
				s << "ASSIGN " << name << " " << value;
				string err = s.str();
				throw TypeMismatch(err);
			} else if(find->type == "string" && isString(value) == 0){
				find->value = value;
				cout << "success";
			} else if(find->type == "string" && isString(value) != 0){
				stringstream s;
				s << "ASSIGN " << name<< " " << value;
				string err = s.str();
				throw TypeMismatch(err);
			}
			break;
		} else{
			if(find->snext == NULL && find->name != name){
				throw Undeclared(name);
			}
		}
		find = find->snext;
	}
	return 0;
	delete find;
}

void symbolList::print_table(){
	symbol *pTemp = this->psym;
	string arr[50];
	int arr1[50];
	int i=0; int count = 0;
	arr[i] = pTemp->name;
	arr1[i] = pTemp->level;
	pTemp = pTemp->snext;
	++i;
	do {
		count =0;
		for(int k=0; k<i; k++){
			if(arr[k] == pTemp->name){
				count++;
				--i;
			}
		}
		if (count == 0){
			arr[i] = pTemp->name;
			arr1[i] = pTemp->level;
		}
		pTemp = pTemp->snext;
		++i;
	} while (pTemp != NULL);
	for (int j=i-1; j>=1; j--){
		cout << arr[j];
		cout<< "//";
		cout << arr1[j];
		cout << " ";
	}
	cout << arr[0]; cout << "//"; cout << arr1[0];
	delete pTemp;
}

void symbolList::rprint_table(){
	symbol *pTemp = this->psym;
	
	string arr[50];
	int arr1[50];
	int i=0; int count = 0;
	arr[i] = pTemp->name;
	arr1[i] = pTemp->level;
	pTemp = pTemp->snext;
	++i;
	do {
		count =0;
		for(int k=0; k<i; k++){
			if(arr[k] == pTemp->name){
				count++;
				--i;
			}
		}
		if (count == 0){
			arr[i] = pTemp->name;
			arr1[i] = pTemp->level;
		}
		pTemp = pTemp->snext;
		++i;
	} while (pTemp != NULL);
	for (int j=0; j<i-1; j++){
		cout << arr[j];
		cout<< "//";
		cout << arr1[j];
		cout << " ";
	}
	cout << arr[i-1]; cout << "//"; cout << arr1[i-1];
	delete pTemp;
}

int symbolList::begin_block(int & i){
	/*int r = i + 1;
	i = to_string(r);*/
	i = i+1;
	return i;
}

int symbolList::lookup(string name){
	symbol *look = new symbol();
	look = this->psym;
	if(look == NULL){
		throw Undeclared(name);
	}
	for (int i=0; i < this->size; i++){
		if (look->name == name){
			cout << look->level;
			break;
		} else {
			if(look->snext == NULL && look->name != name) throw Undeclared(name);
		}
		look = look->snext;
	}
	return look->level;
	delete look;
}

int symbolList::end_block(int & k){
	k = k - 1;
	return k;
}