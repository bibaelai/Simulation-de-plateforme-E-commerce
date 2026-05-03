#include "produit.h"

// ============================================================
//  ENUM : Taille
// ============================================================
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
    throw invalid_argument("Taille invalide : " + s +
                           " (valeurs : XS S M L XL XXL)");
}

// ============================================================
//  ENUM : Couleur
// ============================================================
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
//  ENUM : Statut
// ============================================================
string statutToString(Statut s) {
    switch (s) {
        case Statut::DISPONIBLE:    return "Disponible";
        case Statut::RUPTURE_STOCK: return "Rupture de stock";
        case Statut::ARCHIVE:       return "Archivé";
        default:                    return "Inconnu";
    }
}

// ============================================================
//  CLASSE : Produit
// ============================================================

// ── Constructor ────────────────────────────────────────────
Produit::Produit(int id, const string& nom, double prix,
                 int stock, const string& categorie, double note)
    : id(id), nom(nom), prix(prix), stock(stock),
      categorie(categorie), note(note),
      statut(stock > 0 ? Statut::DISPONIBLE : Statut::RUPTURE_STOCK)
{
    if (prix < 0)
        throw invalid_argument("Le prix ne peut pas être négatif");
    if (stock < 0)
        throw invalid_argument("Le stock ne peut pas être négatif");
    if (note < 0 || note > 5)
        throw invalid_argument("La note doit être entre 0 et 5");
}

// ── Destructor ─────────────────────────────────────────────
Produit::~Produit() {}

// ── Getters ────────────────────────────────────────────────
int    Produit::getId()        const { return id; }
string Produit::getNom()       const { return nom; }
double Produit::getPrix()      const { return prix; }
int    Produit::getStock()     const { return stock; }
double Produit::getNote()      const { return note; }
string Produit::getCategorie() const { return categorie; }
Statut Produit::getStatut()    const { return statut; }

// ── Setters ────────────────────────────────────────────────
void Produit::setPrix(double p) {
    if (p < 0) throw invalid_argument("Prix négatif interdit");
    prix = p;
}

void Produit::setStock(int s) {
    if (s < 0) throw invalid_argument("Stock négatif interdit");
    stock = s;
    // Update status automatically
    if (stock == 0 && statut == Statut::DISPONIBLE)
        statut = Statut::RUPTURE_STOCK;
    else if (stock > 0 && statut == Statut::RUPTURE_STOCK)
        statut = Statut::DISPONIBLE;
}

void Produit::setNote(double n) {
    if (n < 0 || n > 5)
        throw invalid_argument("Note hors intervalle [0,5]");
    note = n;
}

void Produit::archiver() {
    statut = Statut::ARCHIVE;
    stock  = 0;
}

// ── Concrete methods ───────────────────────────────────────
void Produit::afficher() const {
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
    afficherDetails();     // polymorphic call to subclass
    cout << "└────────────────────────────────────\n";
}

bool Produit::estDisponible() const {
    return statut == Statut::DISPONIBLE && stock > 0;
}

bool Produit::acheter(int quantite) {
    if (quantite <= 0)
        throw invalid_argument("Quantité doit être > 0");
    if (statut == Statut::ARCHIVE)
        throw runtime_error("Produit archivé, achat impossible");
    if (stock < quantite) return false;   // insufficient stock
    stock -= quantite;
    if (stock == 0) statut = Statut::RUPTURE_STOCK;
    return true;
}

void Produit::reapprovisionner(int quantite) {
    if (quantite <= 0)
        throw invalid_argument("Quantité de réappro doit être > 0");
    stock += quantite;
    if (statut == Statut::RUPTURE_STOCK)
        statut = Statut::DISPONIBLE;
}

string Produit::toString() const {
    return "[" + to_string(id) + "] " + nom +
           " — " + to_string((int)prix) + " MAD";
}

// ── Operator overloading (FRIEND FUNCTION) ─────────────────
ostream& operator<<(ostream& os, const Produit& p) {
    os << fixed << setprecision(2)
       << "[" << p.id << "] " << p.nom
       << " | " << p.prix << " MAD"
       << " | Stock: " << p.stock
       << " | " << statutToString(p.statut);
    return os;
}

bool Produit::operator==(const Produit& autre) const {
    return id == autre.id;
}

bool Produit::operator<(const Produit& autre) const {
    return prix < autre.prix;
}

