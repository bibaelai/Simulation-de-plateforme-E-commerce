#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
using namespace std;

//. On declare les classes 
class Produit;
class Client;
class Paiement;
class Commande;

// ============================================================
//  Declarations des listes (enums)
enum class Taille { XS, S, M, L, XL, XXL };

string tailleToString(Taille t) {
    switch (t) {
        case Taille::XS:  return "XS";
        case Taille::S:   return "S";
        case Taille::M:   return "M";
        case Taille::L:   return "L";
        case Taille::XL:  return "XL";
        case Taille::XXL: return "XXL";
        default:          return "Inconnue";
    }
}

Taille stringToTaille(const string& s) {
    if (s == "XS")  return Taille::XS;
    if (s == "S")   return Taille::S;
    if (s == "M")   return Taille::M;
    if (s == "L")   return Taille::L;
    if (s == "XL")  return Taille::XL;
    if (s == "XXL") return Taille::XXL;
    throw invalid_argument("Taille invalide : " + s);
}


enum class Couleur {
    ROUGE, BLEU, VERT, NOIR, BLANC, GRIS,
    JAUNE, ROSE, ORANGE, VIOLET, MARRON, BEIGE
};

string couleurToString(Couleur c) {
    switch (c) {
        case Couleur::ROUGE:  return "Rouge";
        case Couleur::BLEU:   return "Bleu";
        case Couleur::VERT:   return "Vert";
        case Couleur::NOIR:   return "Noir";
        case Couleur::BLANC:  return "Blanc";
        case Couleur::GRIS:   return "Gris";
        case Couleur::JAUNE:  return "Jaune";
        case Couleur::ROSE:   return "Rose";
        case Couleur::ORANGE: return "Orange";
        case Couleur::VIOLET: return "Violet";
        case Couleur::MARRON: return "Marron";
        case Couleur::BEIGE:  return "Beige";
        default:              return "Inconnue";
    }
}

enum class Statut { DISPONIBLE, RUPTURE_STOCK, ARCHIVE };

string statutToString(Statut s) {
    switch (s) {
        case Statut::DISPONIBLE:    return "Disponible";
        case Statut::RUPTURE_STOCK: return "Rupture de stock";
        case Statut::ARCHIVE:       return "Archivé";
        default:                    return "Inconnu";
    }
}

enum class Categorie {
    TOUS,
    SPORT_FITNESS,
    ELECTRONIQUE,
    VETEMENT,
    ALIMENTATION,
    LIVRE,
    MAISON,
    TICKETS,
    PROMOTION
};

string categorieToString(Categorie c) {
    switch (c) {
        case Categorie::TOUS:          return "Tous";
        case Categorie::SPORT_FITNESS: return "Sport & Fitness";
        case Categorie::ELECTRONIQUE:  return "Électronique";
        case Categorie::VETEMENT:      return "Vêtement";
        case Categorie::ALIMENTATION:  return "Alimentation";
        case Categorie::LIVRE:         return "Livre";
        case Categorie::MAISON:        return "Maison";
        case Categorie::TICKETS:       return "Tickets";
        case Categorie::PROMOTION:     return "Promotion";
        default:                       return "Inconnu";
    }
}

struct Commentaire {
    string auteur;
    string texte;
    int    etoiles;

    Commentaire(string a, string t, int e)
        : auteur(a), texte(t), etoiles(e) {}
};

// class Produit (MOURTAFIAA)
//  Produit est une classe abstraite (classe parent)
class Produit {
protected:
    int    id;
    string nom;
    double prix;
    string description;
    int    stock;
    double note; 
    string categorie;
    Statut statut;
    vector<Commentaire> commentaires;
    bool   enPromotion;
    double prixOriginal;

public:
    Produit(int id, const string& nom, double prix,        //we studied this in the last chapter, I forgot the name of it. 
            int stock, const string& categorie, double note = 0.0)
        : id(id), nom(nom), prix(prix), stock(stock),
          categorie(categorie), note(note), enPromotion(false),
          prixOriginal(prix),
          statut(stock > 0 ? Statut::DISPONIBLE : Statut::RUPTURE_STOCK)
    {
        if (prix < 0)   throw invalid_argument("Prix négatif interdit");
        if (stock < 0)  throw invalid_argument("Stock négatif interdit");
        if (note < 0 || note > 5) throw invalid_argument("Note hors [0,5]");
    }

    virtual ~Produit() {} //destructeur du class Produit
    // getters : 

