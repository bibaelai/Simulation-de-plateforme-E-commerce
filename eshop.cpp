#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <windows.h>
#include <stdexcept>
#include <algorithm>
using namespace std;

// ============================================================
//  FORWARD DECLARATIONS
// ============================================================
class Produit;
class Client;
class Paiement;
class Commande;

// ============================================================
//  ENUM : Taille
// ============================================================
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

// ============================================================
//  ENUM : Couleur
// ============================================================
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

// ============================================================
//  ENUM : Statut produit
// ============================================================
enum class Statut { DISPONIBLE, RUPTURE_STOCK, ARCHIVE };

string statutToString(Statut s) {
    switch (s) {
        case Statut::DISPONIBLE:    return "Disponible";
        case Statut::RUPTURE_STOCK: return "Rupture de stock";
        case Statut::ARCHIVE:       return "Archivé";
        default:                    return "Inconnu";
    }
}

// ============================================================
//  ENUM : Etat commande
// ============================================================
enum EtatCommande {
    EN_ATTENTE,
    CONFIRMEE,
    EN_COURS_LIVRAISON,
    LIVREE,
    ANNULEE
};

string etatToString(EtatCommande e) {
    switch (e) {
        case EN_ATTENTE:           return "En attente";
        case CONFIRMEE:            return "Confirmée";
        case EN_COURS_LIVRAISON:   return "En cours de livraison";
        case LIVREE:               return "Livrée";
        case ANNULEE:              return "Annulée";
        default:                   return "Inconnu";
    }
}

// ============================================================
//  ENUM : Categorie  (for search/filter)
// ============================================================
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

// ============================================================
//  STRUCT : Commentaire
//  Stores a user review: author + text + star rating (1-5)
// ============================================================
struct Commentaire {
    string auteur;
    string texte;
    int    etoiles;   // 1 to 5

    Commentaire(string a, string t, int e)
        : auteur(a), texte(t), etoiles(e) {}

    void afficher() const {
        cout << "  ★ " << etoiles << "/5  — " << auteur << "\n"
             << "  \"" << texte << "\"\n";
    }
};

// ============================================================
//  CLASSE ABSTRAITE : Produit
// ============================================================
class Produit {
protected:
    int    id;
    string nom;
    double prix;
    int    stock;
    double note;
    string categorie;
    Statut statut;
    vector<Commentaire> commentaires;
    bool   enPromotion;
    double prixOriginal;

public:
    // ── Constructor ────────────────────────────────────────
    Produit(int id, const string& nom, double prix,
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

    virtual ~Produit() {}

    // ── Getters ────────────────────────────────────────────
    int    getId()          const { return id; }
    string getNom()         const { return nom; }
    double getPrix()        const { return prix; }
    int    getStock()       const { return stock; }
    double getNote()        const { return note; }
    string getCategorie()   const { return categorie; }
    Statut getStatut()      const { return statut; }
    bool   isEnPromotion()  const { return enPromotion; }
    double getPrixOriginal()const { return prixOriginal; }

    const vector<Commentaire>& getCommentaires() const {
        return commentaires;
    }

    // ── Setters with validation ────────────────────────────
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

    void setNote(double n) {
        if (n < 0 || n > 5) throw invalid_argument("Note hors [0,5]");
        note = n;
    }

    void archiver() { statut = Statut::ARCHIVE; stock = 0; }

    // ── Promotion ──────────────────────────────────────────
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

    // ── Comments ──────────────────────────────────────────
    void ajouterCommentaire(const string& auteur,
                            const string& texte, int etoiles) {
        if (etoiles < 1 || etoiles > 5)
            throw invalid_argument("Étoiles entre 1 et 5");
        commentaires.emplace_back(auteur, texte, etoiles);
        // Recalculate average rating
        double total = 0;
        for (const auto& c : commentaires) total += c.etoiles;
        note = total / commentaires.size();
    }

    void afficherCommentaires() const {
        cout << "\n  --- Avis clients (" << commentaires.size() << ") ---\n";
        if (commentaires.empty()) {
            cout << "  Aucun avis pour l'instant.\n";
            return;
        }
        for (const auto& c : commentaires) c.afficher();
    }

    // ── Display ────────────────────────────────────────────
    void afficher() const {
        cout << fixed << setprecision(2);
        cout << "┌────────────────────────────────────\n"
             << "│ [" << id << "] " << nom
             << "  (" << categorie << ")\n"
             << "├────────────────────────────────────\n";
        if (enPromotion)
            cout << "│ Prix    : " << prix << " MAD"
                 << "  (était " << prixOriginal << " MAD) 🔥PROMO\n";
        else
            cout << "│ Prix    : " << prix << " MAD\n";
        cout << "│ Stock   : " << stock << " unité(s)\n"
             << "│ Note    : " << note << " / 5.0\n"
             << "│ Statut  : " << statutToString(statut) << "\n"
             << "├────────────────────────────────────\n";
        afficherDetails();
        cout << "└────────────────────────────────────\n";
    }

    // Short display for search results / cart
    void afficherCourt() const {
        cout << fixed << setprecision(2)
             << "[" << id << "] " << nom
             << " | " << prix << " MAD"
             << " | Stock: " << stock
             << " | ★ " << note
             << " | " << statutToString(statut);
        if (enPromotion) cout << " 🔥PROMO";
        cout << "\n";
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

    // ── Pure virtual (ABSTRACTION) ─────────────────────────
    virtual double appliquerRemise(double pourcentage) = 0;
    virtual void   afficherDetails() const = 0;
    virtual Categorie getCategorieEnum() const = 0;

    // ── Operator overloading (FRIEND FUNCTION) ─────────────
    friend ostream& operator<<(ostream& os, const Produit& p) {
        os << fixed << setprecision(2)
           << "[" << p.id << "] " << p.nom
           << " | " << p.prix << " MAD"
           << " | Stock: " << p.stock
           << " | " << statutToString(p.statut);
        return os;
    }

    bool operator==(const Produit& autre) const { return id == autre.id; }
    bool operator<(const Produit& autre)  const { return prix < autre.prix; }
};

// ============================================================
//  CLASSE : Vetement  (inherits Produit)
// ============================================================
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
    Vetement(int id, const string& nom, double prix, int stock,
             Taille taille, Couleur couleur,
             const string& matiere, const string& marque,
             bool estNeuf = true)
        : Produit(id, nom, prix, stock, "Vêtement"),
          taille(taille), couleur(couleur),
          matiere(matiere), marque(marque), estNeuf(estNeuf)
    {
        taillesDisponibles  = { Taille::XS, Taille::S, Taille::M,
                                Taille::L,  Taille::XL, Taille::XXL };
        couleursDisponibles = { Couleur::ROUGE, Couleur::BLEU, Couleur::VERT,
                                Couleur::NOIR,  Couleur::BLANC, Couleur::GRIS,
                                Couleur::JAUNE, Couleur::ROSE,  Couleur::ORANGE,
                                Couleur::VIOLET,Couleur::MARRON,Couleur::BEIGE };
    }

    virtual ~Vetement() {}

    Categorie getCategorieEnum() const override { return Categorie::VETEMENT; }

    Taille  getTaille()    const { return taille; }
    Couleur getCouleur()   const { return couleur; }
    string  getMarque()    const { return marque; }
    bool    isNeuf()       const { return estNeuf; }

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

    // Interactive size selection
    bool choisirTaille() {
        cout << "\n=== Choisir une taille pour : " << nom << " ===\n";
        for (size_t i = 0; i < taillesDisponibles.size(); ++i)
            cout << "  [" << i+1 << "] " << tailleToString(taillesDisponibles[i]) << "\n";
        cout << "  Votre choix : ";
        int choix; cin >> choix;
        if (choix < 1 || choix > (int)taillesDisponibles.size()) {
            cout << "Choix invalide.\n"; return false;
        }
        taille = taillesDisponibles[choix - 1];
        cout << "Taille : " << tailleToString(taille) << "\n";
        return true;
    }

    // Interactive color selection
    bool choisirCouleur() {
        cout << "\n=== Choisir une couleur pour : " << nom << " ===\n";
        for (size_t i = 0; i < couleursDisponibles.size(); ++i)
            cout << "  [" << i+1 << "] " << couleurToString(couleursDisponibles[i]) << "\n";
        cout << "  Votre choix : ";
        int choix; cin >> choix;
        if (choix < 1 || choix > (int)couleursDisponibles.size()) {
            cout << "Choix invalide.\n"; return false;
        }
        couleur = couleursDisponibles[choix - 1];
        cout << "Couleur : " << couleurToString(couleur) << "\n";
        return true;
    }

    double appliquerRemise(double pourcentage) override {
        if (pourcentage < 0 || pourcentage > 100)
            throw invalid_argument("Pourcentage invalide");
        prix *= (1.0 - pourcentage / 100.0);
        return prix;
    }

    void afficherDetails() const override {
        cout << "│ Taille  : " << tailleToString(taille)
             << "   Couleur : " << couleurToString(couleur) << "\n"
             << "│ Matière : " << matiere
             << "   Marque  : " << marque << "\n"
             << "│ État    : " << (estNeuf ? "Neuf" : "Occasion") << "\n";
        cout << "│ Tailles dispo  : ";
        for (size_t i = 0; i < taillesDisponibles.size(); ++i)
            cout << tailleToString(taillesDisponibles[i]) << " ";
        cout << "\n│ Couleurs dispo : ";
        for (size_t i = 0; i < couleursDisponibles.size(); ++i)
            cout << couleurToString(couleursDisponibles[i]) << " ";
        cout << "\n";
    }
};

// ============================================================
//  CLASSE : Electronique  (inherits Produit)
// ============================================================
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

