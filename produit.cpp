#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdexcept>
using namespace std;


// ============================================================
//  ENUM : Taille
//  Toutes les tailles disponibles pour les vêtements
// ============================================================
enum class Taille {
    XS,
    S,
    M,
    L,
    XL,
    XXL
};

// Convertit un enum Taille en string lisible
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

// Convertit une string en enum Taille (pour saisie utilisateur)
Taille stringToTaille(const string& s) {
    if (s == "XS")  return Taille::XS;
    if (s == "S")   return Taille::S;
    if (s == "M")   return Taille::M;
    if (s == "L")   return Taille::L;
    if (s == "XL")  return Taille::XL;
    if (s == "XXL") return Taille::XXL;
    throw invalid_argument("Taille invalide : " + s +
                           " (valeurs : XS S M L XL XXL)");
}


// ============================================================
//  ENUM : Couleur
//  Palette de couleurs disponibles pour les vêtements
// ============================================================
enum class Couleur {
    ROUGE,
    BLEU,
    VERT,
    NOIR,
    BLANC,
    GRIS,
    JAUNE,
    ROSE,
    ORANGE,
    VIOLET,
    MARRON,
    BEIGE
};

// Convertit un enum Couleur en string lisible
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

// Convertit une string en enum Couleur
Couleur stringToCouleur(const string& s) {
    if (s == "Rouge"  || s == "ROUGE")  return Couleur::ROUGE;
    if (s == "Bleu"   || s == "BLEU")   return Couleur::BLEU;
    if (s == "Vert"   || s == "VERT")   return Couleur::VERT;
    if (s == "Noir"   || s == "NOIR")   return Couleur::NOIR;
    if (s == "Blanc"  || s == "BLANC")  return Couleur::BLANC;
    if (s == "Gris"   || s == "GRIS")   return Couleur::GRIS;
    if (s == "Jaune"  || s == "JAUNE")  return Couleur::JAUNE;
    if (s == "Rose"   || s == "ROSE")   return Couleur::ROSE;
    if (s == "Orange" || s == "ORANGE") return Couleur::ORANGE;
    if (s == "Violet" || s == "VIOLET") return Couleur::VIOLET;
    if (s == "Marron" || s == "MARRON") return Couleur::MARRON;
    if (s == "Beige"  || s == "BEIGE")  return Couleur::BEIGE;
    throw invalid_argument("Couleur invalide : " + s);
}


// ============================================================
//  ENUM : Statut  (utilisé dans Produit)
// ============================================================
enum class Statut {
    DISPONIBLE,
    RUPTURE_STOCK,
    ARCHIVE
};

string statutToString(Statut s) {
    switch (s) {
        case Statut::DISPONIBLE:    return "Disponible";
        case Statut::RUPTURE_STOCK: return "Rupture de stock";
        case Statut::ARCHIVE:       return "Archivé";
        default:                    return "Inconnu";
    }
}


// ============================================================
//  CLASSE ABSTRAITE : Produit
// ============================================================
class Produit {
protected:
    int id;
    string nom;
    double prix;
    int    stock;
    double note;        // 0.0 à 5.0
    string categorie;
    Statut statut;

public:
    // ── Constructeur ───────────────────────────────────────
    Produit(int id, const string& nom, double prix,
            int stock, const string& categorie,
            double note = 0.0)
        : id(id), nom(nom), prix(prix), stock(stock),
          categorie(categorie), note(note),
          statut(stock > 0 ? Statut::DISPONIBLE
                           : Statut::RUPTURE_STOCK)
    {
        if (prix < 0)
            throw invalid_argument("Le prix ne peut pas être négatif");
        if (stock < 0)
            throw invalid_argument("Le stock ne peut pas être négatif");
        if (note < 0 || note > 5)
            throw invalid_argument("La note doit être entre 0 et 5");
    }

    // Destructeur virtuel — indispensable en hiérarchie POO
    virtual ~Produit() {}

