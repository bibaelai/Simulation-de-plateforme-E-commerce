#include "Panier.h"

int main() {
    cout << "\n========== AMAZON COMMERCE SYSTEM ==========\n" << endl;
    
    // Affichage des conditions
    cout << "*** CONDITIONS D'ELIGIBILITE ***" << endl;
    cout << "Seuil pour devenir Prime: " << PanierPrime::getSeuil() << " DH" << endl;
    cout << "TVA: " << Panier::getTVA() * 100 << "%" << endl;
    cout << "Reduction Prime par defaut: 15%" << endl;
    
    // Création des clients
    Client client1(1001, "Ahmed", "Casablanca", "0612345678");
    Client client2(1002, "Sara", "Rabat", "0687654321");
    Client client3(1003, "Karim", "Tanger", "0678912345");
    
    // Création des articles
    Article pc("PC Gamer", 6000.0, 1);
    Article telephone("iPhone 15", 5000.0, 1);
    Article souris("Souris Gaming", 200.0, 2);
    Article clavier("Clavier Meca", 800.0, 1);
    
    cout << "\n========================================" << endl;
    cout << "1. TEST PANIER STANDARD" << endl;
    cout << "========================================" << endl;
    
    Panier p1(1, client1);
    p1 += pc;
    p1 += souris;
    
    cout << p1;
    
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
    
    Panier p2(2, client2);
    p2 += pc;
    p2 += telephone;
    p2 += souris;
    
    cout << p2;
    
    if (PanierPrime::estEligible(p2)) {
        cout << "\n✅ Client " << client2.getNom() << " est eligible pour Prime!" << endl;
        cout << "   Total HT: " << p2.calculerTotalHT() << " DH" << endl;
        cout << "   Seuil requis: " << PanierPrime::getSeuil() << " DH" << endl;
        
        PanierPrime p2prime(p2, 0.15);
        cout << "\n--- Apres transformation en PanierPrime ---" << endl;
        cout << "Total HT apres reduction 15%: " << p2prime.calculerTotalHT() << " DH" << endl;
        cout << "Total TTC apres reduction: " << p2prime.calculerTotalTTC() << " DH" << endl;
    } else {
        cout << "\n❌ Client " << client2.getNom() << " n'est pas eligible!" << endl;
        cout << "   Besoin de " << PanierPrime::getSeuil() - p2.calculerTotalHT() << " DH de plus" << endl;
    }
    
    cout << "\n========================================" << endl;
    cout << "3. TEST PANIER PRIME" << endl;
    cout << "========================================" << endl;
    
    PanierPrime p3(3, client3, 0.20);
    p3 += pc;
    p3 += telephone;
    p3 += clavier;
    
    cout << p3;
    
    cout << "\n--- Modification du taux de reduction (25%) ---" << endl;
    p3.setTauxReduction(0.25);
    cout << "Nouveau total TTC: " << p3.calculerTotalTTC() << " DH" << endl;
    
    cout << "\n========================================" << endl;
    cout << "4. TEST OPERATEURS" << endl;
    cout << "========================================" << endl;
    
    // Test operator==
    Panier p4(4, client1);
    p4 += pc;
    p4 += souris;
    
    cout << "P1 Total TTC: " << p1.calculerTotalTTC() << " DH" << endl;
    cout << "P4 Total TTC: " << p4.calculerTotalTTC() << " DH" << endl;
    
    if (p1 == p4) {
        cout << "✅ P1 et P4 ont le meme Total TTC!" << endl;
    } else {
        cout << "❌ P1 et P4 ont des Totaux differents" << endl;
    }
    
    // Test operator[]
    try {
        cout << "\n--- Test operator[] ---" << endl;
        cout << "Premier article du panier P1: " << p1[0].ref << endl;
        p1[0].qte = 5;
        cout << "Nouvelle quantite: " << p1[0].qte << endl;
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
