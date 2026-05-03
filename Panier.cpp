#include "Panier.h"
#include <algorithm>

double Panier::tva = 0.20;
double PanierPrime::SEUIL_ELIGIBILITE = 10000.0;

// ========== Panier ==========
Panier::Panier(int _idPanier, const Client& _client) 
    : idPanier(_idPanier), client(_client) {}

Panier::~Panier() {
    cout << "Destruction du Panier ID: " << idPanier << endl;
}

void Panier::ajouterArticle(const Article& a) {
    for (auto& item : items) {
        if (item.ref == a.ref) {
            item.qte += a.qte;
            sauvegarderAction("Ajout quantite: " + a.ref);
            return;
        }
    }
    items.push_back(a);
    sauvegarderAction("Ajout article: " + a.ref);
}

double Panier::calculerTotalHT() const {
    double total = 0;
    for (const auto& item : items) {
        total += (item.prix * item.qte);
    }
    return total;
}

double Panier::calculerTotalTTC() const {
    return calculerTotalHT() * (1 + tva);
}

void Panier::sauvegarderAction(string action) const {
    ofstream fichier("historique_panier.txt", ios::app);
    if (fichier.is_open()) {
        fichier << "Panier " << idPanier << " - Client ID: " << client.getId()
                << " - Action: " << action << endl;
        fichier.close();
    }
}

void Panier::viderPanier() {
    items.clear();
    sauvegarderAction("Panier vide");
}

int Panier::getTaille() const {
    return items.size();
}

int Panier::getIdPanier() const { return idPanier; }
Client Panier::getClient() const { return client; }
int Panier::getIdClient() const { return client.getId(); }
string Panier::getClientNom() const { return client.getNom(); }

double Panier::getTVA() { return tva; }
void Panier::setTVA(double nouvelle) { tva = nouvelle; }

// ========== Opérateurs ==========
Panier& Panier::operator++() {
    if (items.empty()) return *this;
    for (auto& item : items) item.qte++;
    sauvegarderAction("++ operateur");
    return *this;
}

Panier& Panier::operator--() {
    if (items.empty()) return *this;
    for (auto& item : items) if (item.qte > 0) item.qte--;
    items.erase(remove_if(items.begin(), items.end(),
                 [](const Article& a) { return a.qte <= 0; }), items.end());
    sauvegarderAction("-- operateur");
    return *this;
}

Panier& Panier::operator+=(const Article& a) {
    ajouterArticle(a);
    return *this;
}

Panier& Panier::operator-=(string ref) {
    auto it = remove_if(items.begin(), items.end(),
               [&ref](const Article& a) { return a.ref == ref; });
    if (it != items.end()) {
        items.erase(it, items.end());
        sauvegarderAction("Suppression: " + ref);
    }
    return *this;
}

Panier& Panier::operator=(const Panier& source) {
    if (this != &source) {
        this->idPanier = source.idPanier;
        this->client = source.client;
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

Article& Panier::operator[](int index) {
    if (index < 0 || index >= (int)items.size())
        throw out_of_range("Index hors limites!");
    return items[index];
}

const Article& Panier::operator[](int index) const {
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
    p.client.afficher();
    os << endl;
    os << "----------------------------------------" << endl;
    
    if (p.items.empty()) {
        os << "  Le panier est vide!" << endl;
    } else {
        int i = 1;
        for (const auto& item : p.items) {
            os << "  " << i++ << ". " << item.ref 
               << " | " << fixed << setprecision(2) << item.prix 
               << " DH | x" << item.qte 
               << " = " << (item.prix * item.qte) << " DH" << endl;
        }
    }
    
    os << "----------------------------------------" << endl;
    os << "Total HT : " << fixed << setprecision(2) << p.calculerTotalHT() << " DH" << endl;
    os << "Total TTC: " << fixed << setprecision(2) << p.calculerTotalTTC() << " DH" << endl;
    os << "========================================" << endl;
    return os;
}

// ========== PanierPrime ==========
PanierPrime::PanierPrime(int _idPanier, const Client& _client, double _taux) 
    : Panier(_idPanier, _client), tauxReduction(_taux) {
    if (tauxReduction < 0) tauxReduction = 0;
    if (tauxReduction > 0.5) tauxReduction = 0.5;
}

PanierPrime::PanierPrime(const Panier& p, double _taux) 
    : Panier(p), tauxReduction(_taux) {
    if (tauxReduction < 0) tauxReduction = 0;
    if (tauxReduction > 0.5) tauxReduction = 0.5;
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
        sauvegarderAction("Taux reduction: " + to_string(nouveauTaux * 100) + "%");
    }
}

bool PanierPrime::estEligible(const Panier& p) {
    return p.calculerTotalHT() >= SEUIL_ELIGIBILITE;
}

double PanierPrime::getSeuil() { return SEUIL_ELIGIBILITE; }
void PanierPrime::setSeuil(double nouveauSeuil) { SEUIL_ELIGIBILITE = nouveauSeuil; }