    string getMarque()    const { return marque; }
    int    getGarantie()  const { return garantieMois; }

    double appliquerRemise(double pourcentage) override {
        prix *= (1.0 - pourcentage / 100.0);
        return prix;
    }

    void afficherDetails() const override {
        cout << "│  Marque   : " << marque
             << "  Garantie : " << garantieMois << " mois\n"
             << "│  Puissance: " << puissanceWatts
             << " W   Tension: " << tensionVolts << " V\n";
    }
};

// ── Smartphone ─────────────────────────────────────────────
class Smartphone : public Electronique {
private:
    int    ramGo;
    string os;
    string processeur;
    int    cameraMp;
    bool   support5G;
    double tailleEcranPouces;

public:
    Smartphone(int id, const string& nom, double prix, int stock,
               const string& marque, int garantieMois,
               int ramGo, const string& os, const string& processeur,
               int cameraMp, bool support5G, double ecran)
        : Electronique(id, nom, prix, stock, marque, garantieMois),
          ramGo(ramGo), os(os), processeur(processeur),
          cameraMp(cameraMp), support5G(support5G),
          tailleEcranPouces(ecran) { categorie = "Smartphone"; }

    Categorie getCategorieEnum() const override { return Categorie::ELECTRONIQUE; }

    void afficherDetails() const override {
        Electronique::afficherDetails();
        cout << "│  RAM: " << ramGo << " Go  OS: " << os
             << "  CPU: " << processeur << "\n"
             << "│  Caméra: " << cameraMp << " Mp"
             << "  5G: " << (support5G ? "Oui" : "Non")
             << "  Écran: " << tailleEcranPouces << "\"\n";
    }
};

// ── Ordinateur ─────────────────────────────────────────────
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
        Electronique::afficherDetails();
        cout << "│  CPU: " << cpu << "  GPU: " << gpu << "\n"
             << "│  Stockage: " << stockageGo << " Go (" << typeStockage
             << ")  Type: " << typeOrdi << "\n";
    }
};

// ============================================================
//  CLASSE : Alimentation  (inherits Produit)
// ============================================================
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
        cout << "│  Expiration : " << dateExpiration
             << "  Poids: " << poidsKg << " kg\n"
             << "│  Calories   : " << caloriesPer100g
             << "/100g  Bio: " << (bio ? "Oui" : "Non") << "\n"
             << "│  Allergènes : " << (allergenes.empty() ? "Aucun" : allergenes) << "\n"
             << "│  Origine    : " << paysOrigine << "\n";
    }
};

