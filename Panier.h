#ifndef PANIER_H
#define PANIER_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "produit.h"
#include "personne.h"
#include "vetement.h"

using namespace std;

class Panier {
protected:
    int idPanier;
    Personne& personne;
    vector<Produit*> items;
    static double tva;

public:
    Panier(int _idPanier, Personne& _personne);
    virtual ~Panier();
    
    void ajouterProduit(Produit* a);
    virtual double calculerTotalHT() const;
    virtual double calculerTotalTTC() const;
    void viderPanier();
    int getTaille() const;
    
    int getIdPanier() const;
    Client getPersonne() const;
    int getIdPersonne() const;
    string getPersonneNom() const;
    
    Panier& operator++();
    Panier& operator--();
    Panier& operator+=(Produit* a);
    Panier& operator-=(int ref);
    Panier& operator=(const Panier& source);
    
    bool operator==(const Panier& other) const;
    bool operator!=(const Panier& other) const;
    
    Produit* operator[](int index);
    const Produit* operator[](int index) const;
    
    operator double() const;
    
    friend ostream& operator<<(ostream& os, const Panier& p);
    
    static double getTVA();
    static void setTVA(double nouvelle);
};

class PanierPrime : public Panier {
private:
    double tauxReduction;
    static double SEUIL_ELIGIBILITE;

public:
    PanierPrime(int _idPanier,Personne& _personne, double _taux = 0.15);
    PanierPrime(const Panier& p, double _taux = 0.15);
    
    double calculerTotalHT() const override;
    double calculerTotalTTC() const override;
    
    double getTauxReduction() const;
    void setTauxReduction(double nouveauTaux);
    
    static bool estEligible(const Panier& p);
    static double getSeuil();
    static void setSeuil(double nouveauSeuil);
};

#endif
