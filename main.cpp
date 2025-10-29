#include <iostream>
#include <limits>
#include <string>
#include <algorithm>

#include "library.h"
#include "filemanager.h"

using namespace std;

// Clears the screen (works on both Windows and Linux)
void clearScreen() {
    system("cls || clear");
}

// Pauses until Enter is pressed
void pauseForInput() {
    cout << "\nAppuyez sur Entrée pour continuer...";
    string dummy;
    getline(cin, dummy);
}

// Trim leading and trailing whitespace
static inline void trim(string& s) {
    const auto first = s.find_first_not_of(" \t\n\r");
    if (first == string::npos) { s.clear(); return; }
    const auto last = s.find_last_not_of(" \t\n\r");
    s = s.substr(first, last - first + 1);
}

// Reads a non-empty input line
string getInput(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        trim(input);
        
        if (input.empty()) {
            cout << "Erreur : Ce champ ne peut pas être vide. Veuillez réessayer.\n";
        } else {
            return input;
        }
    }
}

// Displays the main menu
void displayMenu() {
    cout << "\n=== SYSTÈME DE GESTION DE BIBLIOTHÈQUE PERSONNELLE ===\n";
    cout << "1.  Ajouter un Livre\n";
    cout << "2.  Supprimer un Livre\n";
    cout << "3.  Rechercher des Livres par Titre\n";
    cout << "4.  Rechercher des Livres par Auteur\n";
    cout << "5.  Afficher Tous les Livres\n";
    cout << "6.  Afficher les Livres Disponibles\n";
    cout << "7.  Ajouter un Utilisateur\n";
    cout << "8.  Afficher Tous les Utilisateurs\n";
    cout << "9.  Emprunter un Livre\n";
    cout << "10. Retourner un Livre\n";
    cout << "11. Statistiques de la Bibliothèque\n";
    cout << "12. Sauvegarder les Données\n";
    cout << "13. Créer une Sauvegarde\n";
    cout << "0.  Quitter\n";
    cout << "======================================================\n";
    cout << "Entrez votre choix : ";
}