// ============================================================
//  CLASSE : Vetement
// ============================================================

// Constructor — all sizes and colors available by default
Vetement::Vetement(int id, const string& nom, double prix, int stock,
                   Taille taille, Couleur couleur,
                   const string& matiere, const string& marque,
                   bool estNeuf)
    : Produit(id, nom, prix, stock, "Vêtement"),
      taille(taille), couleur(couleur),
      matiere(matiere), marque(marque), estNeuf(estNeuf)
{
    taillesDisponibles  = { Taille::XS, Taille::S, Taille::M,
                            Taille::L,  Taille::XL, Taille::XXL };
    couleursDisponibles = { Couleur::ROUGE, Couleur::BLEU,
                            Couleur::VERT,  Couleur::NOIR,
                            Couleur::BLANC, Couleur::GRIS,
                            Couleur::JAUNE, Couleur::ROSE,
                            Couleur::ORANGE,Couleur::VIOLET,
                            Couleur::MARRON,Couleur::BEIGE };
}

// Constructor — custom sizes and colors
Vetement::Vetement(int id, const string& nom, double prix, int stock,
                   Taille taille, Couleur couleur,
                   const string& matiere, const string& marque,
                   const vector<Taille>&  tailles,
                   const vector<Couleur>& couleurs,
                   bool estNeuf)
    : Produit(id, nom, prix, stock, "Vêtement"),
      taille(taille), couleur(couleur),
      matiere(matiere), marque(marque), estNeuf(estNeuf),
      taillesDisponibles(tailles), couleursDisponibles(couleurs)
{}

Vetement::~Vetement() {}

// ── Getters ────────────────────────────────────────────────
Taille  Vetement::getTaille()     const { return taille; }
Couleur Vetement::getCouleur()    const { return couleur; }
string  Vetement::getTailleStr()  const { return tailleToString(taille); }
string  Vetement::getCouleurStr() const { return couleurToString(couleur); }
string  Vetement::getMatiere()    const { return matiere; }
string  Vetement::getMarque()     const { return marque; }
bool    Vetement::isNeuf()        const { return estNeuf; }

const vector<Taille>&  Vetement::getTaillesDisponibles()  const {
    return taillesDisponibles;
}
const vector<Couleur>& Vetement::getCouleursDisponibles() const {
    return couleursDisponibles;
}

// ── Setters ────────────────────────────────────────────────
void Vetement::setTaille(Taille t) {
    if (!tailleDisponible(t))
        throw invalid_argument("Taille non disponible : " +
                               tailleToString(t));
    taille = t;
}

void Vetement::setCouleur(Couleur c) {
    if (!couleurDisponible(c))
        throw invalid_argument("Couleur non disponible : " +
                               couleurToString(c));
    couleur = c;
}

void Vetement::setTailleStr(const string& s)  { setTaille(stringToTaille(s)); }
void Vetement::setCouleurStr(const string& s) { setCouleur(stringToCouleur(s)); }

// ── Availability checks ────────────────────────────────────
bool Vetement::tailleDisponible(Taille t) const {
    for (const auto& td : taillesDisponibles)
        if (td == t) return true;
    return false;
}

bool Vetement::couleurDisponible(Couleur c) const {
    for (const auto& cd : couleursDisponibles)
        if (cd == c) return true;
    return false;
}

void Vetement::ajouterTaille(Taille t) {
    if (!tailleDisponible(t)) taillesDisponibles.push_back(t);
}

void Vetement::retirerTaille(Taille t) {
    for (auto it = taillesDisponibles.begin();
         it != taillesDisponibles.end(); ++it) {
        if (*it == t) { taillesDisponibles.erase(it); return; }
    }
}

void Vetement::ajouterCouleur(Couleur c) {
    if (!couleurDisponible(c)) couleursDisponibles.push_back(c);
}

// ── Interactive console selection ──────────────────────────
bool Vetement::choisirTaille() {
    cout << "\n=== Choisir une taille pour : " << nom << " ===\n";
    for (size_t i = 0; i < taillesDisponibles.size(); ++i) {
        cout << "  [" << i + 1 << "] "
             << tailleToString(taillesDisponibles[i]) << "\n";
    }
    cout << "  Votre choix (1-" << taillesDisponibles.size() << ") : ";
    int choix;
    cin >> choix;
    if (choix < 1 || choix > (int)taillesDisponibles.size()) {
        cout << "Choix invalide.\n";
        return false;
    }
    taille = taillesDisponibles[choix - 1];
    cout << "Taille sélectionnée : " << tailleToString(taille) << "\n";
    return true;
}

