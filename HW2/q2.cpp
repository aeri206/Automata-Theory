#include <iostream>
#include <stdio.h>
#include <map>
#include <set>


using namespace std;

map <char, set<char> > prod_word;
map< pair<char,char>, set<char> > prod_var;

void my_flush(){
		    while(cin.get()!='\n');
}

/*  Input CNF grammar has two form
	(1) A -> BC
	(2) A -> 1
*/

int main() {
	int size;
	char start_var;
	cin >> size;
	my_flush();
	for (int i = 0 ; i < size ; i++){
		
		string line;
		cin >> line;
		if (i==0) start_var = line.at(0);
		if (line.size() ==3){  // case 2 : A -> 1 (A Variable -> An alphabet)
			prod_word[line.at(2)].insert(line.at(0));
		}
		
		else { // A -> BC (A variable -> pair of variable)
			pair<char,char> p_tmp = make_pair(line.at(2),line.at(3));
			prod_var[p_tmp].insert(line.at(0));
		}

	}

	string which;
	cin >> which;
	size = which.length();
	set<char> cyk[size][size]; 	// make matrix for CYK algorithm
	for (int i = 0 ; i < size ; i++){ // Fill CYK(i,i) if there exist variable that make i-th alphabet in string
		if (prod_word.find(which.at(i)) != prod_word.end())
			cyk[i][i].insert(prod_word[which.at(i)].begin(),prod_word[which.at(i)].end());
	}
	/* 
	map <char, set<char> > prod_word;
	map< pair<char,char>, set<char> > prod_var;
	set<char> cyk[size][size];
	*/

	set<char>::iterator set_itr_1;
	set<char>::iterator set_itr_2;

	// Fill Every Entry of CYK algorithm matrix using previos index (Dynamic Programming)
	for (int d = 1 ; d < size ; d++){
		for (int i = 0 ; i < size - d ; i++){
			int j = i + d ;
			for (int k = i ; k < j ; k++){
				// V_ij = V_ij + {A:A->BC, B in V_ik, C in V_k+1,j}
				set<char> B = cyk[i][k];
				set<char> C = cyk[k+1][j];
				for (set_itr_1 = B.begin() ; set_itr_1 != B.end() ; ++set_itr_1){
					for (set_itr_2 = C.begin () ; set_itr_2 != C.end() ; ++set_itr_2){
						char b = *set_itr_1;
						char c = *set_itr_2;
						pair<char, char> tmp = make_pair(b,c);
						if (prod_var.find(tmp)!=prod_var.end()){
							cyk[i][j].insert(prod_var[tmp].begin(),prod_var[tmp].end());
						}
					}
				}
			}
		}
	}
	// check if Start variable exist in cyk[1][n]
	if ((cyk[0][size-1]).find(start_var)!=(cyk[0][size-1]).end())
		cout << "Yes\n";
	else cout << "No\n";
	
	return 0;
}
