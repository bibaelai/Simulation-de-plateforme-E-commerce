
#ifndef VETEMENT_H
#define VETEMENT_H

#include <vector>
#include <string>
#include "Produit.h"

using namespace std;

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
    // ── Constructeur simple (tailles/couleurs par défaut) ──
    Vetement(int id, const string& nom, double prix, int stock,
             Taille taille, Couleur couleur,
             const string& matiere, const string& marque,
             bool estNeuf = true);

    // ── Constructeur avec listes personnalisées ────────────
    Vetement(int id, const string& nom, double prix, int stock,
             Taille taille, Couleur couleur,
             const string& matiere, const string& marque,
             const vector<Taille>& tailles,
             const vector<Couleur>& couleurs,
             bool estNeuf = true);

    // ── Méthodes virtuelles ────────────────────────────────
    double appliquerRemise(double pourcentage) override;
    void   afficherDetails() const override;

    // ── Sélection interactive ──────────────────────────────
    bool choisirTaille();
    bool choisirCouleur();

    // ── Vérifications ─────────────────────────────────────
    bool tailleDisponible(Taille t)  const;
    bool couleurDisponible(Couleur c) const;

    // ── Gestion des listes ────────────────────────────────
    void ajouterTaille(Taille t);
    void retirerTaille(Taille t);
    void ajouterCouleur(Couleur c);

    // ── Setters ───────────────────────────────────────────
    void setTaille(Taille t);
    void setCouleur(Couleur c);
    void setTailleStr(const string& s);
    void setCouleurStr(const string& s);

    // ── Getters ───────────────────────────────────────────
    Taille  getTaille()    const;
    Couleur getCouleur()   const;
    string  getTailleStr() const;
    string  getCouleurStr() const;
    string  getMatiere()   const;
    string  getMarque()    const;
    bool    isNeuf()       const;

    const vector<Taille>&  getTaillesDisponibles()  const;
    const vector<Couleur>& getCouleursDisponibles() const;

    // ── Surcharge opérateur << ────────────────────────────
    friend ostream& operator<<(ostream& os, const Vetement& v);
};

#endif
