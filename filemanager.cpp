#include <fstream>
#include <iostream>
#include <filesystem>
#include "filemanager.h"

using namespace std;
namespace fs = std::filesystem;

// Constructor
FileManager::FileManager(const string& booksFile, const string& usersFile) {
    // Automatically detect correct data folder
    if (fs::exists("../data/books.txt")) {
        booksFileName = "../data/books.txt";
        usersFileName = "../data/users.txt";
    } 
    else if (fs::exists("data/books.txt")) {
        booksFileName = "data/books.txt";
        usersFileName = "data/users.txt";
    } 
    else {
        // fallback if data folder not found
        booksFileName = "books.txt";
        usersFileName = "users.txt";
    }
}

// Save all library data
bool FileManager::saveLibraryData(Library& library) {
    return saveBooksToFile(library) && saveUsersToFile(library);
}

// Load all library data
bool FileManager::loadLibraryData(Library& library) {
    bool booksLoaded = loadBooksFromFile(library);
    bool usersLoaded = loadUsersFromFile(library);
    return booksLoaded || usersLoaded; // Return true if at least one file was loaded
}

// Save books to file
bool FileManager::saveBooksToFile(Library& library) {
    ofstream file(booksFileName);
    if (!file.is_open()) {
        cout << "Erreur : Impossible d'ouvrir " << booksFileName << " en écriture.\n";
        return false;
    }
    
    auto books = library.getAllBooks();
    for (Book* book : books) {
        file << book->toFileFormat() << "\n";
    }
    
    file.close();
    return true;
}

// Save users to file
bool FileManager::saveUsersToFile(Library& library) {
    ofstream file(usersFileName);
    if (!file.is_open()) {
        cout << "Erreur : Impossible d'ouvrir " << usersFileName << " en écriture.\n";
        return false;
    }
    
    auto users = library.getAllUsers();
    for (User* user : users) {
        file << user->toFileFormat() << "\n";
    }
    
    file.close();
    return true;
}

// Load books from file
bool FileManager::loadBooksFromFile(Library& library) {
    ifstream file(booksFileName);
    if (!file.is_open()) {
        cout << "Aucun fichier de livres existant trouvé. Démarrage avec une bibliothèque vide.\n";
        return false;
    }
    
    string line;
    int count = 0;
    while (getline(file, line)) {
        if (!line.empty()) {
            Book book;
            book.fromFileFormat(line);
            library.addBook(book);
            count++;
        }
    }
    
    file.close();
    cout << "Chargé " << count << " livre(s) depuis le fichier.\n";
    return true;
}

// Load users from file
bool FileManager::loadUsersFromFile(Library& library) {
    ifstream file(usersFileName);
    if (!file.is_open()) {
        cout << "Aucun fichier d'utilisateurs existant trouvé. Démarrage sans utilisateurs enregistrés.\n";
        return false;
    }
    
    string line;
    int count = 0;
    while (getline(file, line)) {
        if (!line.empty()) {
            User user;
            user.fromFileFormat(line);
            library.addUser(user);
            count++;
        }
    }
    
    file.close();
    cout << "Chargé " << count << " utilisateur(s) depuis le fichier.\n";
    return true;
}

// Check if file exists
bool FileManager::fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

// Create backup
void FileManager::createBackup() {
    try {
        if (fileExists(booksFileName)) {
            filesystem::copy_file(booksFileName, booksFileName + ".backup", filesystem::copy_options::overwrite_existing);
        }
        
        if (fileExists(usersFileName)) {
            filesystem::copy_file(usersFileName, usersFileName + ".backup", filesystem::copy_options::overwrite_existing);
        }
        
        cout << "Fichiers de sauvegarde créés.\n";
    } catch (const filesystem::filesystem_error& e) {
        cerr << "Erreur lors de la création de la sauvegarde : " << e.what() << "\n";
    }
}