    int    getId()           const { return id; }
    string getNom()          const { return nom; }
    double getPrix()         const { return prix; }
    int    getStock()        const { return stock; }
    double getNote()         const { return note; }
    string getDescription()  const { return description; }   
    string getCategorie()    const { return categorie; }
    Statut getStatut()       const { return statut; }
    bool   isEnPromotion()   const { return enPromotion; }
    double getPrixOriginal() const { return prixOriginal; }

    const vector<Commentaire>& getCommentaires() const { return commentaires; }

    void setPrix(double p) {
        if (p < 0) throw invalid_argument("Prix négatif interdit");
        prix = p;
    }

    void setStock(int s) {
        if (s < 0) throw invalid_argument("Stock négatif interdit");
        stock = s;
        if (stock == 0 && statut == Statut::DISPONIBLE)
            statut = Statut::RUPTURE_STOCK;
        else if (stock > 0 && statut == Statut::RUPTURE_STOCK)
            statut = Statut::DISPONIBLE;
    }

    void setDescription(string p) { description = p; } 

    void setNote(double n) {
        if (n < 0 || n > 5) throw invalid_argument("Note hors [0,5]");
        note = n;
    }

    void archiver() { statut = Statut::ARCHIVE; stock = 0; }

    void activerPromotion(double pourcentage) {
        if (pourcentage <= 0 || pourcentage >= 100)
            throw invalid_argument("Pourcentage invalide");
        prixOriginal = prix;
        prix        *= (1.0 - pourcentage / 100.0);
        enPromotion  = true;
    }

    void desactiverPromotion() {
        prix        = prixOriginal;
        enPromotion = false;
    }

    void ajouterCommentaire(const string& auteur,
                            const string& texte, int etoiles) {
        if (etoiles < 1 || etoiles > 5)
            throw invalid_argument("Étoiles entre 1 et 5");
        commentaires.emplace_back(auteur, texte, etoiles);
        double total = 0;
        for (const auto& c : commentaires) total += c.etoiles;
        note = total / commentaires.size();
    }

    bool estDisponible() const {
        return statut == Statut::DISPONIBLE && stock > 0;
    }

    bool acheter(int quantite = 1) {
        if (quantite <= 0)
            throw invalid_argument("Quantité doit être > 0");
        if (statut == Statut::ARCHIVE)
            throw runtime_error("Produit archivé");
        if (stock < quantite) return false;
        stock -= quantite;
        if (stock == 0) statut = Statut::RUPTURE_STOCK;
        return true;
    }

    void reapprovisionner(int quantite) {
        if (quantite <= 0)
            throw invalid_argument("Quantité > 0 requise");
        stock += quantite;
        if (statut == Statut::RUPTURE_STOCK)
            statut = Statut::DISPONIBLE;
    }

    string toString() const {
        return "[" + to_string(id) + "] " + nom +
               " — " + to_string((int)prix) + " MAD";
    }

    virtual double appliquerRemise(double pourcentage) = 0;
    virtual void   afficherDetails() const = 0;
    virtual Categorie getCategorieEnum() const = 0;

    bool operator==(const Produit& autre) const { return id == autre.id; }
    bool operator<(const Produit& autre)  const { return prix < autre.prix; }
};

// ============================================================
//  CLASSE : Vetement
// ============================================================
class Vetement : public Produit {
protected:
    Taille  taille;
    Couleur couleur;
    string  matiere;
    string  marque;
    vector<Taille>  taillesDisponibles;
    vector<Couleur> couleursDisponibles;

public:
    Vetement(int id, const string& nom, double prix, int stock,
             Taille taille, Couleur couleur,
             const string& matiere, const string& marque)
        : Produit(id, nom, prix, stock, "Vêtement"),    // Constructeur de class Vetement par L'heritage (Produit) !!
          taille(taille), couleur(couleur),
          matiere(matiere), marque(marque)
    {
        taillesDisponibles  = { Taille::XS, Taille::S, Taille::M,
                                Taille::L,  Taille::XL, Taille::XXL };
        couleursDisponibles = { Couleur::ROUGE, Couleur::BLEU, Couleur::VERT,
                                Couleur::NOIR,  Couleur::BLANC, Couleur::GRIS,
                                Couleur::JAUNE, Couleur::ROSE,  Couleur::ORANGE,
                                Couleur::VIOLET,Couleur::MARRON,Couleur::BEIGE };
    }

    virtual ~Vetement() {}  //destructeur