    // ── Getters ────────────────────────────────────────────
    int    getId()        const { return id; }
    string getNom()       const { return nom; }
    double getPrix()      const { return prix; }
    int    getStock()     const { return stock; }
    double getNote()      const { return note; }
    string getCategorie() const { return categorie; }
    Statut getStatut()    const { return statut; }

    // ── Setters avec validation ────────────────────────────
    void setPrix(double p) {
        if (p < 0) throw invalid_argument("Prix négatif interdit");
        prix = p;
    }

    void setStock(int s) {
        if (s < 0) throw invalid_argument("Stock négatif interdit");
        stock = s;
        // Met à jour le statut automatiquement
        if (stock == 0 && statut == Statut::DISPONIBLE)
            statut = Statut::RUPTURE_STOCK;
        else if (stock > 0 && statut == Statut::RUPTURE_STOCK)
            statut = Statut::DISPONIBLE;
    }

    void setNote(double n) {
        if (n < 0 || n > 5)
            throw invalid_argument("Note hors intervalle [0,5]");
        note = n;
    }

    void archiver() {
        statut = Statut::ARCHIVE;
        stock  = 0;
    }

    // ── Méthodes concrètes ─────────────────────────────────

    // Affichage complet (appelle afficherDetails() polymorphe)
    void afficher() const {
        cout << fixed << setprecision(2);
        cout << "┌────────────────────────────────────\n"
             << "│ [" << id << "] " << nom
             << "  (" << categorie << ")\n"
             << "├────────────────────────────────────\n"
             << "│ Prix    : " << prix << " MAD\n"
             << "│ Stock   : " << stock << " unité(s)\n"
             << "│ Note    : " << note << " / 5.0\n"
             << "│ Statut  : " << statutToString(statut) << "\n"
             << "├────────────────────────────────────\n";
        afficherDetails();     // appel polymorphe vers la sous-classe
        cout << "└────────────────────────────────────\n";
    }

    // Vérifie la disponibilité
    bool estDisponible() const {
        return statut == Statut::DISPONIBLE && stock > 0;
    }

    // Tente d'acheter une quantité — décrémente le stock
    bool acheter(int quantite = 1) {
        if (quantite <= 0)
            throw invalid_argument("Quantité doit être > 0");
        if (statut == Statut::ARCHIVE)
            throw runtime_error("Produit archivé, achat impossible");
        if (stock < quantite) return false;   // stock insuffisant

        stock -= quantite;
        if (stock == 0) statut = Statut::RUPTURE_STOCK;
        return true;
    }

    // Réapprovisionne le stock
    void reapprovisionner(int quantite) {
        if (quantite <= 0)
            throw invalid_argument("Quantité de réappro doit être > 0");
        stock += quantite;
        if (statut == Statut::RUPTURE_STOCK)
            statut = Statut::DISPONIBLE;
    }

    // Représentation courte en string
    string toString() const {
        return "[" + to_string(id) + "] " + nom +
               " — " + to_string((int)prix) + " MAD";
    }

    // ── Méthodes virtuelles pures (obligatoires en sous-classe) ──
    virtual double appliquerRemise(double pourcentage) = 0;
    virtual void   afficherDetails() const = 0;

    // ── Surcharge opérateurs ──────────────────────────────
    friend ostream& operator<<(ostream& os, const Produit& p) {
        os << fixed << setprecision(2)
           << "[" << p.id << "] " << p.nom
           << " | " << p.prix << " MAD"
           << " | Stock: " << p.stock
           << " | " << statutToString(p.statut);
        return os;
    }

    bool operator==(const Produit& autre) const {
        return id == autre.id;
    }

    bool operator<(const Produit& autre) const {
        return prix < autre.prix;
    }
};


