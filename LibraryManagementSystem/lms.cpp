#include <bits/stdc++.h>
using namespace std;


// Forward declaration
class IssueRecord;


// Enums

enum class SubjectType {
    MATH,
    PHYSICS,
    HINDI
};


enum class BookStatus {
    AVAILABLE,
    ISSUED
};


enum class IssueStatus {
    ACTIVE,
    RETURNED
};



// User Class

class User {

private:

    string name;
    string userId;
    vector<IssueRecord*> issueRecords;


public:

    User(string name, string userId) {
        this->name = name;
        this->userId = userId;
    }


    string getName() {
        return name;
    }


    string getUserId() {
        return userId;
    }


    vector<IssueRecord*> getIssueRecords() {
        return issueRecords;
    }


    void addIssueRecord(IssueRecord* record) {
        issueRecords.push_back(record);
    }

};




// Book Class

class Book {

private:

    string title;
    int bookId;
    string details;

    SubjectType subject;
    BookStatus status;


public:

    Book(
        string title,
        int bookId,
        string details,
        SubjectType subject
    ) {

        this->title = title;
        this->bookId = bookId;
        this->details = details;
        this->subject = subject;
        this->status = BookStatus::AVAILABLE;

    }



    string getTitle() {
        return title;
    }


    int getBookId() {
        return bookId;
    }


    string getDetails() {
        return details;
    }


    SubjectType getSubject() {
        return subject;
    }


    bool isAvailable() {

        return status == BookStatus::AVAILABLE;

    }



    void issueBook() {

        status = BookStatus::ISSUED;

    }



    void returnBook() {

        status = BookStatus::AVAILABLE;

    }

};





// IssueRecord Class

class IssueRecord {

private:

    User* user;
    Book* book;

    string issueDate;
    string returnDate;

    int transactionId;
    int fineAmount;

    IssueStatus status;


public:


    IssueRecord(
        User* user,
        Book* book,
        int transactionId
    ) {

        this->user = user;
        this->book = book;

        this->transactionId = transactionId;

        this->fineAmount = 0;

        this->status = IssueStatus::ACTIVE;


        this->issueDate = "01-08-2026";
        this->returnDate = "";

    }




    User* getUser() {
        return user;
    }



    Book* getBook() {
        return book;
    }



    string getIssueDate() {
        return issueDate;
    }



    string getReturnDate() {
        return returnDate;
    }



    int getTransactionId() {
        return transactionId;
    }



    int getFineAmount() {
        return fineAmount;
    }



    IssueStatus getStatus() {
        return status;
    }



    void completeReturn() {

        status = IssueStatus::RETURNED;

        returnDate = "11-08-2026";

    }

};







// Library Class

class Library {


private:

    string name;

    unordered_map<SubjectType, vector<Book*>> books;



public:


    Library(string name) {

        this->name = name;

    }




    void addBook(Book* book) {

        books[book->getSubject()].push_back(book);

    }





    vector<Book*> searchBook(string title) {


        vector<Book*> result;


        for(auto entry : books) {


            for(Book* book : entry.second) {


                if(book->getTitle() == title) {

                    result.push_back(book);

                }

            }

        }


        return result;

    }





    vector<Book*> filterBook(SubjectType subject) {


        if(books.find(subject) != books.end()) {

            return books[subject];

        }


        return {};

    }





    void issueBook(User* user, Book* book) {


        if(!book->isAvailable()) {

            cout<<"Book not available\n";
            return;

        }



        book->issueBook();



        IssueRecord* record =
            new IssueRecord(
                user,
                book,
                rand()
            );



        user->addIssueRecord(record);



        cout<<"Book issued successfully\n";

    }





    void returnBook(User* user, Book* book) {


        for(IssueRecord* record : user->getIssueRecords()) {


            if(record->getBook() == book &&
               record->getStatus() == IssueStatus::ACTIVE) {


                book->returnBook();

                record->completeReturn();


                cout<<"Book returned successfully\n";

                return;

            }

        }


        cout<<"No active issue found\n";

    }

};






// Main Function

int main() {


    // Create User

    User* user =
        new User(
            "Rahul",
            "U101"
        );



    // Create Books

    Book* physicsBook =
        new Book(
            "Physics Fundamentals",
            1,
            "Physics book",
            SubjectType::PHYSICS
        );



    Book* mathBook =
        new Book(
            "Math Basics",
            2,
            "Math book",
            SubjectType::MATH
        );





    // Create Library

    Library* library =
        new Library(
            "City Library"
        );




    // Add books

    library->addBook(physicsBook);

    library->addBook(mathBook);





    // Search Book

    cout<<"Search Result:\n";


    vector<Book*> result =
        library->searchBook(
            "Physics Fundamentals"
        );



    for(Book* book : result) {

        cout<<book->getTitle()<<endl;

    }






    // Filter Book

    cout<<"\nPhysics Books:\n";


    vector<Book*> physicsBooks =
        library->filterBook(
            SubjectType::PHYSICS
        );



    for(Book* book : physicsBooks) {

        cout<<book->getTitle()<<endl;

    }






    // Issue Book

    cout<<"\nIssuing Book:\n";


    library->issueBook(
        user,
        physicsBook
    );







    // Display Issue Record


    cout<<"\nIssue Records:\n";


    for(IssueRecord* record : user->getIssueRecords()) {


        cout<<"Transaction Id : "
            <<record->getTransactionId()
            <<endl;


        cout<<"Book : "
            <<record->getBook()->getTitle()
            <<endl;


        cout<<"Issue Date : "
            <<record->getIssueDate()
            <<endl;


    }






    // Return Book

    cout<<"\nReturning Book:\n";


    library->returnBook(
        user,
        physicsBook
    );






    // Cleanup

    delete physicsBook;

    delete mathBook;

    delete user;

    delete library;



    return 0;

}