    Categorie getCategorieEnum() const override { return Categorie::VETEMENT; }

    Taille  getTaille()    const { return taille; }
    Couleur getCouleur()   const { return couleur; }
    string  getMarque()    const { return marque; }
    // on a besoin de ...ToString, pour notre base de donnees 
    string getTailleStr()  const { return tailleToString(taille); }
    string getCouleurStr() const { return couleurToString(couleur); }

    void setTaille(Taille t) {
        for (auto& td : taillesDisponibles)
            if (td == t) { taille = t; return; }
        throw invalid_argument("Taille non disponible : " + tailleToString(t));
    }

    void setCouleur(Couleur c) {
        for (auto& cd : couleursDisponibles)
            if (cd == c) { couleur = c; return; }
        throw invalid_argument("Couleur non disponible : " + couleurToString(c));
    }

    double appliquerRemise(double pourcentage) override {
        if (pourcentage < 0 || pourcentage > 100)
            throw invalid_argument("Pourcentage invalide");
        prix *= (1.0 - pourcentage / 100.0);
        return prix;
    }

    void afficherDetails() const override {
        // Replaced by DB query / UI layer
    }
};

class Electronique : public Produit {
protected:
    string marque;
    int    garantieMois;
    double puissanceWatts;
    double tensionVolts;

public:
    Electronique(int id, const string& nom, double prix, int stock,
                 const string& marque, int garantieMois,
                 double puissanceWatts = 0, double tensionVolts = 220)
        : Produit(id, nom, prix, stock, "Électronique"),
          marque(marque), garantieMois(garantieMois),
          puissanceWatts(puissanceWatts), tensionVolts(tensionVolts) {}

    virtual ~Electronique() {}

    Categorie getCategorieEnum() const override { return Categorie::ELECTRONIQUE; }

    string getMarque()   const { return marque; }
    int    getGarantie() const { return garantieMois; }

    double appliquerRemise(double pourcentage) override {
        prix *= (1.0 - pourcentage / 100.0);
        return prix;
    }

    void afficherDetails() const override {
        // Replaced by DB query / UI layer
    }
};

class Ordinateur : public Electronique {
private:
    string cpu;
    string gpu;
    int    stockageGo;
    string typeStockage;
    string typeOrdi;

public:
    Ordinateur(int id, const string& nom, double prix, int stock,
               const string& marque, int garantieMois,
               const string& cpu, const string& gpu,
               int stockageGo, const string& typeStockage,
               const string& typeOrdi)
        : Electronique(id, nom, prix, stock, marque, garantieMois),
          cpu(cpu), gpu(gpu), stockageGo(stockageGo),
          typeStockage(typeStockage), typeOrdi(typeOrdi) {
        categorie = "Ordinateur";
    }

    Categorie getCategorieEnum() const override { return Categorie::ELECTRONIQUE; }

    void afficherDetails() const override {
        // Replaced by DB query / UI layer
    }
};

class Alimentation : public Produit {
private:
    string dateExpiration;
    double poidsKg;
    int    caloriesPer100g;
    bool   bio;
    string allergenes;
    string paysOrigine;

public:
    Alimentation(int id, const string& nom, double prix, int stock,
                 const string& dateExp, double poids,
                 int calories, bool bio,
                 const string& allergenes = "",
                 const string& pays = "Maroc")
        : Produit(id, nom, prix, stock, "Alimentation"),
          dateExpiration(dateExp), poidsKg(poids),
          caloriesPer100g(calories), bio(bio),
          allergenes(allergenes), paysOrigine(pays) {}

    Categorie getCategorieEnum() const override { return Categorie::ALIMENTATION; }

    bool estPerime(const string& dateAujourdhui) const {
        return dateExpiration < dateAujourdhui;
    }

    double appliquerRemise(double pourcentage) override {
        prix *= (1.0 - pourcentage / 100.0);
        return prix;
    }

    void afficherDetails() const override {
        // Replaced by DB query / UI layer
    }
};

// ============================================================
//  CLASSE : Livre
// ============================================================
class Livre : public Produit {
private:
    string auteur;
    string isbn;
    int    nombrePages;
    string editeur;
    string langue;
    string genre;

public:
    Livre(int id, const string& nom, double prix, int stock,
          const string& auteur, const string& isbn,
          int pages, const string& editeur,
          const string& langue = "Français",
          const string& genre = "Roman")
        : Produit(id, nom, prix, stock, "Livre"),
          auteur(auteur), isbn(isbn), nombrePages(pages),
          editeur(editeur), langue(langue), genre(genre) {}

