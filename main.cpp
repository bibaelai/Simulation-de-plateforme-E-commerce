#include <iostream>
#include "Produit.h"
#include "Vetement.h"

using namespace std;

int main() {

    cout << "╔══════════════════════════════════════╗\n";
    cout << "║       GESTION DES VÊTEMENTS          ║\n";
    cout << "╚══════════════════════════════════════╝\n\n";

    // ── Création de vêtements ─────────────────────────────
    Vetement v1(1, "T-Shirt Coton", 199.99, 50,
                Taille::M, Couleur::BLANC,
                "Coton", "Zara");

    Vetement v2(2, "Jean Slim", 499.00, 20,
                Taille::L, Couleur::BLEU,
                "Denim", "Levi's",
                { Taille::S, Taille::M, Taille::L, Taille::XL },
                { Couleur::BLEU, Couleur::NOIR, Couleur::GRIS });

    Vetement v3(3, "Robe d'été", 349.00, 0,    // stock 0 → RUPTURE
                Taille::S, Couleur::ROSE,
                "Lin", "H&M");

    // ── Affichage via polymorphisme ───────────────────────
    cout << "━━━ Catalogue ━━━━━━━━━━━━━━━━━━━━━━━━\n\n";

    // Tableau de pointeurs vers la classe mère Produit
    Produit* catalogue[] = { &v1, &v2, &v3 };
    int nbProduits = 3;

    for (int i = 0; i < nbProduits; i++) {
        catalogue[i]->afficher();   // appel polymorphique
        cout << "\n";
    }

    // ── Opérateur << ─────────────────────────────────────
    cout << "━━━ Résumé rapide ━━━━━━━━━━━━━━━━━━━━\n";
    cout << v1 << "\n";
    cout << v2 << "\n";
    cout << v3 << "\n\n";

    // ── Achat ─────────────────────────────────────────────
    cout << "━━━ Test d'achat ━━━━━━━━━━━━━━━━━━━━━\n";
    v1.acheter(3);
    cout << "Stock restant : " << v1.getStock() << "\n\n";

    // Achat impossible (rupture de stock)
    cout << "Tentative sur stock épuisé :\n";
    v3.acheter(1);
    cout << "\n";

    // ── Remise ────────────────────────────────────────────
    cout << "━━━ Application de remise ━━━━━━━━━━━━\n";
    cout << "Prix avant : " << v2.getPrix() << " MAD\n";
    v2.appliquerRemise(20);   // -20%
    cout << "Prix après remise 20% : " << v2.getPrix() << " MAD\n\n";

    // ── Choisir taille et couleur (interactif) ────────────
    cout << "━━━ Sélection interactive ━━━━━━━━━━━━\n";
    v1.choisirTaille();
    v1.choisirCouleur();
    cout << "\n";

    // ── Vérifications ─────────────────────────────────────
    cout << "━━━ Vérifications ━━━━━━━━━━━━━━━━━━━━\n";
    cout << "Taille XL disponible pour Jean Slim ? "
         << (v2.tailleDisponible(Taille::XL) ? "Oui" : "Non") << "\n";
    cout << "Couleur Rouge disponible pour Jean Slim ? "
         << (v2.couleurDisponible(Couleur::ROUGE) ? "Oui" : "Non") << "\n\n";

    // ── Réapprovisionnement ───────────────────────────────
    cout << "━━━ Réapprovisionnement ━━━━━━━━━━━━━━\n";
    cout << "Robe d'été — stock avant : " << v3.getStock() << "\n";
    v3.reapprovisionner(15);
    cout << "Robe d'été — stock après : " << v3.getStock() << "\n\n";

    cout << "╔══════════════════════════════════════╗\n";
    cout << "║            FIN DU PROGRAMME          ║\n";
    cout << "╚══════════════════════════════════════╝\n";

    return 0;
}
