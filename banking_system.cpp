// Banking System 

// 6. iostream.h (modern header iostream used instead of old iostream.h)
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>   // 10. Derived datatype (vector)
#include<map>      // 10. Derived datatype (map)

using namespace std;

#define MIN_BALANCE 500   // 9. Token (macro definition)

class InsufficientFunds{};   // User defined datatype


// 2. Class
class Account
{
    
// 5. Encapsulation (private data hidden from outside)
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    
    static long NextAccountNumber;   // Static Data Member

    // 8. Private Member Function (Nesting of member function)
    void secretNote() {
        cout << "This is a private note\n";
    }

public:

    Account(){}

    // 15. Function Prototyping
    Account(string fname,string lname,float balance);

    long getAccNo(){return accountNumber;}
    string getFirstName(){return firstName;}
    string getLastName(){return lastName;}
    float getBalance(){return balance;}
    
    void Deposit(float amount);
    void Withdraw(float amount);

    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();
    
    // 12. Friend Function
    friend ofstream & operator<<(ofstream &ofs,Account &acc);
    friend ifstream & operator>>(ifstream &ifs,Account &acc);
    friend ostream & operator<<(ostream &os,Account &acc);
};


// 11. Scope Resolution Operator
long Account::NextAccountNumber = 1000;


// 4. Inheritance (Demo Example)
class PremiumAccount : public Account
{
public:
    PremiumAccount(string fname,string lname,float balance)
        : Account(fname,lname,balance){}

    // 3. Polymorphism (Function Overriding Example)
    void specialFeature()
    {
        cout<<"Premium Account Feature Activated\n";
    }
};


// Bank Class
class Bank
{
private:
    map<long,Account> accounts;   // 10. Derived datatype (map)

public:
    Bank();

    Account OpenAccount(string fname,string lname,float balance=0); // 20. Default Argument
    
    // 17. Call by value
    Account BalanceEnquiry(long accountNumber);
    
    Account Deposit(long accountNumber,float amount);
    Account Withdraw(long accountNumber,float amount);
    
    void CloseAccount(long accountNumber);
    void ShowAllAccounts();
    ~Bank();
};


// 19. Inline function
inline void greet(string &name ) {
    cout << "Hello, " << name<< "! Welcome to the program." << endl;
}


// 21. Function Overloading (Sample Code)
int add(int a,int b){ return a+b; }
float add(float a,float b){ return a+b; }


// 18. Return by reference (Sample Code)
float& updateBalanceReference(float &bal){
    return bal;
}


// 7. Return type of main
int main()
{
    // 1. Object
    Bank b;
    Account acc;
    
    // 8. Variable/operator/cin/cout
    int choice;
    string fname,lname;
    
    long accountNumber;
    float balance;
    float amount;

    cout<<"***Banking System***"<<endl;

    do
    {
        cout<<"\n\t1 Open Account";
        cout<<"\n\t2 Balance Enquiry";
        cout<<"\n\t3 Deposit";
        cout<<"\n\t4 Withdrawal";
        cout<<"\n\t5 Close Account";
        cout<<"\n\t6 Show All";
        cout<<"\n\t7 Quit";
        cout<<"\nEnter choice: ";
        cin>>choice;

        switch(choice)
        {
            case 1:
                cout<<"Enter First Name: ";
                cin>>fname;

                cout<<"Enter Last Name: ";
                cin>>lname;

                cout<<"Enter Balance: ";
                cin>>balance;

                acc=b.OpenAccount(fname,lname,balance);

                greet(fname);

                cout<<acc;

                break;

            case 2:
                cout<<"Enter Account Number:";
                cin>>accountNumber;

                acc=b.BalanceEnquiry(accountNumber);

                cout<<endl<<"Your Account Details"<<endl;

                cout<<acc;

                break;

            case 3:
                cout<<"Enter Account Number:";
                cin>>accountNumber;

                cout<<"Enter Amount:";
                cin>>amount;

                acc=b.Deposit(accountNumber, amount);

                cout<<acc;

                break;

            case 4:
                cout<<"Enter Account Number:";
                cin>>accountNumber;

                cout<<"Enter Amount:";
                cin>>amount;

                try{
                    acc=b.Withdraw(accountNumber, amount);
                    cout<<acc;
                }
                catch(InsufficientFunds){
                    cout<<"Insufficient Balance!\n";
                }

                break;

            case 5:
                cout<<"Enter Account Number:";
                cin>>accountNumber;

                b.CloseAccount(accountNumber);

                break;

            case 6:
                b.ShowAllAccounts();

                break;

            case 7:
                break;

            default:
                cout<<"Wrong Choice";
                exit(0);
        }

    }while(choice!=7);

    return 0;
}


