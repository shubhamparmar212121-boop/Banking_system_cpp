#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<map>

using namespace std;

#define MIN_BALANCE 500

// Exception Classes
class InsufficientFunds{};
class InvalidAccount{};
class DepositLimitExceeded{};
class WithdrawLimitExceeded{};
class EmptyFirstName{};

class Account
{
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;

    static long NextAccountNumber;

    void secretNote(){
        cout<<"Private Note\n";
    }

public:
    Account(){}

    Account(string fname,string lname,float balance);

    long getAccNo(){return accountNumber;}
    string getFirstName(){return firstName;}
    string getLastName(){return lastName;}
    float getBalance(){return balance;}

    void Deposit(float amount);
    void Withdraw(float amount);

    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();

    friend ofstream & operator<<(ofstream &ofs,Account &acc);
    friend ifstream & operator>>(ifstream &ifs,Account &acc);
    friend ostream & operator<<(ostream &os,Account &acc);
};

long Account::NextAccountNumber=1000;


// Inheritance Example
class PremiumAccount : public Account
{
public:
    PremiumAccount(string fname,string lname,float balance)
        :Account(fname,lname,balance){}

    void specialFeature(){
        cout<<"Premium Feature Activated\n";
    }
};


class Bank
{
private:
    map<long,Account> accounts;

public:
    Bank();

    Account OpenAccount(string fname,string lname,float balance=0);

    Account BalanceEnquiry(long accountNumber);

    Account Deposit(long accountNumber,float amount);

    Account Withdraw(long accountNumber,float amount);

    void CloseAccount(long accountNumber);

    void ShowAllAccounts();

    ~Bank();
};


// Inline function
inline void greet(string &name){
    cout<<"Hello "<<name<<" Welcome!\n";
}


// Function Overloading
int add(int a,int b){ return a+b; }
float add(float a,float b){ return a+b; }


// Return by reference
float& updateBalanceReference(float &bal){
    return bal;
}


// MAIN FUNCTION
int main()
{
    Bank b;
    Account acc;

    int choice;
    string fname,lname;
    long accountNumber;
    float balance;
    float amount;

    cout<<"*** BANKING SYSTEM ***\n";

    do
    {
        cout<<"\n1 Open Account";
        cout<<"\n2 Balance Enquiry";
        cout<<"\n3 Deposit";
        cout<<"\n4 Withdraw";
        cout<<"\n5 Close Account";
        cout<<"\n6 Show All Accounts";
        cout<<"\n7 Quit";
        cout<<"\nEnter Choice: ";

        cin>>choice;

        switch(choice)
        {

        case 1:
        try{
            cout<<"Enter First Name: ";
            cin>>fname;

            if(fname=="")
                throw EmptyFirstName();

            cout<<"Enter Last Name: ";
            cin>>lname;

            cout<<"Enter Balance: ";
            cin>>balance;

            acc=b.OpenAccount(fname,lname,balance);

            greet(fname);

            cout<<acc;
        }
        catch(EmptyFirstName){
            cout<<"First name cannot be empty!\n";
        }
        break;


        case 2:
        try{
            cout<<"Enter Account Number: ";
            cin>>accountNumber;

            acc=b.BalanceEnquiry(accountNumber);

            cout<<"\nAccount Details\n";

            cout<<acc;
        }
        catch(InvalidAccount){
            cout<<"Invalid Account Number!\n";
        }
        break;


        case 3:
        try{
            cout<<"Enter Account Number: ";
            cin>>accountNumber;

            cout<<"Enter Amount: ";
            cin>>amount;

            acc=b.Deposit(accountNumber,amount);

            cout<<acc;
        }
        catch(InvalidAccount){
            cout<<"Invalid Account Number!\n";
        }
        catch(DepositLimitExceeded){
            cout<<"Deposit limit exceeded! Max 100000 allowed\n";
        }
        break;


        case 4:
        try{
            cout<<"Enter Account Number: ";
            cin>>accountNumber;

            cout<<"Enter Amount: ";
            cin>>amount;

            acc=b.Withdraw(accountNumber,amount);

            cout<<acc;
        }
        catch(InvalidAccount){
            cout<<"Invalid Account Number!\n";
        }
        catch(WithdrawLimitExceeded){
            cout<<"Withdraw limit exceeded! Max 50000 allowed\n";
        }
        catch(InsufficientFunds){
            cout<<"Insufficient Balance!\n";
        }
        break;


        case 5:
            cout<<"Enter Account Number: ";
            cin>>accountNumber;

            b.CloseAccount(accountNumber);
        break;


        case 6:
            b.ShowAllAccounts();
        break;


        case 7:
            break;

        default:
            cout<<"Wrong Choice\n";
            exit(0);
        }

    }while(choice!=7);

    return 0;
}



// ACCOUNT FUNCTIONS

Account::Account(string fname,string lname,float bal)
{
    NextAccountNumber++;

    accountNumber=NextAccountNumber;

    firstName=fname;

    lastName=lname;

    balance=bal;
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



// FILE HANDLING

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
    os<<"First Name: "<<acc.getFirstName()<<endl;
    os<<"Last Name: "<<acc.getLastName()<<endl;
    os<<"Account Number: "<<acc.getAccNo()<<endl;
    os<<"Balance: "<<acc.getBalance()<<endl;

    return os;
}



// BANK FUNCTIONS

Bank::Bank()
{
    Account account;

    ifstream infile("Bank.data");

    if(!infile)
        return;

    while(infile>>account)
    {
        accounts.insert(pair<long,Account>(account.getAccNo(),account));
    }

    Account::setLastAccountNumber(account.getAccNo());

    infile.close();
}


Account Bank::OpenAccount(string fname,string lname,float balance)
{
    if(fname.empty())
        throw EmptyFirstName();

    Account account(fname,lname,balance);

    accounts.insert(pair<long,Account>(account.getAccNo(),account));

    return account;
}


Account Bank::BalanceEnquiry(long accountNumber)
{
    if(accounts.find(accountNumber)==accounts.end())
        throw InvalidAccount();

    return accounts.find(accountNumber)->second;
}


Account Bank::Deposit(long accountNumber,float amount)
{
    if(accounts.find(accountNumber)==accounts.end())
        throw InvalidAccount();

    if(amount>100000)
        throw DepositLimitExceeded();

    accounts.find(accountNumber)->second.Deposit(amount);

    return accounts.find(accountNumber)->second;
}


Account Bank::Withdraw(long accountNumber,float amount)
{
    if(accounts.find(accountNumber)==accounts.end())
        throw InvalidAccount();

    if(amount>50000)
        throw WithdrawLimitExceeded();

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