// Rule to remember

// Ask yourself one question:

// Who is responsible for deleting this object?

// If the answer is

// "This class."

// Use

// unique_ptr

// If the answer is

// "Someone else."

// Use

// Book*


// 1. Creating an object

// Instead of

// Book* b = new Book(...);

// Modern C++:

// unique_ptr<Book> b = make_unique<Book>(...);

// or simply

// auto b = make_unique<Book>(...);

// ✅ Correct.

// 2. unique_ptr

// One object has exactly one owner.

// Library
//   │
// unique_ptr<Book>
//   │
//   ▼
//  Book

// Only the Library owns the Book.

// When the Library dies, the Book is deleted automatically.

// ✅ Correct.

// 3. shared_ptr

// Use it when multiple objects actually share ownership.

// Example:

// Course
//   │
// shared_ptr<Student>

// Club
//   │
// shared_ptr<Student>

// Both Course and Club keep the student alive.

// The student is deleted only when both release their shared_ptr.

// ✅ Correct.

// 4. .get()
// unique_ptr<Book> book = make_unique<Book>(...);

// Book* ptr = book.get();

// .get() does not transfer ownership.

// It simply says:

// "Here's the raw pointer. I'm still the owner."

// Think of it like lending someone your house key to look at the house—you still own the house.

// ✅ Correct.

// 5. std::move()

// This is the biggest idea.

// Suppose

// auto book = make_unique<Book>();

// Ownership:

// book
//  │
//  ▼
// Book

// Now

// library->addBook(move(book));

// Ownership becomes

// library
//   │
// unique_ptr<Book>
//   │
//   ▼
// Book

// book == nullptr

// The old owner no longer owns it.

// ✅ Correct.

// 6. When should I use unique_ptr?

// Ask yourself one question:

// Who is responsible for deleting this object?

// If the answer is

// This class owns it and should delete it.

// Use

// unique_ptr<T>

// Examples:

// class Library {
//     vector<unique_ptr<Book>> books;
// };

// The library owns books.

// class User {
//     vector<unique_ptr<IssueRecord>> records;
// };

// The user owns issue records.

// 7. When should I use a raw pointer?

// If a class doesn't own the object and just needs to refer to it.

// Example:

// class IssueRecord {

//     User* user;
//     Book* book;
// };

// IssueRecord should not delete the User or the Book.

// It only remembers:

// "This user borrowed this book."

// So raw pointers are perfectly fine here.


#include <bits/stdc++.h>
#include <memory>

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

// ==================== User ====================

class User {

private:

    string name;
    string userId;

    // User owns all IssueRecords
    vector<unique_ptr<IssueRecord>> issueRecords;

public:

    User(string name, string userId)
        : name(name), userId(userId) {}

    string getName() {
        return name;
    }

    string getUserId() {
        return userId;
    }
    
    
    // if I return vector<unique_ptr<IssueRecord>> , class User loss all the record . So we pass the ref & not actual IssueRecord

    vector<unique_ptr<IssueRecord>>& getIssueRecords() {
        return issueRecords;
    }

    void addIssueRecord(unique_ptr<IssueRecord> record) {
        issueRecords.push_back(move(record));
    }
};

// ==================== Book ====================

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
    ) :
        title(title),
        bookId(bookId),
        details(details),
        subject(subject),
        status(BookStatus::AVAILABLE) {}

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

// ==================== IssueRecord ====================

class IssueRecord {

private:

    // Non-owning pointers
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
    )
        : user(user),
          book(book),
          transactionId(transactionId),
          fineAmount(0),
          status(IssueStatus::ACTIVE),
          issueDate("01-08-2026"),
          returnDate("")
    {
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

// ==================== Library ====================

class Library {

private:

    string name;

    // Library owns all Books
    unordered_map<SubjectType, vector<unique_ptr<Book>>> books;

public:

    Library(string name)
        : name(name) {}

    void addBook(unique_ptr<Book> book) {

        SubjectType subject = book->getSubject();

        books[subject].push_back(move(book));
    }

    vector<Book*> searchBook(string title) {

        vector<Book*> result;

        for (auto& entry : books) {

            for (auto& book : entry.second) {

                if (book->getTitle() == title) {

                    result.push_back(book.get());
                }
            }
        }

        return result;
    }

    vector<Book*> filterBook(SubjectType subject) {

        vector<Book*> result;

        if (books.find(subject) != books.end()) {

            for (auto& book : books[subject]) {

                result.push_back(book.get());
            }
        }

        return result;
    }

    void issueBook(User* user, Book* book) {

        if (!book->isAvailable()) {

            cout << "Book not available\n";
            return;
        }

        book->issueBook();

        auto record = make_unique<IssueRecord>(
            user,
            book,
            rand()
        );

        user->addIssueRecord(move(record));

        cout << "Book issued successfully\n";
    }

    void returnBook(User* user, Book* book) {

        for (auto& record : user->getIssueRecords()) {

            if (record->getBook() == book &&
                record->getStatus() == IssueStatus::ACTIVE) {

                book->returnBook();

                record->completeReturn();

                cout << "Book returned successfully\n";

                return;
            }
        }

        cout << "No active issue found\n";
    }
};

int main() {

    // Create User
    auto user = make_unique<User>(
        "Rahul",
        "U101"
    );

    // Create Books
    auto physicsBook = make_unique<Book>(
        "Physics Fundamentals",
        1,
        "Physics book",
        SubjectType::PHYSICS
    );

    auto mathBook = make_unique<Book>(
        "Math Basics",
        2,
        "Math book",
        SubjectType::MATH
    );

    // Create Library
    auto library = make_unique<Library>(
        "City Library"
    );

    // Add books (ownership transferred to Library)
    library->addBook(move(physicsBook));
    library->addBook(move(mathBook));

    // Search Book
    cout << "Search Result:\n";

    vector<Book*> result =
        library->searchBook("Physics Fundamentals");

    for(Book* book : result) {
        cout << book->getTitle() << endl;
    }

    // Filter Book
    cout << "\nPhysics Books:\n";

    vector<Book*> physicsBooks =
        library->filterBook(SubjectType::PHYSICS);

    for(Book* book : physicsBooks) {
        cout << book->getTitle() << endl;
    }

    // Since ownership was transferred,
    // get the pointer back from the library.
    Book* physics =
        library->searchBook("Physics Fundamentals")[0];

    // Issue Book
    cout << "\nIssuing Book:\n";

    library->issueBook(
        user.get(),
        physics
    );

    // Display Issue Records
    cout << "\nIssue Records:\n";

    for(auto& record : user->getIssueRecords()) {

        cout << "Transaction Id : "
             << record->getTransactionId()
             << endl;

        cout << "Book : "
             << record->getBook()->getTitle()
             << endl;

        cout << "Issue Date : "
             << record->getIssueDate()
             << endl;
    }

    // Return Book
    cout << "\nReturning Book:\n";

    library->returnBook(
        user.get(),
        physics
    );

    // No delete required!

    return 0;
}