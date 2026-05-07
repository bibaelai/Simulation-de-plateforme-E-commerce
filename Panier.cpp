#include "Panier.h"
#include <algorithm>

double Panier::tva = 0.20;
double PanierPrime::SEUIL_ELIGIBILITE = 10000.0;

// ========== Panier ==========

//________CONSTRUCTEUR_______
Panier::Panier(int _idPanier, Personne& _personne) 
    : idPanier(_idPanier), personne(_personne) {}

//________DESTRUCTEUR________
Panier::~Panier() {
    items.clear();
    cout << "Destruction du Panier ID: " << idPanier << endl;
}

void Panier::ajouterProduit(Produit* a) {
    for (auto* item : items) {
        if (item->getId() == a->getId()) {
            item->reapprovisionner(a->getStock()); 
            return;
        }
    }
    items.push_back(a);
}

double Panier::calculerTotalHT() const {
    double total = 0;
    for (const auto* item : items) {
        total += (item->getPrix() * item->getStock());
    }
    return total;
}

double Panier::calculerTotalTTC() const {
    return calculerTotalHT() * (1 + tva);
}

void Panier::viderPanier() {
    items.clear();
}

int Panier::getTaille() const {
    return items.size();
}

int Panier::getIdPanier() const { return idPanier; }
int Panier::getIdPersonne() const { return personne.getId(); }
string Panier::getPersonneNom() const { return personne.getNom(); }

double Panier::getTVA() { return tva; }
void Panier::setTVA(double nouvelle) { tva = nouvelle; }

// ========== Opérateurs ==========
Panier& Panier::operator++() {
    if (items.empty()) return *this;
    for (auto* item : items) {
        item->acheter(1); 
    }
    return *this;
}

Panier& Panier::operator--() {
    if (items.empty()) return *this;
    for (auto* item : items) {
        if (item->getStock() > 0) {
        }
    }
    items.erase(remove_if(items.begin(), items.end(), [](Produit* a) {
        return a->getStock() <= 0; 
    }), items.end());
    return *this;
}
Panier& Panier::operator+=(Produit* a) {
    ajouterProduit(a);
    return *this;
}

Panier& Panier::operator-=(int ref) {
    auto it = remove_if(items.begin(), items.end(), 
    [&ref](const Produit* a) { return a->getId() == ref; });
    if (it != items.end()) {
        items.erase(it, items.end());
    }
    return *this;
}

Panier& Panier::operator=(const Panier& source) {
    if (this != &source) {
        this->idPanier = source.idPanier;
        this->personne = source.personne;
        this->items = source.items;
    }
    return *this;
}

bool Panier::operator==(const Panier& other) const {
    return (this->calculerTotalTTC() == other.calculerTotalTTC());
}

bool Panier::operator!=(const Panier& other) const {
    return !(*this == other);
}

Produit* Panier::operator[](int index) {
    if (index < 0 || index >= (int)items.size())
        throw out_of_range("Index hors limites!");
    return items[index];
}

const Produit* Panier::operator[](int index) const {
    if (index < 0 || index >= (int)items.size())
        throw out_of_range("Index hors limites!");
    return items[index];
}

Panier::operator double() const {
    return calculerTotalTTC();
}

ostream& operator<<(ostream& os, const Panier& p) {
    os << "\n========================================" << endl;
    os << "PANIER ID: [" << p.idPanier << "] | ";
    p.personne.afficherRole();
    os << endl;
    os << "----------------------------------------" << endl;
    
    if (p.items.empty()) {
        os << "  Le panier est vide!" << endl;
    } else {
        int i = 1;
        for (const auto& item : p.items) {
            os << "  " << i++ << ". " << item->getId() << " | " 
        << fixed << setprecision(2) << item->getPrix() << " DH" 
        << " x" << item->getStock() 
        << " = " << (item->getPrix() * item->getStock()) << " DH" << endl;
    }
}
    
    os << "----------------------------------------" << endl;
    os << "Total HT : " << fixed << setprecision(2) << p.calculerTotalHT() << " DH" << endl;
    os << "Total TTC: " << fixed << setprecision(2) << p.calculerTotalTTC() << " DH" << endl;
    os << "========================================" << endl;
    return os;
}

// ========== PanierPrime ==========

PanierPrime::PanierPrime(int _idPanier, Personne& _personne, double _taux) 
    : Panier(_idPanier, _personne) 
{ 
    if (_taux < 0) tauxReduction = 0;
    else if (_taux > 0.5) tauxReduction = 0.5;
    else tauxReduction = _taux;
}
PanierPrime::PanierPrime(const Panier& p, double _taux) 
    : Panier(p) 
{
    if (_taux < 0) tauxReduction = 0;
    else if (_taux > 0.5) tauxReduction = 0.5;
    else tauxReduction = _taux;

}
double PanierPrime::calculerTotalHT() const {
    return Panier::calculerTotalHT() * (1 - tauxReduction);
}

double PanierPrime::calculerTotalTTC() const {
    return calculerTotalHT() * (1 + tva);
}

double PanierPrime::getTauxReduction() const {
    return tauxReduction;
}

void PanierPrime::setTauxReduction(double nouveauTaux) {
    if (nouveauTaux >= 0 && nouveauTaux <= 0.5) {
        tauxReduction = nouveauTaux;
    }
}

bool PanierPrime::estEligible(const Panier& p) {
    return p.calculerTotalHT() >= SEUIL_ELIGIBILITE;
}

double PanierPrime::getSeuil() { return SEUIL_ELIGIBILITE; }
void PanierPrime::setSeuil(double nouveauSeuil) { SEUIL_ELIGIBILITE = nouveauSeuil; }
