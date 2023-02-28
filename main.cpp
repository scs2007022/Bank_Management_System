#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>
#include <limits>

using namespace std;

class account{
    private:
        int amount;
        string password;
        bool logged = false;
    public:
        int no;
        string name;
        bool login(){
            string input;
            cout << "Please Input Your Password:\t";
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cin >> input;
            if (password==input){
                logged = true;
                return true;
            }
            else{
                logged = false;
                cout << "\nIncorrect Password." << endl;
            }
            return false;
        }
        void logout(){logged = false;}

        void detail(){
            if (!logged)return;
            cout << "BALANCE DETAILS\n" << endl;
            cout << "Account No.:        \t\t" << no << endl;
            cout << "Account Holder Name:\t\t" << name << endl;
            cout << "Balance amount:     \t\t" << amount << endl; 
        }

        void deposit(){
            int deposit_amount;
            cout << "Please input the amount of money deposit:\t";
            cin >> deposit_amount;
            amount += deposit_amount;
            cout << "The amount is deposited.";
        }

        void withdraw(){
            if (!logged)return;
            int withdraw_amount;
            cout << "Please input the amount of money withdrawal:\t";
            cin >> withdraw_amount;
            if (withdraw_amount>amount){
                cout << "Not enought ssaving."<<endl;
                return;
            }
            amount -= withdraw_amount;
            cout << "The amount is withdrawn.";
        }

        ~account(){
            if (!logged)return;
            cout << "Remaining amount:\t"<<amount<<"\nThe account is now closed." << endl;
        }
        
        account(string n, string N, string a, string p){
            no=stoi(n);
            name=N;
            amount=stoi(a);
            password=p;
            logged = false;
        }
        account(int n, string N, int a, string p){
            no=n;
            name=N;
            amount=a;
            password=p;
            logged = false;
        }
};

account* find_account(std::map<int,account> &database){
    int account_no;
    cout << "Please Input the Account No:\t";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin >> account_no;
    map<int,account>::iterator ac_it = database.find(account_no);
    if (ac_it == database.end()){
        cout << "\nAccount not found." << endl;
        return nullptr;
    }
    return &ac_it->second;
    // return ;
}

void create(std::map<int,account> &database,int &current_account_no){
    string name;
    string currency;
    string password;
    int amount;
    cout << "\n------------------------------------------"<<endl;

    cout << "\nPlease Input Your Full Name:\t";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(cin,name);

    cout << "\nPlease Input Your Password:\t";
    // cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin >> password;

    currency = "$";
    amount = 0;
    current_account_no++;
    database.insert(std::make_pair(current_account_no,account(current_account_no,name,amount,password)));
    return;
}
void deposit(std::map<int,account> &database){
    cout << "\n------------------------------------------"<<endl;
    account* ptr = find_account(database);
    if (ptr==nullptr)return;
    ptr->deposit();
    return;
}
void withdraw(std::map<int,account> &database){
    cout << "\n------------------------------------------"<<endl;
    account* ptr = find_account(database);
    if (ptr==nullptr)return;
    ptr->login();
    ptr->withdraw();
    ptr->logout();
    return;
}
void enquiry(std::map<int,account> &database){
    cout << "\n------------------------------------------"<<endl;
    account* ptr = find_account(database);
    if (ptr==nullptr)return;
    ptr->login();
    ptr->detail();
    ptr->logout();
}
void holder(std::map<int,account> &database){
    cout << "\n------------------------------------------"<<endl;
    cout << std::setw(12) << "Account No.:" << std::setw(18) << "Account Holder" << endl;
    cout << std::setw(12) << "------------" << std::setw(18) << "--------------" << endl;
    for (auto it=database.begin();it!=database.end();++it){
        cout << std::setw(12) << it->first << std::setw(18) << it->second.name << endl;
    }
    return;
}
void close(std::map<int,account> &database){
    cout << "\n------------------------------------------"<<endl;
    int account_no;
    
    cout << "Please Input the Account No:\t";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin >> account_no;

    map<int,account>::iterator ac_it = database.find(account_no);
    if (ac_it == database.end()){
        cout << "\nAccount not found." << endl;
        return;
    }
    if(ac_it->second.login()){
        // ac_it->second.~account();
        database.erase(ac_it);
    }
    return;
}

void loading_existing_record(std::map<int,account> &database,int &current_account_no){
    std::ifstream record("record.csv");
    while(record.is_open()){
        std::string line;
        while(std::getline(record,line)){
            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string value;
            while (std::getline(ss,value,',')){
                row.push_back(value);
            }
            database.insert(std::make_pair(stoi(row[0]),account(row[0],row[1],row[2],row[3])));
            if (stoi(row[0])>current_account_no)current_account_no = stoi(row[0]);
        }
    }
    record.close();
}

int menu(){
    cout << "\n------------------------------------------"<<endl;
    cout << "\nMAIN MENU\n" << endl;
    cout << "01. NEW ACCOUNT" << '\n' << endl;
    cout << "02. DEPOSIT AMOUNT" << '\n' << endl;
    cout << "03. WITHDRAW AMOUNT" << '\n' << endl;
    cout << "04. BALANCE ENQUIRY" << '\n' << endl;
    cout << "05. ALL ACCOUNT HODER LIST" << '\n' << endl;
    cout << "06. CLOSE AN ACCOUNT" << '\n' << endl;
    cout << "07. EXIT" << '\n' << endl;

    cout << "Select Your Option (1-7):\t";
    int res;
    // cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin >> res;

    return res;   
}

int main(){

    cout << "------------------------------------------" << endl;
    cout << "BANK MANAGEMENT SYSTEM" << endl;
    cout << "\n";
    cout << "MADE BY:\tLO Tsz Yu, Brian" << endl;
    cout << "------------------------------------------" << endl;

    cout << "Loading..." << endl;

    std::map<int, account> database;
    // std::vector<account> database;
    int current_account_no = 0;

    loading_existing_record(database,current_account_no);

    int select;
    while (true){
        select = menu();
        if (select==1){create(database,current_account_no);continue;}
        else if (select==2){deposit(database);continue;}
        else if (select==3){withdraw(database);continue;}
        else if (select==4){enquiry(database);continue;}
        else if (select==5){holder(database);continue;}
        else if (select==6){close(database);continue;}
        else if (select==7)return 1;
        else {
            cout << "Incorrect Input.\n" << endl;
        }
    }
    return 1;
}