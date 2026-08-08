#include <bits/stdc++.h>
using namespace std;


// ================= USER =================

class User {
private:
    int id;
    string name;

public:
    User(int id, string name) {
        this->id = id;
        this->name = name;
    }

    int getId() {
        return id;
    }

    string getName() {
        return name;
    }
};


// ================= EXPENSE SPLIT =================

class ExpenseSplit {
private:
    User* user;
    double amount;

public:
    ExpenseSplit(User* user, double amount) {
        this->user = user;
        this->amount = amount;
    }

    User* getUser() {
        return user;
    }

    double getAmount() {
        return amount;
    }
};


// ================= EXPENSE =================

class Expense {
private:
    int id;
    double amount;
    User* paidBy;

    vector<ExpenseSplit*> splits;

public:

    Expense(int id, double amount, User* paidBy) {
        this->id = id;
        this->amount = amount;
        this->paidBy = paidBy;
    }

    void addSplit(ExpenseSplit* split) {
        splits.push_back(split);
    }

    double getAmount() {
        return amount;
    }

    User* getPaidBy() {
        return paidBy;
    }

    vector<ExpenseSplit*> getSplits() {
        return splits;
    }
};


// ================= SPLIT STRATEGY =================

class SplitStrategy {
public:

    virtual vector<ExpenseSplit*> split(
        double amount,
        vector<User*> users,
        vector<double> values
    ) = 0;

    virtual ~SplitStrategy() {}
};


// ================= EQUAL SPLIT =================

class EqualSplit : public SplitStrategy {

public:

    vector<ExpenseSplit*> split(
        double amount,
        vector<User*> users,
        vector<double> values
    ) override {

        vector<ExpenseSplit*> result;

        double share = amount / users.size();

        for (User* user : users) {

            result.push_back(
                new ExpenseSplit(user, share)
            );
        }

        return result;
    }
};


// ================= EXACT SPLIT =================

class ExactSplit : public SplitStrategy {

public:

    vector<ExpenseSplit*> split(
        double amount,
        vector<User*> users,
        vector<double> values
    ) override {

        double total = 0;

        for (double value : values) {
            total += value;
        }

        if (total != amount) {
            throw invalid_argument(
                "Exact amounts don't match expense"
            );
        }

        vector<ExpenseSplit*> result;

        for (int i = 0; i < users.size(); i++) {

            result.push_back(
                new ExpenseSplit(
                    users[i],
                    values[i]
                )
            );
        }

        return result;
    }
};


// ================= PERCENTAGE SPLIT =================

class PercentageSplit : public SplitStrategy {

public:

    vector<ExpenseSplit*> split(
        double amount,
        vector<User*> users,
        vector<double> values
    ) override {

        double totalPercentage = 0;

        for (double percentage : values) {
            totalPercentage += percentage;
        }

        if (totalPercentage != 100) {
            throw invalid_argument(
                "Percentage must be 100"
            );
        }

        vector<ExpenseSplit*> result;

        for (int i = 0; i < users.size(); i++) {

            double userAmount =
                amount * values[i] / 100.0;

            result.push_back(
                new ExpenseSplit(
                    users[i],
                    userAmount
                )
            );
        }

        return result;
    }
};


// ================= BALANCE SHEET =================

class BalanceSheet {

private:

    // balance[A][B]
    //
    // means:
    // A owes B some amount

    unordered_map<
        int,
        unordered_map<int, double>
    > balance;


public:

    void updateDebt(
        int debtor,
        int creditor,
        double amount
    ) {

        /*
            Suppose:

            A -> B = 500
            B -> A = 100

            If we add:

            B -> A = 100

            We first check the opposite:

            A -> B = 500

            Cancel 100 from it.

            Result:

            A -> B = 400
        */

        if (balance[creditor][debtor] > 0) {

            double opposite =
                balance[creditor][debtor];


            // Opposite debt is enough
            // to completely cancel
            // this new debt.

            if (opposite >= amount) {

                balance[creditor][debtor]
                    = amount;

                return;
            }


            // Opposite debt is smaller.

            amount -= opposite;

            balance[creditor][debtor] = 0;
        }


        // Whatever remains becomes
        // the actual debt.

        balance[debtor][creditor]
            = amount;
    }


    void settleDebt(
        int debtor,
        int creditor,
        double amount
    ) {

        if (balance[debtor][creditor] < amount) {

            throw invalid_argument(
                "Settlement amount is greater than debt"
            );
        }

        balance[debtor][creditor] -= amount;


        if (balance[debtor][creditor] == 0) {

            balance[debtor].erase(
                creditor
            );
        }
    }


    void showBalances(
        unordered_map<int, User*>& users
    ) {

        cout << "\n===== BALANCES =====\n";

        for (auto& debtorEntry : balance) {

            int debtorId =
                debtorEntry.first;


            for (auto& creditorEntry :
                 debtorEntry.second) {

                int creditorId =
                    creditorEntry.first;

                double amount =
                    creditorEntry.second;


                if (amount > 0) {

                    cout
                        << users[debtorId]->getName()
                        << " owes "
                        << users[creditorId]->getName()
                        << " : Rs "
                        << amount
                        << endl;
                }
            }
        }
    }
};


// ================= GROUP =================

class Group {

private:

    int id;
    string name;

    vector<User*> members;
    vector<Expense*> expenses;

