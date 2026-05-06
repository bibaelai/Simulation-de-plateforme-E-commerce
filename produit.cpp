#include "produit.h"

// ============================================================
// ENUM : Taille
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

    throw invalid_argument("Taille invalide : " + s);
}

// ============================================================
// ENUM : Couleur
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
// ENUM : Statut
// ============================================================

string statutToString(Statut s) {
    switch (s) {
        case Statut::DISPONIBLE:    return "Disponible";
        case Statut::RUPTURE_STOCK: return "Rupture de stock";
        case Statut::ARCHIVE:       return "Archive";
        default:                    return "Inconnu";
    }
}

// ============================================================
// CLASSE ABSTRAITE : Produit
// ============================================================

Produit::Produit(int id, const string& nom, double prix,
                 int stock, const string& categorie)
    : id(id), nom(nom), prix(prix), stock(stock),
      categorie(categorie)
{
    if (prix < 0) {
        throw invalid_argument("Le prix ne peut pas etre negatif");
    }

    if (stock < 0) {
        throw invalid_argument("Le stock ne peut pas etre negatif");
    }

    if (stock > 0) {
        statut = Statut::DISPONIBLE;
    } else {
        statut = Statut::RUPTURE_STOCK;
    }
}

Produit::~Produit() {
}

// Getters
int Produit::getId() const {
    return id;
}

string Produit::getNom() const {
    return nom;
}

double Produit::getPrix() const {
    return prix;
}

int Produit::getStock() const {
    return stock;
}

// Affichage général
void Produit::afficher() const {
    cout << fixed << setprecision(2);

    cout << "----------------------------------------" << endl;
    cout << "ID        : " << id << endl;
    cout << "Nom       : " << nom << endl;
    cout << "Categorie : " << categorie << endl;
    cout << "Prix      : " << prix << " MAD" << endl;
    cout << "Stock     : " << stock << endl;
    cout << "Statut    : " << statutToString(statut) << endl;

    afficherDetails();

    cout << "----------------------------------------" << endl;
}

// Acheter une quantité
bool Produit::acheter(int qte) {
    if (qte <= 0) {
        throw invalid_argument("La quantite doit etre superieure a 0");
    }

    if (statut == Statut::ARCHIVE) {
        cout << "Produit archive. Achat impossible." << endl;
        return false;
    }

    if (qte > stock) {
        cout << "Stock insuffisant." << endl;
        return false;
    }

    stock -= qte;

    if (stock == 0) {
        statut = Statut::RUPTURE_STOCK;
    }

    return true;
}

// Réapprovisionner le stock
void Produit::reapprovisionner(int qte) {
    if (qte <= 0) {
        throw invalid_argument("La quantite doit etre superieure a 0");
    }

    stock += qte;

    if (statut == Statut::RUPTURE_STOCK) {
        statut = Statut::DISPONIBLE;
    }
}

// Opérateur <<
ostream& operator<<(ostream& os, const Produit& p) {
    os << fixed << setprecision(2);
    os << "[" << p.id << "] "
       << p.nom
       << " | Prix: " << p.prix << " MAD"
       << " | Stock: " << p.stock
       << " | Statut: " << statutToString(p.statut);

    return os;
}

// ============================================================
// CLASSE : Electronique
// ============================================================

Electronique::Electronique(int id, const string& nom, double prix, int stock,
                           const string& marque, int garantie)
    : Produit(id, nom, prix, stock, "Electronique"),
      marque(marque), garantie(garantie)
{
}

double Electronique::appliquerRemise(double p) {
    if (p < 0 || p > 100) {
        throw invalid_argument("Le pourcentage doit etre entre 0 et 100");
    }

    prix = prix - (prix * p / 100);
    return prix;
}

void Electronique::afficherDetails() const {
    cout << "Marque    : " << marque << endl;
    cout << "Garantie  : " << garantie << " mois" << endl;
}

// ============================================================
// CLASSE : Smartphone
// ============================================================

Smartphone::Smartphone(int id, const string& nom, double prix, int stock,
                       const string& marque, int garantie, int ram)
    : Electronique(id, nom, prix, stock, marque, garantie),
      ram(ram)
{
    categorie = "Smartphone";
}

void Smartphone::afficherDetails() const {
    Electronique::afficherDetails();
    cout << "RAM       : " << ram << " Go" << endl;
}

// ============================================================
// CLASSE : Ordinateur
// ============================================================

Ordinateur::Ordinateur(int id, const string& nom, double prix, int stock,
                       const string& marque, int garantie, const string& cpu)
    : Electronique(id, nom, prix, stock, marque, garantie),
      cpu(cpu)
{
    categorie = "Ordinateur";
}

void Ordinateur::afficherDetails() const {
    Electronique::afficherDetails();
    cout << "CPU       : " << cpu << endl;
}

// ============================================================
// CLASSE : Alimentation
// ============================================================

Alimentation::Alimentation(int id, const string& nom, double prix, int stock,
                           const string& dateExp)
    : Produit(id, nom, prix, stock, "Alimentation"),
      dateExpiration(dateExp)
{
}

double Alimentation::appliquerRemise(double p) {
    if (p < 0 || p > 100) {
        throw invalid_argument("Le pourcentage doit etre entre 0 et 100");
    }

    prix = prix - (prix * p / 100);
    return prix;
}

void Alimentation::afficherDetails() const {
    cout << "Date expiration : " << dateExpiration << endl;
}

// ============================================================
// CLASSE : Livre
// ============================================================

Livre::Livre(int id, const string& nom, double prix, int stock,
             const string& auteur)
    : Produit(id, nom, prix, stock, "Livre"),
      auteur(auteur)
{
}

double Livre::appliquerRemise(double p) {
    if (p < 0 || p > 100) {
        throw invalid_argument("Le pourcentage doit etre entre 0 et 100");
    }

    prix = prix - (prix * p / 100);
    return prix;
}

void Livre::afficherDetails() const {
    cout << "Auteur    : " << auteur << endl;
}