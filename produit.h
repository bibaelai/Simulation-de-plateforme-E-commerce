#ifndef PRODUIT_H
#define PRODUIT_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdexcept>

using namespace std;

// ============================================================
// ENUMS
// ============================================================

enum class Taille { XS, S, M, L, XL, XXL };
string tailleToString(Taille t);
Taille stringToTaille(const string& s);

enum class Couleur {
    ROUGE, BLEU, VERT, NOIR, BLANC, GRIS,
    JAUNE, ROSE, ORANGE, VIOLET, MARRON, BEIGE
};
string couleurToString(Couleur c);
Couleur stringToCouleur(const string& s);

enum class Statut { DISPONIBLE, RUPTURE_STOCK, ARCHIVE };
string statutToString(Statut s);

// ============================================================
// CLASSE ABSTRAITE : Produit
// ============================================================

class Produit {
protected:
    int id;
    string nom;
    double prix;
    int stock;
    string categorie;
    Statut statut;

public:
    Produit(int id, const string& nom, double prix,
            int stock, const string& categorie);

    virtual ~Produit();

    // Getters
    int getId() const;
    string getNom() const;
    double getPrix() const;
    int getStock() const;// Setters
void setNom(const string& nom);
void setPrix(double prix);
void setStock(int stock);
void setCategorie(const string& categorie);
void setStatut(Statut statut);


    // Méthodes
    void afficher() const;
    bool acheter(int qte);
    void reapprovisionner(int qte);

    // Méthodes virtuelles pures
    virtual double appliquerRemise(double p) = 0;
    virtual void afficherDetails() const = 0;

    // Opérateur
    friend ostream& operator<<(ostream& os, const Produit& p);
};

// ============================================================
// CLASSES FILLES
// ============================================================

// ── Electronique ─────────────────────────────
class Electronique : public Produit {
protected:
    string marque;
    int garantie;

public:
    Electronique(int id, const string& nom, double prix, int stock,
                 const string& marque, int garantie);

    double appliquerRemise(double p) override;
    void afficherDetails() const override;
};

// ── Smartphone ─────────────────────────────
class Smartphone : public Electronique {
private:
    int ram;

public:
    Smartphone(int id, const string& nom, double prix, int stock,
               const string& marque, int garantie, int ram);

    void afficherDetails() const override;
};

// ── Ordinateur ─────────────────────────────
class Ordinateur : public Electronique {
private:
    string cpu;

public:
    Ordinateur(int id, const string& nom, double prix, int stock,
               const string& marque, int garantie, const string& cpu);

    void afficherDetails() const override;
};

// ── Alimentation ─────────────────────────────
class Alimentation : public Produit {
private:
    string dateExpiration;

public:
    Alimentation(int id, const string& nom, double prix, int stock,
                 const string& dateExp);

    double appliquerRemise(double p) override;
    void afficherDetails() const override;
};

// ── Livre ─────────────────────────────
class Livre : public Produit {
private:
    string auteur;

public:
    Livre(int id, const string& nom, double prix, int stock,
          const string& auteur);

    double appliquerRemise(double p) override;
    void afficherDetails() const override;
};

#endif