// ============================================================
//  CLASSE : Livre  (inherits Produit)
// ============================================================
class Livre : public Produit {
private:
    string auteur;
    string isbn;
    int    nombrePages;
    string editeur;
    string langue;
    int    anneePublication;
    string genre;

public:
    Livre(int id, const string& nom, double prix, int stock,
          const string& auteur, const string& isbn,
          int pages, const string& editeur,
          const string& langue = "Français",
          int annee = 2024,
          const string& genre = "Roman")
        : Produit(id, nom, prix, stock, "Livre"),
          auteur(auteur), isbn(isbn), nombrePages(pages),
          editeur(editeur), langue(langue),
          anneePublication(annee), genre(genre) {}

    Categorie getCategorieEnum() const override { return Categorie::LIVRE; }

    double appliquerRemise(double pourcentage) override {
        prix *= (1.0 - pourcentage / 100.0);
        return prix;
    }

    void afficherDetails() const override {
        cout << "│  Auteur : " << auteur  << "  Éditeur : " << editeur << "\n"
             << "│  ISBN   : " << isbn   << "  Pages   : " << nombrePages << "\n"
             << "│  Langue : " << langue << "  Année   : " << anneePublication << "\n"
             << "│  Genre  : " << genre << "\n";
    }
};

// ============================================================
//  CLASSE : SportFitness  (NEW — inherits Produit)
//  Covers sports equipment, gym gear, etc.
// ============================================================
class SportFitness : public Produit {
private:
    string sport;        // football, yoga, running…
    string marque;
    string materiau;
    double poidsKg;
    string niveauUtilisateur;  // débutant, intermédiaire, pro

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
        cout << "│  Sport    : " << sport   << "  Marque  : " << marque << "\n"
             << "│  Matériau : " << materiau << "  Poids   : " << poidsKg << " kg\n"
             << "│  Niveau   : " << niveauUtilisateur << "\n";
    }
};

// ============================================================
//  CLASSE : MaisonDecor  (NEW — inherits Produit)
//  House elements / home decor
// ============================================================
class MaisonDecor : public Produit {
private:
    string piece;       // salon, chambre, cuisine…
    string style;       // moderne, rustique, scandinave…
    string matiere;
    string dimensions;  // "L120 x l60 x H80 cm"
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
        cout << "│  Pièce      : " << piece    << "  Style : " << style << "\n"
             << "│  Matière    : " << matiere  << "\n"
             << "│  Dimensions : " << dimensions << "\n"
             << "│  Marque     : " << marque << "\n";
    }
};

// ============================================================
//  CLASSE : Ticket  (NEW — inherits Produit)
//  Event tickets: concerts, sport matches, cinema…
// ============================================================
class Ticket : public Produit {
private:
    string evenement;
    string lieu;
    string dateEvenement;
    string typePlace;    // VIP, standard, pelouse…
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
        cout << "│  Événement : " << evenement << "\n"
             << "│  Lieu      : " << lieu      << "\n"
             << "│  Date      : " << dateEvenement << "\n"
             << "│  Place     : " << typePlace << "\n"
             << "│  Restantes : " << placesRestantes << "\n";
    }
};

// ============================================================
//  CLASSE : Paiement  (NEW)
//  Handles payment — card or PayPal
// ============================================================
class Paiement {
public:
    enum class Mode { CARTE, PAYPAL };

private:
    Mode   mode;
    bool   valide;
    double montant;
    string reference;  // transaction ID

    // Card details (ENCAPSULATION — private)
    string numeroCarte;
    string titulaire;
    string dateExpiration;
    string cvv;

    // PayPal details
    string emailPaypal;

public:
    // Default constructor
    Paiement() : mode(Mode::CARTE), valide(false), montant(0) {}

    // Parameterized constructor
    Paiement(Mode m, double montant)
        : mode(m), valide(false), montant(montant) {
        reference = "REF" + to_string(rand() % 900000 + 100000);
    }

    // ── Pay by card ────────────────────────────────────────
    bool payerParCarte(const string& num, const string& tit,
                       const string& exp, const string& c) {
        if (num.length() != 16) {
            cout << "  Numéro de carte invalide (16 chiffres requis).\n";
            return false;
        }
        numeroCarte   = num;
        titulaire     = tit;
        dateExpiration = exp;
        cvv           = c;
        valide        = true;
        cout << "\n  ✅ Paiement par carte confirmé.\n"
             << "  Référence : " << reference << "\n";
        return true;
    }

    // ── Pay by PayPal ──────────────────────────────────────
    bool payerParPaypal(const string& email, const string& /*pwd*/) {
        if (email.find('@') == string::npos) {
            cout << "  Email PayPal invalide.\n";
            return false;
        }
        emailPaypal = email;
        valide      = true;
        cout << "\n  ✅ Paiement PayPal confirmé.\n"
             << "  Référence : " << reference << "\n";
        return true;
    }

    // ── Interactive payment flow ───────────────────────────
    bool effectuerPaiement() {
        cout << "\n╔══════════════════════════════════╗\n"
             << "║       MODE DE PAIEMENT           ║\n"
             << "╚══════════════════════════════════╝\n"
             << "  Montant à payer : " << fixed << setprecision(2)
             << montant << " MAD\n\n"
             << "  1. Carte Bancaire\n"
             << "  2. PayPal\n"
             << "  Choix : ";
        int choix; cin >> choix;

        if (choix == 1) {
            mode = Mode::CARTE;
            string num, tit, exp, cvvInput;
            cout << "\n  Numéro de carte (16 chiffres) : "; cin >> num;
            cin.ignore();
            cout << "  Nom du titulaire               : "; getline(cin, tit);
            cout << "  Date d'expiration (MM/AA)      : "; cin >> exp;
            cout << "  CVV                            : "; cin >> cvvInput;
            return payerParCarte(num, tit, exp, cvvInput);
        }
        else if (choix == 2) {
            mode = Mode::PAYPAL;
            string em, pwd;
            cout << "\n  Email PayPal    : "; cin >> em;
            cout << "  Mot de passe    : "; cin >> pwd;
            return payerParPaypal(em, pwd);
        }
        cout << "Choix invalide.\n";
        return false;
    }

    bool   estValide()    const { return valide; }
    double getMontant()   const { return montant; }
    string getReference() const { return reference; }
    Mode   getMode()      const { return mode; }
};