int main() {
    Library library;
    FileManager fileManager;

    // Load existing data
    cout << "Chargement des données de la bibliothèque...\n";
    fileManager.loadLibraryData(library);

    int choice;
    bool running = true;

    while (running) {
        displayMenu();

        if (!(cin >> choice)) {
            cout << "Saisie invalide. Veuillez entrer un nombre.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pauseForInput();
            continue;
        }
        // Clear leftover newline before getline
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {

            // =======================
            //  AJOUT DE LIVRE AVEC VALIDATION ROBUSTE
            // =======================
            case 1: {
                string title  = getInput("Entrez le titre du livre : ");
                string author = getInput("Entrez l'auteur du livre : ");
                string isbn;

                bool isbnValide = false;

                // boucle pour que l'utilisateur saisisse un ISBN valide
                while (!isbnValide) {
                    isbn = getInput("Entrez l'ISBN du livre (13 chiffres) : ");

                    // vide
                    if (isbn.empty()) {
                        cout << "Erreur : L'ISBN ne peut pas être vide.\n";
                        continue;
                    }
                    // taille exacte
                    if (isbn.size() != 13) {
                        cout << "Erreur : L'ISBN doit contenir exactement 13 chiffres.\n";
                        continue;
                    }
                    // que des chiffres
                    if (!all_of(isbn.begin(), isbn.end(), ::isdigit)) {
                        cout << "Erreur : L'ISBN doit contenir uniquement des chiffres.\n";
                        continue;
                    }
                    // doublon d’ISBN
                    if (library.findBookByISBN(isbn)) {
                        cout << "Erreur : Un livre avec l'ISBN " << isbn << " existe déjà.\n";
                        continue;
                    }

                    
                    isbnValide = true;
                }

                // verifier les doublons (titre + auteur)
                auto toLower = [](string s) {
                    transform(s.begin(), s.end(), s.begin(), ::tolower);
                    return s;
                };

                string titreMin  = toLower(title);
                string auteurMin = toLower(author);
                bool doublon = false;

                auto possibles = library.searchBooksByTitle(title);
                for (auto* b : possibles) {
                    if (toLower(b->getAuthor()) == auteurMin) {
                        doublon = true;
                        break;
                    }
                }

                if (doublon) {
                    cout << "Attention : un livre avec le même titre et auteur existe déjà.\n";
                    pauseForInput();
                    break;
                }

                // ajouter le livre si tout est valide
                Book nouveau(title, author, isbn);
                library.addBook(nouveau);
                cout << "Livre ajouté avec succès !\n";
                pauseForInput();
                break;
            }

            case 2: { // Remove Book
                string isbn = getInput("Entrez l'ISBN du livre à supprimer : ");
                if (library.removeBook(isbn)) {
                    cout << "Livre supprimé avec succès !\n";
                } else {
                    cout << "Livre non trouvé.\n";
                }
                pauseForInput();
                break;
            }

            case 3: { // Search by Title
                string title = getInput("Entrez le titre à rechercher : ");
                auto results = library.searchBooksByTitle(title);

                if (results.empty()) {
                    cout << "Aucun livre trouvé avec ce titre.\n";
                } else {
                    cout << "\n=== RÉSULTATS DE RECHERCHE ===\n";
                    for (size_t i = 0; i < results.size(); ++i) {
                        cout << "\nRésultat " << (i + 1) << " :\n";
                        cout << results[i]->toString() << "\n";
                        cout << "-----------------------------\n";
                    }
                }
                pauseForInput();
                break;
            }

            case 4: { // Search by Author
                string author = getInput("Entrez l'auteur à rechercher : ");
                auto results = library.searchBooksByAuthor(author);

                if (results.empty()) {
                    cout << "Aucun livre trouvé de cet auteur.\n";
                } else {
                    cout << "\n=== RÉSULTATS DE RECHERCHE ===\n";
                    for (size_t i = 0; i < results.size(); ++i) {
                        cout << "\nRésultat " << (i + 1) << " :\n";
                        cout << results[i]->toString() << "\n";
                        cout << "-----------------------------\n";
                    }
                }
                pauseForInput();
                break;
            }

            case 5: // Display All Books
                library.displayAllBooks();
                pauseForInput();
                break;

            case 6: // Display Available Books
                library.displayAvailableBooks();
                pauseForInput();
                break;

            case 7: { // Add User
                string name = getInput("Entrez le nom de l'utilisateur : ");
                string userId = getInput("Entrez l'ID de l'utilisateur : ");

                if (library.findUserById(userId)) {
                    cout << "Erreur : Un utilisateur avec l'ID " << userId << " existe déjà.\n";
                } else {
                    User newUser(name, userId);
                    library.addUser(newUser);
                    cout << "Utilisateur ajouté avec succès !\n";
                }
                pauseForInput();
                break;
            }

            case 8: // Display All Users
                library.displayAllUsers();
                pauseForInput();
                break;

            case 9: { // Check Out Book
                string isbn = getInput("Entrez l'ISBN du livre à emprunter : ");
                string userId = getInput("Entrez l'ID de l'utilisateur : ");

                if (library.checkOutBook(isbn, userId)) {
                    cout << "Livre emprunté avec succès !\n";
                } else {
                    cout << "Erreur : Impossible d'emprunter le livre. Vérifiez l'ISBN, l'ID utilisateur et la disponibilité du livre.\n";
                }
                pauseForInput();
                break;
            }

            case 10: { // Return Book
                string isbn = getInput("Entrez l'ISBN du livre à retourner : ");
                if (library.returnBook(isbn)) {
                    cout << "Livre retourné avec succès !\n";
                } else {
                    cout << "Erreur : Impossible de retourner le livre.\n";
                }
                pauseForInput();
                break;
            }

            case 11: { // Library Statistics
                cout << "\n=== STATISTIQUES DE LA BIBLIOTHÈQUE ===\n";
                cout << "Total des Livres : " << library.getTotalBooks() << "\n";
                cout << "Livres Disponibles : " << library.getAvailableBookCount() << "\n";
                cout << "Livres Empruntés : " << library.getCheckedOutBookCount() << "\n";
                cout << "Total des Utilisateurs : " << library.getAllUsers().size() << "\n";
                pauseForInput();
                break;
            }

            case 12: { // Save Data
                if (fileManager.saveLibraryData(library)) {
                    cout << "Données de la bibliothèque sauvegardées avec succès !\n";
                } else {
                    cout << "Erreur lors de la sauvegarde des données de la bibliothèque.\n";
                }
                pauseForInput();
                break;
            }

            case 13: { // Create Backup
                fileManager.createBackup();
                pauseForInput();
                break;
            }

            case 0: // Exit
                cout << "Sauvegarde des données avant la fermeture...\n";
                fileManager.saveLibraryData(library);
                cout << "Merci d'avoir utilisé le Système de Gestion de Bibliothèque Personnelle !\n";
                running = false;
                break;

            default:
                cout << "Choix invalide. Veuillez réessayer.\n";
                pauseForInput();
                break;
        }
    }

    return 0;
}
