#include "personne.h"

// ==================== PERSONNE ====================

// Default constructor
Personne::Personne() {
    identifiant  = 0;
    prenom       = "NULL";
    nom          = "NULL";
    email        = "NULL";
    motDePasse   = "NULL";
    estConnecte  = false;
}

// Parameterized constructor
Personne::Personne(int id, string p, string n, string em, string pwd) {
    this->identifiant = id;
    this->prenom      = p;
    this->nom         = n;
    this->email       = em;
    this->motDePasse  = pwd;
    this->estConnecte = false;
}

// Virtual destructor
Personne::~Personne() {
    cout << "Personne " << getNomComplet() << " destroyed.\n";
}

// Getters
int    Personne::getId()              const { return identifiant; }
string Personne::getPrenom()          const { return prenom; }
string Personne::getNom()             const { return nom; }
string Personne::getNomComplet()      const { return prenom + " " + nom; }
string Personne::getEmail()           const { return email; }
bool   Personne::getStatutConnexion() const { return estConnecte; }

// Setters
void Personne::setPrenom(string p) { prenom = p; }
void Personne::setNom(string n)    { nom    = n; }

void Personne::setEmail(string e) {
    if (e.find('@') != string::npos) email = e;
    else cout << "Email invalide : doit contenir '@'\n";
    // ✅ FIX : removed throw, replaced with simple message
}

// Login
bool Personne::seConnecter(string em, string pwd) {
    if (em == email && pwd == motDePasse) {
        estConnecte = true;
        cout << "Bienvenue, " << getNomComplet() << "!\n";
        return true;
    }
    cout << "Email ou mot de passe incorrect !\n";
    return false;
}

// Logout
void Personne::seDeconnecter() {
    estConnecte = false;
    cout << "\nDéconnecté. À bientôt, " << prenom << "!\n";
}

// Sign up
void Personne::sInscrire(string p, string n, string em, string pwd) {
    prenom     = p;
    nom        = n;
    email      = em;
    motDePasse = pwd;
    cout << "Compte créé pour " << getNomComplet() << "!\n";
}

// Friend function (FRIEND FUNCTION)
ostream& operator<<(ostream& out, const Personne& p) {
    out << "Personne #" << p.identifiant
        << " | "        << p.getNomComplet()
        << " | "        << p.email
        << " | "        << (p.estConnecte ? "En ligne" : "Hors ligne");
    return out;
}

// ==================== CLIENT ====================

// Default constructor
Client::Client() : Personne() {
    adresse     = "NULL";
    telephone   = "0";
    remisePromo = 0;
}

// Parameterized constructor
// ✅ FIX : added password parameter + passed it to Personne
Client::Client(int id, string p, string n, string em, string pwd)
    : Personne(id, p, n, em, pwd) {
    this->adresse     = "NULL";
    this->telephone   = "0";
    this->remisePromo = 0;
    // address and phone not asked at signup — asked at payment ✅
}

// Destructor
Client::~Client() {
    cout << "Client " << getNomComplet() << " destroyed.\n";
}

// Getters
string Client::getAdresse()   const { return adresse; }
string Client::getTelephone() const { return telephone; }

// Setters
void Client::setAdresse(string a) { adresse = a; }

// ✅ FIX : removed the semicolon after if condition
void Client::setTelephone(string t) {
    if (t.length() == 10 && (t.substr(0, 2) == "06" || t.substr(0, 2) == "07"))
        telephone = t;
    else
        cout << "Le téléphone doit commencer par 06 ou 07 et avoir 10 chiffres\n";
}

// POLYMORPHISM : override pure virtual methods
void Client::afficherRole() {
    cout << "Rôle : CLIENT (Acheteur)\n";
}

void Client::tableauDeBord() {
    cout << "\n=== TABLEAU DE BORD CLIENT ===\n";
    cout << "Bienvenue, " << getNomComplet() << "!\n";
    cout << "1. Ajouter des produits au panier\n";
    cout << "2. Voir le panier\n";
    cout << "3. Appliquer un code promo\n";
    cout << "4. Supprimer un article du panier\n";
    cout << "5. Payer\n";
    cout << "6. Se déconnecter\n";
    cout << "==============================\n";
}

