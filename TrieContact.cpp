/*************************************************************
*  Project: TrieContact - A Fast Trie-based Contact Manager
*  Description: Console-based C++ contact book with auto-suggestions
*               using two Tries for names and numbers.
*
*  Author: Tushar (@tusharb-25)
*  GitHub: https://github.com/tusharb-25/TrieContact
*  Date: July 2025
*  Language: C++
*************************************************************/


#include<bits/stdc++.h>
using namespace std;

void ART()
{ 
    cout << R"(
        #######   #####   #######   #######   #######   #######   ##    #   #######   #######   #######   #######
           #      #   #      #      #         #         #     #   # #   #      #      #     #   #            #  
           #      #####      #      #######   #         #     #   #  #  #      #      #######   #            #  
           #      # #        #      #         #         #     #   #   # #      #      #     #   #            # 
           #      #   #   #######   #######   #######   #######   #    ##      #      #     #   #######      # 
    )" << '\n';
}

long long totalcontacts=0;  

map<string,string> nametonum;
map<string,string> numtoname;

// Storing name
struct triename{
	map<char, struct triename*> namechild;
	int isleaf;
	triename(){
		isleaf=0;
        namechild.clear();
	}
};

// Storing number
struct trienum{
	struct trienum* numchild[10];
	int isleaf;
	trienum(){
		isleaf=0;
        for(int i=0;i<10;i++) numchild[i]=NULL;
	}
};

// Start TrieContact
struct triename *rootname= NULL;
struct trienum *rootnum= NULL;

// Add contact
void insertname(string& name,string& number){

    // INSERT NAME
	int len = name.length();
	struct triename *itr=rootname;
	int i;
	for(i=0;i<len;i++){
		struct triename *nextnode = (itr->namechild.find(name[i])!=itr->namechild.end()) ? itr->namechild[name[i]] : NULL;
		if(nextnode==NULL){
			nextnode= new triename();
			itr->namechild[name[i]]=nextnode;
		}
		itr= nextnode;
	}
	itr->isleaf=1;
    
}

void insertnum(string& name,string& number){

    // INSERT NUMBER
    int digits = number.length();
	struct trienum *curr=rootnum;
	int i;
	for(i=0;i<digits;i++){
		struct trienum *nextnode = curr->numchild[number[i]-'0'];
		if(nextnode==NULL){
			nextnode= new trienum();
			curr->numchild[number[i]-'0']=nextnode;
		}
		curr= nextnode;
	}
	curr->isleaf=1;

}

void insertcontact(string& name,string& number){
    // INSERT IN TRIE
    insertname(name,number);
    insertnum(name,number);
    totalcontacts++;

    // STORE BOTH TO RETRIVE LATER
    numtoname[number]=name;
    nametonum[name]=number;
}


// RECURSIVE DELETION FROM NAME TRIE
bool deletenamehelper(string& name,int depth,triename* node){
    if(!node) return false;
    if(depth==name.length()){
        if(node->isleaf==0) return false;
        node->isleaf=0;
        if(node->namechild.empty()) return true;
        return false;
    }

    char ch=name[depth];
    auto it = node->namechild.find(ch);
    if(it==node->namechild.end()) return false;

    if(deletenamehelper(name,depth+1,it->second)){
        delete it->second;
        node->namechild.erase(it);
        if(node->isleaf) return false;
        if(node->namechild.empty()) return true;
        return false;
    }

    return false;
}

void deletename(string& name, string& number){
    triename* curr=rootname;
    deletenamehelper(name,0,curr);
    nametonum.erase(name);
}


// RECURSIVE DELETION FROM NUM TRIE
bool deletenumhelper(string& number,int depth,trienum* node){
    if(!node) return false;
    if(depth==number.length()){
        if(node->isleaf==0) return false;
        node->isleaf=0;
        for(int i=0;i<10;i++){
            if(node->numchild[i]!=NULL) return false;
        }
        return true;
    }
    int idx=number[depth]-'0';
    if(deletenumhelper(number,depth+1,node->numchild[idx])){
        delete node->numchild[idx];
        node->numchild[idx]=NULL;
        if(node->isleaf) return false;
        node->isleaf=0;
        for(int i=0;i<10;i++){
            if(node->numchild[i]!=NULL) return false;
        }
        return true;
    }
    return false;
}

