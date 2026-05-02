#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ==================== PERSON (Abstract Base) ====================

class Person {
protected:
    int id;
    string firstName;
    string lastName;
    string email;
    string password;
    bool isLoggedIn;

public:
    // Default constructor
    Person() {
        id          = 0;
        firstName   = "NULL";
        lastName    = "NULL";
        email       = "NULL";
        password    = "NULL";
        isLoggedIn  = false;
    }

    // Parameterized constructor
    Person(int id, string fn, string ln, string email, string password) {
        this->id        = id;
        this->firstName = fn;
        this->lastName  = ln;
        this->email     = email;
        this->password  = password;
        this->isLoggedIn = false;
    }

    // Virtual destructor (needed for polymorphism)
    virtual ~Person() {
        cout << "Personne " << getFullName() << " destroyed.\n";
    }

    // Getters
    int    getId()          const { return id; }
    string getFirstName()   const { return firstName; }
    string getLastName()    const { return lastName; }
    string getFullName()    const { return firstName + " " + lastName; }
    string getEmail()       const { return email; }
    bool   getLoginStatus() const { return isLoggedIn; }

    // Setters
    void setFirstName(string fn) { firstName = fn; }
    void setLastName(string ln)  { lastName  = ln; }

    void setEmail(string e) {
        if (e.find('@') != string::npos) email = e;
        else cout << "Email invalide : doit contenir '@'\n";
        // ✅ FIX : removed throw, replaced with simple message
    }

    // Pure virtual = ABSTRACTION
    virtual void displayRole() = 0;
    virtual void dashboard()   = 0;

    // Login
    bool login(string em, string pwd) {
        if (em == email && pwd == password) {
            isLoggedIn = true;
            cout << "Bienvenue, " << getFullName() << "!\n";
            return true;
        }
        cout << "Email ou mot de passe incorrect !\n";
        return false;
    }

    // Logout
    void logout() {
        isLoggedIn = false;
        cout << "\nDéconnecté. À bientôt, " << firstName << "!\n";
    }

    // Sign up
    void signUp(string fn, string ln, string em, string pwd) {
        firstName = fn;
        lastName  = ln;
        email     = em;
        password  = pwd;
        cout << "Compte créé pour " << getFullName() << "!\n";
    }

    // Friend function (FRIEND FUNCTION)
    friend ostream& operator<<(ostream& out, const Person& p) {
        out << "Personne #" << p.id
            << " | "      << p.getFullName()
            << " | "      << p.email
            << " | "      << (p.isLoggedIn ? "En ligne" : "Hors ligne");
        return out;
    }
};

// ==================== CLIENT (Buyer) ====================

class Client : public Person {
private:
    string       address;
    string       phone;
    vector<int>  cart;
    vector<int>  quantities;
    string       promoCode;
    float        promoDiscount;

public:
    // Default constructor
    Client() : Person() {
        address       = "NULL";
        phone         = "0";
        promoDiscount = 0;
    }

    // Parameterized constructor
    // ✅ FIX : added password parameter + passed it to Person
    Client(int id, string fn, string ln, string email, string pwd)
        : Person(id, fn, ln, email, pwd) {
        this->address       = "NULL";
        this->phone         = "0";
        this->promoDiscount = 0;
        // address and phone not asked at signup — asked at payment ✅
    }

    // Destructor
    ~Client() {
        cout << "Client " << getFullName() << " destroyed.\n";
    }

    // Getters
    string getAddress() const { return address; }
    string getPhone()   const { return phone; }

    // Setters
    void setAddress(string a) { address = a; }

    // ✅ FIX : removed the semicolon after if condition
    void setPhone(string p) {
        if (p.length() == 10 && (p.substr(0, 2) == "06" || p.substr(0, 2) == "07"))
            phone = p;
        else
            cout << "Le téléphone doit commencer par 06 ou 07 et avoir 10 chiffres\n";
    }

    // POLYMORPHISM : override pure virtual methods
    void displayRole() override {
        cout << "Rôle : CLIENT (Acheteur)\n";
    }