// Add to cart
void Client::ajouterAuPanier(int idProduit, int quantite) {
    if (!estConnecte) { cout << "Veuillez vous connecter d'abord !\n"; return; }
    for (size_t i = 0; i < panier.size(); i++) {
        if (panier[i] == idProduit) {
            quantites[i] += quantite;
            cout << "Quantité mise à jour : " << quantites[i]
                 << " pour le produit #" << idProduit << "\n";
            return;
        }
    }
    panier.push_back(idProduit);
    quantites.push_back(quantite);
    cout << "Ajouté " << quantite << "x produit #" << idProduit << " au panier.\n";
}

// Remove from cart
void Client::retirerDuPanier(int idProduit) {
    for (size_t i = 0; i < panier.size(); i++) {
        if (panier[i] == idProduit) {
            panier.erase(panier.begin() + i);
            quantites.erase(quantites.begin() + i);
            cout << "Produit #" << idProduit << " retiré du panier.\n";
            return;
        }
    }
    cout << "Produit #" << idProduit << " introuvable dans le panier !\n";
}

// Apply promo code
void Client::insererCodePromo(string code, float pourcentageRemise) {
    codePromo   = code;
    remisePromo = pourcentageRemise;
    cout << "Promo '" << code << "' appliquée : " << pourcentageRemise << "% de réduction !\n";
}

// Display cart
void Client::afficherPanier() {
    cout << "\n--- VOTRE PANIER ---\n";
    if (panier.empty()) { cout << "Le panier est vide !\n\n"; return; }
    for (size_t i = 0; i < panier.size(); i++) {
        cout << "Produit #" << panier[i]
             << " | Qty: "  << quantites[i] << "\n";
    }
    cout << "-----------------\n\n";
}

// ✅ NEW : Pay — asks address, phone THEN payment method
void Client::payer(float montant) {
    if (!estConnecte) { cout << "Veuillez vous connecter d'abord !\n"; return; }
    if (panier.empty()) { cout << "Le panier est vide !\n"; return; }

    // Step 1 : calculate total with promo
    float remise = montant * (remisePromo / 100.0f);
    float total  = montant - remise;

    cout << "\n--- RÉSUMÉ DE LA COMMANDE ---\n";
    afficherPanier();
    cout << "Sous-total : " << montant << " MAD\n";
    if (remisePromo > 0)
        cout << "Réduction : -" << remise << " MAD (" << codePromo << ")\n";
    cout << "TOTAL    : " << total << " MAD\n";
    cout << "-----------------------------\n";

    // Step 2 : ask delivery address and phone
    cout << "\n--- INFORMATIONS DE LIVRAISON ---\n";
    cout << "Entrez votre adresse : ";
    cin.ignore();
    getline(cin, adresse);

    cout << "Entrez votre numéro de téléphone : ";
    string tel;
    getline(cin, tel);
    setTelephone(tel);

    // Step 3 : choose payment method
    cout << "\n--- MODE DE PAIEMENT ---\n";
    cout << "1. Carte Bancaire\n";
    cout << "2. PayPal\n";
    cout << "Choix : ";
    int choixPaiement;
    cin >> choixPaiement;

    if (choixPaiement == 1) {
        // Bank card details
        string numeroCarte, titulaire, dateExpiration, cvv;
        cout << "\n  Numéro de carte (16 chiffres) : ";
        cin.ignore();
        getline(cin, numeroCarte);
        cout << "  Nom du titulaire               : ";
        getline(cin, titulaire);
        cout << "  Date d'expiration (MM/AA)      : ";
        getline(cin, dateExpiration);
        cout << "  CVV                            : ";
        getline(cin, cvv);
        cout << "\n  Traitement du paiement...\n";
        cout << "  Paiement par carte bancaire confirmé.\n";
    }
    else if (choixPaiement == 2) {
        // PayPal details
        string emailPaypal, motDePassePaypal;
        cout << "\n  PayPal Email    : ";
        cin.ignore();
        getline(cin, emailPaypal);
        cout << "  Mot de passe PayPal : ";
        getline(cin, motDePassePaypal);
        cout << "\n  Traitement du paiement...\n";
        cout << "  Paiement via PayPal confirmé.\n";
    }
    else {
        cout << "Choix invalide. Commande annulée.\n";
        return;
    }

    // Step 4 : success message
    cout << "\n========================================\n";
    cout << "      MERCI POUR VOTRE COMMANDE !\n";
    cout << "==============================================\n";
    cout << "  Nom       : " << getNomComplet() << "\n";
    cout << "  Email     : " << email << "\n";
    cout << "  Téléphone : " << telephone << "\n";
    cout << "  Adresse   : " << adresse << "\n";
    cout << "  Total     : " << total << " MAD\n";
    cout << "  Confirmation envoyée à " << email << "\n";
    cout << "==============================================\n\n";

    // Step 5 : clear cart + reset promo → back to dashboard (still logged in ✅)
    panier.clear();
    quantites.clear();
    codePromo   = "";
    remisePromo = 0;
    cout << "Redirection vers la page principale...\n";
}