void deletenum(string& name, string& number){
    trienum* curr=rootnum;
    deletenumhelper(number,0,curr);
    numtoname.erase(number);
}


//  DELETE A CONTACT
void deletecontact(string&name, string& number){
    deletename(name,number);
    deletenum(name,number);
    totalcontacts--;
}


// DELETE WHOLE TRIECONTACT

// DELETE NAME TRIE
void deleteNameTrie(triename* node){
    for(auto &p:node->namechild){
        deleteNameTrie(p.second);
    }
    delete node;
}

// DELETE NUM TRUE
void deleteNumTrie(trienum* node){
    for(int i=0;i<10;i++){
        if(node->numchild[i]){
            deleteNumTrie(node->numchild[i]);
        }
    }
    delete node;
}

// DELETEION DONE


// PRINT ALL OPTIONS
void instructions()
{
    cout << endl;
    cout << "AVAILABLE OPTIONS:" << endl;
    cout << "Enter 1: ADD A CONTACT" << endl;
    cout << "Enter 2: UPDATE AN EXISTING CONTACT" << endl;
    cout << "Enter 3: DELETE A CONTACT" << endl;
    cout << "Enter 4: SEARCH FOR A CONTACT" << endl;
    cout << "Enter 5: PRINT ALL CONTACTS" << endl;
    cout << "Enter 6: CHECK IF TRIECONTACT IS EMPTY" << endl;
    cout << "Enter 7: TOTAL NUMBER OF CONTACTS PRESENT" << endl;
    cout << "Enter 8: PRINT INSTRUCTIONS" << endl;
    cout << "Enter 9: DELETE ALL CONTACTS" << endl;
    cout << "Enter 0: EXIT TRIECONTACT" << endl;

    return;
}


// VALIDIATE NUMBER
bool validiatenum(string& number){
    int l=number.length();
    for(int i=0;i<l;i++){
        if(isdigit(number[i])) continue;
        return false;
    }
    return true;
}

// CHECK IF ALREADY EXIST
int alreadycontain(string& name, string& number){

    // Check name already exist
    if(nametonum.find(name)!=nametonum.end()){
        return 1;
    }

    // CHECK number already exist
    if(numtoname.find(number)!=numtoname.end()){
        return 2;
    }

    return 0;
}


// suggestiob by nametrie

void dfsname(triename* node,string& prefix,vector<string>& results,int& topk){
    if(results.size()>=topk) return;
    if(node->isleaf){
        results.push_back(prefix);
    }
    for(auto& child : node->namechild){
        prefix.push_back(child.first);
        dfsname(child.second, prefix, results,topk);
        prefix.pop_back();  // backtrack
    }
}

void suggestname(string& name){
    triename* curr = rootname;
    for(char& c : name){
        if(curr->namechild.find(c) == curr->namechild.end()){
            cout << "No suggestions found for given prefix.\n";
            return;
        }
        curr = curr->namechild[c];
    }

    int topk=10;
    vector<string> results;
    string prefix = name;
    dfsname(curr, prefix, results,topk);

    if(results.empty()){
        cout << "No suggestions found.\n";
    }

    else{
        cout << "Other Suggestions:\n";
        for(string& suggestion : results){
            cout << "NAME: " << suggestion << "\tNUMBER: " << nametonum[suggestion] << "\n";
        }
    }
}

// suggestion by numtrie
void dfsnum(trienum* node, string& prefix, vector<string>& results,int& topk){
    if(results.size()>=topk) return;
    if(node->isleaf){
        results.push_back(prefix);
    }
    for(int i = 0; i < 10; i++){
        if(node->numchild[i]){
            prefix.push_back('0' + i);
            dfsnum(node->numchild[i], prefix, results,topk);
            prefix.pop_back(); 
        }
    }
}