    Categorie getCategorieEnum() const override { return Categorie::LIVRE; }

    double appliquerRemise(double pourcentage) override {
        prix *= (1.0 - pourcentage / 100.0);
        return prix;
    }

    void afficherDetails() const override {
        // Replaced by DB query / UI layer
    }
};

class SportFitness : public Produit {
private:
    string sport;
    string marque;
    string materiau;
    double poidsKg;
    string niveauUtilisateur;

public:
    SportFitness(int id, const string& nom, double prix, int stock,
                 const string& sport, const string& marque,
                 const string& materiau = "", double poids = 0,
                 const string& niveau = "Tous niveaux")
        : Produit(id, nom, prix, stock, "Sport & Fitness"),
          sport(sport), marque(marque), materiau(materiau),
          poidsKg(poids), niveauUtilisateur(niveau) {}

    Categorie getCategorieEnum() const override { return Categorie::SPORT_FITNESS; }

    double appliquerRemise(double pourcentage) override {
        prix *= (1.0 - pourcentage / 100.0);
        return prix;
    }

    void afficherDetails() const override {
        // Replaced by DB query / UI layer
    }
};

class MaisonDecor : public Produit {
private:
    string piece;
    string style;
    string matiere;
    string dimensions;
    string marque;

public:
    MaisonDecor(int id, const string& nom, double prix, int stock,
                const string& piece, const string& style,
                const string& matiere = "",
                const string& dimensions = "",
                const string& marque = "")
        : Produit(id, nom, prix, stock, "Maison"),
          piece(piece), style(style), matiere(matiere),
          dimensions(dimensions), marque(marque) {}

    Categorie getCategorieEnum() const override { return Categorie::MAISON; }

    double appliquerRemise(double pourcentage) override {
        prix *= (1.0 - pourcentage / 100.0);
        return prix;
    }

    void afficherDetails() const override {
        // Replaced by DB query / UI layer
    }
};

class Ticket : public Produit {
private:
    string evenement;
    string lieu;
    string dateEvenement;
    string typePlace;
    int    placesRestantes;

public:
    Ticket(int id, const string& nom, double prix, int stock,
           const string& evenement, const string& lieu,
           const string& dateEvenement,
           const string& typePlace = "Standard")
        : Produit(id, nom, prix, stock, "Ticket"),
          evenement(evenement), lieu(lieu),
          dateEvenement(dateEvenement), typePlace(typePlace),
          placesRestantes(stock) {}

    Categorie getCategorieEnum() const override { return Categorie::TICKETS; }

    double appliquerRemise(double pourcentage) override {
        prix *= (1.0 - pourcentage / 100.0);
        return prix;
    }

    void afficherDetails() const override {
        // Replaced by DB query / UI layer
    }
};

class Paiement {
public:
    enum class Mode { CARTE, PAYPAL };

private:
    Mode   mode;
    bool   valide;
    double montant;
    string reference;
    string numeroCarte;
    string titulaire;
    string dateExpiration;
    string cvv;
    string emailPaypal;

public:
    Paiement() : mode(Mode::CARTE), valide(false), montant(0) {}

    Paiement(Mode m, double montant)
        : mode(m), valide(false), montant(montant) {
        reference = "REF" + to_string(rand() % 900000 + 100000);
    }

    bool payerParCarte(const string& num, const string& tit,
                       const string& exp, const string& c) {
        if (num.length() != 16) return false;
        numeroCarte    = num;
        titulaire      = tit;
        dateExpiration = exp;
        cvv            = c;
        valide         = true;
        return true;
    }

    bool payerParPaypal(const string& email, const string& /*pwd*/) {
        if (email.find('@') == string::npos) return false;
        emailPaypal = email;
        valide      = true;
        return true;
    }

    bool   estValide()    const { return valide; }
    double getMontant()   const { return montant; }
    string getReference() const { return reference; }
    Mode   getMode()      const { return mode; }
};

struct ArticlePanier {
    Produit* produit;
    int      quantite;
    string   tailleChoisie;
    string   couleurChoisie;

    ArticlePanier(Produit* p, int q,
                  const string& t = "", const string& c = "")
        : produit(p), quantite(q),
          tailleChoisie(t), couleurChoisie(c) {}

    double sousTotal() const {
        return produit->getPrix() * quantite;
    }
};

