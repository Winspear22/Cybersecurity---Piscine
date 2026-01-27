# 🕷️ GUIDE DE CONTINUATION - SPIDER

Tu t'es arrêté à la fin du **Module 2**. Voici ce qu'il reste à faire pour demain :

---

## ✅ ÉTAT DES LIEUX

- **Parsing Arguments** (`-r`, `-l`, `-p`) : OK
- **Requêtes HTTP** : OK
- **Parsing HTML** (Liens images) : OK
- **Filtrage Extensions** (`_is_valid_extension`) : OK
- **Téléchargement** : OK (Linéaire pour l'instant)

---

## 🏗️ PROCHAINE ÉTAPE : LE MODULE 3 (BFS / Récursivité)

C'est le gros morceau pour transformer ton "téléchargeur simple" en "vrai spider".

### 1. Modifier `spider.hpp`
Ajouter les structures pour gérer la file d'attente :
```cpp
#include <set>
#include <vector>
#include <utility> // pour std::pair

// Dans la classe Spider (private) :
std::set<std::string>                  _visited_urls; // Pour ne pas tourner en rond
std::vector<std::pair<std::string, int>> _url_queue;    // File d'attente {URL, Profondeur}
```

### 2. Modifier `spider.cpp` -> `run()`
Remplacer la logique linéaire actuelle par une boucle BFS :
```cpp
void Spider::run() {
    // 1. Initialisation
    _url_queue.push_back({_start_url, 0});
    _visited_urls.insert(_start_url);

    // 2. Boucle TANT QUE la file n'est pas vide
    while (!_url_queue.empty()) {
        // A. Récupérer le prochain url à traiter
        std::string current_url = _url_queue.front().first;
        int current_depth = _url_queue.front().second;
        _url_queue.erase(_url_queue.begin());

        // B. Télécharger et Parser les images (comme avant)
        // ... (ton code actuel de téléchargement d'images) ...

        // C. RÉCURSION (Si activée et profondeur ok)
        if (_recursive && current_depth < _max_depth) {
            // Chercher les liens <a> dans la page
            // Pour chaque lien trouvé :
            //    Si pas visité : ajouter à la queue avec (current_depth + 1)
        }
    }
}
```

### 3. Nouvelle fonction : `_parse_links(html)`
Tu auras besoin d'une fonction (similaire à `_parse_html` pour les images) qui cherche les balises `<a href="...">` pour remplir la queue si le mode récursif est activé.

---

## 💾 MODULE 4 : LE DOSSIER DE SAUVEGARDE

Actuellement, tout est sauvegardé à la racine ou dans `./data` par défaut.
Il faudra modifier `Saver.cpp` pour utiliser la variable `_output_dir_path` (l'option `-p`) et créer le dossier si besoin (avec `mkdir`).

---

Bonne nuit et bon courage pour la reprise ! 🚀
