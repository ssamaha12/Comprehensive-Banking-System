#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

class Transaction {
public:
    Transaction(double amount, const std::string& description) : amount(amount), description(description) {}

    friend std::ostream& operator<<(std::ostream& os, const Transaction& transaction) {
        os << "Amount: " << transaction.amount << ", Description: " << transaction.description;
        return os;
    }

private:
    double amount;
    std::string description;
};

class Account {
public:
    Account(const std::string& owner, double balance) : owner(owner), balance(balance) {}

    void deposit(double amount) {
        if (amount <= 0)
            throw std::invalid_argument("Invalid deposit amount");
        balance += amount;
        transactions.push_back(Transaction(amount, "Deposit"));
    }

    void withdraw(double amount) {
        if (amount <= 0 || amount > balance)
            throw std::invalid_argument("Invalid withdrawal amount");
        balance -= amount;
        transactions.push_back(Transaction(-amount, "Withdrawal"));
    }

    void displayTransactions() const {
        for (const auto& transaction : transactions)
            std::cout << transaction << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const Account& account) {
        os << "Owner: " << account.owner << ", Balance: " << account.balance;
        return os;
    }

private:
    std::string owner;
    double balance;
    std::vector<Transaction> transactions;
};

class Bank {
public:
    void createAccount(const std::string& username, const std::string& password, const std::string& owner, double initialBalance) {
        if (accounts.find(username) != accounts.end())
            throw std::invalid_argument("Account already exists");
        accounts[username] = std::make_pair(password, Account(owner, initialBalance));
    }

    void authenticate(const std::string& username, const std::string& password) {
        if (accounts.find(username) == accounts.end() || accounts[username].first != password)
            throw std::invalid_argument("Authentication failed");
        currentUser = username;
    }

    void deposit(double amount) {
        if (currentUser.empty())
            throw std::logic_error("No user logged in");
        accounts[currentUser].second.deposit(amount);
    }

    void withdraw(double amount) {
        if (currentUser.empty())
            throw std::logic_error("No user logged in");
        accounts[currentUser].second.withdraw(amount);
    }

    void displayTransactions() {
        if (currentUser.empty())
            throw std::logic_error("No user logged in");
        accounts[currentUser].second.displayTransactions();
    }

    Account& getCurrentUserAccount() {
        if (currentUser.empty())
            throw std::logic_error("No user logged in");
        return accounts[currentUser].second;
    }

private:
    std::map<std::string, std::pair<std::string, Account>> accounts;
    std::string currentUser;
};

int main() {
    Bank bank;

    // Creating accounts
    bank.createAccount("user1", "password1", "John Doe", 1000);
    bank.createAccount("user2", "password2", "Jane Smith", 2000);

    // Authenticating users
    try {
        bank.authenticate("user1", "password1");
    } catch (const std::exception& ex) {
        std::cerr << "Authentication failed: " << ex.what() << std::endl;
        return 1;
    }

    // Making transactions
    try {
        bank.deposit(500);
        bank.withdraw(200);
    } catch (const std::exception& ex) {
        std::cerr << "Transaction failed: " << ex.what() << std::endl;
        return 1;
    }

    // Displaying transaction history
    bank.displayTransactions();

    // Using operator overloading to display account information
    std::cout << bank.getCurrentUserAccount() << std::endl;

    return 0;
}
