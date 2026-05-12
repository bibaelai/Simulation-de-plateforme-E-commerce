#ifndef COMMANDES_H
#define COMMANDES_H
#include "produit.h"
<<<<<<< HEAD
#include "client.h"
#include "panier.h"
=======
<<<<<<< HEAD
=======
#include "client.h"
#include "panier.h"
>>>>>>> 54252bd71ffd31c2f76dfb13ef8acd8086245ae0
>>>>>>> main

#include <iostream>
#include <string>
using namespace std;

enum EtatCommande { //enum remplace des chiffres incompréhensibles par des mots clairs.
    CONFIRMEE,
<<<<<<< HEAD
    EN_ATTENTE,
=======
<<<<<<< HEAD
=======
    EN_ATTENTE,
>>>>>>> 54252bd71ffd31c2f76dfb13ef8acd8086245ae0
>>>>>>> main
    ANNULEE
};

class Commande {
private:
    // ===== ATTRIBUTS =====
    int          id_commande;
    string       date_commande;
    EtatCommande etat;
    Client*      client;  // pointeur vers le client qui a commandé
    Produit**    produits;        // tableau dynamique ex: [*iPhone, *TV, *PC],pointeur vers tableau de pointeurs
    int*         quantites;  // chaque quantites[i] correspond à produits[i]

    int          nb_produits;    
    Paiement*    paiement;
    string       adresse_livraison;
    double       frais_livraison;

public:
    // ===== CONSTRUCTEUR / DESTRUCTEUR =====
    Commande(int id, Client* c, string date,string adresse, int nb);
    ~Commande();

    // ===== METHODES =====
    void   ajouterProduit(Produit* p, int quantite);
    void   supprimerProduit(int id_produit);
    double calculerTotal();
    double calculerTotalAvecLivraison();
    void   changerEtat(EtatCommande nouvelEtat);
    void   afficherCommande();
    bool   estVide();

    // ===== GETTERS =====
    int          getId();
    string       getDate();    EtatCommande getEtat();
    int          getNbProduits();
    double       getFraisLivraison();
    string       getAdresse();

    // ===== SETTERS =====
    void setPaiement(Paiement* p);
    void setAdresse(string adresse);
    void setFraisLivraison(double f);
};
#endif