// ============================================================
//  STRUCT : ArticlePanier
//  One line in the cart: product + quantity + chosen size/color
// ============================================================
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

    void afficher() const {
        cout << fixed << setprecision(2)
             << "  [" << produit->getId() << "] "
             << produit->getNom();
        if (!tailleChoisie.empty())   cout << "  Taille: "  << tailleChoisie;
        if (!couleurChoisie.empty())  cout << "  Couleur: " << couleurChoisie;
        cout << "\n  Qty: " << quantite
             << "  x  " << produit->getPrix() << " MAD"
             << "  =  " << sousTotal() << " MAD\n";
    }
};

// ============================================================
//  CLASSE : Panier
// ============================================================
class Panier {
protected:
    int                  id;
    string               client;
    vector<ArticlePanier> articles;
    string               codePromo;
    double               remisePromo;   // percent

public:
    Panier(int id, const string& client)
        : id(id), client(client), remisePromo(0) {}

    virtual ~Panier() {
        cout << "Nettoyage du panier ID: " << id << "\n";
    }

    // ── Add to cart ────────────────────────────────────────
    void ajouterArticle(Produit* p, int quantite,
                        const string& taille = "",
                        const string& couleur = "") {
        // If product already in cart, increase quantity
        for (auto& a : articles) {
            if (a.produit->getId() == p->getId()) {
                a.quantite += quantite;
                cout << "  Quantité mise à jour : "
                     << a.quantite << "x " << p->getNom() << "\n";
                return;
            }
        }
        articles.emplace_back(p, quantite, taille, couleur);
        cout << "  ✅ " << quantite << "x " << p->getNom()
             << " ajouté au panier.\n";
    }

    // ── Remove from cart ───────────────────────────────────
    void supprimerArticle(int idProduit) {
        for (auto it = articles.begin(); it != articles.end(); ++it) {
            if (it->produit->getId() == idProduit) {
                cout << "  🗑 " << it->produit->getNom()
                     << " retiré du panier.\n";
                articles.erase(it);
                return;
            }
        }
        cout << "  Produit #" << idProduit << " introuvable dans le panier.\n";
    }

    // ── Apply promo code ───────────────────────────────────
    void appliquerCodePromo(const string& code, double pourcentage) {
        codePromo   = code;
        remisePromo = pourcentage;
        cout << "  🏷 Code \"" << code << "\" appliqué : "
             << pourcentage << "% de réduction !\n";
    }

    void supprimerCodePromo() {
        codePromo   = "";
        remisePromo = 0;
        cout << "  Code promo supprimé.\n";
    }

    // ── Total calculation (virtual for PanierPrime) ────────
    virtual double calculerTotal() const {
        double total = 0;
        for (const auto& a : articles) total += a.sousTotal();
        return total;
    }

    double calculerTotalApresPromo() const {
        double total   = calculerTotal();
        double remise  = total * (remisePromo / 100.0);
        return total - remise;
    }

    bool estVide() const { return articles.empty(); }

    void vider() {
        articles.clear();
        codePromo   = "";
        remisePromo = 0;
    }

    // ── Display cart ───────────────────────────────────────
    void afficher() const {
        cout << fixed << setprecision(2)
             << "\n╔══════════════════════════════════╗\n"
             << "║         VOTRE PANIER             ║\n"
             << "╚══════════════════════════════════╝\n";
        if (articles.empty()) {
            cout << "  Le panier est vide.\n\n";
            return;
        }
        for (const auto& a : articles) { a.afficher(); cout << "\n"; }
        cout << "─────────────────────────────────\n"
             << "  Sous-total : " << calculerTotal() << " MAD\n";
        if (remisePromo > 0)
            cout << "  Promo (" << codePromo << ") : -"
                 << calculerTotal() * remisePromo / 100.0
                 << " MAD (" << remisePromo << "%)\n"
                 << "  TOTAL après promo : "
                 << calculerTotalApresPromo() << " MAD\n";
        cout << "─────────────────────────────────\n\n";
    }

    // Getters
    int    getId()          const { return id; }
    string getClient()      const { return client; }
    double getRemisePromo() const { return remisePromo; }
    string getCodePromo()   const { return codePromo; }
    const  vector<ArticlePanier>& getArticles() const { return articles; }

    // ── Operator overloading (FRIEND FUNCTION) ─────────────
    bool operator==(const Panier& other) const {
        return calculerTotal() == other.calculerTotal();
    }
    bool operator!=(const Panier& other) const { return !(*this == other); }

    Panier& operator=(const Panier& src) {
        if (this != &src) {
            id         = src.id;
            client     = src.client;
            articles   = src.articles;
            codePromo  = src.codePromo;
            remisePromo= src.remisePromo;
        }
        return *this;
    }

    friend ostream& operator<<(ostream& os, const Panier& p) {
        os << fixed << setprecision(2)
           << "Panier [" << p.id << "] | " << p.client
           << " | " << p.articles.size() << " article(s)"
           << " | Total: " << p.calculerTotalApresPromo() << " MAD";
        return os;
    }
};

// ── PanierPrime : automatic discount for premium members ──
class PanierPrime : public Panier {
private:
    double tauxReduction;

public:
    PanierPrime(int id, const string& client, double taux = 0.15)
        : Panier(id, client), tauxReduction(taux) {}

    // POLYMORPHISM : overrides calculerTotal
    double calculerTotal() const override {
        return Panier::calculerTotal() * (1.0 - tauxReduction);
    }
};

// ============================================================
//  CLASSE : Commande  (NEW — unified + fixed)
// ============================================================
class Commande {
private:
    int           idCommande;
    string        dateCommande;
    string        adresseLivraison;
    EtatCommande  etat;
    double        fraisLivraison;
    Paiement*     paiement;

    // Using vector instead of raw array (safer)
    vector<Produit*> produits;
    vector<int>      quantites;

public:
    // ── Constructor ────────────────────────────────────────
    Commande(int id, const string& date, const string& adresse)
        : idCommande(id), dateCommande(date),
          adresseLivraison(adresse),
          etat(EN_ATTENTE), fraisLivraison(25.0), paiement(nullptr) {}