    BalanceSheet balanceSheet;


public:

    Group(int id, string name) {

        this->id = id;
        this->name = name;
    }


    void addMember(User* user) {

        members.push_back(user);
    }


    void addExpense(Expense* expense) {

        expenses.push_back(expense);
    }


    BalanceSheet& getBalanceSheet() {

        return balanceSheet;
    }


    vector<User*> getMembers() {

        return members;
    }
};


// ================= SPLITWISE SERVICE =================

class SplitwiseService {

private:

    int nextUserId = 1;
    int nextGroupId = 1;
    int nextExpenseId = 1;


    unordered_map<int, User*> users;

    unordered_map<int, Group*> groups;


public:

    // -------- CREATE USER --------

    User* createUser(string name) {

        User* user =
            new User(
                nextUserId++,
                name
            );

        users[user->getId()] = user;

        return user;
    }


    // -------- CREATE GROUP --------

    Group* createGroup(string name) {

        Group* group =
            new Group(
                nextGroupId++,
                name
            );

        groups[nextGroupId - 1] = group;

        return group;
    }


    // -------- ADD MEMBER --------

    void addMember(
        Group* group,
        User* user
    ) {

        group->addMember(user);
    }


    // -------- ADD EXPENSE --------

    void addExpense(
        Group* group,
        double amount,
        User* paidBy,
        vector<User*> usersInExpense,
        SplitStrategy* strategy,
        vector<double> values = {}
    ) {

        // 1. Calculate the split

        vector<ExpenseSplit*> splits =
            strategy->split(
                amount,
                usersInExpense,
                values
            );


        // 2. Create Expense

        Expense* expense =
            new Expense(
                nextExpenseId++,
                amount,
                paidBy
            );


        // 3. Store ExpenseSplits

        for (ExpenseSplit* split : splits) {

            expense->addSplit(split);
        }


        // 4. Add expense to Group

        group->addExpense(expense);


        // 5. Update BalanceSheet

        for (ExpenseSplit* split : splits) {

            User* user =
                split->getUser();

            double share =
                split->getAmount();


            // Person who paid doesn't
            // owe themselves.

            if (user->getId()
                == paidBy->getId()) {

                continue;
            }


            group->getBalanceSheet()
                .updateDebt(
                    user->getId(),
                    paidBy->getId(),
                    share
                );
        }
    }


    // -------- SETTLE --------

    void settle(
        Group* group,
        User* debtor,
        User* creditor,
        double amount
    ) {

        group->getBalanceSheet()
            .settleDebt(
                debtor->getId(),
                creditor->getId(),
                amount
            );
    }


    // -------- SHOW BALANCES --------

    void showBalances(
        Group* group
    ) {

        group->getBalanceSheet()
            .showBalances(users);
    }
};


// ================= MAIN =================

int main() {

    SplitwiseService splitwise;


    // ================= USERS =================

    User* rahul =
        splitwise.createUser("Rahul");

    User* amit =
        splitwise.createUser("Amit");

    User* urvashi =
        splitwise.createUser("Urvashi");


    // ================= GROUP =================

    Group* trip =
        splitwise.createGroup("Goa Trip");


    splitwise.addMember(trip, rahul);
    splitwise.addMember(trip, amit);
    splitwise.addMember(trip, urvashi);


    // ================= EQUAL SPLIT =================

    EqualSplit equalSplit;


    vector<User*> users = {
        rahul,
        amit,
        urvashi
    };


    /*
        Rahul pays Rs 900.

        Equal split:

        Rahul    -> Rs 300
        Amit     -> Rs 300
        Urvashi  -> Rs 300

        Rahul doesn't owe himself.

        Result:

        Amit    -> Rahul Rs 300
        Urvashi -> Rahul Rs 300
    */

    splitwise.addExpense(
        trip,
        900,
        rahul,
        users,
        &equalSplit
    );


    splitwise.showBalances(trip);


    // ================= EXACT SPLIT =================

    ExactSplit exactSplit;


    /*
        Amit pays Rs 1000.

        Rahul    -> Rs 500
        Amit     -> Rs 300
        Urvashi  -> Rs 200
    */

    vector<double> exactAmounts = {
        500,
        300,
        200
    };


    splitwise.addExpense(
        trip,
        1000,
        amit,
        users,
        &exactSplit,
        exactAmounts
    );


    cout << "\nAfter exact split:\n";

    splitwise.showBalances(trip);


    // ================= PERCENTAGE SPLIT =================

    PercentageSplit percentageSplit;


    /*
        Urvashi pays Rs 1000.

        Rahul    -> 50%
        Amit     -> 30%
        Urvashi  -> 20%

        Rahul    -> Rs 500
        Amit     -> Rs 300
        Urvashi  -> Rs 200
    */

    vector<double> percentages = {
        50,
        30,
        20
    };


    splitwise.addExpense(
        trip,
        1000,
        urvashi,
        users,
        &percentageSplit,
        percentages
    );


    cout << "\nAfter percentage split:\n";

    splitwise.showBalances(trip);


    // ================= SETTLEMENT =================

    /*
        Suppose Amit pays Rahul Rs 100.
    */

    splitwise.settle(
        trip,
        amit,
        rahul,
        100
    );


    cout << "\nAfter settlement:\n";

    splitwise.showBalances(trip);


    return 0;
}