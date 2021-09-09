#include "SymbolTable.h"

void tokenize (string cuts[], string s, string del = " "){
    //Used for testcase tokenizer, students don't need to take care of this function.
    //vector<string> res;
    int start = 0;
    int end = (int)s.find(del);
    int i = 0;
    while (end != -1) {
        cuts[i] = s.substr(start, end - start);
        i++;
        start = (int)end + (int)del.size();
        end = (int)s.find(del, start);
    }
    cuts[i] = s.substr(start, end - start);
    return;
}
bool checkCharInName(string checkName) {
    if (checkName[0] < 'a' || checkName[0] > 'z') return false;
    for (int i = 1; i < checkName.size(); i++) {
        if( (checkName[i] >= 'a' && checkName[i] <= 'z') ||
        (checkName[i] >= 'A' && checkName[i] <= 'Z') ||
        checkName[i] == '_' || (checkName[i]>= '0' && checkName[i] <= '9'))
            continue;
        else return false;
    }
    return true;
}
bool checkValueType(string str) {
    // nhan so dau tien khac 0 fixbug
    if (str[0] >= '0' && str[0] <= '9') {
        for (size_t i = 1; i < str.size(); i++) {
            if(str[i] < '0' || str[i] > '9') return false;
        }
        return true;
    }
    else if (int(str[0]) == 39 && int(str[str.size() - 1]) == 39) {
        for (size_t i = 1; i < str.size() - 1; i++) {
            if( !(str[i] >= '0' &&  str[i] <= '9') ||
            !( (str[i] >= 'a') && (str[i] <= 'z')) ||
            !( str[i] >= 'A' && str[i] <= 'Z') ||
            str[i] != ' ' ) 
                return false;
        }
        return true;
    }
    else {
        if(!checkCharInName(str)) return false;
            return true;
    }
    return false;
}
bool checkInstruction(string cutS[]) {
    if(cutS[0] == "BEGIN"|| cutS[0] == "END") {
        if(cutS[1] == "" && cutS[2] == "" ) return true; return false;
    }
    else if (cutS[0] == "INSERT") {
        
        if (!(cutS[2] == "string" || cutS[2] == "number"))  return false;

        if (!checkCharInName(cutS[1])) return false;
        
        return true;
    }
    else if (cutS[0] == "ASSIGN") {
        if (!checkCharInName(cutS[1]) || !checkValueType(cutS[2]))
            return false;
        return true;
    }
    else if (cutS[0] == "LOOKUP") {
        if (!checkCharInName(cutS[1]) || cutS[2] != "") return false;
        return true;
    }
    return false;
}
void SymbolTable::run(string filename) {
    ifstream fileIn;
    fileIn.open(filename, ios:: in);

    while(!fileIn.eof()) {
        char c[500];
        string s, name, typeOVal, ide;
        
        fileIn.getline(c,500);
        s = c;
        string cutS[4];
        tokenize(cutS, s, " ");
        if (!checkInstruction(cutS)) 
            throw InvalidInstruction(s);
        
        ide = cutS[0];

        if(ide == "BEGIN") {

        }
        else if(ide == "END") {

            //delete cac bien trong scope
        }
        else {
            name = cutS[1];
            typeOVal = cutS[2];
            //delete cutS;
            if (ide == "INSERT") {
                Node* e = new Node(name, typeOVal, "",scopeCur);
                if (containsInScope(record, name, scopeCur))
                    throw Redeclared(s);
                
                insert(record, e);
                //loi redeclared
            }
            else if (ide == "ASSIGN") {
                if(typeOVal[0] >= '0' && typeOVal[0] <= '9') {
                    int cnt0 = 0;
                    for (size_t i = 0; s[i] != 0; i++)
                        cnt0 ++;
                    typeOVal.erase(0,cnt0);
                    
                    
                }
                else if (typeOVal[0] >= 'a' && typeOVal[0] <= 'z') {
                
                    /* code */
                }
                else if ((int)typeOVal[0] == 39)
                {
                    /* code */
                }
                
                


                // loi undeclared
                // type mismatch
                
            }
            
            else if (ide == "LOOKUP") {
                

                // undeclared
            }
        }
        
        cout << "success\n" ;
    }
    fileIn.close();
}

bool SymbolTable :: insert(RecordVar& record, Node* element){
    if(record.head == NULL) {
        record.head = element;
        record.head ->next = NULL;
        record.size ++; return true;
    }
        
    else  {
        Node *p = record.head;
        while (p->next)
            p = p -> next;
        p -> next = element;
        if(p != record.head) delete p;
        record.size ++;
        return true;
    }
    
    //else return false;
}

bool SymbolTable :: isEmpty(RecordVar& record) {
    if (record.size == 0 ) return true;
    else return false;
}

bool SymbolTable :: containsInScope(RecordVar& record, string name, int scope) {
    if(record.head == nullptr && record.size == 0) return false;
    Node* p = record.head;
    while(p) {
        if(p->name == name && p->scope == scope) {delete p; return true;}
        p = p ->next;
    }
    delete p; return false;
}

bool SymbolTable :: searchAndAssign(RecordVar& record, string name) {
    if(record.head == nullptr && record.size == 0) return false;
    Node* p = record.head;
    while(p) {
        if (p->name == name) {delete p; return true;}
        p = p->next;
    }
    delete p; return false;
}