// ============================================================
//  CLASSE : Vetement  (hérite de Produit)
//  MODIFICATION : taille via enum Taille (XS→XXL)
//                 couleur via enum Couleur (12 choix)
//                 + choix interactif en console
// ============================================================
class Vetement : public Produit {
protected:
    Taille taille;           // enum : XS S M L XL XXL
    Couleur couleur;         // enum : Rouge Bleu Noir…
    string matiere;          // coton, polyester, laine…
    string marque;
    bool   estNeuf;
    vector<Taille>  taillesDisponibles;  // tailles en stock
    vector<Couleur> couleursDisponibles; // couleurs disponibles

public:
    // ── Constructeur principal ─────────────────────────────
    Vetement(int id, const string& nom, double prix, int stock,
             Taille taille, Couleur couleur,
             const string& matiere, const string& marque,
             bool estNeuf = true)
        : Produit(id, nom, prix, stock, "Vêtement"),
          taille(taille), couleur(couleur),
          matiere(matiere), marque(marque), estNeuf(estNeuf)
    {
        // Par défaut : toutes les tailles et couleurs disponibles
        taillesDisponibles  = { Taille::XS, Taille::S, Taille::M,
                                Taille::L,  Taille::XL, Taille::XXL };
        couleursDisponibles = { Couleur::ROUGE, Couleur::BLEU,
                                Couleur::VERT,  Couleur::NOIR,
                                Couleur::BLANC, Couleur::GRIS,
                                Couleur::JAUNE, Couleur::ROSE,
                                Couleur::ORANGE,Couleur::VIOLET,
                                Couleur::MARRON,Couleur::BEIGE };
    }

    // ── Constructeur avec listes personnalisées ────────────
    Vetement(int id, const string& nom, double prix, int stock,
             Taille taille, Couleur couleur,
             const string& matiere, const string& marque,
             const vector<Taille>&  tailles,
             const vector<Couleur>& couleurs,
             bool estNeuf = true)
        : Produit(id, nom, prix, stock, "Vêtement"),
          taille(taille), couleur(couleur),
          matiere(matiere), marque(marque), estNeuf(estNeuf),
          taillesDisponibles(tailles),
          couleursDisponibles(couleurs)
    {}

    virtual ~Vetement() {}

    // ── Méthodes virtuelles ────────────────────────────────
    double appliquerRemise(double pourcentage) override {
        if (pourcentage < 0 || pourcentage > 100)
            throw invalid_argument("Pourcentage invalide (0-100)");
        prix *= (1.0 - pourcentage / 100.0);
        return prix;
    }

    void afficherDetails() const override {
        cout << "│ Taille  : " << tailleToString(taille)
             << "   Couleur : " << couleurToString(couleur) << "\n"
             << "│ Matière : " << matiere
             << "   Marque  : " << marque << "\n"
             << "│ État    : " << (estNeuf ? "Neuf" : "Occasion") << "\n";

        // Tailles disponibles
        cout << "│ Tailles dispo : ";
        for (size_t i = 0; i < taillesDisponibles.size(); ++i) {
            cout << tailleToString(taillesDisponibles[i]);
            if (i < taillesDisponibles.size() - 1) cout << "  ";
        }
        cout << "\n";

        // Couleurs disponibles
        cout << "│ Couleurs dispo: ";
        for (size_t i = 0; i < couleursDisponibles.size(); ++i) {
            cout << couleurToString(couleursDisponibles[i]);
            if (i < couleursDisponibles.size() - 1) cout << "  ";
        }
        cout << "\n";
    }

    // ── Sélection interactive de taille ───────────────────
    // Affiche le menu et attend la saisie de l'utilisateur
    bool choisirTaille() {
        cout << "\n=== Choisir une taille pour : " << nom << " ===\n";
        for (size_t i = 0; i < taillesDisponibles.size(); ++i) {
            cout << "  [" << i + 1 << "] "
                 << tailleToString(taillesDisponibles[i]) << "\n";
        }
        cout << "  Votre choix (1-"
             << taillesDisponibles.size() << ") : ";

        int choix;
        cin >> choix;

        if (choix < 1 || choix > (int)taillesDisponibles.size()) {
            cout << "Choix invalide.\n";
            return false;
        }
        taille = taillesDisponibles[choix - 1];
        cout << "Taille sélectionnée : "
             << tailleToString(taille) << "\n";
        return true;
    }

