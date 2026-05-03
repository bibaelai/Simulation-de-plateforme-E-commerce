#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdexcept>
using namespace std;

// ============================================================
//  ENUM : Taille
// ============================================================
enum class Taille { XS, S, M, L, XL, XXL };

string tailleToString(Taille t);
Taille stringToTaille(const string& s);

// ============================================================
//  ENUM : Couleur
// ============================================================
enum class Couleur {
    ROUGE, BLEU, VERT, NOIR, BLANC, GRIS,
    JAUNE, ROSE, ORANGE, VIOLET, MARRON, BEIGE
};

string couleurToString(Couleur c);
Couleur stringToCouleur(const string& s);

// ============================================================
//  ENUM : Statut
// ============================================================
enum class Statut { DISPONIBLE, RUPTURE_STOCK, ARCHIVE };

string statutToString(Statut s);

// ============================================================
//  CLASSE ABSTRAITE : Produit
// ============================================================
class Produit {
protected:
    int    id;
    string nom;
    double prix;
    int    stock;
    double note;
    string categorie;
    Statut statut;

public:
    Produit(int id, const string& nom, double prix,
            int stock, const string& categorie,
            double note = 0.0);

    virtual ~Produit();

    // Getters
    int    getId()        const;
    string getNom()       const;
    double getPrix()      const;
    int    getStock()     const;
    double getNote()      const;
    string getCategorie() const;
    Statut getStatut()    const;

    // Setters
    void setPrix(double p);
    void setStock(int s);
    void setNote(double n);
    void archiver();

    // Concrete methods
    void   afficher()               const;
    bool   estDisponible()          const;
    bool   acheter(int quantite = 1);
    void   reapprovisionner(int quantite);
    string toString()               const;

    // Pure virtual methods (ABSTRACTION)
    virtual double appliquerRemise(double pourcentage) = 0;
    virtual void   afficherDetails()                   const = 0;

    // Operator overloading (FRIEND FUNCTION)
    friend ostream& operator<<(ostream& os, const Produit& p);
    bool operator==(const Produit& autre) const;
    bool operator<(const Produit& autre)  const;
};

// ============================================================
//  CLASSE : Vetement  (inherits Produit)
// ============================================================
class Vetement : public Produit {
protected:
    Taille  taille;
    Couleur couleur;
    string  matiere;
    string  marque;
    bool    estNeuf;
    vector<Taille>  taillesDisponibles;
    vector<Couleur> couleursDisponibles;

public:
    // Constructor — all sizes and colors available by default
    Vetement(int id, const string& nom, double prix, int stock,
             Taille taille, Couleur couleur,
             const string& matiere, const string& marque,
             bool estNeuf = true);

    // Constructor — custom sizes and colors
    Vetement(int id, const string& nom, double prix, int stock,
             Taille taille, Couleur couleur,
             const string& matiere, const string& marque,
             const vector<Taille>&  tailles,
             const vector<Couleur>& couleurs,
             bool estNeuf = true);

    virtual ~Vetement();

    // Getters
    Taille  getTaille()     const;
    Couleur getCouleur()    const;
    string  getTailleStr()  const;
    string  getCouleurStr() const;
    string  getMatiere()    const;
    string  getMarque()     const;
    bool    isNeuf()        const;
    const vector<Taille>&  getTaillesDisponibles()  const;
    const vector<Couleur>& getCouleursDisponibles() const;

    // Setters
    void setTaille(Taille t);
    void setCouleur(Couleur c);
    void setTailleStr(const string& s);
    void setCouleurStr(const string& s);

    // Availability checks
    bool tailleDisponible(Taille t)   const;
    bool couleurDisponible(Couleur c) const;
    void ajouterTaille(Taille t);
    void retirerTaille(Taille t);
    void ajouterCouleur(Couleur c);

    // Interactive console selection
    bool choisirTaille();
    bool choisirCouleur();

