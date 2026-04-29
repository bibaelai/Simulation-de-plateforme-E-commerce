#include <iostream>
#include <string>
using namespace std;

enum EtatCommande { //enum remplace des chiffres incompréhensibles par des mots clairs.
    EN_ATTENTE,
    CONFIRMEE,
    EN_LIVRAISON,
    LIVREE,
    ANNULEE
};

enum EtatCommande {
    EN_ATTENTE,
    CONFIRMEE,
    EN_LIVRAISON,
    LIVREE,
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

    
Commande(int id, Client* c, string date,string adresse, int nb) { 
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
~Commande() {
    delete[] produits;
    delete[] quantites;
}
    // ===== METHODES =====
void ajouterProduit(Produit* p, int quantite) {
    for (int i = 0; i < nb_produits; i++) {
        if (produits[i] == NULL) {
            produits[i]  = p;
            quantites[i] = quantite;
            return;
        }
    }
}
void supprimerProduit(int id_produit) {
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
double calculerTotal() {
    double total = 0;
    for (int i = 0; i < nb_produits; i++) {
        total += produits[i]->getPrix() * quantites[i];
    }
    return total;
}
double calculerTotalAvecLivraison() {
    return calculerTotal() + frais_livraison;
}
void changerEtat(EtatCommande nouvelEtat) {
    etat = nouvelEtat;
}

void afficherCommande() {
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
bool estVide() {
    return nb_produits == 0;
}

    // ===== GETTERS =====
int          getId()             { return id_commande; }
string       getDate()           { return date_commande; }    EtatCommande getEtat()           { return etat; }
int          getNbProduits()     { return nb_produits; }
double       getFraisLivraison() { return frais_livraison; }
string       getAdresse()        { return adresse_livraison; }

    // ===== SETTERS =====
void setPaiement(Paiement* p)    { paiement = p; }
void setAdresse(string adresse)  { adresse_livraison = adresse; }
void setFraisLivraison(double f) { frais_livraison = f; }
};