bool Vetement::choisirCouleur() {
    cout << "\n=== Choisir une couleur pour : " << nom << " ===\n";
    for (size_t i = 0; i < couleursDisponibles.size(); ++i) {
        cout << "  [" << i + 1 << "] "
             << couleurToString(couleursDisponibles[i]) << "\n";
    }
    cout << "  Votre choix (1-" << couleursDisponibles.size() << ") : ";
    int choix;
    cin >> choix;
    if (choix < 1 || choix > (int)couleursDisponibles.size()) {
        cout << "Choix invalide.\n";
        return false;
    }
    couleur = couleursDisponibles[choix - 1];
    cout << "Couleur sélectionnée : " << couleurToString(couleur) << "\n";
    return true;
}

// ── POLYMORPHISM : overrides ───────────────────────────────
double Vetement::appliquerRemise(double pourcentage) {
    if (pourcentage < 0 || pourcentage > 100)
        throw invalid_argument("Pourcentage invalide (0-100)");
    prix *= (1.0 - pourcentage / 100.0);
    return prix;
}

void Vetement::afficherDetails() const {
    cout << "│ Taille  : " << tailleToString(taille)
         << "   Couleur : " << couleurToString(couleur) << "\n"
         << "│ Matière : " << matiere
         << "   Marque  : " << marque << "\n"
         << "│ État    : " << (estNeuf ? "Neuf" : "Occasion") << "\n";
    cout << "│ Tailles dispo : ";
    for (size_t i = 0; i < taillesDisponibles.size(); ++i) {
        cout << tailleToString(taillesDisponibles[i]);
        if (i < taillesDisponibles.size() - 1) cout << "  ";
    }
    cout << "\n";
    cout << "│ Couleurs dispo: ";
    for (size_t i = 0; i < couleursDisponibles.size(); ++i) {
        cout << couleurToString(couleursDisponibles[i]);
        if (i < couleursDisponibles.size() - 1) cout << "  ";
    }
    cout << "\n";
}

// ── Operator overloading (FRIEND FUNCTION) ─────────────────
ostream& operator<<(ostream& os, const Vetement& v) {
    os << "[Vêtement] " << v.nom
       << " | " << v.marque
       << " | Taille: "  << tailleToString(v.taille)
       << " | Couleur: " << couleurToString(v.couleur)
       << " | " << v.prix << " MAD";
    return os;
}

// ============================================================
//  CLASSE : Electronique
// ============================================================
Electronique::Electronique(int id, const string& nom, double prix, int stock,
                           const string& marque, int garantieMois,
                           double puissanceWatts, double tensionVolts)
    : Produit(id, nom, prix, stock, "Électronique"),
      marque(marque), garantieMois(garantieMois),
      puissanceWatts(puissanceWatts), tensionVolts(tensionVolts) {}

Electronique::~Electronique() {}

string Electronique::getMarque()    const { return marque; }
int    Electronique::getGarantie()  const { return garantieMois; }
double Electronique::getPuissance() const { return puissanceWatts; }
double Electronique::getTension()   const { return tensionVolts; }

double Electronique::appliquerRemise(double pourcentage) {
    prix *= (1.0 - pourcentage / 100.0);
    return prix;
}

void Electronique::afficherDetails() const {
    cout << "│  Marque   : " << marque
         << "  Garantie: " << garantieMois << " mois\n"
         << "│  Puissance: " << puissanceWatts
         << " W  Tension: " << tensionVolts << " V\n";
}

// ============================================================
//  CLASSE : Smartphone
// ============================================================
Smartphone::Smartphone(int id, const string& nom, double prix, int stock,
                       const string& marque, int garantieMois,
                       int ramGo, const string& os, const string& processeur,
                       int cameraMp, bool support5G, double ecran)
    : Electronique(id, nom, prix, stock, marque, garantieMois),
      ramGo(ramGo), os(os), processeur(processeur),
      cameraMp(cameraMp), support5G(support5G),
      tailleEcranPouces(ecran)
{
    categorie = "Smartphone";
}

