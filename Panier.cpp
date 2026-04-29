#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;


class Article {
public:
    string ref;
    double prix;
    int qte;

    Article(string r, double p, int q) : ref(r), prix(p), qte(q) {}
};

class Panier {
protected:
    int id;
    string client;
    vector<Article> items; 

public:
    
    Panier(int _id, string _client) : id(_id), client(_client) {}

    virtual ~Panier() {
        cout << "Nettoyage du Panier ID: " << id << " (Mémoire libérée)" << endl;
    }

    void ajouterArticle(const Article& a) {
        items.push_back(a);
    }

    virtual double calculerTotal() const {
        double total = 0;
        for (const auto& item : items) {
            total += (item.prix * item.qte);
        }
        return total;
    }

    // --- Surcharge des Opérateurs (Members) ---

    Panier& operator=(const Panier& source) {
        if (this != &source) { 
            this->id = source.id;
            this->client = source.client;
            this->items = source.items; 
        }
        return *this; 
    }

    bool operator==(const Panier& other) const {
        return (this->calculerTotal() == other.calculerTotal());
    }

    bool operator!=(const Panier& other) const {
        return !(*this == other);
    }

    friend ostream& operator<<(ostream& os, const Panier& p);
};

ostream& operator<<(ostream& os, const Panier& p) {
    os << "ID: [" << p.id << "] | Client: " << p.client 
       << " | Total: " << fixed << setprecision(2) << p.calculerTotal() << " DH";
    return os;
}


class PanierPrime : public Panier {
private:
    double tauxReduction;

public:
    PanierPrime(int _id, string _client, double _taux = 0.15) 
        : Panier(_id, _client), tauxReduction(_taux) {}

double calculerTotal() const override {
        return Panier::calculerTotal() * (1 - tauxReduction);
    }
};

int main() {
   
    Article pc("MacBook Pro", 15000.0, 1);
    Article mouse("Logitech MX", 800.0, 1);

    Panier p1(1, "Ahmed");
    p1.ajouterArticle(pc);
    p1.ajouterArticle(mouse);

    PanierPrime p2(2, "Sara", 0.10);
    p2.ajouterArticle(pc);
    p2.ajouterArticle(mouse);

    Panier* systeme[2];
    systeme[0] = &p1;
    systeme[1] = &p2;

    cout << "--- Amazon Checkout System ---" << endl;
    for (int i = 0; i < 2; i++) {
        cout << *(systeme[i]) << endl;
    }
    cout << "\n--- Testing Operators ---" << endl;
    if (p1 != p2) {
        cout << "Les prix sont différents (Normal car p2 est Prime)" << endl;
    }

    Panier p3(3, "Inconnu");
    p3 = p1; 
    cout << "Copie Re-order (p3) : " << p3 << endl;

    return 0;
}
 