// Cancel order
void Client::annulerCommande() {
    if (panier.empty()) { cout << "Aucune commande à annuler !\n"; return; }
    panier.clear();
    quantites.clear();
    codePromo   = "";
    remisePromo = 0;
    cout << "Commande annulée. Le panier est maintenant vide.\n";
}

// ==================== VENDEUR ====================

// Default constructor
Vendeur::Vendeur() : Personne() {
    nomBoutique     = "NULL";
    chiffreAffaires = 0;
}

// Parameterized constructor
// ✅ FIX : removed phone — Personne doesn't have phone
Vendeur::Vendeur(int id, string p, string n, string em, string pwd, string boutique)
    : Personne(id, p, n, em, pwd) {
    this->nomBoutique     = boutique;
    this->chiffreAffaires = 0;
}

// Destructor
Vendeur::~Vendeur() {
    cout << "Vendeur " << getNomComplet() << " destroyed.\n";
}

// Getters / Setters
string Vendeur::getNomBoutique()     const { return nomBoutique; }
float  Vendeur::getChiffreAffaires() const { return chiffreAffaires; }
void   Vendeur::setNomBoutique(string b)   { nomBoutique = b; }

// POLYMORPHISM : override
void Vendeur::afficherRole() {
    cout << "Rôle : VENDEUR\n";
}

void Vendeur::tableauDeBord() {
    cout << "\n=== TABLEAU DE BORD VENDEUR ===\n";
    cout << "Bienvenue, " << getNomComplet() << "!\n";
    cout << "Boutique : " << nomBoutique << "\n";
    cout << "1. Ajouter un produit\n";
    cout << "2. Voir mes produits\n";
    cout << "3. Chiffre d'affaires : " << chiffreAffaires << " MAD\n";
    cout << "4. Se déconnecter\n";
    cout << "==============================\n";
}

// Seller methods
void Vendeur::ajouterProduit(int idProduit) {
    if (!estConnecte) { cout << "Veuillez vous connecter d'abord !\n"; return; }
    mesProduits.push_back(idProduit);
    cout << "Produit #" << idProduit << " ajouté à votre boutique.\n";
}

void Vendeur::afficherProduits() {
    cout << "\n--- VOS PRODUITS ---\n";
    if (mesProduits.empty()) { cout << "Aucun produit pour l'instant !\n"; return; }
    for (int idProduit : mesProduits) cout << "Produit #" << idProduit << "\n";
    cout << "-----------------------------\n";
}

void Vendeur::enregistrerVente(float montant) {
    chiffreAffaires += montant;
    cout << "Vente enregistrée ! Chiffre d'affaires : " << chiffreAffaires << " MAD\n";
}

// ==================== HELPER FUNCTION ====================

