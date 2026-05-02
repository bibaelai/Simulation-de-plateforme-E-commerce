#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ==================== PERSONNE (Abstract Base) ====================

class Personne {
protected:
    int    identifiant;
    string prenom;
    string nom;
    string email;
    string motDePasse;
    bool   estConnecte;

public:
    // Default constructor
    Personne();

    // Parameterized constructor
    Personne(int id, string prenom, string nom, string email, string motDePasse);

    // Virtual destructor (needed for polymorphism)
    virtual ~Personne();

    // Getters
    int    getId()             const;
    string getPrenom()         const;
    string getNom()            const;
    string getNomComplet()     const;
    string getEmail()          const;
    bool   getStatutConnexion() const;

    // Setters
    void setPrenom(string prenom);
    void setNom(string nom);
    void setEmail(string email);

    // Pure virtual = ABSTRACTION
    virtual void afficherRole()    = 0;
    virtual void tableauDeBord()   = 0;

    // Login
    bool seConnecter(string email, string motDePasse);

    // Logout
    void seDeconnecter();

    // Sign up
    void sInscrire(string prenom, string nom, string email, string motDePasse);

    // Friend function (FRIEND FUNCTION)
    friend ostream& operator<<(ostream& out, const Personne& p);
};

// ==================== CLIENT (Buyer) ====================

class Client : public Personne {
private:
    string       adresse;
    string       telephone;
    vector<int>  panier;
    vector<int>  quantites;
    string       codePromo;
    float        remisePromo;

public:
    // Default constructor
    Client();

    // Parameterized constructor
    Client(int id, string prenom, string nom, string email, string motDePasse);

    // Destructor
    ~Client();

    // Getters
    string getAdresse()   const;
    string getTelephone() const;

    // Setters
    void setAdresse(string adresse);
    void setTelephone(string telephone);

    // POLYMORPHISM : override pure virtual methods
    void afficherRole()  override;
    void tableauDeBord() override;

    // Add to cart
    void ajouterAuPanier(int idProduit, int quantite);

    // Remove from cart
    void retirerDuPanier(int idProduit);

    // Apply promo code
    void insererCodePromo(string code, float pourcentageRemise);

    // Display cart
    void afficherPanier();

    // Pay
    void payer(float montant);

    // Cancel order
    void annulerCommande();
};

// ==================== VENDEUR ====================

class Vendeur : public Personne {
private:
    string      nomBoutique;
    vector<int> mesProduits;
    float       chiffreAffaires;

public:
    // Default constructor
    Vendeur();

    // Parameterized constructor
    Vendeur(int id, string prenom, string nom, string email,
            string motDePasse, string nomBoutique);

    // Destructor
    ~Vendeur();

    // Getters / Setters
    string getNomBoutique()      const;
    float  getChiffreAffaires()  const;
    void   setNomBoutique(string nomBoutique);

    // POLYMORPHISM : override
    void afficherRole()  override;
    void tableauDeBord() override;

    // Seller methods
    void ajouterProduit(int idProduit);
    void afficherProduits();
    void enregistrerVente(float montant);
};

// ==================== HELPER FUNCTION ====================

Personne* inscrireUtilisateur();