int    Smartphone::getRAM()    const { return ramGo; }
string Smartphone::getOS()     const { return os; }
int    Smartphone::getCamera() const { return cameraMp; }
bool   Smartphone::has5G()     const { return support5G; }

void Smartphone::afficherDetails() const {
    Electronique::afficherDetails();
    cout << "│  RAM: " << ramGo << " Go  OS: " << os
         << "  CPU: " << processeur << "\n"
         << "│  Caméra: " << cameraMp
         << " Mp  5G: " << (support5G ? "Oui" : "Non")
         << "  Écran: " << tailleEcranPouces << "\"\n";
}

// ============================================================
//  CLASSE : Ordinateur
// ============================================================
Ordinateur::Ordinateur(int id, const string& nom, double prix, int stock,
                       const string& marque, int garantieMois,
                       const string& cpu, const string& gpu,
                       int stockageGo, const string& typeStockage,
                       const string& typeOrdi)
    : Electronique(id, nom, prix, stock, marque, garantieMois),
      cpu(cpu), gpu(gpu), stockageGo(stockageGo),
      typeStockage(typeStockage), typeOrdi(typeOrdi)
{
    categorie = "Ordinateur";
}

string Ordinateur::getCPU()      const { return cpu; }
string Ordinateur::getGPU()      const { return gpu; }
int    Ordinateur::getStockage() const { return stockageGo; }
string Ordinateur::getTypeOrdi() const { return typeOrdi; }

void Ordinateur::afficherDetails() const {
    Electronique::afficherDetails();
    cout << "│  CPU: " << cpu << "  GPU: " << gpu << "\n"
         << "│  Stockage: " << stockageGo << " Go (" << typeStockage
         << ")  Type: " << typeOrdi << "\n";
}

// ============================================================
//  CLASSE : Alimentation
// ============================================================
Alimentation::Alimentation(int id, const string& nom, double prix, int stock,
                           const string& dateExp, double poids,
                           int calories, bool bio,
                           const string& allergenes,
                           const string& pays)
    : Produit(id, nom, prix, stock, "Alimentation"),
      dateExpiration(dateExp), poidsKg(poids),
      caloriesPer100g(calories), bio(bio),
      allergenes(allergenes), paysOrigine(pays) {}

string Alimentation::getDateExpiration() const { return dateExpiration; }
double Alimentation::getPoids()          const { return poidsKg; }
int    Alimentation::getCalories()       const { return caloriesPer100g; }
bool   Alimentation::isBio()             const { return bio; }
string Alimentation::getAllergenes()     const { return allergenes; }

bool Alimentation::estPerime(const string& dateAujourdhui) const {
    return dateExpiration < dateAujourdhui;
}

double Alimentation::appliquerRemise(double pourcentage) {
    prix *= (1.0 - pourcentage / 100.0);
    return prix;
}

void Alimentation::afficherDetails() const {
    cout << "│  Expiration: " << dateExpiration
         << "  Poids: " << poidsKg << " kg\n"
         << "│  Calories: " << caloriesPer100g
         << "/100g  Bio: " << (bio ? "Oui" : "Non") << "\n"
         << "│  Allergènes: "
         << (allergenes.empty() ? "Aucun" : allergenes) << "\n";
}

// ============================================================
//  CLASSE : Livre
// ============================================================
Livre::Livre(int id, const string& nom, double prix, int stock,
             const string& auteur, const string& isbn,
             int pages, const string& editeur,
             const string& langue, int annee,
             const string& genre)
    : Produit(id, nom, prix, stock, "Livre"),
      auteur(auteur), isbn(isbn), nombrePages(pages),
      editeur(editeur), langue(langue),
      anneePublication(annee), genre(genre) {}

string Livre::getAuteur()  const { return auteur; }
string Livre::getISBN()    const { return isbn; }
int    Livre::getPages()   const { return nombrePages; }
string Livre::getEditeur() const { return editeur; }
string Livre::getGenre()   const { return genre; }

double Livre::appliquerRemise(double pourcentage) {
    prix *= (1.0 - pourcentage / 100.0);
    return prix;
}

void Livre::afficherDetails() const {
    cout << "│  Auteur : " << auteur << "  Éditeur: " << editeur << "\n"
         << "│  ISBN   : " << isbn  << "  Pages  : " << nombrePages << "\n"
         << "│  Langue : " << langue << "  Année  : " << anneePublication << "\n"
         << "│  Genre  : " << genre << "\n";
}
