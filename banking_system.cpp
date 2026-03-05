#include<iostream>
#include<string>


using namespace std;
#define MIN_BALANCE 500;

//CLASS ACCOUNT
class Account{
    private:
    long AccountNumber;
    string FirstName;
    string LastName;
    float balance;
    static long NextAccountNumber;
    public:
    Account(){}
    Account(string fname,string lname,float balance);
    long getAccNo(){return AccountNumber;}
    string getFirstname(){return FirstName;}
    string getLastName(){return LastName;}
    float getBalance(){return balance;}
    void diposit(float mount);
    void withdraw(float amount);
    static void setlastaccountnumber(long accountnumber);
    static long setlastaccountnumber();

};




int main(){
    return 0;
}