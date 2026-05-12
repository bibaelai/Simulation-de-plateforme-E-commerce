#include "Commandes.h"

    // ===== CONSTRUCTEUR / DESTRUCTEUR =====

    
Commande::Commande(int id, Client* c, string date,string adresse, int nb) { 
    id_commande       = id;
    client            = c;
    date_commande     = date;
    adresse_livraison = adresse;  // ex: "12 Rue Hassan II, Agadir"
    nb_produits       = nb;       // nombre de produits dans la commande
    etat              = EN_ATTENTE;
    frais_livraison   = 25.0; // fixé à 25.0 DH par défaut
    paiement          = NULL;

    // Allocation selon la taille du Panier
    produits  = new Produit*[nb_produits];
    quantites = new int[nb_produits];
}
Commande::~Commande() {
    delete[] produits;
    delete[] quantites;
}
    // ===== METHODES =====
void Commande::ajouterProduit(Produit* p, int quantite) {
    for (int i = 0; i < nb_produits; i++) {
        if (produits[i] == NULL) {
            produits[i]  = p;
            quantites[i] = quantite;
            return;
        }
    }
    cout << "Erreur : panier plein." << endl;
}
void Commande::supprimerProduit(int id_produit) {
    for (int i = 0; i < nb_produits; i++) {
        if (produits[i]->getId() == id_produit) {
            for (int j = i; j < nb_produits - 1; j++) {
                produits[j]  = produits[j+1];
                quantites[j] = quantites[j+1];
            }
            nb_produits--;
            cout << "Produit supprime." << endl;
            return;
        }
    }
    cout << "Produit non trouve." << endl;
}
double commande::calculerTotal() {  //le prix du produit selon sa quantité
    double total = 0;
    for (int i = 0; i < nb_produits; i++) {
        total += produits[i]->getPrix() * quantites[i];
    }
    return total;
}
<<<<<<< HEAD
double Commande::calculerTotalAvecLivraison() {   //le prix du produit selon sa quantité avec les frais de livraison 
=======
<<<<<<< HEAD
double commande::calculerTotalAvecLivraison() {   //le prix du produit selon sa quantité avec les frais de livraison 
=======
double Commande::calculerTotalAvecLivraison() {   //le prix du produit selon sa quantité avec les frais de livraison 
>>>>>>> 54252bd71ffd31c2f76dfb13ef8acd8086245ae0
>>>>>>> main
    return calculerTotal() + frais_livraison;
}
void Commande::changerEtat(EtatCommande nouvelEtat) {
    etat = nouvelEtat;
}

void Commande::afficherCommande() {
    cout << "===== Commande #" << id_commande << " =====" << endl;
    cout << "Date       : " << date_commande        << endl;
    cout << "Adresse    : " << adresse_livraison     << endl;
    cout << "--- Produits ---"                       << endl;
    for (int i = 0; i < nb_produits; i++) {
        cout << "- " << produits[i]->getNom()
             << " x" << quantites[i]
             << " = " << produits[i]->getPrix() * quantites[i]
             << " DH" << endl;
    }
    cout << "Sous-total : " << calculerTotal()             << " DH" << endl;
    cout << "Livraison  : " << frais_livraison             << " DH" << endl;
    cout << "TOTAL      : " << calculerTotalAvecLivraison()<< " DH" << endl;
}
bool Commande::estVide() {
    return nb_produits == 0;
}

    // ===== GETTERS =====
int          Commande::getId() { return id_commande; }
string       Commande::getDate() { return date_commande; }  
EtatCommande Commande::getEtat() { return etat; }
int          Commande::getNbProduits() { return nb_produits; }
double       Commande::getFraisLivraison() { return frais_livraison; }
string       Commande::getAdresse()        { return adresse_livraison; }

    // ===== SETTERS =====
void Commande::setPaiement(Paiement* p)    { paiement = p; }
void Commande::setAdresse(string adresse)  { adresse_livraison = adresse; }
void Commande::setFraisLivraison(double f) { frais_livraison = f; }
