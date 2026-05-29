# Simulation de Plateforme E-Commerce (eShop)

> Un projet de groupe simulant une mini boutique en ligne style Amazon utilisant les concepts **POO en C++**, une base de données **MySQL**, et un frontend en **JS vanilla**.

![Diagramme UML](uml_diagram.png)

---

## 👥 Contributeurs
- **bibaelai** (Elharchaoui Hiba)
- **soumayamourtafiaa-afk**
- **FatimaEzzahraOulghazi**
- **NouiriFatimazahra**

---

##  Structure du Projet

```
├── eshop-fin.cpp            # Serveur backend C++ (API HTTP + classes POO)
├── index.html               # Frontend (interface web de la boutique)
├── script.js                # Logique frontend (appels API, rendu UI)
├── styles.css               # Styles
├── eShop_DB.sql             # Schéma complet de la base de données MySQL + données
├── httplib.h                # Bibliothèque serveur HTTP (header-only)
├── json.hpp                 # Bibliothèque JSON (header-only)
├── uml_diagram.png          # Diagramme de classes UML
└── eshop_Presentation.pptx  # Presentation Explicative du Project
```

---

## Comment Lancer le Projet ??

---

###  Windows

#### Étape 1 — Installer WSL (Sous-système Windows pour Linux)
Ouvrez **PowerShell en tant qu'Administrateur** et exécutez :
```powershell
wsl --install
```
Redémarrez votre ordinateur. Cela installe Ubuntu sur Windows — à faire une seule fois.

#### Étape 2 — Ouvrir Ubuntu et installer les dépendances
```bash
sudo apt update
sudo apt install g++ libmysqlclient-dev mysql-server -y
```

#### Étape 3 — Cloner le projet
```bash
git clone https://github.com/bibaelai/Simulation-de-plateforme-E-commerce.git
cd Simulation-de-plateforme-E-commerce
```

#### Étape 4 — Démarrer MySQL et importer la base de données
```bash
sudo service mysql start
mysql -u root -p < eShop_DB.sql
```
> Lorsqu'un mot de passe est demandé, appuyez simplement sur **Entrée** (aucun mot de passe par défaut).

#### Étape 5 — Compiler
```bash
g++ -std=c++17 eshop-fin.cpp -o eshop $(mysql_config --cflags --libs)
```

#### Étape 6 — Lancer le serveur
```bash
./eshop
```
Vous devriez voir :
```
C++ eShop server running on http://localhost:3000
```

#### Étape 7 — Ouvrir le frontend
Ouvrez `index.html` dans votre navigateur (Chrome ou Edge). ✅
By ouvrir le fichier "index.html" dans visual studio code, click droit, choisir "ouvrir avec live server"
(vous devez avoir l'extention "live server" installer) 

---

### Mac

#### Étape 1 — Installer Homebrew (si ce n'est pas déjà fait)
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

#### Étape 2 — Installer les dépendances
```bash
brew install mysql
brew services start mysql
```

#### Étape 3 — Cloner le projet
```bash
git clone https://github.com/bibaelai/Simulation-de-plateforme-E-commerce.git
cd Simulation-de-plateforme-E-commerce
```

#### Étape 4 — Importer la base de données
```bash
mysql -u root -p < eShop_DB.sql
```
> Appuyez sur **Entrée** si aucun mot de passe n'est défini.

#### Étape 5 — Compiler
```bash
g++ -std=c++17 eshop-fin.cpp -o eshop $(mysql_config --cflags --libs)
```

#### Étape 6 — Lancer le serveur
```bash
./eshop
```
Vous devriez voir :
```
C++ eShop server running on http://localhost:3000
```

#### Étape 7 — Ouvrir le frontend
Ouvrez `index.html` dans Safari ou Chrome. 

---

### 🐧 Linux (Ubuntu/Debian)

#### Étape 1 — Installer les dépendances
```bash
sudo apt update
sudo apt install g++ libmysqlclient-dev mysql-server -y
```

#### Étape 2 — Démarrer MySQL
```bash
sudo service mysql start
```

#### Étape 3 — Cloner le projet
```bash
git clone https://github.com/bibaelai/Simulation-de-plateforme-E-commerce.git
cd Simulation-de-plateforme-E-commerce
```

#### Étape 4 — Importer la base de données
```bash
mysql -u root -p < eShop_DB.sql
```
> Appuyez sur **Entrée** si aucun mot de passe n'est défini.

#### Étape 5 — Compiler
```bash
g++ -std=c++17 eshop-fin.cpp -o eshop $(mysql_config --cflags --libs)
```

#### Étape 6 — Lancer le serveur
```bash
./eshop
```
Vous devriez voir :
```
C++ eShop server running on http://localhost:3000
```

#### Étape 7 — Ouvrir le frontend
Ouvrez `index.html` dans votre navigateur. ✅

---

## ⚠️ Résolution des Problèmes

| Problème | Solution |
|---|---|
| `mysql_config not found` | Exécutez `sudo apt install libmysqlclient-dev` (Linux/WSL) ou `brew install mysql` (Mac) |
| `Can't connect to MySQL` | Exécutez `sudo service mysql start` (Linux/WSL) ou `brew services start mysql` (Mac) |
| `Access denied for user root` | Exécutez `sudo mysql` puis `ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY ''; FLUSH PRIVILEGES;` |
| Port 3000 déjà utilisé | Terminez le processus : `lsof -ti:3000 \| xargs kill` |
| Images non affichées | Assurez-vous de lancer `./eshop` depuis le dossier racine du projet |

---

## 📌 Notes

- Le serveur tourne sur `http://localhost:3000`
- Le frontend se connecte automatiquement au backend 
- Les images des produits sont stockées localement dans `uploads/produits/` 
- La base de données inclut des produits, utilisateurs et commandes exemples
