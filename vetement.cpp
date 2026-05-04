#include "Vetement.h"
#include <iostream>
#include <limits>

using namespace std;

// ── Constructeur principal ─────────────────────────────
Vetement::Vetement(int id, const string& nom, double prix, int stock,
                   Taille taille, Couleur couleur,
                   const string& matiere, const string& marque,
                   bool estNeuf)
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
                            Couleur::ORANGE, Couleur::VIOLET,
                            Couleur::MARRON, Couleur::BEIGE };
}

// ── Constructeur avec listes personnalisées ────────────
Vetement::Vetement(int id, const string& nom, double prix, int stock,
                   Taille taille, Couleur couleur,
                   const string& matiere, const string& marque,
                   const vector<Taille>& tailles,
                   const vector<Couleur>& couleurs,
                   bool estNeuf)
    : Produit(id, nom, prix, stock, "Vêtement"),
      taille(taille), couleur(couleur),
      matiere(matiere), marque(marque), estNeuf(estNeuf),
      taillesDisponibles(tailles),
      couleursDisponibles(couleurs)
{}

// ── Méthodes virtuelles ────────────────────────────────
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
bool Vetement::choisirTaille() {
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
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    taille = taillesDisponibles[choix - 1];
    cout << "Taille sélectionnée : "
         << tailleToString(taille) << "\n";
    return true;
}

// ── Sélection interactive de couleur ──────────────────
bool Vetement::choisirCouleur() {
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
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    couleur = couleursDisponibles[choix - 1];
    cout << "Couleur sélectionnée : "
         << couleurToString(couleur) << "\n";
    return true;
}

// ── Vérifications ─────────────────────────────────────
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
    if (!tailleDisponible(t))
        taillesDisponibles.push_back(t);
}

void Vetement::retirerTaille(Taille t) {
    for (auto it = taillesDisponibles.begin();
         it != taillesDisponibles.end(); ++it) {
        if (*it == t) {
            taillesDisponibles.erase(it);
            return;
        }
    }
}

void Vetement::ajouterCouleur(Couleur c) {
    if (!couleurDisponible(c))
        couleursDisponibles.push_back(c);
}

// ── Setters ───────────────────────────────────────────
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

void Vetement::setTailleStr(const string& s) {
    setTaille(stringToTaille(s));
}

void Vetement::setCouleurStr(const string& s) {
    setCouleur(stringToCouleur(s));
}

// ── Getters ───────────────────────────────────────────
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

// ── Surcharge opérateur << ────────────────────────────
ostream& operator<<(ostream& os, const Vetement& v) {
    os << "[Vêtement] " << v.nom
       << " | " << v.marque
       << " | Taille: " << tailleToString(v.taille)
       << " | Couleur: " << couleurToString(v.couleur)
       << " | " << v.prix << " MAD";
    return os;
}