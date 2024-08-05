#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

class BankAccount {
private:
    string accountNumber;
    int pin;
    double balance;

public:
    BankAccount(string accNum, int p, double bal) : accountNumber(accNum), pin(p), balance(bal) {}

    string getAccountNumber() const { return accountNumber; }
    bool verifyPin(int inputPin) const { return pin == inputPin; }
    double getBalance() const { return balance; }
    void deposit(double amount) { balance += amount; }
    bool withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }
};

string generateAccountNumber() {
    string accNum = "";
    for (int i = 0; i < 10; i++) {
        accNum += to_string(rand() % 10);
    }
    return accNum;
}

void saveAccount(const BankAccount& account) {
    ofstream file("accounts.txt", ios::app);
    file << account.getAccountNumber() << " " << account.getBalance() << endl;
    file.close();
}

BankAccount* findAccount(const string& accountNumber) {
    ifstream file("accounts.txt");
    string accNum;
    double balance;
    while (file >> accNum >> balance) {
        if (accNum == accountNumber) {
            file.close();
            return new BankAccount(accNum, 0, balance); // PIN is not stored in file for security
        }
    }
    file.close();
    return nullptr;
}

void updateAccountBalance(const BankAccount& account) {
    ifstream inFile("accounts.txt");
    ofstream outFile("temp.txt");
    string accNum;
    double balance;
    while (inFile >> accNum >> balance) {
        if (accNum == account.getAccountNumber()) {
            outFile << accNum << " " << account.getBalance() << endl;
        } else {
            outFile << accNum << " " << balance << endl;
        }
    }
    inFile.close();
    outFile.close();
    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");
}

int main() {
    srand(time(0));
    int choice, pin;
    string accountNumber;
    double amount;
    BankAccount* account = nullptr;

    while (true) {
        cout << "\nBanking System Menu:\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Check Balance\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                accountNumber = generateAccountNumber();
                cout << "Enter PIN for your new account: ";
                cin >> pin;
                account = new BankAccount(accountNumber, pin, 0);
                saveAccount(*account);
                cout << "Account created successfully. Your account number is: " << accountNumber << endl;
                break;
            case 2:
                cout << "Enter account number: ";
                cin >> accountNumber;
                account = findAccount(accountNumber);
                if (account) {
                    cout << "Enter amount to deposit: ";
                    cin >> amount;
                    account->deposit(amount);
                    updateAccountBalance(*account);
                    cout << "Deposit successful. New balance: $" << fixed << setprecision(2) << account->getBalance() << endl;
                } else {
                    cout << "Account not found.\n";
                }
                break;
            case 3:
                cout << "Enter account number: ";
                cin >> accountNumber;
                account = findAccount(accountNumber);
                if (account) {
                    cout << "Enter PIN: ";
                    cin >> pin;
                    if (account->verifyPin(pin)) {
                        cout << "Enter amount to withdraw: ";
                        cin >> amount;
                        if (account->withdraw(amount)) {
                            updateAccountBalance(*account);
                            cout << "Withdrawal successful. New balance: $" << fixed << setprecision(2) << account->getBalance() << endl;
                        } else {
                            cout << "Insufficient funds.\n";
                        }
                    } else {
                        cout << "Incorrect PIN.\n";
                    }
                } else {
                    cout << "Account not found.\n";
                }
                break;
            case 4:
                cout << "Enter account number: ";
                cin >> accountNumber;
                account = findAccount(accountNumber);
                if (account) {
                    cout << "Enter PIN: ";
                    cin >> pin;
                    if (account->verifyPin(pin)) {
                        cout << "Current balance: $" << fixed << setprecision(2) << account->getBalance() << endl;
                    } else {
                        cout << "Incorrect PIN.\n";
                    }
                } else {
                    cout << "Account not found.\n";
                }
                break;
            case 5:
                cout << "Thank you for using our banking system. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

        delete account;
        account = nullptr;
    }

    return 0;
}