    // ── Sélection interactive de couleur ──────────────────
    bool choisirCouleur() {
        cout << "\n=== Choisir une couleur pour : " << nom << " ===\n";
        for (size_t i = 0; i < couleursDisponibles.size(); ++i) {
            cout << "  [" << i + 1 << "] "
                 << couleurToString(couleursDisponibles[i]) << "\n";
        }
        cout << "  Votre choix (1-"
             << couleursDisponibles.size() << ") : ";

        int choix;
        cin >> choix;

        if (choix < 1 || choix > (int)couleursDisponibles.size()) {
            cout << "Choix invalide.\n";
            return false;
        }
        couleur = couleursDisponibles[choix - 1];
        cout << "Couleur sélectionnée : "
             << couleurToString(couleur) << "\n";
        return true;
    }

    // ── Vérifications ─────────────────────────────────────

    // Vérifie si une taille est disponible
    bool tailleDisponible(Taille t) const {
        for (const auto& td : taillesDisponibles)
            if (td == t) return true;
        return false;
    }

    // Vérifie si une couleur est disponible
    bool couleurDisponible(Couleur c) const {
        for (const auto& cd : couleursDisponibles)
            if (cd == c) return true;
        return false;
    }

    // Ajoute une taille à la liste (si pas déjà présente)
    void ajouterTaille(Taille t) {
        if (!tailleDisponible(t))
            taillesDisponibles.push_back(t);
    }

    // Retire une taille de la liste
    void retirerTaille(Taille t) {
        for (auto it = taillesDisponibles.begin();
             it != taillesDisponibles.end(); ++it) {
            if (*it == t) {
                taillesDisponibles.erase(it);
                return;
            }
        }
    }

    // Ajoute une couleur à la liste
    void ajouterCouleur(Couleur c) {
        if (!couleurDisponible(c))
            couleursDisponibles.push_back(c);
    }

    // ── Setters ───────────────────────────────────────────
    void setTaille(Taille t) {
        if (!tailleDisponible(t))
            throw invalid_argument("Taille non disponible : " +
                                   tailleToString(t));
        taille = t;
    }

    void setCouleur(Couleur c) {
        if (!couleurDisponible(c))
            throw invalid_argument("Couleur non disponible : " +
                                   couleurToString(c));
        couleur = c;
    }

    // Setter via string (pratique pour saisie console)
    void setTailleStr(const string& s) {
        setTaille(stringToTaille(s));
    }

    void setCouleurStr(const string& s) {
        setCouleur(stringToCouleur(s));
    }

    // ── Getters ───────────────────────────────────────────
    Taille  getTaille()     const { return taille; }
    Couleur getCouleur()    const { return couleur; }
    string  getTailleStr()  const { return tailleToString(taille); }
    string  getCouleurStr() const { return couleurToString(couleur); }
    string  getMatiere()    const { return matiere; }
    string  getMarque()     const { return marque; }
    bool    isNeuf()        const { return estNeuf; }

    const vector<Taille>&  getTaillesDisponibles()  const {
        return taillesDisponibles;
    }
    const vector<Couleur>& getCouleursDisponibles() const {
        return couleursDisponibles;
    }

    // ── Surcharge opérateur << ────────────────────────────
    friend ostream& operator<<(ostream& os, const Vetement& v) {
        os << "[Vêtement] " << v.nom
           << " | " << v.marque
           << " | Taille: " << tailleToString(v.taille)
           << " | Couleur: " << couleurToString(v.couleur)
           << " | " << v.prix << " MAD";
        return os;
    }
};