// ================== FUNCTION DEFINITIONS ==================

Account::Account(string fname,string lname,float balance)
{
    NextAccountNumber++;

    accountNumber=NextAccountNumber;

    firstName=fname;

    lastName=lname;

    this->balance=balance;  // 14. this pointer
}


void Account::Deposit(float amount)
{
    balance+=amount;
}


void Account::Withdraw(float amount)
{
    if(balance-amount<MIN_BALANCE)
        throw InsufficientFunds();

    balance-=amount;
}


void Account::setLastAccountNumber(long accountNumber)
{
    NextAccountNumber=accountNumber;
}


long Account::getLastAccountNumber()
{
    return NextAccountNumber;
}


// Friend function (File handling)

ofstream & operator<<(ofstream &ofs,Account &acc)
{
    ofs<<acc.accountNumber<<endl;
    ofs<<acc.firstName<<endl;
    ofs<<acc.lastName<<endl;
    ofs<<acc.balance<<endl;

    return ofs;
}


ifstream & operator>>(ifstream &ifs,Account &acc)
{
    ifs>>acc.accountNumber;
    ifs>>acc.firstName;
    ifs>>acc.lastName;
    ifs>>acc.balance;

    return ifs;
}


ostream & operator<<(ostream &os,Account &acc)
{
    os<<"First Name:"<<acc.getFirstName()<<endl;
    os<<"Last Name:"<<acc.getLastName()<<endl;
    os<<"Account Number:"<<acc.getAccNo()<<endl;
    os<<"Balance:"<<acc.getBalance()<<endl;

    return os;
}


// ================= BANK FUNCTIONS =================

Bank::Bank()
{
    Account account;

    ifstream infile("Bank.data");

    if(!infile)
        return;

    while(infile >> account)
    {
        accounts.insert(pair<long,Account>(account.getAccNo(),account));
    }

    Account::setLastAccountNumber(account.getAccNo());

    infile.close();
}


Account Bank::OpenAccount(string fname,string lname,float balance)
{
    Account account(fname,lname,balance);

    accounts.insert(pair<long,Account>(account.getAccNo(),account));

    return account;
}


Account Bank::BalanceEnquiry(long accountNumber)
{
    return accounts.find(accountNumber)->second;
}


Account Bank::Deposit(long accountNumber,float amount)
{
    accounts.find(accountNumber)->second.Deposit(amount);

    return accounts.find(accountNumber)->second;
}


Account Bank::Withdraw(long accountNumber,float amount)
{
    accounts.find(accountNumber)->second.Withdraw(amount);

    return accounts.find(accountNumber)->second;
}


void Bank::CloseAccount(long accountNumber)
{
    cout<<"Account Deleted\n";

    cout<<accounts.find(accountNumber)->second;

    accounts.erase(accountNumber);
}


void Bank::ShowAllAccounts()
{
    for(auto itr=accounts.begin();itr!=accounts.end();itr++)
    {
        cout<<itr->second<<endl;
    }
}


Bank::~Bank()
{
    ofstream outfile("Bank.data");

    for(auto itr=accounts.begin();itr!=accounts.end();itr++)
    {
        outfile<<itr->second;
    }

    outfile.close();
}