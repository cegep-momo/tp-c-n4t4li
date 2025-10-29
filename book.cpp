#include "book.h"
#include <sstream>
#include <iostream>
using namespace std;

//  constructeurs
Book::Book() : title(""), author(""), isbn(""), isAvailable(true), borrowerName("") {}

Book::Book(const string& title, const string& author, const string& isbn)
    : title(title), author(author), isbn(isbn), isAvailable(true), borrowerName("") {}

//  getters
string Book::getTitle() const { return title; }
string Book::getAuthor() const { return author; }
string Book::getISBN() const { return isbn; }
bool Book::getAvailability() const { return isAvailable; }
string Book::getBorrowerName() const { return borrowerName; }

// setters
void Book::setTitle(const string& title) { this->title = title; }
void Book::setAuthor(const string& author) { this->author = author; }
void Book::setISBN(const string& isbn) { this->isbn = isbn; }
void Book::setAvailability(bool available) { this->isAvailable = available; }
void Book::setBorrowerName(const string& name) { this->borrowerName = name; }

// emprunt d’un livre
void Book::checkOut(const string& borrower) {
    if (isAvailable) {
        isAvailable = false;
        borrowerName = borrower;
    }
}

// retouner un livre
void Book::returnBook() {
    isAvailable = true;
    borrowerName = "";
}

// afficher un livre
string Book::toString() const {
    string etat = isAvailable ? "Disponible" : "Emprunté par " + borrowerName;
    return "Titre: " + title + "\nAuteur: " + author +
           "\nISBN: " + isbn + "\nStatut: " + etat;
}

//  fichier texte
string Book::toFileFormat() const {
    return title + "|" + author + "|" + isbn + "|" +
           (isAvailable ? "1" : "0") + "|" + borrowerName;
}


// lire a partir du fichier texte
void Book::fromFileFormat(const string& line) {
    stringstream ss(line);
    string dispo;
    getline(ss, title, '|');
    getline(ss, author, '|');
    getline(ss, isbn, '|');
    getline(ss, dispo, '|');
    getline(ss, borrowerName, '|');
    isAvailable = (dispo == "1");
}