    // ── Destructor ─────────────────────────────────────────
    ~Commande() {
        delete paiement;
    }

    // ── Add product ────────────────────────────────────────
    void ajouterProduit(Produit* p, int quantite) {
        produits.push_back(p);
        quantites.push_back(quantite);
    }

    // ── Remove product ─────────────────────────────────────
    void supprimerProduit(int idProduit) {
        for (size_t i = 0; i < produits.size(); ++i) {
            if (produits[i]->getId() == idProduit) {
                produits.erase(produits.begin() + i);
                quantites.erase(quantites.begin() + i);
                cout << "Produit supprimé de la commande.\n";
                return;
            }
        }
        cout << "Produit introuvable dans la commande.\n";
    }

    // ── Total ──────────────────────────────────────────────
    double calculerTotal() const {
        double total = 0;
        for (size_t i = 0; i < produits.size(); ++i)
            total += produits[i]->getPrix() * quantites[i];
        return total;
    }

    double calculerTotalAvecLivraison() const {
        return calculerTotal() + fraisLivraison;
    }

    // ── Display ────────────────────────────────────────────
    void afficherCommande() const {
        cout << fixed << setprecision(2)
             << "\n===== Commande #" << idCommande << " =====\n"
             << "Date       : " << dateCommande     << "\n"
             << "Adresse    : " << adresseLivraison  << "\n"
             << "État       : " << etatToString(etat) << "\n"
             << "--- Produits ---\n";
        for (size_t i = 0; i < produits.size(); ++i)
            cout << "  - " << produits[i]->getNom()
                 << " x" << quantites[i]
                 << " = " << produits[i]->getPrix() * quantites[i]
                 << " MAD\n";
        cout << "─────────────────────\n"
             << "Sous-total  : " << calculerTotal() << " MAD\n"
             << "Livraison   : " << fraisLivraison  << " MAD\n"
             << "TOTAL       : " << calculerTotalAvecLivraison() << " MAD\n";
    }

    bool estVide() const { return produits.empty(); }

    // ── Getters ────────────────────────────────────────────
    int          getId()              const { return idCommande; }
    string       getDate()            const { return dateCommande; }
    EtatCommande getEtat()            const { return etat; }
    int          getNbProduits()      const { return (int)produits.size(); }
    double       getFraisLivraison()  const { return fraisLivraison; }
    string       getAdresse()         const { return adresseLivraison; }

    // ── Setters ────────────────────────────────────────────
    void changerEtat(EtatCommande nouvelEtat) { etat = nouvelEtat; }
    void setAdresse(const string& a)          { adresseLivraison = a; }
    void setFraisLivraison(double f)          { fraisLivraison = f; }
    void setPaiement(Paiement* p)             { paiement = p; }
};

// ============================================================
//  CLASSE ABSTRAITE : Personne  (Abstract Base)
// ============================================================
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
    // ── Default constructor ────────────────────────────────
    Personne()
        : identifiant(0), prenom("NULL"), nom("NULL"),
          email("NULL"), telephone("NULL"),
          motDePasse("NULL"), estConnecte(false) {}

    // ── Parameterized constructor ──────────────────────────
    Personne(int id, const string& p, const string& n,
             const string& em, const string& tel,
             const string& pwd)
        : identifiant(id), prenom(p), nom(n),
          email(em), telephone(tel),
          motDePasse(pwd), estConnecte(false) {}

    // Virtual destructor
    virtual ~Personne() {
        cout << "Personne " << getNomComplet() << " détruite.\n";
    }

    // ── Getters ────────────────────────────────────────────
    int    getId()              const { return identifiant; }
    string getPrenom()          const { return prenom; }
    string getNom()             const { return nom; }
    string getNomComplet()      const { return prenom + " " + nom; }
    string getEmail()           const { return email; }
    string getTelephone()       const { return telephone; }
    bool   getStatutConnexion() const { return estConnecte; }

    // ── Setters ────────────────────────────────────────────
    void setPrenom(const string& p) { prenom = p; }
    void setNom(const string& n)    { nom    = n; }

    void setEmail(const string& e) {
        if (e.find('@') != string::npos) email = e;
        else cout << "Email invalide : doit contenir '@'\n";
    }

    void setTelephone(const string& t) {
        if (t.length() == 10 &&
           (t.substr(0, 2) == "06" || t.substr(0, 2) == "07"))
            telephone = t;
        else
            cout << "Téléphone invalide (doit commencer par 06/07, 10 chiffres)\n";
    }

    // ── Authentication ─────────────────────────────────────
    bool seConnecter(const string& em, const string& pwd) {
        if (em == email && pwd == motDePasse) {
            estConnecte = true;
            cout << "Bienvenue, " << getNomComplet() << "!\n";
            return true;
        }
        cout << "Email ou mot de passe incorrect.\n";
        return false;
    }

    void seDeconnecter() {
        estConnecte = false;
        cout << "\nDéconnecté. À bientôt, " << prenom << "!\n";
    }

    void sInscrire(const string& p, const string& n,
                   const string& em, const string& tel,
                   const string& pwd) {
        prenom     = p;
        nom        = n;
        email      = em;
        telephone  = tel;
        motDePasse = pwd;
        cout << "Compte créé pour " << getNomComplet() << "!\n";
    }

    // ── Pure virtual (ABSTRACTION + POLYMORPHISM) ──────────
    virtual void afficherRole()    = 0;
    virtual void tableauDeBord()   = 0;

    // ── Friend function (FRIEND FUNCTION) ─────────────────
    friend ostream& operator<<(ostream& os, const Personne& p) {
        os << "Personne #" << p.identifiant
           << " | " << p.getNomComplet()
           << " | " << p.email
           << " | " << p.telephone
           << " | " << (p.estConnecte ? "En ligne" : "Hors ligne");
        return os;
    }
};

// ============================================================
//  CLASSE : Client  (inherits Personne)
// ============================================================
class Client : public Personne {
private:
    string  adresse;
    Panier* panier;
    vector<Commande*> historiqueCommandes;
    bool    estPrime;
    static int prochainIdCommande;

public:
    // ── Constructor ────────────────────────────────────────
    Client(int id, const string& p, const string& n,
           const string& em, const string& tel,
           const string& pwd, bool prime = false)
        : Personne(id, p, n, em, tel, pwd),
          adresse(""), estPrime(prime)
    {
        // Create cart: PanierPrime if prime member, regular otherwise
        if (prime)
            panier = new PanierPrime(id, getNomComplet());
        else
            panier = new Panier(id, getNomComplet());
    }

