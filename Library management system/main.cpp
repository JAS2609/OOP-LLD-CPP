#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

using namespace std;

enum class BookStatus {
    AVAILABLE,
    ISSUED,
    LOST,
    UNDER_MAINTENANCE
};

enum class UserType {
    LIBRARIAN,
    STAFF,
    CUSTOMER
};

class Book {
private:
    int bookId;
    string title;
    string author;
    BookStatus status;

public:
    Book(int id, const string& bookTitle, const string& bookAuthor)
        : bookId(id),
          title(bookTitle),
          author(bookAuthor),
          status(BookStatus::AVAILABLE) {
        if (id <= 0) {
            throw invalid_argument("Book ID must be positive.");
        }
        if (title.empty() || author.empty()) {
            throw invalid_argument("Book title and author cannot be empty.");
        }
    }

    int getBookId() const {
        return bookId;
    }

    const string& getTitle() const {
        return title;
    }

    const string& getAuthor() const {
        return author;
    }

    BookStatus getStatus() const {
        return status;
    }

    // A Book controls its own valid state transitions.
    bool markIssued() {
        if (status != BookStatus::AVAILABLE) {
            return false;
        }

        status = BookStatus::ISSUED;
        return true;
    }

    bool markReturned() {
        if (status != BookStatus::ISSUED) {
            return false;
        }

        status = BookStatus::AVAILABLE;
        return true;
    }

    bool markLost() {
        if (status == BookStatus::ISSUED) {
            return false;
        }

        status = BookStatus::LOST;
        return true;
    }

    bool markUnderMaintenance() {
        if (status == BookStatus::ISSUED) {
            return false;
        }

        status = BookStatus::UNDER_MAINTENANCE;
        return true;
    }
};

class User {
private:
    int userId;
    string name;
    UserType role;
    int bookingLimit;
    unordered_set<int> borrowedBookIds;

public:
    User(int id, const string& userName, UserType userRole, int limit)
        : userId(id),
          name(userName),
          role(userRole),
          bookingLimit(limit) {
        if (id <= 0) {
            throw invalid_argument("User ID must be positive.");
        }
        if (name.empty()) {
            throw invalid_argument("User name cannot be empty.");
        }
        if (limit < 0) {
            throw invalid_argument("Booking limit cannot be negative.");
        }
        if (role == UserType::CUSTOMER && limit == 0) {
            throw invalid_argument("A customer's booking limit must be positive.");
        }
    }

    int getUserId() const {
        return userId;
    }

    const string& getName() const {
        return name;
    }

    UserType getUserType() const {
        return role;
    }

    int getBookingLimit() const {
        return bookingLimit;
    }

    size_t getBorrowedBookCount() const {
        return borrowedBookIds.size();
    }

    bool canBorrow() const {
        return role == UserType::CUSTOMER &&
               borrowedBookIds.size() < static_cast<size_t>(bookingLimit);
    }

    bool hasBorrowedBook(int bookId) const {
        return borrowedBookIds.find(bookId) != borrowedBookIds.end();
    }

    // Returns false if the limit is reached or the ID is already present.
    bool addBorrowedBook(int bookId) {
        if (!canBorrow()) {
            return false;
        }

        return borrowedBookIds.insert(bookId).second;
    }

    bool removeBorrowedBook(int bookId) {
        return borrowedBookIds.erase(bookId) == 1;
    }

    const unordered_set<int>& getBorrowedBookIds() const {
        return borrowedBookIds;
    }
};

class Library {
private:
    string name;
    string address;
    unordered_map<int, Book> books;
    unordered_map<int, User> users;

    bool hasStaffPrivileges(int userId) const {
        auto userIt = users.find(userId);

        if (userIt == users.end()) {
            return false;
        }

        UserType role = userIt->second.getUserType();
        return role == UserType::LIBRARIAN || role == UserType::STAFF;
    }

public:
    // One-time setup helper for this standalone exercise.
    // Do not expose an equivalent unauthenticated endpoint in a real system.
    bool bootstrapInitialLibrarian(const User& librarian) {
        if (librarian.getUserType() != UserType::LIBRARIAN || !users.empty()) {
            return false;
        }
        return users.emplace(librarian.getUserId(), librarian).second;
    }

    Library(const string& libraryName, const string& libraryAddress)
        : name(libraryName), address(libraryAddress) {
        if (name.empty() || address.empty()) {
            throw invalid_argument("Library name and address cannot be empty.");
        }
    }

    const string& getName() const {
        return name;
    }

    const string& getAddress() const {
        return address;
    }

    // Only librarians and staff can add books.
    bool addBook(const Book& book, int actorId) {
        if (!hasStaffPrivileges(actorId)) {
            return false;
        }

        return books.emplace(book.getBookId(), book).second;
    }

    // Issued books cannot be removed from the catalogue.
    bool removeBook(int bookId, int actorId) {
        if (!hasStaffPrivileges(actorId)) {
            return false;
        }

        auto bookIt = books.find(bookId);
        if (bookIt == books.end()) {
            return false;
        }

        if (bookIt->second.getStatus() == BookStatus::ISSUED) {
            return false;
        }

        books.erase(bookIt);
        return true;
    }

    // Only librarians and staff can register users in this simplified model.
    bool registerUser(const User& user, int actorId) {
        if (!hasStaffPrivileges(actorId)) {
            return false;
        }

        return users.emplace(user.getUserId(), user).second;
    }