// Class panier (Nouiri)
class Panier {
protected:
    int                   id;
    string                client;
    vector<ArticlePanier> articles;
    string                codePromo;
    double                remisePromo;

public:
    Panier(int id, const string& client)
        : id(id), client(client), remisePromo(0) {}

    virtual ~Panier() {}

    void ajouterArticle(Produit* p, int quantite,
                        const string& taille = "",
                        const string& couleur = "") {
        for (auto& a : articles) {
            if (a.produit->getId() == p->getId()) {
                a.quantite += quantite;
                return;
            }
        }
        articles.emplace_back(p, quantite, taille, couleur);
    }

    void supprimerArticle(int idProduit) {
        for (auto it = articles.begin(); it != articles.end(); ++it) {
            if (it->produit->getId() == idProduit) {
                articles.erase(it);
                return;
            }
        }
    }

    void appliquerCodePromo(const string& code, double pourcentage) {
        codePromo   = code;
        remisePromo = pourcentage;
    }

    void supprimerCodePromo() {
        codePromo   = "";
        remisePromo = 0;
    }

    virtual double calculerTotal() const {
        double total = 0;
        for (const auto& a : articles) total += a.sousTotal();
        return total;
    }

    double calculerTotalApresPromo() const {
        double total  = calculerTotal();
        double remise = total * (remisePromo / 100.0);
        return total - remise;
    }

    bool estVide() const { return articles.empty(); }

    void vider() {
        articles.clear();
        codePromo   = "";
        remisePromo = 0;
    }

    int    getId()          const { return id; }
    string getClient()      const { return client; }
    double getRemisePromo() const { return remisePromo; }
    string getCodePromo()   const { return codePromo; }
    const  vector<ArticlePanier>& getArticles() const { return articles; }

    bool operator==(const Panier& other) const {
        return calculerTotal() == other.calculerTotal();
    }
    bool operator!=(const Panier& other) const { return !(*this == other); }

    Panier& operator=(const Panier& src) {
        if (this != &src) {
            id          = src.id;
            client      = src.client;
            articles    = src.articles;
            codePromo   = src.codePromo;
            remisePromo = src.remisePromo;
        }
        return *this;
    }
};

// CLASSE COMMANDE ! (OULGHAZI)
class Commande {
private:
    int           idCommande;
    string        dateCommande;
    string        adresseLivraison;
    double        fraisLivraison;
    Paiement*     paiement;
    vector<Produit*> produits;
    vector<int>      quantites;

public:
    Commande(int id, const string& date, const string& adresse)
        : idCommande(id), dateCommande(date),
          adresseLivraison(adresse),
          fraisLivraison(25.0), paiement(nullptr) {}

    ~Commande() {
        delete paiement;
    }

    void ajouterProduit(Produit* p, int quantite) {
        produits.push_back(p);
        quantites.push_back(quantite);
    }

    void supprimerProduit(int idProduit) {
        for (size_t i = 0; i < produits.size(); ++i) {
            if (produits[i]->getId() == idProduit) {
                produits.erase(produits.begin() + i);
                quantites.erase(quantites.begin() + i);
                return;
            }
        }
    }

    double calculerTotal() const {
        double total = 0;
        for (size_t i = 0; i < produits.size(); ++i)
            total += produits[i]->getPrix() * quantites[i];
        return total;
    }

    double calculerTotalAvecLivraison() const {
        return calculerTotal() + fraisLivraison;
    }

    bool estVide() const { return produits.empty(); }

    int          getId()             const { return idCommande; }
    string       getDate()           const { return dateCommande; }
    int          getNbProduits()     const { return (int)produits.size(); }
    double       getFraisLivraison() const { return fraisLivraison; }
    string       getAdresse()        const { return adresseLivraison; }

    void setAdresse(const string& a)          { adresseLivraison = a; }
    void setFraisLivraison(double f)          { fraisLivraison = f; }
    void setPaiement(Paiement* p)             { paiement = p; }
};

// CLASSE PERSONNE (ELHARCHAOUI)
class Personne {
protected:
    int    identifiant;
    string prenom;
    string nom;
    string email;
    string telephone;
    string motDePasse;
    bool   estConnecte;

public:
    Personne()
        : identifiant(0), prenom("NULL"), nom("NULL"),
          email("NULL"), telephone("NULL"),
          motDePasse("NULL"), estConnecte(false) {}

    Personne(int id, const string& p, const string& n,
             const string& em, const string& tel,
             const string& pwd)
        : identifiant(id), prenom(p), nom(n),
          email(em), telephone(tel),
          motDePasse(pwd), estConnecte(false) {}