    void dashboard() override {
        cout << "\n=== TABLEAU DE BORD CLIENT ===\n";
        cout << "Bienvenue, " << getFullName() << "!\n";
        cout << "1. Ajouter des produits au panier\n";
        cout << "2. Voir le panier\n";
        cout << "3. Appliquer un code promo\n";
        cout << "4. Supprimer un article du panier\n";
        cout << "5. Payer\n";
        cout << "6. Se déconnecter\n";
        cout << "==============================\n";
    }

    // Add to cart
    void addToCart(int productId, int qty) {
        if (!isLoggedIn) { cout << "Veuillez vous connecter d'abord !\n"; return; }
        for (size_t i = 0; i < cart.size(); i++) {
            if (cart[i] == productId) {
                quantities[i] += qty;
                cout << "Quantité mise à jour : " << quantities[i]
                     << " pour le produit #" << productId << "\n";
                return;
            }
        }
        cart.push_back(productId);
        quantities.push_back(qty);
        cout << "Ajouté " << qty << "x produit #" << productId << " au panier.\n";
    }

    // Remove from cart
    void removeFromCart(int productId) {
        for (size_t i = 0; i < cart.size(); i++) {
            if (cart[i] == productId) {
                cart.erase(cart.begin() + i);
                quantities.erase(quantities.begin() + i);
                cout << "Produit #" << productId << " retiré du panier.\n";
                return;
            }
        }
        cout << "Produit #" << productId << " introuvable dans le panier !\n";
    }

    // Apply promo code
    void insererCodePromo(string code, float discountPercent) {
        promoCode     = code;
        promoDiscount = discountPercent;
        cout << "Promo '" << code << "' appliquée : " << discountPercent << "% de réduction !\n";
    }

    // Display cart
    void displayCart() {
        cout << "\n--- VOTRE PANIER ---\n";
        if (cart.empty()) { cout << "Le panier est vide !\n\n"; return; }
        for (size_t i = 0; i < cart.size(); i++) {
            cout << "Produit #" << cart[i]
                 << " | Qty: " << quantities[i] << "\n";
        }
        cout << "-----------------\n\n";
    }

    // ✅ NEW : Pay — asks address, phone THEN payment method
    void pay(float amount) {
        if (!isLoggedIn) { cout << "Veuillez vous connecter d'abord !\n"; return; }
        if (cart.empty()) { cout << "Le panier est vide !\n"; return; }

        // Step 1 : calculate total with promo
        float discount = amount * (promoDiscount / 100.0f);
        float total    = amount - discount;

        cout << "\n--- RÉSUMÉ DE LA COMMANDE ---\n";
        displayCart();
        cout << "Sous-total : " << amount << " MAD\n";
        if (promoDiscount > 0)
            cout << "Réduction : -" << discount << " MAD (" << promoCode << ")\n";
        cout << "TOTAL    : " << total << " MAD\n";
        cout << "-----------------------------\n";

        // Step 2 : ask delivery address and phone
        cout << "\n--- INFORMATIONS DE LIVRAISON ---\n";
        cout << "Entrez votre adresse : ";
        cin.ignore();
        getline(cin, address);

        cout << "Entrez votre numéro de téléphone : ";
        string ph;
        getline(cin, ph);
        setPhone(ph);

        // Step 3 : choose payment method
        cout << "\n--- MODE DE PAIEMENT ---\n";
        cout << "1. Carte Bancaire\n";
        cout << "2. PayPal\n";
        cout << "Choix : ";
        int payChoice;
        cin >> payChoice;

        if (payChoice == 1) {
            // Bank card details
            string cardNumber, cardHolder, expiry, cvv;
            cout << "\n  Numéro de carte (16 chiffres) : ";
            cin.ignore();
            getline(cin, cardNumber);
            cout << "  Nom du titulaire               : ";
            getline(cin, cardHolder);
            cout << "  Date d'expiration (MM/AA)      : ";
            getline(cin, expiry);
            cout << "  CVV                            : ";
            getline(cin, cvv);
            cout << "\n  Traitement du paiement...\n";
            cout << "  Paiement par carte bancaire confirmé.\n";
        }
        else if (payChoice == 2) {
            // PayPal details
            string paypalEmail, paypalPwd;
            cout << "\n  PayPal Email    : ";
            cin.ignore();
            getline(cin, paypalEmail);
            cout << "  Mot de passe PayPal : ";
            getline(cin, paypalPwd);
            cout << "\n  Traitement du paiement...\n";
            cout << "  Paiement via PayPal confirmé.\n";
        }
        else {
            cout << "Choix invalide. Commande annulée.\n";
            return;
        }

        // Step 4 : success message
        cout << "\n========================================\n";
        cout << "      MERCI POUR VOTRE COMMANDE !\n";
        cout << "==============================================\n";
        cout << "  Nom      : " << getFullName() << "\n";
        cout << "  Email    : " << email << "\n";
        cout << "  Téléphone : " << phone << "\n";
        cout << "  Adresse  : " << address << "\n";
        cout << "  Total    : " << total << " MAD\n";
        cout << "  Confirmation envoyée à " << email << "\n";
        cout << "==============================================\n\n";

        // Step 5 : clear cart + reset promo → back to dashboard (still logged in ✅)
        cart.clear();
        quantities.clear();
        promoCode     = "";
        promoDiscount = 0;
        cout << "Redirection vers la page principale...\n";
    }

