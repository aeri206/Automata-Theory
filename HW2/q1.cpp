#include <iostream>
#include <stdio.h>
#include <map>
#include <set>
#include <vector>

using namespace std;


void my_flush(){
    while(cin.get()!='\n');
}


int main() {
	
	int size;
    cin >> size;
    my_flush();
	
	map< char , set<char> > unit_pro_set;
	map< char , vector<char> > unit_pro_vec;
	map< char , set<char> > one_letter;
	map< char , set<string> > prod;
	set<char> input;
	set<char> term_in_str;
	set<char> term;
	char start;
	
	/* Take Input And Categorize into 3 group
	 (1) A->B (A Variable -> A Variable) save in unit_pro_set & unit_pro_vec
	 (2) A-> 1 (A variable -> A alphabet) save in one_letter
	 (3) A -> xyz (A variable -> combination of Var & alphabet (more than 2)) save in prod 
	 Input has all input variable & Terminal
	 */

	for (int i = 0 ; i < size ; i++){
		string line;
		cin >> line;
		if (i==0) start = line.at(0);
			if (line.length() == 3){ // Case (1) or Case (2)
				char from = line.at(0);
				char to = line.at(2);
				input.insert(from); 
				input.insert(to);
				// 'A' < 'z' < 'a' < 'z'
				if (('A' <= to) && ('z' >= to)) { // Case (1)
					unit_pro_set[from].insert(to);
					unit_pro_vec[from].push_back(to);
					}
				else one_letter[from].insert(to); // Case (2)
			}
			else { // Case (3)
				char from = line.at(0);
				string to = line.substr(2);
				prod[from].insert(to);
				input.insert(from);
				for (int i = 0 ; i < to.length() ; i++) {
					input.insert(to.at(i));
					term_in_str.insert(to.at(i));
				}
			}
	} 	
	

	/* Check All member in Input and move Terminal (except number) into set<char> term */

	set<char>::iterator char_itr;
	char_itr = input.begin();
	/* All term + - * / ( ) is small than char '0' -> move into set term*/
	while ((*char_itr) <'0'){
		if (term_in_str.find(*char_itr)!=term_in_str.end()){
		term.insert(*char_itr);
		input.erase(*char_itr);
		}
		char_itr++;
	}
	
	/* To Change All term except number into alphabet or number */
	map<char,char>term_replace; // if any term change, store in map <char,char> term_replace : first -> second
	
	char A = 'A';
	char a = 'a';
	char num = '0';
	bool change = false;

	for (char_itr = term.begin() ; char_itr!=term.end() ; ++char_itr){
		for (;A<='Z';A++){
			if (input.find(A)==input.end()){
			one_letter[A].insert(*char_itr);
			term_replace[*char_itr] = A;
			A++;
			change = true;
			break;
			}
		}
		if (change){
			change = false;
			continue;
		}
		for (;a<='z';a++){
			if (input.find(a)==input.end()){
			one_letter[a].insert(*char_itr);
			term_replace[*char_itr] = a;
			a++;
			change = true;
			break;
			}
		}
		if (change){
			change = false;
			continue;
		}
		for (;num<='9';num++){
			if (input.find(num)==input.end()){
			one_letter[num].insert(*char_itr);
			term_replace[*char_itr] = num;
			num++;
			change = true;
			break;
			}
		}
		if (change){
			change = false;
			continue;
		}
	}
 	map<char,char>::iterator tmp_itr;
	
	/* Change all term in string to variable */
	map <char, set<string> >::iterator string_itr;
		for (string_itr = prod.begin() ; string_itr != prod.end() ; ++string_itr){
			set<string>::iterator it_str;
			set<string> new_string;
			for (it_str = ((*string_itr).second).begin() ; it_str != ((*string_itr).second).end() ; ++it_str){
				string old = *it_str;
				for (tmp_itr = term_replace.begin() ; tmp_itr!=term_replace.end() ; ++tmp_itr){
					if (old.find((*tmp_itr).first)!=string::npos) {
						string new_str;
						for (int i = 0 ; i < old.length() ; i++){
							if (old.at(i) == (*tmp_itr).first) new_str+=(*tmp_itr).second;
							else new_str+=old.at(i);
						}
						old = new_str;
						}
					}
					new_string.insert(old);
			}
			prod[(*string_itr).first] = new_string;
		}
	
	
	
/*		find all unit production by merging data A*=>B 
		After this, all unit production will be stored in unit_pro_set
*/
		map< char, vector<char> >::iterator iter;
		for (iter = unit_pro_vec.begin() ; iter!= unit_pro_vec.end() ; ++iter){
			char key = (*iter).first;			
			for (int i = 0 ; i < unit_pro_vec[key].size();i++){
			char tmp = unit_pro_vec[key].at(i);
			vector<char> insert = unit_pro_vec[tmp];
			for (int j = 0 ; j < insert.size() ; j++){
				if (unit_pro_set[key].find(insert.at(j))==unit_pro_set[key].end()){
					unit_pro_vec[key].push_back(insert.at(j));
					unit_pro_set[key].insert(insert.at(j));
					}
				}
			}
		}

		map< char, set<char> >::iterator iter2;


/* A*=> B, B=>1 : (Add) A=>1 
	Merge (1) Unit_pro_set && (2) one_letter transition
*/
		map< char, set<char> >::iterator itr;
		for (itr = unit_pro_set.begin() ; itr != unit_pro_set.end() ; ++itr){
			char key = (*itr).first;
			set <char> tmp = (*itr).second; // if unit_pro_set has < A, <B,C,D> >
			set <char>::iterator set_itr;
			for (set_itr = tmp.begin() ; set_itr != tmp.end() ; ++set_itr){ // *set_itr is B,C,D
				if (one_letter.find(*set_itr)!=one_letter.end())
				one_letter[key].insert(one_letter[(*set_itr)].begin(),one_letter[(*set_itr)].end());
			}
		}

/* A*=> B, B=>xxx : (Add) A=>xxx 
	Merge (1) Unit_pro_set && (3) production (string)
*/

		for (itr = unit_pro_set.begin() ; itr != unit_pro_set.end() ; ++itr){
			char key = (*itr).first;
			set <char> tmp = (*itr).second; // if unit_pro_set has < A, <B,C,D> >
			set <char>::iterator set_itr;
			
			for (set_itr = tmp.begin() ; set_itr != tmp.end() ; ++set_itr){ // *set_itr is B,C,D
				if (prod.find(*set_itr)!=prod.end()) 
				prod[key].insert(prod[(*set_itr)].begin(),prod[(*set_itr)].end());
			}			
		}

		map<string, set<pair<string, string> > > Final;
	
 		for (string_itr = prod.begin() ; string_itr != prod.end() ; ++string_itr){
			string key;
			key = (*string_itr).first;
			set<string> str_set = (*string_itr).second;
			set<string>::iterator it_str;
			for (it_str = str_set.begin() ; it_str != str_set.end() ; ++it_str){
				if ((*it_str).length()==2){
					string first,second;
					if ( ((*it_str).at(0)>='0') && ((*it_str).at(0)<='9') ) {
						one_letter[(*it_str).at(0)].insert((*it_str).at(0));}
					if ( ((*it_str).at(1)>='0') && ((*it_str).at(1)<='9') ) {
						one_letter[(*it_str).at(1)].insert((*it_str).at(1));}
					first = (*it_str).at(0);
					second = (*it_str).at(1);
					(Final[key]).insert(make_pair(first,second));
					continue;
				}
				else {
					string first,second; // X->ABCD
					first = (*it_str).at(0);  // A
					if ( ((*it_str).at(0)>='0') && ((*it_str).at(0)<='9') ){
						one_letter[(*it_str).at(0)].insert((*it_str).at(0));
					}
					second = (*it_str).substr(1,(*it_str).length()-1); 
					(Final[key]).insert(make_pair(first,second));
				for (int len = second.length() ; len>1 ; len--){
					string tmp_key,val1,val2;
					tmp_key = second.substr(second.length()-len);
					val1 = second.at(second.length()-len);
					if ( (second.at(second.length()-len)>='0') && (second.at(second.length()-len)<='9') ){
						one_letter[second.at(second.length()-len)].insert(second.at(second.length()-len));}
					val2 = second.substr(second.length()-len+1);
					(Final[tmp_key]).insert(make_pair(val1,val2));
					}
					if ( (second.at(second.length()-1)>='0') && (second.at(second.length()-1)<='9') ) {
						one_letter[second.at(second.length()-1)].insert(second.at(second.length()-1));
						}
				}
			}
		}		

	int grammar_num = 0;
	map<string, set<pair<string, string> > >::iterator final_itr;
		for (final_itr = Final.begin() ; final_itr != Final.end() ; ++final_itr){
			set<pair<string, string> >::iterator final_set_itr;
			set<pair<string,string> > set_tmp = (*final_itr).second;
			for (final_set_itr = set_tmp.begin() ; final_set_itr != set_tmp.end() ; ++final_set_itr){
				grammar_num++;
			}
		}

		for (iter2 = one_letter.begin() ; iter2 != one_letter.end() ; ++iter2){
			set<char> set_tmp = (*iter2).second;
			set <char>::iterator ixr;
			for (ixr = set_tmp.begin(); ixr != set_tmp.end() ; ++ixr){
				grammar_num++;
			}
		}
		cout << grammar_num << endl;

		string start_str;
		start_str = start;
		
		//Print Final String Transition by Start variable
		if(Final.find(start_str)!=Final.end()){

			set<pair<string, string> >::iterator final_set_itr;
			set<pair<string,string> > set_tmp = (*Final.find(start_str)).second;
			for (final_set_itr = set_tmp.begin() ; final_set_itr != set_tmp.end() ; ++final_set_itr){
				cout << "<" << (*Final.find(start_str)).first << ">:<" << (*final_set_itr).first << "><" << (*final_set_itr).second << ">" <<endl;
			}
			Final.erase(start_str);
		}

		if (one_letter.find(start)!=one_letter.end()){

			set<char> set_tmp = (*one_letter.find(start)).second;
			set <char>::iterator ixr;
			for (ixr = set_tmp.begin(); ixr != set_tmp.end() ; ++ixr){
				cout << "<" << (*one_letter.find(start)).first << ">:" << *ixr <<endl;
			}
			one_letter.erase(start);
		}

		// Print Final String Transition
	
		for (final_itr = Final.begin() ; final_itr != Final.end() ; ++final_itr){
			set<pair<string, string> >::iterator final_set_itr;
			set<pair<string,string> > set_tmp = (*final_itr).second;
			for (final_set_itr = set_tmp.begin() ; final_set_itr != set_tmp.end() ; ++final_set_itr){
				cout << "<" << (*final_itr).first << ">:<" << (*final_set_itr).first << "><" << (*final_set_itr).second << ">" <<endl;
			}

		} 
			
		for (iter2 = one_letter.begin() ; iter2 != one_letter.end() ; ++iter2){
			set<char> set_tmp = (*iter2).second;
			set <char>::iterator ixr;
			for (ixr = set_tmp.begin(); ixr != set_tmp.end() ; ++ixr){
				cout << "<" << (*iter2).first << ">:" << *ixr <<endl;
			}
		}



		return 0;
}
	
