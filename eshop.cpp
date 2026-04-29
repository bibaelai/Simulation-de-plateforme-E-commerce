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
    string phone;
    string password;
    bool isLoggedIn;

public:
    // Constructor
    Person() {
        id = 0;
        firstName = "NULL";
        lastName = "NULL";
        email = "NULL";
        phone = "0";
        password = "NULL";
        isLoggedIn = false;
    }

    // Parameterized constructor
    Person(int id, string fn, string ln, string email, string phone) {
        this->id = id;
        this->firstName = fn;
        this->lastName = ln;
        this->email = email;
        this->phone = phone;
        this->password = "1234";
        this->isLoggedIn = false;
    }

    // Destructor (virtual for polymorphism)
    virtual ~Person() {
        cout << "Person " << getFullName() << " destroyed.\n";
    }

    // Getters
    int getId() const { return id; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getFullName() const { return firstName + " " + lastName; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    bool getLoginStatus() const { return isLoggedIn; }

    // Setters with validation
    void setFirstName(string fn) { firstName = fn; }
    void setLastName(string ln) { lastName = ln; }
    
    void setEmail(string e) {
        if (e.find('@') != string::npos) email = e;
        else throw invalid_argument("Invalid email");
    }
    
    void setPhone(string p) {
        if (p.length() == 10 && p.substr(0, 2) == "06")
            phone = p;
        else
            throw invalid_argument("Phone must start with 06 and be 10 digits");
    }

    // Pure virtual = ABSTRACTION (must override in child classes)
    virtual void displayRole() = 0;
    virtual void dashboard() = 0;

    // Common methods
    bool login(string em, string pwd) {
        if (em == email && pwd == password) {
            isLoggedIn = true;
            cout << "Welcome back, " << getFullName() << "!\n";
            return true;
        }
        cout << "Wrong email or password!\n";
        return false;
    }

    void logout() {
        isLoggedIn = false;
        cout << "Logged out. Goodbye, " << firstName << "!\n";
    }

    // Sign up
    void signUp(string fn, string ln, string em, string ph, string pwd) {
        firstName = fn;
        lastName = ln;
        email = em;
        setPhone(ph);
        password = pwd;
        cout << "Account created for " << getFullName() << "!\n";
    }

    // Friend function
    friend ostream& operator<<(ostream& out, const Person& p) {
        out << "Person #" << p.id << " | " << p.getFullName() 
            << " | " << p.email << " | " << p.phone;
        return out;
    }
};

// ==================== CLIENT (Buyer) ====================

class Client : public Person {
private:
    string address;
    vector<int> cart;
    vector<int> quantities;
    string promoCode;
    float promoDiscount;

public:
    // Constructors
    Client() : Person() {
        address = "NULL";
        promoCode = "";
        promoDiscount = 0;
    }

    Client(int id, string fn, string ln, string email, string phone, string address) 
        : Person(id, fn, ln, email, phone) {
        this->address = address;
        this->promoCode = "";
        this->promoDiscount = 0;
    }

    // Destructor
    ~Client() {
        cout << "Client " << getFullName() << " destroyed.\n";
    }

    // Getters
    string getAddress() const { return address; }
    void setAddress(string a) { address = a; }

    // Override pure virtual (POLYMORPHISM)
    void displayRole() override {
        cout << "Role: CLIENT (Buyer)\n";
    }

    void dashboard() override {
        cout << "\n=== CLIENT DASHBOARD ===\n";
        cout << "Welcome, " << getFullName() << "!\n";
        cout << "1. Browse products\n";
        cout << "2. Search products\n";
        cout << "3. View cart\n";
        cout << "4. My orders\n";
        cout << "5. Logout\n";
        cout << "========================\n";
    }

    // Cart methods
    void addToCart(int productId, int qty) {
        if (!isLoggedIn) {
            cout << "Please login first!\n";
            return;
        }
        for (size_t i = 0; i < cart.size(); i++) {
            if (cart[i] == productId) {
                quantities[i] += qty;
                cout << "Updated quantity to " << quantities[i] << " for product #" << productId << "\n";
                return;
            }
        }
        cart.push_back(productId);
        quantities.push_back(qty);
        cout << "Added " << qty << "x product #" << productId << " to cart.\n";
    }

    void removeFromCart(int productId) {
        for (size_t i = 0; i < cart.size(); i++) {
            if (cart[i] == productId) {
                cart.erase(cart.begin() + i);
                quantities.erase(quantities.begin() + i);
                cout << "Product #" << productId << " removed from cart.\n";
                return;
            }
        }
        cout << "Product #" << productId << " not found in cart!\n";
    }

    void displayCart() {
        cout << "\n--- YOUR CART ---\n";
        if (cart.empty()) {
            cout << "Cart is empty!\n";
            return;
        }
        for (size_t i = 0; i < cart.size(); i++) {
            cout << "Product #" << cart[i] << " | Qty: " << quantities[i] << "\n";
        }
        cout << "-----------------\n";
    }

    void applyPromoCode(string code, float discountPercent) {
        promoCode = code;
        promoDiscount = discountPercent;
        cout << "Promo '" << code << "' applied: " << discountPercent << "% off!\n";
    }

    void pay(float amount) {
        if (!isLoggedIn) {
            cout << "Please login first!\n";
            return;
        }
        if (cart.empty()) {
            cout << "Cart is empty!\n";
            return;
        }
        float discount = amount * (promoDiscount / 100);
        float total = amount - discount;
        cout << "Payment of $" << total << " processed!\n";
        cout << "Shipping to: " << address << "\n";
        cart.clear();
        quantities.clear();
        promoCode = "";
        promoDiscount = 0;
    }

    void cancelOrder() {
        if (cart.empty()) {
            cout << "No order to cancel!\n";
            return;
        }
        cart.clear();
        quantities.clear();
        promoCode = "";
        promoDiscount = 0;
        cout << "Order cancelled. Cart emptied.\n";
    }

    void askAI(string question) {
        cout << "\n[AI Assistant]\n";
        cout << "Q: \"" << question << "\"\n";
        cout << "A: Check our promotions in Sports & Fitness!\n\n";
    }
};

// ==================== SELLER ====================

class Seller : public Person {
private:
    string shopName;
    vector<int> myProducts;
    float revenue;

public:
    // Constructors
    Seller() : Person() {
        shopName = "NULL";
        revenue = 0;
    }

    Seller(int id, string fn, string ln, string email, string phone, string shopName) 
        : Person(id, fn, ln, email, phone) {
        this->shopName = shopName;
        this->revenue = 0;
    }

    // Destructor
    ~Seller() {
        cout << "Seller " << getFullName() << " destroyed.\n";
    }

    // Getters
    string getShopName() const { return shopName; }
    float getRevenue() const { return revenue; }
    void setShopName(string s) { shopName = s; }

    // Override pure virtual (POLYMORPHISM)
    void displayRole() override {
        cout << "Role: SELLER\n";
    }

    void dashboard() override {
        cout << "\n=== SELLER DASHBOARD ===\n";
        cout << "Welcome, " << getFullName() << "!\n";
        cout << "Shop: " << shopName << "\n";
        cout << "1. Add product\n";
        cout << "2. My products\n";
        cout << "3. View sales\n";
        cout << "4. Revenue: $" << revenue << "\n";
        cout << "5. Logout\n";
        cout << "========================\n";
    }

    // Seller methods
    void addProduct(int productId) {
        if (!isLoggedIn) {
            cout << "Please login first!\n";
            return;
        }
        myProducts.push_back(productId);
        cout << "Product #" << productId << " added to your shop.\n";
    }

    void viewProducts() {
        cout << "\n--- YOUR PRODUCTS ---\n";
        if (myProducts.empty()) {
            cout << "No products yet!\n";
            return;
        }
        for (int pid : myProducts) {
            cout << "Product #" << pid << "\n";
        }
    }

    void makeSale(float amount) {
        revenue += amount;
        cout << "Sale made! Revenue now: $" << revenue << "\n";
    }
};

// ==================== MAIN ====================

int main() {
    cout << "========================================\n";
    cout << "     E-COMMERCE PLATFORM\n";
    cout << "========================================\n\n";

    int choice;
    Person* user = nullptr;  // POLYMORPHISM: can point to Client OR Seller

    cout << "Are you:\n";
    cout << "1. Client (Buyer)\n";
    cout << "2. Seller\n";
    cout << "Choice: ";
    cin >> choice;

    if (choice == 1) {
        user = new Client(1, "Alice", "Smith", "alice@email.com", "0612345678", "123 Main St");
    } else if (choice == 2) {
        user = new Seller(2, "Bob", "Jones", "bob@email.com", "0698765432", "Bob's Electronics");
    } else {
        cout << "Invalid choice!\n";
        return 1;
    }

    // POLYMORPHISM: same call, different behavior
    cout << "\n" << *user << "\n";
    user->displayRole();
    user->dashboard();

    // Login
    cout << "\n--- LOGIN ---\n";
    string em, pwd;
    cout << "Email: ";
    cin >> em;
    cout << "Password: ";
    cin >> pwd;

    if (user->login(em, pwd)) {
        // POLYMORPHISM: dashboard shows different menu based on type
        user->dashboard();

        // Dynamic cast to access specific methods
        if (Client* client = dynamic_cast<Client*>(user)) {
            cout << "\n--- CLIENT ACTIONS ---\n";
            client->addToCart(101, 2);
            client->addToCart(102, 1);
            client->displayCart();
            client->applyPromoCode("SALE20", 20);
            client->pay(150.0);
        }
        else if (Seller* seller = dynamic_cast<Seller*>(user)) {
            cout << "\n--- SELLER ACTIONS ---\n";
            seller->addProduct(201);
            seller->addProduct(202);
            seller->viewProducts();
            seller->makeSale(99.99);
        }
    }

    // Logout and cleanup
    user->logout();
    delete user;

    cout << "\n========================================\n";
    cout << "     GOODBYE!\n";
    cout << "========================================\n";

    return 0;
} 