    virtual ~Personne() {}

    int    getId()              const { return identifiant; }
    string getPrenom()          const { return prenom; }
    string getNom()             const { return nom; }
    string getNomComplet()      const { return prenom + " " + nom; }
    string getEmail()           const { return email; }
    string getTelephone()       const { return telephone; }
    bool   getStatutConnexion() const { return estConnecte; }

    void setPrenom(const string& p) { prenom = p; }
    void setNom(const string& n)    { nom    = n; }

    void setEmail(const string& e) {
        if (e.find('@') != string::npos) email = e;
        else throw invalid_argument("Email invalide : doit contenir '@'");
    }

    void setTelephone(const string& t) {
        if (t.length() == 10 &&
           (t.substr(0, 2) == "06" || t.substr(0, 2) == "07"))
            telephone = t;
        else
            throw invalid_argument("Téléphone invalide (doit commencer par 06/07, 10 chiffres)");
    }

    bool seConnecter(const string& em, const string& pwd) {
        if (em == email && pwd == motDePasse) {
            estConnecte = true;
            return true;
        }
        return false;
    }

    void seDeconnecter() {
        estConnecte = false;
    }

    void sInscrire(const string& p, const string& n,
                   const string& em, const string& tel,
                   const string& pwd) {
        prenom     = p;
        nom        = n;
        email      = em;
        telephone  = tel;
        motDePasse = pwd;
    }

    virtual void afficherRole()  = 0;
    virtual void tableauDeBord() = 0;
};

// utilisation de l'heritage entre les classes Personne et Client / Personne et vendeur 
class Client : public Personne {
private:
    string  adresse;
    Panier* panier;
    vector<Commande*> historiqueCommandes;
    static int prochainIdCommande;

public:
    Client(int id, const string& p, const string& n,
           const string& em, const string& tel,
           const string& pwd)
        : Personne(id, p, n, em, tel, pwd),
          adresse("")
    {
        panier = new Panier(id, getNomComplet());
    }

    ~Client() {
        delete panier;
        for (auto* c : historiqueCommandes) delete c;
    }

    string  getAdresse() const { return adresse; }
    Panier* getPanier()  const { return panier; }

    void setAdresse(const string& a) { adresse = a; }

    void afficherRole() override {
        // Replaced by DB query / UI layer
    }

    void tableauDeBord() override {
        // Replaced by DB query / UI layer
    }

    void ajouterAuPanier(Produit* p, int quantite,
                         const string& taille  = "",
                         const string& couleur = "") {
        if (!estConnecte) throw runtime_error("Connectez-vous d'abord.");
        if (!p->estDisponible()) throw runtime_error("Produit indisponible.");
        if (p->getStock() < quantite) throw runtime_error("Stock insuffisant.");
        panier->ajouterArticle(p, quantite, taille, couleur);
    }

    void retirerDuPanier(int idProduit) {
        panier->supprimerArticle(idProduit);
    }

    void appliquerCodePromo(const string& code, double pourcentage) {
        panier->appliquerCodePromo(code, pourcentage);
    }

    // passerCommande now receives adresse and tel as parameters (no more cin)
    bool passerCommande(const string& adresseLivraison, const string& tel,
                        const string& numCarte, const string& titulaire,
                        const string& expCarte, const string& cvv) {
        if (!estConnecte) throw runtime_error("Connectez-vous d'abord.");
        if (panier->estVide()) throw runtime_error("Panier vide.");

        setAdresse(adresseLivraison);
        setTelephone(tel);

        double totalAPayer = panier->calculerTotalApresPromo() + 25.0;

        Paiement* paiement = new Paiement(Paiement::Mode::CARTE, totalAPayer);
        if (!paiement->payerParCarte(numCarte, titulaire, expCarte, cvv)) {
            delete paiement;
            return false;
        }

        Commande* commande = new Commande(
            prochainIdCommande++, "2025-01-01", adresse
        );

        for (const auto& article : panier->getArticles()) {
            commande->ajouterProduit(article.produit, article.quantite);
            article.produit->acheter(article.quantite);
        }
        commande->setPaiement(paiement);
        historiqueCommandes.push_back(commande);

        panier->vider();
        return true;
    }

    const vector<Commande*>& getHistoriqueCommandes() const {
        return historiqueCommandes;
    }
};

int Client::prochainIdCommande = 1000;

