// file for thinking the entities

enum class BookStatus {
    AVAILABLE,
    ISSUED,
    LOST,
    UNDER_MAINTENANCE
};
1. Class: Book

Private attributes:
bookId
title
author
BookStatus status; 
private methods:
setBookAvailability()
void markIssued();
void markReturned();
void markLost();
Public methods:
getBookId()
getTitle()
getBookAvailability()
Responsibility:
Represents an individual book.
Maintains its availability status.
checks if the book is issued/returned or not

2. class library

Private attributes:
name
address
unordered_map<int, Book> books;
unordered_map<int, User> users;
private methods:
isAdmin()//not really needed can be done via if else can revome if overabstraction
public methods:
getAddress()
getName()
void addBook()
void removeBook()
bool isBookAvailable()
issueBook()
returnBook()
void updateBookAvailabilty()
Responsibility:
managing books(addition /removal)
issuing books
returning books

3. enum class userType{
LIBRARIAN
STAFF
CUSTOMER
}

4. class user
private attributes:
int userId
unordered_set<int> borrowedBookIds;//all books issued
string name
userType usertype
int bookinglimit//max number of books at a time

public methods:
getIssuedBooks()
getBookinglimit()
updateBookingLimit()
addBookId()
removeBookId()
getName()
getUserType()

Responsitbilty:
assign role
manage user's books//issued and due to return
manage their booking limit
initialisation method (to intialise their role and name )