    // ── Destructor ─────────────────────────────────────────
    ~Client() {
        delete panier;
        for (auto* c : historiqueCommandes) delete c;
        cout << "Client " << getNomComplet() << " détruit.\n";
    }

    // ── Getters / Setters ──────────────────────────────────
    string  getAdresse()  const { return adresse; }
    bool    isPrime()     const { return estPrime; }
    Panier* getPanier()   const { return panier; }

    void setAdresse(const string& a) { adresse = a; }

    // ── POLYMORPHISM : override pure virtuals ──────────────
    void afficherRole() override {
        cout << "Rôle : CLIENT" << (estPrime ? " ⭐PRIME" : "") << "\n";
    }

    void tableauDeBord() override {
        cout << "\n╔══════════════════════════════════╗\n"
             << "║     TABLEAU DE BORD CLIENT       ║\n"
             << "╚══════════════════════════════════╝\n"
             << "  Bienvenue, " << getNomComplet()
             << (estPrime ? " ⭐PRIME" : "") << "\n\n"
             << "  1. Parcourir les produits\n"
             << "  2. Rechercher un produit\n"
             << "  3. Filtrer par catégorie\n"
             << "  4. Filtrer par prix\n"
             << "  5. Voir mon panier\n"
             << "  6. Passer la commande\n"
             << "  7. Historique des commandes\n"
             << "  8. Se déconnecter\n"
             << "══════════════════════════════════\n";
    }

    // ── Add to cart ────────────────────────────────────────
    void ajouterAuPanier(Produit* p, int quantite,
                         const string& taille  = "",
                         const string& couleur = "") {
        if (!estConnecte) { cout << "Connectez-vous d'abord.\n"; return; }
        if (!p->estDisponible()) {
            cout << "Produit indisponible.\n"; return;
        }
        if (p->getStock() < quantite) {
            cout << "Stock insuffisant (" << p->getStock() << " restants).\n";
            return;
        }
        panier->ajouterArticle(p, quantite, taille, couleur);
    }

    // ── Remove from cart ───────────────────────────────────
    void retirerDuPanier(int idProduit) {
        panier->supprimerArticle(idProduit);
    }

    // ── Apply promo code ───────────────────────────────────
    void appliquerCodePromo(const string& code, double pourcentage) {
        panier->appliquerCodePromo(code, pourcentage);
    }

    // ── Place order — full checkout flow ──────────────────
    bool passerCommande() {
        if (!estConnecte) { cout << "Connectez-vous d'abord.\n"; return false; }
        if (panier->estVide()) { cout << "Votre panier est vide.\n"; return false; }

        panier->afficher();

        // Step 1 : delivery address
        cout << "╔══════════════════════════════════╗\n"
             << "║     INFORMATIONS DE LIVRAISON    ║\n"
             << "╚══════════════════════════════════╝\n";
        cout << "  Adresse de livraison : ";
        cin.ignore();
        getline(cin, adresse);

        // Step 2 : confirm phone
        cout << "  Téléphone            : ";
        string tel; getline(cin, tel);
        setTelephone(tel);

        // Step 3 : payment
        double totalAPayer = panier->calculerTotalApresPromo() + 25.0;
        cout << "\n  Frais de livraison : 25.00 MAD\n"
             << "  ─────────────────────\n"
             << "  TOTAL FINAL : " << fixed << setprecision(2)
             << totalAPayer << " MAD\n";

        Paiement* paiement = new Paiement(Paiement::Mode::CARTE, totalAPayer);
        if (!paiement->effectuerPaiement()) {
            delete paiement;
            cout << "Paiement échoué. Commande annulée.\n";
            return false;
        }

        // Step 4 : create order
        Commande* commande = new Commande(
            prochainIdCommande++, "2025-01-01", adresse
        );

        for (const auto& article : panier->getArticles()) {
            commande->ajouterProduit(article.produit, article.quantite);
            article.produit->acheter(article.quantite);   // decrement stock
        }
        commande->setPaiement(paiement);
        commande->changerEtat(CONFIRMEE);
        historiqueCommandes.push_back(commande);

        // Step 5 : success message
        cout << "\n╔══════════════════════════════════════╗\n"
             << "║      MERCI POUR VOTRE COMMANDE !     ║\n"
             << "╚══════════════════════════════════════╝\n"
             << "  Nom       : " << getNomComplet() << "\n"
             << "  Email     : " << email << "\n"
             << "  Téléphone : " << telephone << "\n"
             << "  Adresse   : " << adresse << "\n"
             << "  Total     : " << totalAPayer << " MAD\n"
             << "  Réf.      : " << paiement->getReference() << "\n"
             << "  Confirmation envoyée à " << email << "\n"
             << "══════════════════════════════════════\n\n"
             << "  Redirection vers la page principale...\n";

        // Step 6 : clear cart (user stays logged in ✅)
        panier->vider();
        return true;
    }

    // ── Order history ──────────────────────────────────────
    void afficherHistorique() const {
        cout << "\n--- Historique des commandes ---\n";
        if (historiqueCommandes.empty()) {
            cout << "  Aucune commande.\n"; return;
        }
        for (const auto* c : historiqueCommandes)
            c->afficherCommande();
    }
};

int Client::prochainIdCommande = 1000;