class Vendeur : public Personne {
private:
    string      nomBoutique;
    vector<int> idsProduits;
    double      chiffreAffaires;

public:
    Vendeur(int id, const string& p, const string& n,
            const string& em, const string& tel,
            const string& pwd, const string& boutique)
        : Personne(id, p, n, em, tel, pwd),
          nomBoutique(boutique), chiffreAffaires(0) {}

    ~Vendeur() {}

    string getNomBoutique()     const { return nomBoutique; }
    double getChiffreAffaires() const { return chiffreAffaires; }
    void   setNomBoutique(const string& b) { nomBoutique = b; }

    void afficherRole() override {
        // Replaced by DB query / UI layer
    }

    void tableauDeBord() override {
        // Replaced by DB query / UI layer
    }

    void ajouterProduit(int idProduit) {
        if (!estConnecte) throw runtime_error("Connectez-vous d'abord.");
        idsProduits.push_back(idProduit);
    }

    const vector<int>& getProduits() const { return idsProduits; }

    void enregistrerVente(double montant) {
        chiffreAffaires += montant;
    }
};

class Catalogue {
private:
    vector<Produit*> produits;

public:
    ~Catalogue() {
        for (auto* p : produits) delete p;
    }

    void ajouterProduit(Produit* p) {
        produits.push_back(p);
    }

    vector<Produit*> rechercherParNom(const string& motCle) const {
        vector<Produit*> resultats;
        string mc = motCle;
        transform(mc.begin(), mc.end(), mc.begin(), ::tolower);
        for (auto* p : produits) {
            string nomMin = p->getNom();
            transform(nomMin.begin(), nomMin.end(), nomMin.begin(), ::tolower);
            if (nomMin.find(mc) != string::npos)
                resultats.push_back(p);
        }
        return resultats;
    }

    vector<Produit*> filtrerParCategorie(Categorie cat) const {
        vector<Produit*> resultats;
        if (cat == Categorie::TOUS) return produits;
        for (auto* p : produits)
            if (p->getCategorieEnum() == cat)
                resultats.push_back(p);
        return resultats;
    }

    vector<Produit*> filtrerParPrix(double prixMin, double prixMax) const {
        vector<Produit*> resultats;
        for (auto* p : produits)
            if (p->getPrix() >= prixMin && p->getPrix() <= prixMax)
                resultats.push_back(p);
        return resultats;
    }

    vector<Produit*> filtrerPromotions() const {
        vector<Produit*> resultats;
        for (auto* p : produits)
            if (p->isEnPromotion()) resultats.push_back(p);
        return resultats;
    }

    Produit* trouverParId(int id) const {
        for (auto* p : produits)
            if (p->getId() == id) return p;
        return nullptr;
    }

