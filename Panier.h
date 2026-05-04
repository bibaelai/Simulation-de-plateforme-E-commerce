#ifndef PANIER_H
#define PANIER_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include "Article.h"
#include "Client.h"

using namespace std;

class Panier {
protected:
    int idPanier;
    Client client;
    vector<Article> items;
    static double tva;

public:
    Panier(int _idPanier, const Client& _client);
    virtual ~Panier();
    
    void ajouterArticle(const Article& a);
    virtual double calculerTotalHT() const;
    virtual double calculerTotalTTC() const;
    void sauvegarderAction(string action) const;
    void viderPanier();
    int getTaille() const;
    
    int getIdPanier() const;
    Client getClient() const;
    int getIdClient() const;
    string getClientNom() const;
    
    Panier& operator++();
    Panier& operator--();
    Panier& operator+=(const Article& a);
    Panier& operator-=(string ref);
    Panier& operator=(const Panier& source);
    
    bool operator==(const Panier& other) const;
    bool operator!=(const Panier& other) const;
    
    Article& operator[](int index);
    const Article& operator[](int index) const;
    
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
    PanierPrime(int _idPanier, const Client& _client, double _taux = 0.15);
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