// ============================================================
//  CLASSE : Vendeur  (inherits Personne)
// ============================================================
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

    ~Vendeur() {
        cout << "Vendeur " << getNomComplet() << " détruit.\n";
    }

    string getNomBoutique()     const { return nomBoutique; }
    double getChiffreAffaires() const { return chiffreAffaires; }
    void   setNomBoutique(const string& b) { nomBoutique = b; }

    // ── POLYMORPHISM : override ────────────────────────────
    void afficherRole() override {
        cout << "Rôle : VENDEUR | Boutique : " << nomBoutique << "\n";
    }

    void tableauDeBord() override {
        cout << "\n╔══════════════════════════════════╗\n"
             << "║     TABLEAU DE BORD VENDEUR      ║\n"
             << "╚══════════════════════════════════╝\n"
             << "  Bienvenue, " << getNomComplet() << "\n"
             << "  Boutique : " << nomBoutique << "\n\n"
             << "  1. Ajouter un produit\n"
             << "  2. Voir mes produits\n"
             << "  3. Chiffre d'affaires : "
             << fixed << setprecision(2) << chiffreAffaires << " MAD\n"
             << "  4. Se déconnecter\n"
             << "══════════════════════════════════\n";
    }

    void ajouterProduit(int idProduit) {
        if (!estConnecte) { cout << "Connectez-vous d'abord.\n"; return; }
        idsProduits.push_back(idProduit);
        cout << "Produit #" << idProduit << " ajouté à " << nomBoutique << "\n";
    }

    void afficherProduits() const {
        cout << "\n--- Produits de " << nomBoutique << " ---\n";
        if (idsProduits.empty()) { cout << "  Aucun produit.\n"; return; }
        for (int id : idsProduits) cout << "  Produit #" << id << "\n";
    }

    void enregistrerVente(double montant) {
        chiffreAffaires += montant;
        cout << "Vente enregistrée ! CA total : "
             << fixed << setprecision(2) << chiffreAffaires << " MAD\n";
    }
};

// ============================================================
//  CLASSE : Catalogue  (NEW)
//  Manages all products: browsing, search, filter by category,
//  filter by price range
// ============================================================
class Catalogue {
private:
    vector<Produit*> produits;

public:
    ~Catalogue() {
        for (auto* p : produits) delete p;
    }

    // ── Add product ────────────────────────────────────────
    void ajouterProduit(Produit* p) {
        produits.push_back(p);
    }

    // ── Browse all ─────────────────────────────────────────
    void afficherTous() const {
        cout << "\n╔══════════════════════════════════╗\n"
             << "║         CATALOGUE COMPLET        ║\n"
             << "╚══════════════════════════════════╝\n";
        for (const auto* p : produits) {
            p->afficherCourt();
        }
        cout << "\n";
    }

    // ── Search by name keyword ─────────────────────────────
    vector<Produit*> rechercherParNom(const string& motCle) const {
        vector<Produit*> resultats;
        string mc = motCle;
        // Case-insensitive: convert to lowercase
        transform(mc.begin(), mc.end(), mc.begin(), ::tolower);
        for (auto* p : produits) {
            string nomMin = p->getNom();
            transform(nomMin.begin(), nomMin.end(), nomMin.begin(), ::tolower);
            if (nomMin.find(mc) != string::npos)
                resultats.push_back(p);
        }
        return resultats;
    }

    // ── Filter by category ─────────────────────────────────
    vector<Produit*> filtrerParCategorie(Categorie cat) const {
        vector<Produit*> resultats;
        if (cat == Categorie::TOUS) return produits;
        for (auto* p : produits)
            if (p->getCategorieEnum() == cat)
                resultats.push_back(p);
        return resultats;
    }

    // ── Filter by price range ──────────────────────────────
    vector<Produit*> filtrerParPrix(double prixMin, double prixMax) const {
        vector<Produit*> resultats;
        for (auto* p : produits)
            if (p->getPrix() >= prixMin && p->getPrix() <= prixMax)
                resultats.push_back(p);
        return resultats;
    }

    // ── Filter promotions only ─────────────────────────────
    vector<Produit*> filtrerPromotions() const {
        vector<Produit*> resultats;
        for (auto* p : produits)
            if (p->isEnPromotion()) resultats.push_back(p);
        return resultats;
    }

    // ── Get product by ID ──────────────────────────────────
    Produit* trouverParId(int id) const {
        for (auto* p : produits)
            if (p->getId() == id) return p;
        return nullptr;
    }

    // ── Display search results ─────────────────────────────
    static void afficherResultats(const vector<Produit*>& resultats,
                                  const string& titre = "Résultats") {
        cout << "\n--- " << titre << " ("
             << resultats.size() << " produit(s)) ---\n";
        if (resultats.empty()) {
            cout << "  Aucun produit trouvé.\n\n"; return;
        }
        for (const auto* p : resultats) p->afficherCourt();
        cout << "\n";
    }

    int getNbProduits() const { return (int)produits.size(); }
};

// ============================================================
//  HELPER : sign up
// ============================================================
Personne* inscrireUtilisateur(Catalogue& catalogue) {
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
        cout << "\n  ✅ Compte client créé !\n";
        return client;
    }
    else if (choixRole == 2) {
        string boutique;
        cout << "  Nom de la boutique : ";
        cin.ignore();
        getline(cin, boutique);
        Vendeur* vendeur = new Vendeur(prochainId++, prenom, nom,
                                       em, tel, pwd, boutique);
        cout << "\n  ✅ Compte vendeur créé !\n";
        return vendeur;
    }
    cout << "  Choix invalide.\n";
    return nullptr;
}

// ============================================================
//  CLIENT MENU LOOP
// ============================================================
void menuClient(Client* client, Catalogue& catalogue) {
    bool enCours = true;
    while (enCours) {
        client->tableauDeBord();
        cout << "  Votre choix : ";
        int choix; cin >> choix;

        switch (choix) {

            // ── Browse all products ─────────────────────
            case 1:
               catalogue.afficherTous();
            {
                cout << "  Entrer l'ID d'un produit pour l'ajouter au panier (0 = retour) : ";
                int pid; cin >> pid;
                if (pid != 0) {
                  Produit* p = catalogue.trouverParId(pid);
                  if (p) {
                  p->afficher();
                  cout << "  Quantite : "; int qty; cin >> qty;
                  string taille, couleur;
                  if (Vetement* v = dynamic_cast<Vetement*>(p)) {
                    v->choisirTaille();
                    v->choisirCouleur();
                    taille  = tailleToString(v->getTaille());
                    couleur = couleurToString(v->getCouleur());
                  }
                  client->ajouterAuPanier(p, qty, taille, couleur);
                 } else {
                 cout << "  Produit introuvable.\n";
                 }
                }
            }
            break;

            // ── Search by keyword ───────────────────────
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
                                cout << "  Quantité : "; int qty; cin >> qty;
                                string taille, couleur;
                                // Ask size/color for clothing
                                if (Vetement* v = dynamic_cast<Vetement*>(p)) {
                                    v->choisirTaille();
                                    v->choisirCouleur();
                                    taille  = tailleToString(v->getTaille());
                                    couleur = couleurToString(v->getCouleur());
                                }
                                client->ajouterAuPanier(p, qty, taille, couleur);
                            }
                        }
                    }
                }
                break;
            }

            // ── Filter by category ──────────────────────
            case 3: {
                cout << "\n  Catégories :\n"
                     << "  1. Tous\n"
                     << "  2. Sport & Fitness\n"
                     << "  3. Électronique\n"
                     << "  4. Vêtement\n"
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

            // ── Filter by price range ───────────────────
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

            // ── View cart ───────────────────────────────
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

            // ── Checkout ────────────────────────────────
            case 6:
                client->passerCommande();
                break;

            // ── Order history ────────────────────────────
            case 7:
                client->afficherHistorique();
                break;

            // ── Logout ──────────────────────────────────
            case 8:
                client->seDeconnecter();
                enCours = false;
                break;

            default:
                cout << "  Choix invalide.\n";
        }
    }
}

