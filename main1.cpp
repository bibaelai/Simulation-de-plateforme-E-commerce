#include "Panier.h"

int main() {
    cout << "\n========== AMAZON COMMERCE SYSTEM ==========\n" << endl;
    
    // Affichage des conditions
    cout << "*** CONDITIONS D'ELIGIBILITE ***" << endl;
    cout << "Seuil pour devenir Prime: " << PanierPrime::getSeuil() << " DH" << endl;
    cout << "TVA: " << Panier::getTVA() * 100 << "%" << endl;
    cout << "Reduction Prime par defaut: 15%" << endl;
    
    // Création des clients
    Client c1(1001, "Ahmed", "Bennani", "ahmed.bennani@email.com", "pass123");
    Personne& personne1 = c1;

    Client c2(1002, "Sara", "Amrani", "sara.amrani@email.com", "sara2026");
    Personne& personne2 = c2;

    Client c3(1003, "Karim", "Rachidi", "karim.rachidi@email.com", "karim_secure");
    Personne& personne3 = c3;

    Client c4(1004, "Laila", "Mansouri", "laila.m@email.com", "laila_pass");
    Personne& personne4 = c4;

    Client c5(1005, "Youssef", "Tazi", "y.tazi@email.com", "youssef99");
    Personne& personne5 = c5;
    
    // Création des articles
    // id, nom, prix, stock, taille, couleur, matiere, marque, estNeuf
    Vetement* v1 = new Vetement(301, "T-shirt Cotton", 150.0, 10, Taille::M, Couleur::BLEU, "Coton", "Zara", true);
    Vetement* v2 = new Vetement(302, "Jean Slim", 350.0, 5, Taille::L, Couleur::NOIR, "Denim", "Levi's", true);
    Vetement* v3 = new Vetement(303, "Veste Cuir", 1200.0, 2, Taille::XL, Couleur::MARRON, "Cuir", "Gucci", false);
    Vetement* v4 = new Vetement(304, "Pull Over", 250.0, 8, Taille::S, Couleur::GRIS, "Laine", "H&M", true);
    Vetement* v5 = new Vetement(305, "Pantalon Cargo", 400.0, 4, Taille::M, Couleur::VERT, "Synthetique", "Nike", true);

//  Test Panier Standard (p1)
    cout << "\n========================================" << endl;
    cout << "1. TEST PANIER STANDARD" << endl;
    cout << "========================================" << endl;

    Panier p1(1, c1);
    p1 += v1;
    p1 += v2;
    p1 += v3;

    cout << p1;

    Panier p2(2, c2);
    p2 += v3;
    p2 += v4;
    cout << p2;

//  Test Panier Prime (p4)
    cout << "\n========================================" << endl;
    cout << "2. TEST PANIER PRIME (Reduction 15%)" << endl;
    cout << "========================================" << endl;

    PanierPrime p4(4, c4, 0.15);
    p4 += v1; 
    p4 += v4;
    p4 += v5;

    cout << p4;
    
    // Test ++
    cout << "\n--- Apres ++ (Augmentation) ---" << endl;
    ++p1;
    cout << p1;
    
    // Test --
    cout << "\n--- Apres -- (Diminution) ---" << endl;
    --p1;
    cout << p1;
    
    cout << "\n========================================" << endl;
    cout << "2. TEST ELIGIBILITE PRIME" << endl;
    cout << "========================================" << endl;
    
    
    
    if (PanierPrime::estEligible(p1)) {
        cout << "\n Client " << c2.getNom() << " est eligible pour Prime!" << endl;
        cout << "   Total HT: " << p1.calculerTotalHT() << " DH" << endl;
        cout << "   Seuil requis: " << PanierPrime::getSeuil() << " DH" << endl;
        
        PanierPrime p1prime(p1, 0.15);
        cout << "\n--- Apres transformation en PanierPrime ---" << endl;
        cout << "Total HT apres reduction 15%: " << p1prime.calculerTotalHT() << " DH" << endl;
        cout << "Total TTC apres reduction: " << p1prime.calculerTotalTTC() << " DH" << endl;
    } else {
        cout << "\n Client " << c2.getNom() << " n'est pas eligible!" << endl;
        cout << "   Besoin de " << PanierPrime::getSeuil() - p1.calculerTotalHT() << " DH de plus" << endl;
    }
    
    cout << "\n========================================" << endl;
    cout << "3. TEST PANIER PRIME" << endl;
    cout << "========================================" << endl;
    
    
    Panier p3(3, c3);
    p3 += v1; // T-shirt
    p3 += v3; // Veste Cuir 
    p3 += v5; // Pantalon Cargo
    
    // Conversion de Panier P3 en PANIER PRIME
    PanierPrime p3Prime(p3, 0.15); 

    p3Prime.setTauxReduction(0.25); 
    cout << "Nouveau total TTC (Prime): " << p3Prime.calculerTotalTTC() << " DH" << endl;

    cout << "\n========================================" << endl;
    cout << "4. TEST OPERATEURS" << endl;
    cout << "========================================" << endl;
    
    // Test operator==
    cout << "P1 Total TTC: " << p1.calculerTotalTTC() << " DH" << endl;
    cout << "P4 Total TTC: " << p4.calculerTotalTTC() << " DH" << endl;
    
    if (p1 == p4) {
        cout << " P1 et P4 ont le meme Total TTC!" << endl;
    } else {
        cout << " P1 et P4 ont des Totaux differents" << endl;
    }
    
    // Test operator[]
    try {
      cout << "\n--- Test operator[] ---" << endl;
    
      cout << "Premier article du panier P1: " << p1[0]->getId() << endl;
    
      p1[0]->reapprovisionner(5); 

      cout << "Nouvelle quantite: " << p1[0]->getStock() << endl;
    
    } catch (const exception& e) {
     cerr << "Erreur: " << e.what() << endl;
    }
    // Test conversion
    cout << "\n--- Test conversion en double ---" << endl;
    double total = p1;
    cout << "Total TTC de P1 = " << fixed << setprecision(2) << total << " DH" << endl;
    
    // Test polymorphisme
    cout << "\n--- Test polymorphisme ---" << endl;
    Panier* paniers[] = {&p1, &p3};
    for (int i = 0; i < 2; i++) {
        cout << "Panier " << i+1 << " Total TTC: " 
             << paniers[i]->calculerTotalTTC() << " DH" << endl;
    }
    
    cout << "\n========== FIN DU PROGRAMME ==========\n" << endl;
    
    return 0;
}