    bool issueBook(int userId, int bookId) {
        auto userIt = users.find(userId);
        auto bookIt = books.find(bookId);

        if (userIt == users.end() || bookIt == books.end()) {
            return false;
        }

        User& user = userIt->second;
        Book& book = bookIt->second;

        if (user.getUserType() != UserType::CUSTOMER) {
            return false;
        }

        if (!user.canBorrow() || user.hasBorrowedBook(bookId)) {
            return false;
        }

        if (book.getStatus() != BookStatus::AVAILABLE) {
            return false;
        }

        // Update the User first. If insertion fails, the Book is unchanged.
        if (!user.addBorrowedBook(bookId)) {
            return false;
        }

        // If the Book transition unexpectedly fails, roll back the User update.
        if (!book.markIssued()) {
            user.removeBorrowedBook(bookId);
            return false;
        }

        return true;
    }

    bool returnBook(int userId, int bookId) {
        auto userIt = users.find(userId);
        auto bookIt = books.find(bookId);

        if (userIt == users.end() || bookIt == books.end()) {
            return false;
        }

        User& user = userIt->second;
        Book& book = bookIt->second;

        if (!user.hasBorrowedBook(bookId)) {
            return false;
        }

        if (book.getStatus() != BookStatus::ISSUED) {
            return false;
        }

        if (!book.markReturned()) {
            return false;
        }

        // The membership check above makes this removal expected to succeed.
        if (!user.removeBorrowedBook(bookId)) {
            // Defensive rollback if the two sides ever become inconsistent.
            book.markIssued();
            return false;
        }

        return true;
    }

    const Book& getBook(int bookId) const {
        auto it = books.find(bookId);
        if (it == books.end()) {
            throw out_of_range("Book not found.");
        }

        return it->second;
    }

    const User& getUser(int userId) const {
        auto it = users.find(userId);
        if (it == users.end()) {
            throw out_of_range("User not found.");
        }

        return it->second;
    }

    size_t getBookCount() const {
        return books.size();
    }

    size_t getUserCount() const {
        return users.size();
    }
};

string toString(BookStatus status) {
    switch (status) {
        case BookStatus::AVAILABLE: return "AVAILABLE";
        case BookStatus::ISSUED: return "ISSUED";
        case BookStatus::LOST: return "LOST";
        case BookStatus::UNDER_MAINTENANCE: return "UNDER_MAINTENANCE";
    }
    return "UNKNOWN";
}

int main() {
    try {
        Library library("Central Library", "Delhi");

        // Create staff accounts first. The initial librarian is bootstrapped
        // directly here; a real application would have a secure setup flow.
        User librarian(1, "Asha", UserType::LIBRARIAN, 0);
        User staff(2, "Rohan", UserType::STAFF, 0);
        User alice(100, "Alice", UserType::CUSTOMER, 3);
        User bob(101, "Bob", UserType::CUSTOMER, 3);

        // Bootstrap the first librarian directly for this standalone demo.
        // In a real application, this would be handled by an administrative
        // setup process rather than an unauthenticated public method.
        // The following setup uses a small initial registration helper:
        // construct a separate library setup is intentionally avoided, so
        // the first librarian is registered through the bootstrap method below.
        //
        // For this demo, Library exposes no public privilege bypass. We create
        // the initial librarian via a dedicated bootstrap operation.
        //
        // See bootstrapInitialLibrarian() below in the class for production
        // alternatives; here the initial account is added through the method
        // provided specifically for first-time setup.
        library.bootstrapInitialLibrarian(librarian);

        cout << boolalpha;
        cout << "Register staff: " << library.registerUser(staff, 1) << '\n';
        cout << "Register Alice: " << library.registerUser(alice, 2) << '\n';
        cout << "Register Bob: " << library.registerUser(bob, 2) << '\n';

        cout << "Add book 101: "
             << library.addBook(Book(101, "Clean Code", "Robert C. Martin"), 1) << '\n';
        cout << "Add book 102: "
             << library.addBook(Book(102, "The Pragmatic Programmer", "Andrew Hunt"), 1) << '\n';
        cout << "Add book 103: "
             << library.addBook(Book(103, "DDIA", "Martin Kleppmann"), 1) << '\n';
        cout << "Add book 104: "
             << library.addBook(Book(104, "Effective C++", "Scott Meyers"), 1) << '\n';

        cout << "\nIssue 101 to Alice: " << library.issueBook(100, 101) << '\n';
        cout << "Issue 101 to Bob (already issued): "
             << library.issueBook(101, 101) << '\n';
        cout << "Bob returns Alice's book: "
             << library.returnBook(101, 101) << '\n';

        cout << "\nIssue 102 to Alice: " << library.issueBook(100, 102) << '\n';
        cout << "Issue 103 to Alice: " << library.issueBook(100, 103) << '\n';
        cout << "Issue 104 to Alice (limit reached): "
             << library.issueBook(100, 104) << '\n';

        cout << "\nRemove issued book 101: " << library.removeBook(101, 1) << '\n';
        cout << "Alice returns 101: " << library.returnBook(100, 101) << '\n';
        cout << "Remove returned book 101: " << library.removeBook(101, 1) << '\n';

        cout << "\nBook 102 status: "
             << toString(library.getBook(102).getStatus()) << '\n';
        cout << "Alice currently has "
             << library.getUser(100).getBorrowedBookCount()
             << " borrowed books.\n";

        cout << "\nTotal books in catalogue: " << library.getBookCount() << '\n';
        cout << "Total registered users: " << library.getUserCount() << '\n';
    }
    catch (const exception& ex) {
        cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