    // POLYMORPHISM : override pure virtuals
    double appliquerRemise(double pourcentage) override;
    void   afficherDetails()                   const override;

    // Operator overloading (FRIEND FUNCTION)
    friend ostream& operator<<(ostream& os, const Vetement& v);
};

// ============================================================
//  CLASSE : Electronique  (inherits Produit)
// ============================================================
class Electronique : public Produit {
protected:
    string marque;
    int    garantieMois;
    double puissanceWatts;
    double tensionVolts;

public:
    Electronique(int id, const string& nom, double prix, int stock,
                 const string& marque, int garantieMois,
                 double puissanceWatts = 0, double tensionVolts = 220);

    virtual ~Electronique();

    // Getters
    string getMarque()    const;
    int    getGarantie()  const;
    double getPuissance() const;
    double getTension()   const;

    // POLYMORPHISM : override pure virtuals
    double appliquerRemise(double pourcentage) override;
    void   afficherDetails()                   const override;
};

// ============================================================
//  CLASSE : Smartphone  (inherits Electronique)
// ============================================================
class Smartphone : public Electronique {
private:
    int    ramGo;
    string os;
    string processeur;
    int    cameraMp;
    bool   support5G;
    double tailleEcranPouces;

public:
    Smartphone(int id, const string& nom, double prix, int stock,
               const string& marque, int garantieMois,
               int ramGo, const string& os, const string& processeur,
               int cameraMp, bool support5G, double ecran);

    // Getters
    int    getRAM()    const;
    string getOS()     const;
    int    getCamera() const;
    bool   has5G()     const;

    // POLYMORPHISM : override
    void afficherDetails() const override;
};

// ============================================================
//  CLASSE : Ordinateur  (inherits Electronique)
// ============================================================
class Ordinateur : public Electronique {
private:
    string cpu;
    string gpu;
    int    stockageGo;
    string typeStockage;
    string typeOrdi;

public:
    Ordinateur(int id, const string& nom, double prix, int stock,
               const string& marque, int garantieMois,
               const string& cpu, const string& gpu,
               int stockageGo, const string& typeStockage,
               const string& typeOrdi);

    // Getters
    string getCPU()      const;
    string getGPU()      const;
    int    getStockage() const;
    string getTypeOrdi() const;

    // POLYMORPHISM : override
    void afficherDetails() const override;
};

// ============================================================
//  CLASSE : Alimentation  (inherits Produit)
// ============================================================
class Alimentation : public Produit {
private:
    string dateExpiration;
    double poidsKg;
    int    caloriesPer100g;
    bool   bio;
    string allergenes;
    string paysOrigine;

public:
    Alimentation(int id, const string& nom, double prix, int stock,
                 const string& dateExp, double poids,
                 int calories, bool bio,
                 const string& allergenes = "",
                 const string& pays = "Maroc");

    // Getters
    string getDateExpiration() const;
    double getPoids()          const;
    int    getCalories()       const;
    bool   isBio()             const;
    string getAllergenes()     const;

    bool estPerime(const string& dateAujourdhui) const;

    // POLYMORPHISM : override pure virtuals
    double appliquerRemise(double pourcentage) override;
    void   afficherDetails()                   const override;
};

// ============================================================
//  CLASSE : Livre  (inherits Produit)
// ============================================================
class Livre : public Produit {
private:
    string auteur;
    string isbn;
    int    nombrePages;
    string editeur;
    string langue;
    int    anneePublication;
    string genre;

public:
    Livre(int id, const string& nom, double prix, int stock,
          const string& auteur, const string& isbn,
          int pages, const string& editeur,
          const string& langue = "Français",
          int annee = 2024,
          const string& genre = "Roman");

    // Getters
    string getAuteur()  const;
    string getISBN()    const;
    int    getPages()   const;
    string getEditeur() const;
    string getGenre()   const;

    // POLYMORPHISM : override pure virtuals
    double appliquerRemise(double pourcentage) override;
    void   afficherDetails()                   const override;
};