Personne* inscrireUtilisateur() {
    cout << "\n========================================\n";
    cout << "         CRÉER VOTRE COMPTE\n";
    cout << "==============================================\n";

    int choixRole;
    cout << "Choisissez votre rôle :\n";
    cout << "1. Client (Acheteur)\n";
    cout << "2. Vendeur\n";
    cout << "Choix : ";
    cin >> choixRole;

    string prenom, nom, em, pwd;
    cout << "\nPrénom       : "; cin >> prenom;
    cout << "Nom          : "; cin >> nom;
    cout << "Email        : "; cin >> em;
    cout << "Mot de passe : "; cin >> pwd;

    static int prochainId = 100;

    if (choixRole == 1) {
        Client* client = new Client(prochainId++, prenom, nom, em, pwd);
        client->sInscrire(prenom, nom, em, pwd);   // prototype — kept as requested
        cout << "\nCompte client créé avec succès !\n";
        return client;
    }
    else if (choixRole == 2) {
        string boutique;
        cout << "Nom de la boutique : ";
        cin.ignore();
        getline(cin, boutique);
        Vendeur* vendeur = new Vendeur(prochainId++, prenom, nom, em, pwd, boutique);
        vendeur->sInscrire(prenom, nom, em, pwd);   // prototype — kept as requested
        cout << "\nCompte vendeur créé avec succès !\n";
        return vendeur;
    }
    else {
        cout << "Choix de rôle invalide !\n";
        return nullptr;
    }
}

// ==================== MAIN ====================

int main() {
    cout << "==============================================\n";
    cout << "       BIENVENUE SUR NOTRE E-SHOP\n";
    cout << "==============================================\n\n";

    Personne* utilisateur = nullptr;

    // Step 1 : sign up
    utilisateur = inscrireUtilisateur();
    if (!utilisateur) { cout << "Au revoir !\n"; return 0; }

    // ✅ FIX : login step added — user was never logged in before
    cout << "\n--- Veuillez vous connecter ---\n";
    string em, pwd;
    cout << "Email        : "; cin >> em;
    cout << "Mot de passe : "; cin >> pwd;

    if (!utilisateur->seConnecter(em, pwd)) {
        cout << "Connexion échouée. Au revoir !\n";
        delete utilisateur;
        return 0;
    }

    // Step 2 : show info + role
    cout << "\n" << *utilisateur << "\n";   // FRIEND FUNCTION : operator<<
    utilisateur->afficherRole();             // POLYMORPHISM

    // Step 3 : main loop — stays until logout (user stays logged in)
    bool enCours = true;
    while (enCours) {

        utilisateur->tableauDeBord();   // POLYMORPHISM : different per role

        if (Client* client = dynamic_cast<Client*>(utilisateur)) {
            int choix;
            cout << "Votre choix : ";
            cin >> choix;

            switch (choix) {
                case 1:
                    client->ajouterAuPanier(101, 2);
                    client->ajouterAuPanier(102, 1);
                    break;
                case 2:
                    client->afficherPanier();
                    break;
                case 3: {
                    string code; float remise;
                    cout << "Code promo     : "; cin >> code;
                    cout << "Réduction (%%) : "; cin >> remise;
                    client->insererCodePromo(code, remise);
                    break;
                }
                case 4: {
                    int idProduit;
                    cout << "ID du produit à supprimer : "; cin >> idProduit;
                    client->retirerDuPanier(idProduit);
                    break;
                }
                case 5:
                    // asks address + phone + payment → success → back here
                    client->payer(150.0f);
                    break;
                case 6:
                    utilisateur->seDeconnecter();
                    enCours = false;
                    break;
                default:
                    cout << "Choix invalide !\n";
            }
        }
        else if (Vendeur* vendeur = dynamic_cast<Vendeur*>(utilisateur)) {
            int choix;
            cout << "Votre choix : ";
            cin >> choix;

            switch (choix) {
                case 1: {
                    int idProduit;
                    cout << "ID du produit : "; cin >> idProduit;
                    vendeur->ajouterProduit(idProduit);
                    break;
                }
                case 2: vendeur->afficherProduits(); break;
                case 3: cout << "Chiffre d'affaires : " << vendeur->getChiffreAffaires() << " MAD\n"; break;
                case 4:
                    utilisateur->seDeconnecter();
                    enCours = false;
                    break;
                default:
                    cout << "Choix invalide !\n";
            }
        }
    }

    delete utilisateur;

    cout << "\n========================================\n";
    cout << "              AU REVOIR !\n";
    cout << "==============================================\n";

    return 0;
}