class Electronique : public Produit {
protected:
    string marque;
    int    garantieMois;   // durée de garantie
    double puissanceWatts;
    double tensionVolts;

public:
    Electronique(int id, const string& nom, double prix, int stock,
                 const string& marque, int garantieMois,
                 double puissanceWatts = 0, double tensionVolts = 220)
        : Produit(id, nom, prix, stock, "Électronique"),
          marque(marque), garantieMois(garantieMois),
          puissanceWatts(puissanceWatts), tensionVolts(tensionVolts) {}

    string getMarque()       const { return marque; }
    int    getGarantie()     const { return garantieMois; }
    double getPuissance()    const { return puissanceWatts; }
    double getTension()      const { return tensionVolts; }

    double appliquerRemise(double pourcentage) override {
        prix *= (1.0 - pourcentage / 100.0);
        return prix;
    }

    void afficherDetails() const override {
        cout << "│  Marque   : " << marque
             << "  Garantie: " << garantieMois << " mois\n"
             << "│  Puissance: " << puissanceWatts
             << " W  Tension: " << tensionVolts << " V\n";
    }
};

class Smartphone : public Electronique {
private:
    int    ramGo;
    string os;           // Android, iOS
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
          tailleEcranPouces(ecran) {
        categorie = "Smartphone";
    }

    int    getRAM()      const { return ramGo; }
    string getOS()       const { return os; }
    int    getCamera()   const { return cameraMp; }
    bool   has5G()       const { return support5G; }

    void afficherDetails() const override {
        Electronique::afficherDetails();
        cout << "│  RAM: " << ramGo << " Go  OS: " << os
             << "  CPU: " << processeur << "\n"
             << "│  Caméra: " << cameraMp
             << " Mp  5G: " << (support5G ? "Oui" : "Non")
             << "  Écran: " << tailleEcranPouces << "\"\n";
    }
};

class Ordinateur : public Electronique {
private:
    string cpu;
    string gpu;
    int    stockageGo;
    string typeStockage;   // SSD, HDD
    string typeOrdi;       // laptop, desktop, tablette

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

    string getCPU()        const { return cpu; }
    string getGPU()        const { return gpu; }
    int    getStockage()   const { return stockageGo; }
    string getTypeOrdi()   const { return typeOrdi; }

    void afficherDetails() const override {
        Electronique::afficherDetails();
        cout << "│  CPU: " << cpu << "  GPU: " << gpu << "\n"
             << "│  Stockage: " << stockageGo << " Go (" << typeStockage
             << ")  Type: " << typeOrdi << "\n";
    }
};


// ============================================================
// 3. ALIMENTATION
// ============================================================
class Alimentation : public Produit {
private:
    string dateExpiration;  // format "AAAA-MM-JJ"
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

    string getDateExpiration() const { return dateExpiration; }
    double getPoids()          const { return poidsKg; }
    int    getCalories()       const { return caloriesPer100g; }
    bool   isBio()             const { return bio; }
    string getAllergenes()     const { return allergenes; }

    bool estPerime(const string& dateAujourdhui) const {
        return dateExpiration < dateAujourdhui;
    }

    double appliquerRemise(double pourcentage) override {
        prix *= (1.0 - pourcentage / 100.0);
        return prix;
    }

    void afficherDetails() const override {
        cout << "│  Expiration: " << dateExpiration
             << "  Poids: " << poidsKg << " kg\n"
             << "│  Calories: " << caloriesPer100g
             << "/100g  Bio: " << (bio ? "Oui" : "Non") << "\n"
             << "│  Allergènes: " << (allergenes.empty() ? "Aucun" : allergenes) << "\n";
    }
};


// ============================================================
// 4. LIVRE
// ============================================================
class Livre : public Produit {
private:
    string auteur;
    string isbn;
    int    nombrePages;
    string editeur;
    string langue;
    int    anneePublication;
    string genre;    // roman, science, histoire…

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

    string getAuteur()  const { return auteur; }
    string getISBN()    const { return isbn; }
    int    getPages()   const { return nombrePages; }
    string getEditeur() const { return editeur; }
    string getGenre()   const { return genre; }

    double appliquerRemise(double pourcentage) override {
        prix *= (1.0 - pourcentage / 100.0);
        return prix;
    }