// ============================================================
//  SELLER MENU LOOP
// ============================================================
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

// ============================================================
//  MAIN
// ============================================================
int main() {
    SetConsoleOutputCP(CP_UTF8);   // encodage UTF-8
    SetConsoleCP(CP_UTF8);         // encodage entrée aussi
    cout << "╔══════════════════════════════════════╗\n"
         << "║     BIENVENUE SUR NOTRE E-SHOP       ║\n"
         << "╚══════════════════════════════════════╝\n\n";

    // ── Build the product catalogue ────────────────────────
    Catalogue catalogue;

    // Vêtements
    Vetement* v1 = new Vetement(1, "T-shirt Classic", 129.0, 50,
                                Taille::M, Couleur::BLANC,
                                "100% Coton", "Zara");
    v1->ajouterCommentaire("Ahmed", "Très confortable, je recommande !", 5);
    v1->ajouterCommentaire("Sara",  "Bonne qualité pour le prix.", 4);
    catalogue.ajouterProduit(v1);

    Vetement* v2 = new Vetement(2, "Jean Slim Fit", 399.0, 30,
                                Taille::L, Couleur::BLEU,
                                "Denim", "Levi's");
    v2->ajouterCommentaire("Karim", "Coupe parfaite.", 5);
    catalogue.ajouterProduit(v2);

    // Électronique
    Smartphone* s1 = new Smartphone(3, "Samsung Galaxy S25", 8999.0, 20,
                                    "Samsung", 24, 12, "Android",
                                    "Snapdragon 8", 200, true, 6.7);
    s1->ajouterCommentaire("Nadia", "Excellent smartphone, caméra incroyable.", 5);
    catalogue.ajouterProduit(s1);

    Ordinateur* o1 = new Ordinateur(4, "MacBook Pro M3", 22000.0, 10,
                                    "Apple", 12, "Apple M3", "Apple M3 GPU",
                                    512, "SSD", "Laptop");
    catalogue.ajouterProduit(o1);

    // Alimentation
    Alimentation* a1 = new Alimentation(5, "Huile d'olive extra vierge",
                                        45.0, 500,
                                        "2026-12-31", 0.75, 0, true);
    catalogue.ajouterProduit(a1);

    // Livre
    Livre* l1 = new Livre(6, "Le Petit Prince", 79.0, 200,
                          "Antoine de Saint-Exupéry",
                          "978-2-07-040850-4",
                          96, "Gallimard", "Français", 1943, "Roman");
    catalogue.ajouterProduit(l1);

    // Sport
    SportFitness* sf1 = new SportFitness(7, "Ballon de Football Nike",
                                         199.0, 100,
                                         "Football", "Nike",
                                         "Caoutchouc", 0.45, "Tous niveaux");
    catalogue.ajouterProduit(sf1);

    SportFitness* sf2 = new SportFitness(8, "Tapis de Yoga Adidas",
                                         289.0, 60,
                                         "Yoga", "Adidas",
                                         "TPE", 1.2, "Débutant");
    catalogue.ajouterProduit(sf2);

    // Maison
    MaisonDecor* m1 = new MaisonDecor(9, "Canapé Scandinave 3 Places",
                                      3500.0, 5,
                                      "Salon", "Scandinave",
                                      "Tissu + Bois", "220x85x80 cm",
                                      "IKEA");
    catalogue.ajouterProduit(m1);

    // Ticket
    Ticket* t1 = new Ticket(10, "Concert Gnawa Festival",
                             150.0, 500,
                             "Gnawa World Music Festival",
                             "Essaouira", "2025-06-20", "Standard");
    catalogue.ajouterProduit(t1);

    // Apply a promotion on the T-shirt
    v1->activerPromotion(20);

    // ── Sign up flow ───────────────────────────────────────
    Personne* utilisateur = inscrireUtilisateur(catalogue);
    if (!utilisateur) { cout << "Au revoir !\n"; return 0; }

    // ── Login ──────────────────────────────────────────────
    cout << "\n--- Connexion ---\n";
    string em, pwd;
    cout << "  Email        : "; cin >> em;
    cout << "  Mot de passe : "; cin >> pwd;

    if (!utilisateur->seConnecter(em, pwd)) {
        cout << "Connexion échouée. Au revoir !\n";
        delete utilisateur;
        return 0;
    }

    // ── Show info + role (POLYMORPHISM + FRIEND FUNCTION) ──
    cout << "\n" << *utilisateur << "\n";
    utilisateur->afficherRole();

    // ── Main menu (POLYMORPHISM) ───────────────────────────
    if (Client* client = dynamic_cast<Client*>(utilisateur))
        menuClient(client, catalogue);
    else if (Vendeur* vendeur = dynamic_cast<Vendeur*>(utilisateur))
        menuVendeur(vendeur);

    delete utilisateur;

    cout << "\n╔══════════════════════════════════╗\n"
         << "║           AU REVOIR !            ║\n"
         << "╚══════════════════════════════════╝\n";

    return 0;
}