#ifndef VETEMENT_H
#define VETEMENT_H
#include <vector>
#include <string>
using namespace std;
#include "Produit.h"

class Vetement : public Produit {
protected:
    Taille taille;
    Couleur couleur;
    string matiere;
    string marque;
    bool   estNeuf;
    vector<Taille>  taillesDisponibles;
    vector<Couleur> couleursDisponibles;

public:
    Vetement(int id, const string& nom, double prix, int stock,
             Taille taille, Couleur couleur,
             const string& matiere, const string& marque,
             bool estNeuf = true);
    // ... autres méthodes    Vetemen Vetement(int id, const string& nom, double prix, int stock,t(int id, const string& nom, double prix, int stock,

         Vetement(int id, const string& nom, double prix, int stock,     Taille taille, Couleur couleur,
             const string& matiere, const string& marque,
             const vector<Taille>& tailles,
             const vector<Couleur>& couleurs,
             bool estNeuf = true);

    double appliquerRemise(double pourcentage) override;
    void afficherDetails() const override;
     bool choisirTaille();
    bool choisirCouleur();

    // Vérification
    bool tailleDisponible(Taille t) const;
    bool couleurDisponible(Couleur c) const;

    // Gestion
    void ajouterTaille(Taille t);
    void retirerTaille(Taille t);
    void ajouterCouleur(Couleur c);

    // Setters
    void setTaille(Taille t);
    void setCouleur(Couleur c);
    void setTailleStr(const string& s);
    void setCouleurStr(const string& s);

    // Getters
    Taille getTaille() const;
    Couleur getCouleur() const;
    string getTailleStr() const;
    string getCouleurStr() const;
    string getMatiere() const;
    string getMarque() const;
    bool isNeuf() const;

    const vector<Taille>& getTaillesDisponibles() const;
    const vector<Couleur>& getCouleursDisponibles() const;

    // Surcharge opérateur
    friend ostream& operator<<(ostream& os, const Vetement& v);
};
    // etc.


#endif