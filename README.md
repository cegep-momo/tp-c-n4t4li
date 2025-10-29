# Construire le projet
Vous pouvez utiliser un dev container de base C++ de VScode.
Le projet utilise cmake, pensez à l'inclure dans votre dev container.

Voici les lignes de commandes pour compiler le projet:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

# Répertoire data

Il contient 2 fichiers `books.txt`et `users.txt` que vous pouvez utilisez pour tester votre code.
Pour ca il suffit de les copiers dans le repertoire `build` avec l'application `bibliotheque`


# Veille technologique

# Question 1 : C++

J'ai utilisé les std::unique_ptr pour gérer la mémoire automatiquement. C'est comme un pointeur intelligent qui se détruit tout seul quand on n'en a plus besoin. Pas besoin de faire delete, pas de fuites de mémoire. Les livres et utilisateurs sont dans des vecteurs de unique_ptr, donc quand la bibliothèque ferme, tout se nettoie tout seul. J'utilise aussi std::find_if pour chercher des livres rapidement. C'est plus safe et plus simple que les pointeurs normaux.

# Question 2 : Options de développement possible

Pour gérer des millions de livres, je créerais une API REST avec Python et FastAPI, couplée à une base de données PostgreSQL. Notre application C++ deviendrait un client léger qui communiquerait avec l'API via des requêtes HTTP. J'utiliserais la bibliothèque cpr pour envoyer des requêtes depuis le C++ et nlohmann/json pour parser les réponses. Cette architecture permettrait de scaler horizontalement le backend tout en conservant notre interface C++ existante, et pourrait même supporter des applications web et mobiles simultanément.