    void afficherDetails() const override {
        cout << "│  Auteur : " << auteur << "  Éditeur: " << editeur << "\n"
             << "│  ISBN   : " << isbn  << "  Pages  : " << nombrePages << "\n"
             << "│  Langue : " << langue << "  Année  : " << anneePublication << "\n"
             << "│  Genre  : " << genre << "\n";
    }
};




// ============================================================
//  PROGRAMME PRINCIPAL — DÉMONSTRATION
// ============================================================
int main() {
    cout << "╔══════════════════════════════════╗\n"
         << "║   MINI AMAZON — Produit & Vetement ║\n"
         << "╚══════════════════════════════════╝\n\n";

    // ── 1. Création avec enum directement ─────────────────
    Vetement t1(1, "T-shirt Classic", 129.0, 50,
                Taille::M, Couleur::BLANC,
                "100% Coton", "Zara");
    t1.afficher();
    cout << t1 << "\n\n";

    // ── 2. Création avec tailles/couleurs limitées ─────────
    Vetement jeans(2, "Jean Slim Fit", 399.0, 30,
                   Taille::L, Couleur::BLEU,
                   "Denim", "Levi's",
                   { Taille::S, Taille::M, Taille::L, Taille::XL },
                   { Couleur::BLEU, Couleur::NOIR, Couleur::GRIS });
    jeans.afficher();

    // ── 3. Changer la taille via setter ───────────────────
    cout << "Changement de taille M → XL\n";
    try {
        t1.setTaille(Taille::XL);
        cout << "Nouvelle taille : " << t1.getTailleStr() << "\n";
    } catch (const exception& e) {
        cerr << "Erreur : " << e.what() << "\n";
    }

    // ── 4. Changer la couleur via string ──────────────────
    cout << "Changement couleur → Noir\n";
    try {
        t1.setCouleurStr("Noir");
        cout << "Nouvelle couleur : " << t1.getCouleurStr() << "\n";
    } catch (const exception& e) {
        cerr << "Erreur : " << e.what() << "\n";
    }

    // ── 5. Taille non disponible → exception ──────────────
    cout << "\nTentative taille XXL sur jean (non dispo) :\n";
    try {
        jeans.setTaille(Taille::XXL);
    } catch (const invalid_argument& e) {
        cout << "Exception capturée : " << e.what() << "\n";
    }

    // ── 6. Sélection interactive (décommenté en mode console)
    // t1.choisirTaille();
    // t1.choisirCouleur();
    Smartphone s1(1, "Samsung Galaxy S25", 8999.0, 50,
                  "Samsung", 24, 12, "Android", "Snapdragon 8",
                  200, true, 6.7);

    Livre l1(2, "Le Petit Prince", 79.0, 200,
             "Antoine de Saint-Exupéry", "978-2-07-040850-4",
             96, "Gallimard", "Français", 1943, "Roman");


    Alimentation a1(4, "Huile d'olive extra vierge", 45.0, 500,
                    "2025-12-31", 0.75, 0, true, "Aucun", "Maroc");

   
    // --- Affichage ---
    s1.afficher();
    l1.afficher();

    // ── 7. Polymorphisme via Produit* ─────────────────────
    cout << "\n--- Test polymorphisme ---\n";
    Produit* p = &t1;
    p->afficher();

    // ── 8. Remise ─────────────────────────────────────────
    cout << "\nRemise 30% → " << t1.appliquerRemise(30) << " MAD\n";

    // ── 9. Stock & statut ─────────────────────────────────
    cout << "\nAchat de 48 unités...\n";
    t1.acheter(48);
    cout << t1 << "\n";
    t1.acheter(2);  // épuise le stock
    cout << t1 << "\n";

    // ── 10. Réapprovisionnement ───────────────────────────
    cout << "\nRéappro +20 unités\n";
    t1.reapprovisionner(20);
    cout << t1 << "\n";

    return 0;
}