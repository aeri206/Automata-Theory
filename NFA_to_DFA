#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <list>
#include <map>


using namespace std;

// to store e-trans function in NFA
map< int , set <int> > e_trans;

struct NFA_s{
    int state_num;
    set<int> trans_0;
    set<int> trans_1;
    set<int> trans_e;
    bool final_state;
}NFA_s;

struct DFA_s{
    set<int> n_state;
    int state_num;
    int trans_0;
    int trans_1;
    bool final_state;
}DFA_s; 

// get string with int parsed by comma, and return parsed Int inside set structure
set<int> parseInt(string str){
                set<int> v;
                string tmp;
                stringstream ss(str);
                int in;
                while (getline(ss,tmp,',')){
                    stringstream stream(tmp);
                    stream >> in;
                    v.insert(in);
                }
                return v;
            }


// clean input buffer before using getline function
void my_flush(){
    
    while(cin.get()!='\n');

}

// to fill e-transition function
void test_e(struct NFA_s* NFA, int init_state, int find_state){ // n-th member of NFA의　trans를 map형태의 e_trans에　추가하라．　
    
    if (NFA[find_state].trans_e.empty()) return;   

    set<int>::iterator itr;
    set<int>::iterator test;
    map<int,set<int> >::iterator iter_map;    
    
    for (itr = NFA[find_state].trans_e.begin();itr !=  NFA[find_state].trans_e.end();itr++){
        // check all result from e-trans from find state

        iter_map = e_trans.find(init_state); // to check where that init_state has set<int>value
        
        set<int> tmp = iter_map -> second; // cp tmp set of int to store
        test = tmp.find(*itr); // find if there is *itr in map (already exist)
        
        if (test != tmp.end()) continue; // when already *itr exist in set

        else {
            tmp.insert(*itr); // does not exist -> insert 
            e_trans[init_state]=tmp; // store new set (with *itr store)
            test_e(NFA,init_state,*itr); // check that state's trans also
        }
    }
}


int main() {

    // get nfa_size as int
    int nfa_size;
    cin >> nfa_size;
    my_flush();
    
// get NFA as input and make it into DFA
    // make array with NFA structure
    struct NFA_s NFA[nfa_size];

    // get input && save data in struct
	for (int i = 0 ; i < nfa_size ; i++){

        struct NFA_s NFA_in;
        int num;
        bool final_state;
        string zero_in;
        string one_in;
        string none_in;

        string line;
        getline(cin,line);
        istringstream ss(line);
        ss >> num >> final_state >> zero_in >> one_in >> none_in;
        NFA_in.state_num = num;
        NFA_in.final_state = final_state;
        
        // if transition state isn't - : there exist transition by 0,1,e
        if (zero_in.compare("-")!=0){
            NFA_in.trans_0 = parseInt(zero_in);}
        
         if (one_in.compare("-")!=0){
            NFA_in.trans_1 = parseInt(one_in);}
        
        if (none_in.compare("-")!=0){
            NFA_in.trans_e = parseInt(none_in);
        }
        // save new NFA state into NFA array
        NFA[num]=NFA_in;
	} 


// make transition function by e(epsilon)

    for (int i = 0 ; i < nfa_size ; i++){
        if (!NFA[i].trans_e.empty()){ // only when e-trans from that state exist
            set<int> s;
            e_trans[i]=s;
            test_e(NFA,i,i); // call function to check next state by e
        }
    }

    vector <struct DFA_s> DFA; // declare empty vector to fill with new DFA states.
    
    int index = 0;
    
    struct DFA_s DFA_in;

    // to do only with initial state
    set<int> init_n_state;
    init_n_state.insert(index);
    set<int>it = e_trans.find(index) -> second;
    init_n_state.insert(it.begin(),it.end());
    DFA_in.n_state = init_n_state;
    DFA_in.state_num = 0;
    DFA.push_back(DFA_in);
    
    int i=0;
    
    // for every state from init state to end, check what they will be after transition by 0,1
    for (int i = 0 ; i < DFA.size() ; i++){
        
        set<int> present_state = DFA.at(i).n_state;
        set<int>::iterator itr;
        
        DFA.at(i).final_state = 0;

        // check that state is final or not
        for (itr = present_state.begin();itr!=present_state.end();itr++){
            if (NFA[*itr].final_state) {
                DFA.at(i).final_state = 1;
                break;
            }
        }

        set<int> zero_trans;
        set<int> one_trans;
        set<int> tmp;
            
            // check transition by 0
            for (itr = present_state.begin() ; itr !=  present_state.end() ; itr++){
                zero_trans.insert(NFA[*itr].trans_0.begin(),NFA[*itr].trans_0.end());
            }
            
            // check transition by e after transition by 0 (0->e->e...->e)
            // 'it' is set<int> 
            for (itr = zero_trans.begin() ; itr != zero_trans.end() ; itr++ ){
                it = e_trans.find(*itr) -> second;
                tmp.insert(it.begin(),it.end());
            }
            
            // complete after zero trans, what states of nfa are in that state
            zero_trans.insert(tmp.begin(),tmp.end());
            
            //check whether new_state or not
            bool new_state = true;
            for (int ck = 0 ; ck < DFA.size();ck++){
                if((DFA.at(ck).n_state) == zero_trans) {
                    //there exist state
                    DFA.at(i).trans_0 = DFA.at(ck).state_num;
                    new_state = false;
                    break;
                     }
            }
            // if it is new state
            if (new_state){
                // make new state 
                DFA_in.state_num = DFA.size();
                DFA.at(i).trans_0 = DFA_in.state_num;
                DFA_in.n_state = zero_trans;
                DFA.push_back(DFA_in);
            }
            
            //check transition by 1
            tmp.clear();
            for (itr = present_state.begin() ; itr !=  present_state.end() ; itr++){
                one_trans.insert(NFA[*itr].trans_1.begin(),NFA[*itr].trans_1.end());
            }

            for (itr = one_trans.begin() ; itr != one_trans.end() ; itr++ ){
                set<int>it = e_trans.find(*itr) -> second;
                tmp.insert(it.begin(),it.end());
            }

            one_trans.insert(tmp.begin(),tmp.end());
            
            //check new_state
            new_state = true;
            for (int ck = 0 ; ck < DFA.size();ck++){
                if((DFA.at(ck).n_state) == one_trans) {
                    //there exist state
                    DFA.at(i).trans_1 = DFA.at(ck).state_num;
                    new_state = false;
                    break;
                     }
            }
            if (new_state){
                DFA_in.state_num = DFA.size();
                DFA.at(i).trans_1 = DFA_in.state_num;
                DFA_in.n_state = one_trans;
                DFA.push_back(DFA_in);
            }
    }

// print result
cout << DFA.size() << endl;
    for (i = 0 ; i < DFA.size() ; i++) {
        printf("%d %d %d %d\n",DFA.at(i).state_num , DFA.at(i).final_state ? 1 : 0 ,DFA.at(i).trans_0,DFA.at(i).trans_1);
    }
    return 0;
}