    // Cancel order
    void annulerCommande() {
        if (cart.empty()) { cout << "Aucune commande à annuler !\n"; return; }
        cart.clear();
        quantities.clear();
        promoCode     = "";
        promoDiscount = 0;
        cout << "Commande annulée. Le panier est maintenant vide.\n";
    }
};

// ==================== SELLER ====================

class Seller : public Person {
private:
    string      shopName;
    vector<int> myProducts;
    float       revenue;

public:
    // Default constructor
    Seller() : Person() {
        shopName = "NULL";
        revenue  = 0;
    }

    // Parameterized constructor
    // ✅ FIX : removed phone — Person doesn't have phone
    Seller(int id, string fn, string ln, string email, string pwd, string shopName)
        : Person(id, fn, ln, email, pwd) {
        this->shopName = shopName;
        this->revenue  = 0;
    }

    // Destructor
    ~Seller() {
        cout << "Vendeur " << getFullName() << " destroyed.\n";
    }

    // Getters / Setters
    string getShopName() const { return shopName; }
    float  getRevenue()  const { return revenue; }
    void   setShopName(string s) { shopName = s; }

    // POLYMORPHISM : override
    void displayRole() override {
        cout << "Rôle : VENDEUR\n";
    }

    void dashboard() override {
        cout << "\n=== TABLEAU DE BORD VENDEUR ===\n";
        cout << "Bienvenue, " << getFullName() << "!\n";
        cout << "Boutique : " << shopName << "\n";
        cout << "1. Ajouter un produit\n";
        cout << "2. Voir mes produits\n";
        cout << "3. Chiffre d'affaires : " << revenue << " MAD\n";
        cout << "4. Se déconnecter\n";
        cout << "==============================\n";
    }

    // Vendeur methods
    void addProduct(int productId) {
        if (!isLoggedIn) { cout << "Veuillez vous connecter d'abord !\n"; return; }
        myProducts.push_back(productId);
        cout << "Produit #" << productId << " ajouté à votre boutique.\n";
    }

    void viewProducts() {
        cout << "\n--- VOS PRODUITS ---\n";
        if (myProducts.empty()) { cout << "Aucun produit pour l'instant !\n"; return; }
        for (int pid : myProducts) cout << "Produit #" << pid << "\n";
        cout << "-----------------------------\n";
    }

    void makeSale(float amount) {
        revenue += amount;
        cout << "Vente enregistrée ! Chiffre d'affaires : " << revenue << " MAD\n";
    }
};

// ==================== HELPER FUNCTIONS ====================