void suggestnum(string& number){
    trienum* curr = rootnum;
    for(char c : number){
        int idx = c - '0';
        if(curr->numchild[idx] == NULL){
            cout << "No suggestions found for given prefix.\n";
            return;
        }
        curr = curr->numchild[idx];
    }

    vector<string> results;
    string prefix = number;
    int topk=10;
    dfsnum(curr, prefix, results,topk);

    if(results.empty()){
        cout << "No suggestions found.\n";
    }
    else{
        cout << "Suggestions:\n";
        for(string& suggestion : results){
            cout << "NUMBER: " << suggestion << "\tNAME: " << numtoname[suggestion] << "\n";
        }
    }
}


int main(){

    rootname= new triename();
    rootnum= new trienum();

    ART();
    cout<<"Welcome to TrieContact!\n" 
    <<"A Trie-based contact book built for lightning-fast search, auto-suggestions, and clean scalability.\n";

    instructions();

    string choice="a";
    while(true){
        cout << "\n";
        cout << "CHOOSE ONE OPTION [Press 8 to see the INSTRUCTIONS]: ";

        cin>>choice;
        cout << "\n";


        // ADD CONTACT
        if(choice=="1"){

            string name, number;
            while(true){
                cout<<"ENTER NAME: ";
                cin.ignore();
                getline(cin,name);
                if(name==""){
                    cout << "INVALID INPUT!\n NAME CANNOT BE AN EMPTY STRING. PLEASE TRY AGAIN\n" << endl;
                }
                else break;
            }
            while(true){
                cout<<"\nENTER NUMBER: ";
                cin>>number;
                cout<<"\n";
                // Check validness
                if(validiatenum(number)==false){
                    cout<<"Wrong number entered!! Number can only contain digits[0-9]\n";
                }
                else break;
            }

            int temp= alreadycontain(name,number);
            if(temp==1){
                if(nametonum[name]==number) cout<<"Contact already exist\n";
                else cout<<"Given name already saved with a different number\n";
                continue;
            }
            else if(temp==2){
                if(numtoname[number]==name) cout<<"Contact already exist\n";
                else cout<<"Number already exists with different name\n";
                continue;
            }

            // INSERT UNIQUE ONE
            insertcontact(name,number);
            cout<< name<< " successfully inserted with "<<number<<"\n";

        }


        // UPDATE CONTACT
        else if(choice=="2"){

            cout<<"Update by: \n 1.Update by name\n 2.Update by number\n 3.Cancel updation\n";
            int currchoice=0;
            while(true){
                cout<<"ENTER YOUR CHOICE: ";
                cin>>currchoice;
                cout<<"\n";
                if(currchoice!=1 && currchoice!=2 && currchoice!=3) {
                    cout<<"INVALID CHOICE SELECTED!!\n";
                    continue;
                }
                else break;
            }
            if(currchoice==3){
                cout<<"UPDATION CANCELLED.\n";
                continue;
            }
            if(currchoice==1){
                string oldname;
                while(true){
                    cout<<"ENTER OLD NAME:\t";  
                    cin.ignore();
                    getline(cin, oldname);
                    if (oldname == "") {
                        cout << "INVALID INPUT!\n NAME CANNOT BE AN EMPTY STRING. PLEASE TRY AGAIN\n" << endl;
                    }
                    else break;
                }

                // CHANGE BY NAME
                if(nametonum.find(oldname)!=nametonum.end()){
                    string newname,newnumber;

                    while(true){
                        cout<<"ENTER NEW NAME:\t";
                        getline(cin, newname);
                        if(newname==""){
                            cout << "INVALID INPUT!\n NAME CANNOT BE AN EMPTY STRING. PLEASE TRY AGAIN\n" << endl;
                        }
                        else break;
                    }

                    while(true){
                        cout<<"ENTER NEW NUMBER:   ";  
                        cin>>newnumber;
                        cout<<"\n";
                        // Check validness
                        if(validiatenum(newnumber)==false){
                            cout<<"Wrong number entered!! Number can only contain digits[0-9]\n";
                        }
                        else break;
                    }

                    cout<<"Old contact removed successfully.\n";
                    deletecontact(oldname,nametonum[oldname]);

                    int temp=alreadycontain(newname,newnumber);

                    if(temp==0){
                        insertcontact(newname,newnumber);
                        cout<<"Old contact updated with name "<< newname<<" and number "<<newnumber<<" successfully!!\n";
                        continue;
                    }
                    
                    if(temp==1){
                        if(nametonum[newname]==newnumber) cout<<"Given contact already exist.\n";
                        else cout<<"Given name already saved with a different number.\n";
                        continue;
                    }

                    if(temp==2){
                        if(numtoname[newnumber]==newname) cout<<"Contact already exist.\n";
                        else cout<<"Number already exists with different name.\n";
                        continue;
                    }
                }

                else{
                    cout<<"No contact exists with given name.\n";
                }

            }

            else{
                string oldnumber;

                // Check validness
                while (true){
                    cout<<"\nENTER OLD NUMBER: ";
                    cin>>oldnumber;
                    if(validiatenum(oldnumber)==false){
                        cout<<"Wrong number entered!! Number can only contain digits[0-9]\n";
                    } 
                    else break;
                }

                // CHANGE BY NUMBER
                if(numtoname.find(oldnumber)!=numtoname.end()){
                    string newname,newnumber;
                    cout<<"ENTER NEW NAME: \t";

                    getline(cin, newname);
                    cout<<"ENTER NEW NUMBER: ";  
                    cin>>newnumber;

                    cout<<"Old contact removed successfully.\n";
                    deletecontact(numtoname[oldnumber],oldnumber);
                    

                    int temp=alreadycontain(newname,newnumber);

                    if(temp==0){
                        insertcontact(newname,newnumber);
                        cout<<"Contact updated with name "<< newname<<" and number "<<newnumber<<" successfully!!\n";
                        continue;
                    }
                    
                    if(temp==1){
                        if(nametonum[newname]==newnumber) cout<<"Given updated contact already exist.\n";
                        else cout<<"Given new name already saved with a different number.\n";
                        continue;
                    }

                    if(temp==2){
                        if(numtoname[newnumber]==newname) cout<<"Contact already exist.\n";
                        else cout<<"Given new number already exists with different name.\n";
                        continue;
                    }
                }

                else{
                    cout<<"No contact exist with given number.\n";
                }

            }

        }

        // DELETION OF CONTACT
        else if(choice=="3"){

            cout<<"Delete by: \n 1.Delete by name\n  2.Delete by number\n 3.Cancel deletion\n";
            int currchoice=0;
            while(true) {
                cout<<"Enter your choice: ";
                cin>>currchoice;
                if(currchoice!=1 && currchoice!=2 && currchoice!=3) {
                    cout<<"INVALID CHOICE SELECTED!!\n";
                }
                else break;
            }
            if(currchoice==3){
                cout<<"DELETION CANCELLED.\n";
                continue;
            }

            if(currchoice==1){
                string name;
                while(true){
                    cout<<"ENTER NAME TO BE DELETED: \t";
                    cin.ignore();
                    getline(cin, name);
                    if (name == "") {
                        cout << "INVALID INPUT!\n NAME CANNOT BE AN EMPTY STRING. PLEASE TRY AGAIN" << endl;
                    }
                    else break;
                }

                // DELETE BY NAME
                if(nametonum.find(name)!=nametonum.end()){
                    string number=nametonum[name];
                    deletecontact(name,number);
                    cout<<"CONTACT DELETED SUCCESSFULLY!!\n";
                }
                else{
                    cout<<"No contact exists with given name.\n";
                }
            }
            else{
                string number;

                // Check validness
                while (true){
                    cout<<"\nENTER NUMBER TO BE DELETED: ";
                    cin>>number;
                    if(validiatenum(number)==false){
                        cout<<"Wrong number entered!! Number can only contain digits[0-9]\n";
                    } 
                    else break;
                }

                // DELETE BY NUMBER
               if(numtoname.find(number)!=numtoname.end()){
                    deletecontact(numtoname[number],number);
                    cout<<"CONTACT DELETED SUCCESSFULLY!!\n";
                }
                else{
                    cout<<"No contact exist with given number.\n";
                }
            }
        }

        // SEARCH A CONTACT
        else if(choice=="4"){

            // INPUT WAY
            cout<<"Search a contact by: \n 1.Name Prefix\n  2.Number Prefix\n 3.Cancel Search\n";
            int currchoice=0;
            while(true) {
                cout<<"Enter your choice: ";
                cin>>currchoice;
                if(currchoice!=1 && currchoice!=2 && currchoice!=3) {
                    cout<<"INVALID CHOICE SELECTED!!\n";
                }
                else break;
            }

            // CANCEL SEARCH
            if(currchoice==3){
                cout<<"SEARCHING CANCELLED.\n";
                continue;
            }

            // SEARCH AS CHOICE
            if(currchoice==1){
                string name;
                cout<<"ENTER NAME PREFIX: ";
                cin.ignore();
                getline(cin,name);
                cout<<"\n";
                if(nametonum.find(name)==nametonum.end()){
                    cout<<"No contact exists by exact given name prefix.\n";
                }
                else{
                    cout<<"Contact found!!\n";
                    cout<<"Name: "<<name<<"\t Number: "<<nametonum[name]<<"\n";
                }
                suggestname(name);
            }
            else{
                string number;

                while(true){
                    cout<<"ENTER NUMBER: ";
                    cin>>number;
                    cout<<"\n";
                    if(validiatenum(number)==false){
                        cout<<"Wrong number entered!! Number can only contain digits[0-9]\n";
                    }
                    else break;
                }

                if(numtoname.find(number)==numtoname.end()){
                    cout<<"No contact exists by exact given number.\n";
                }
                else{
                    cout<<"Exact contact found!!\n";
                    cout<<"Name: "<<numtoname[number]<<"\t Number: "<<number<<"\n";
                }
                suggestnum(number);
            }
        }

        // PRINT ALL CONTACTS
        else if(choice=="5"){
            if(totalcontacts==0){
                cout<<"No contacts present in TrieContact.\n";
                continue;
            }
            for(auto&p:nametonum){
                cout<<"NAME: "<<p.first<<"\t";
                cout<<"NUMBER: "<<p.second<<"\n";
            }
        }

        // TRIECONTACT EMPTY
        else if(choice=="6"){
            if(totalcontacts==0) cout<<"TRIECONTACT IS EMPTY\n";
            else{
                cout<<"TRIECONTACT IS NOT EMPTY\n";
                cout<<"TRIECONTACT HAVE " <<totalcontacts<<" contacts. \n";
            }
        }

        // TOTAL CONTACTS
        else if(choice=="7"){
            cout<<"TRIECONTACT HAVE " <<totalcontacts<<" contacts. \n";
        }

        // PRINT INSTRUCTIONS
        else if(choice=="8"){
            instructions();
        }

        // DELETE ALL CONTACTS
        else if(choice=="9"){
            int inp=-1;
            while(true){
                cout << "Do you want to delete all the contacts? [Press 1: YES | Press 0: NO]: ";
                cin >> inp;
                if(inp!=1 && inp!=0){
                    cout<<"ENTERED AN INVALID INPUT!! ENTER AGAIN\n";
                }
                else break;
            }
            
            if(inp==0) {
                cout<<"Action cancelled!\n";
                continue;
            }
            else{
                nametonum.clear();
                numtoname.clear();
                deleteNameTrie(rootname);
                deleteNumTrie(rootnum); 
                cout<<"ALL CONTACTS DELETED.\n";
                totalcontacts=0;

                // recreate
                rootname= new triename();
                rootnum=new trienum();
            }
        }

        // EXIT TRIECONTACT
        else if(choice=="0"){
            break;
        }

        else{
            cout<<"INVALID OPTION SELECTED!!\n";
        }

    }
    deleteNameTrie(rootname);
    deleteNumTrie(rootnum);
    nametonum.clear();
    numtoname.clear();
    cout<<"THANKS FOR USING TRIECONTACT:)\n";
    return 0;
}