    int getNbProduits() const { return (int)produits.size(); }
};
/*
    cout << "\n╔══════════════════════════════════╗\n"
         << "║        CRÉER VOTRE COMPTE        ║\n"
         << "╚══════════════════════════════════╝\n"
         << "  1. Client (Acheteur)\n"
         << "  2. Vendeur\n"
         << "  Choix : ";
    int choixRole; cin >> choixRole;

    string prenom, nom, em, tel, pwd;
    cout << "  Prénom        : "; cin >> prenom;
    cout << "  Nom           : "; cin >> nom;
    cout << "  Email         : "; cin >> em;
    cout << "  Téléphone     : "; cin >> tel;
    cout << "  Mot de passe  : "; cin >> pwd;

    static int prochainId = 100;

    if (choixRole == 1) {
        Client* client = new Client(prochainId++, prenom, nom, em, tel, pwd);
        cout << "\n  Compte client créé !\n";
        return client;
    }
    else if (choixRole == 2) {
        string boutique;
        cout << "  Nom de la boutique : ";
        cin.ignore();
        getline(cin, boutique);
        Vendeur* vendeur = new Vendeur(prochainId++, prenom, nom,
                                       em, tel, pwd, boutique);
        cout << "\n  Compte vendeur créé !\n";
        return vendeur;
    }
    cout << "  Choix invalide.\n";
    return nullptr;
}

Le menu de client ( une loop )
void menuClient(Client* client, Catalogue& catalogue) {
    bool enCours = true;
    while (enCours) {
        client->tableauDeBord();
        cout << "  Votre choix : ";
        int choix; cin >> choix;

        switch (choix) {

            case 1:
                catalogue.afficherTous();
                break;

            case 2: {
                cout << "  Mot-clé : ";
                string motCle;
                cin.ignore(); getline(cin, motCle);
                auto res = catalogue.rechercherParNom(motCle);
                Catalogue::afficherResultats(res, "Recherche : " + motCle);

                if (!res.empty()) {
                    cout << "  Voir détails d'un produit ? (ID ou 0) : ";
                    int pid; cin >> pid;
                    if (pid != 0) {
                        Produit* p = catalogue.trouverParId(pid);
                        if (p) {
                            p->afficher();
                            p->afficherCommentaires();

                            cout << "\n  Ajouter au panier ? (1=Oui / 0=Non) : ";
                            int rep; cin >> rep;
                            if (rep == 1) {
                                cout << "  Quantité : ";
                                int qty; cin >> qty;
                                string taille, couleur;
                                // BUG FIX: getTailleStr() / getCouleurStr() now defined
                                if (Vetement* v = dynamic_cast<Vetement*>(p)) {
                                    v->choisirTaille();
                                    v->choisirCouleur();
                                    taille  = v->getTailleStr();
                                    couleur = v->getCouleurStr();
                                }
                                client->ajouterAuPanier(p, qty, taille, couleur);
                            }
                        }
                    }
                }
                break;
            }

            case 3: {
                cout << "\n  Catégories :\n"
                     << "  1. Tous\n"
                     << "  2. Sport & Fitness\n"
                     << "  3. Electronique\n"
                     << "  4. Vetement\n"
                     << "  5. Alimentation\n"
                     << "  6. Livre\n"
                     << "  7. Maison\n"
                     << "  8. Tickets\n"
                     << "  9. Promotions\n"
                     << "  Choix : ";
                int catChoix; cin >> catChoix;
                vector<Produit*> res;
                if (catChoix == 9)
                    res = catalogue.filtrerPromotions();
                else {
                    Categorie cats[] = {
                        Categorie::TOUS, Categorie::SPORT_FITNESS,
                        Categorie::ELECTRONIQUE, Categorie::VETEMENT,
                        Categorie::ALIMENTATION, Categorie::LIVRE,
                        Categorie::MAISON, Categorie::TICKETS
                    };
                    if (catChoix >= 1 && catChoix <= 8)
                        res = catalogue.filtrerParCategorie(cats[catChoix - 1]);
                }
                Catalogue::afficherResultats(res, "Catégorie filtrée");
                break;
            }

            case 4: {
                double pMin, pMax;
                cout << "  Prix minimum (MAD) : "; cin >> pMin;
                cout << "  Prix maximum (MAD) : "; cin >> pMax;
                auto res = catalogue.filtrerParPrix(pMin, pMax);
                Catalogue::afficherResultats(
                    res,
                    "Prix entre " + to_string((int)pMin) +
                    " et " + to_string((int)pMax) + " MAD"
                );
                break;
            }

            case 5: {
                client->getPanier()->afficher();
                cout << "  1. Appliquer un code promo\n"
                     << "  2. Supprimer un article\n"
                     << "  3. Retour\n"
                     << "  Choix : ";
                int sousChoix; cin >> sousChoix;
                if (sousChoix == 1) {
                    string code; double remise;
                    cout << "  Code promo    : "; cin >> code;
                    cout << "  Réduction (%) : "; cin >> remise;
                    client->appliquerCodePromo(code, remise);
                }
                else if (sousChoix == 2) {
                    cout << "  ID du produit à retirer : ";
                    int pid; cin >> pid;
                    client->retirerDuPanier(pid);
                }
                break;
            }

            case 6:
                client->passerCommande();
                break;

            case 7:
                client->afficherHistorique();
                break;

            case 8:
                client->seDeconnecter();
                enCours = false;
                break;

            default:
                cout << "  Choix invalide.\n";
        }
    }
}


void menuVendeur(Vendeur* vendeur) {
    bool enCours = true;
    while (enCours) {
        vendeur->tableauDeBord();
        cout << "  Votre choix : ";
        int choix; cin >> choix;

        switch (choix) {
            case 1: {
                int pid;
                cout << "  ID du produit : "; cin >> pid;
                vendeur->ajouterProduit(pid);
                break;
            }
            case 2: vendeur->afficherProduits(); break;
            case 3: cout << "  CA : " << fixed << setprecision(2)
                         << vendeur->getChiffreAffaires() << " MAD\n"; break;
            case 4:
                vendeur->seDeconnecter();
                enCours = false;
                break;
            default: cout << "  Choix invalide.\n";
        }
    }
}
*/