Person* signUpUser() {
    cout << "\n========================================\n";
    cout << "         CRÉER VOTRE COMPTE\n";
    cout << "==============================================\n";

    int roleChoice;
    cout << "Choisissez votre rôle :\n";
    cout << "1. Client (Acheteur)\n";
    cout << "2. Vendeur\n";
    cout << "Choix : ";
    cin >> roleChoice;

    string fn, ln, em, pwd;
    cout << "\nPrénom     : "; cin >> fn;
    cout << "Nom        : "; cin >> ln;
    cout << "Email      : "; cin >> em;
    cout << "Mot de passe : "; cin >> pwd;

    static int nextId = 100;

    if (roleChoice == 1) {
        Client* client = new Client(nextId++, fn, ln, em, pwd);
        client->signUp(fn, ln, em, pwd);   // prototype — kept as requested
        cout << "\nCompte client créé avec succès !\n";
        return client;
    }
    else if (roleChoice == 2) {
        string shop;
        cout << "Nom de la boutique : ";
        cin.ignore();
        getline(cin, shop);
        Seller* seller = new Seller(nextId++, fn, ln, em, pwd, shop);
        seller->signUp(fn, ln, em, pwd);   // prototype — kept as requested
        cout << "\nCompte vendeur créé avec succès !\n";
        return seller;
    }
    else {
        cout << "Choix de rôle invalide !\n";
        return nullptr;
    }
}

// ==================== MAIN ====================

int main() {
    cout << "==============================================\n";
    cout << "       BIENVENUE SUR NOTRE E-SHOP\n";
    cout << "==============================================\n\n";

    Person* user = nullptr;

    // Step 1 : sign up
    user = signUpUser();
    if (!user) { cout << "Au revoir !\n"; return 0; }

    // ✅ FIX : login step added — user was never logged in before
    cout << "\n--- Veuillez vous connecter ---\n";
    string em, pwd;
    cout << "Email        : "; cin >> em;
    cout << "Mot de passe : "; cin >> pwd;

    if (!user->login(em, pwd)) {
        cout << "Connexion échouée. Au revoir !\n";
        delete user;
        return 0;
    }

    // Step 2 : show info + role
    cout << "\n" << *user << "\n";  // FRIEND FUNCTION : operator<<
    user->displayRole();             // POLYMORPHISM

    // Step 3 : main loop — stays until logout (user stays logged in)
    bool running = true;
    while (running) {

        user->dashboard();   // POLYMORPHISM : different per role

        if (Client* client = dynamic_cast<Client*>(user)) {
            int choice;
            cout << "Votre choix : ";
            cin >> choice;

            switch (choice) {
                case 1:
                    client->addToCart(101, 2);
                    client->addToCart(102, 1);
                    break;
                case 2:
                    client->displayCart();
                    break;
                case 3: {
                    string code; float disc;
                    cout << "Code promo     : "; cin >> code;
                    cout << "Réduction (%%) : "; cin >> disc;
                    client->insererCodePromo(code, disc);
                    break;
                }
                case 4: {
                    int pid;
                    cout << "ID du produit à supprimer : "; cin >> pid;
                    client->removeFromCart(pid);
                    break;
                }
                case 5:
                    // asks address + phone + payment → success → back here
                    client->pay(150.0f);
                    break;
                case 6:
                    user->logout();
                    running = false;
                    break;
                default:
                    cout << "Choix invalide !\n";
            }
        }
        else if (Seller* seller = dynamic_cast<Seller*>(user)) {
            int choice;
            cout << "Votre choix : ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    int pid;
                    cout << "ID du produit : "; cin >> pid;
                    seller->addProduct(pid);
                    break;
                }
                case 2: seller->viewProducts(); break;
                case 3: cout << "Chiffre d'affaires : " << seller->getRevenue() << " MAD\n"; break;
                case 4:
                    user->logout();
                    running = false;
                    break;
                default:
                    cout << "Choix invalide !\n";
            }
        }
    }

    delete user;

    cout << "\n========================================\n";
    cout << "              AU REVOIR !\n";
    cout << "==============================================\n";

    return 0;
}