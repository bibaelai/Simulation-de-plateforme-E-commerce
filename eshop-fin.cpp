
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <mysql/mysql.h>
#include "httplib.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;


// On declare les classes
class Produit;
class Client;
class Paiement;
class Commande;

// Connection avec notre database (sql)
class Database {
private:
    MYSQL* conn;
    mutable std::mutex db_mutex; 

public:
    Database(const string& host   = "localhost",
         const string& user   = "root",
         const string& pwd    = "",
         const string& dbname = "eShop_db") {
    conn = mysql_init(nullptr);
    if (!conn)
        throw runtime_error("mysql_init a échoué");
    
    // Enable automatic reconnection
    bool reconnect = 1;
    mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect);
    
    if (!mysql_real_connect(conn,
                host.c_str(), user.c_str(),
                pwd.c_str(),  dbname.c_str(),
                3306, nullptr, 0))
            throw runtime_error(mysql_error(conn));
        mysql_set_character_set(conn, "utf8mb4");
    }

    ~Database() { mysql_close(conn); }

    // Requêtes de lecture 

    // Retourne TOUTES les lignes avec vector<vector<string>>
    vector<vector<string>> fetchAll(const string& sql) {
        if (mysql_query(conn, sql.c_str()))
            throw runtime_error(string("fetchAll: ") + mysql_error(conn));

        MYSQL_RES* res = mysql_store_result(conn);
        if (!res) return {};

        int cols = (int)mysql_num_fields(res);
        vector<vector<string>> rows;
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            vector<string> r;
            r.reserve(cols);
            for (int i = 0; i < cols; ++i)
                r.push_back(row[i] ? row[i] : "NULL");
            rows.push_back(r);
        }
        mysql_free_result(res);
        return rows;
    }

    vector<string> fetchOne(const string& sql) {
        auto rows = fetchAll(sql);
        return rows.empty() ? vector<string>{} : rows[0];
    }

    // Retourne un map<colonne, valeur> pour la première ligne
    map<string,string> fetchOneMap(const string& sql) {
        if (mysql_query(conn, sql.c_str()))
            throw runtime_error(string("fetchOneMap: ") + mysql_error(conn));

        MYSQL_RES* res = mysql_store_result(conn);
        if (!res) return {};

        MYSQL_FIELD* fields = mysql_fetch_fields(res);
        int cols = (int)mysql_num_fields(res);

        map<string,string> result;
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row)
            for (int i = 0; i < cols; ++i)
                result[fields[i].name] = row[i] ? row[i] : "";

        mysql_free_result(res);
        return result;
    }

    // ── Requêtes de modification ─────────────────────────────

    void execute(const string& sql) {
        if (mysql_query(conn, sql.c_str()))
            throw runtime_error(string("execute: ") + mysql_error(conn));
    }

    int lastInsertId() { return (int)mysql_insert_id(conn); }

    // safe et escape will prevent our project from the sql injections 
    string safe(const string& val) {
        char buf[val.size() * 2 + 1];
        mysql_real_escape_string(conn, buf, val.c_str(), val.size());
        return "'" + string(buf) + "'";
    }
    string escape(const string& val) {
        char buf[val.size() * 2 + 1];
        mysql_real_escape_string(conn, buf, val.c_str(), val.size());
        return string(buf);
    }
};


// declaration des listes (enums)

enum class Taille  { XS, S, M, L, XL, XXL };
enum class Couleur { ROUGE, BLEU, VERT, NOIR, BLANC, GRIS,
                     JAUNE, ROSE, ORANGE, VIOLET, MARRON, BEIGE };
enum class Statut  { DISPONIBLE, RUPTURE_STOCK, ARCHIVE };
enum class Categorie {
    TOUS, SPORT_FITNESS, ELECTRONIQUE, VETEMENT,
    ALIMENTATION, LIVRE, MAISON, TICKETS, PROMOTION
};

string tailleToString(Taille t) {
    switch(t){
        case Taille::XS: return "XS"; case Taille::S: return "S";
        case Taille::M:  return "M";  case Taille::L: return "L";
        case Taille::XL: return "XL"; case Taille::XXL: return "XXL";
        default: return "Inconnue";
    }
}
Taille stringToTaille(const string& s){
    if(s=="XS") return Taille::XS; if(s=="S")  return Taille::S;
    if(s=="M")  return Taille::M;  if(s=="L")  return Taille::L;
    if(s=="XL") return Taille::XL; if(s=="XXL") return Taille::XXL;
    throw invalid_argument("Taille invalide: "+s);
}

string couleurToString(Couleur c){
    switch(c){
        case Couleur::ROUGE:  return "Rouge";  case Couleur::BLEU:   return "Bleu";
        case Couleur::VERT:   return "Vert";   case Couleur::NOIR:   return "Noir";
        case Couleur::BLANC:  return "Blanc";  case Couleur::GRIS:   return "Gris";
        case Couleur::JAUNE:  return "Jaune";  case Couleur::ROSE:   return "Rose";
        case Couleur::ORANGE: return "Orange"; case Couleur::VIOLET: return "Violet";
        case Couleur::MARRON: return "Marron"; case Couleur::BEIGE:  return "Beige";
        default: return "Inconnue";
    }
}
Couleur stringToCouleur(const string& s){
    if(s=="Rouge")  return Couleur::ROUGE;  if(s=="Bleu")   return Couleur::BLEU;
    if(s=="Vert")   return Couleur::VERT;   if(s=="Noir")   return Couleur::NOIR;
    if(s=="Blanc")  return Couleur::BLANC;  if(s=="Gris")   return Couleur::GRIS;
    if(s=="Jaune")  return Couleur::JAUNE;  if(s=="Rose")   return Couleur::ROSE;
    if(s=="Orange") return Couleur::ORANGE; if(s=="Violet") return Couleur::VIOLET;
    if(s=="Marron") return Couleur::MARRON; if(s=="Beige")  return Couleur::BEIGE;
    throw invalid_argument("Couleur invalide: "+s);
}

string statutToString(Statut s){
    switch(s){
        case Statut::DISPONIBLE:    return "Disponible";
        case Statut::RUPTURE_STOCK: return "Rupture de stock";
        case Statut::ARCHIVE:       return "Archivé";
        default: return "Inconnu";
    }
}
Statut stringToStatut(const string& s){
    if(s=="Disponible")       return Statut::DISPONIBLE;
    if(s=="Rupture de stock") return Statut::RUPTURE_STOCK;
    if(s=="Archivé")          return Statut::ARCHIVE;
    return Statut::DISPONIBLE;
}

string categorieToString(Categorie c){
    switch(c){
        case Categorie::TOUS:          return "Tous";
        case Categorie::SPORT_FITNESS: return "Sport & Fitness";
        case Categorie::ELECTRONIQUE:  return "Électronique";
        case Categorie::VETEMENT:      return "Vêtement";
        case Categorie::ALIMENTATION:  return "Alimentation";
        case Categorie::LIVRE:         return "Livre";
        case Categorie::MAISON:        return "Maison";
        case Categorie::TICKETS:       return "Tickets";
        case Categorie::PROMOTION:     return "Promotion";
        default: return "Inconnu";
    }
}

struct Commentaire {
    string auteur, texte;
    int    etoiles;
    Commentaire(string a,string t,int e):auteur(a),texte(t),etoiles(e){}
    void afficher() const {
        cout << "  ★ " << etoiles << "/5  — " << auteur
             << "\n  \"" << texte << "\"\n";
    }
};


// class Produit (MOURTAFIAA)
//  Produit est une classe abstraite (classe parent)
class Produit {
protected:
    int    id;
    string nom;
    double prix;
    string description;
    int    stock;
    double note;
    string categorie;
    Statut statut;
    vector<Commentaire> commentaires;
    bool   enPromotion;
    double prixOriginal;

public:
    Produit(int id,const string& nom,double prix,
            int stock,const string& categorie,double note=0.0)
        : id(id),nom(nom),prix(prix),stock(stock),
          categorie(categorie),note(note),
          enPromotion(false),prixOriginal(prix),
          statut(stock>0 ? Statut::DISPONIBLE : Statut::RUPTURE_STOCK)
    {
        if(prix<0)          throw invalid_argument("Prix négatif interdit");
        if(stock<0)         throw invalid_argument("Stock négatif interdit");
        if(note<0||note>5)  throw invalid_argument("Note hors [0,5]");
    }

    virtual ~Produit(){} // //destructeur du class Produit

    // Getters :
    int    getId()           const { return id; }
    string getNom()          const { return nom; }
    double getPrix()         const { return prix; }
    int    getStock()        const { return stock; }
    double getNote()         const { return note; }
    string getDescription()  const { return description; }
    string getCategorie()    const { return categorie; }
    Statut getStatut()       const { return statut; }
    bool   isEnPromotion()   const { return enPromotion; }
    double getPrixOriginal() const { return prixOriginal; }
    const  vector<Commentaire>& getCommentaires() const { return commentaires; }

    // Setters :
    void setPrix(double p){
        if(p<0) throw invalid_argument("Prix négatif interdit");
        prix=p;
    }
    void setStock(int s){
        if(s<0) throw invalid_argument("Stock négatif interdit");
        stock=s;
        if(stock==0 && statut==Statut::DISPONIBLE) statut=Statut::RUPTURE_STOCK;
        else if(stock>0 && statut==Statut::RUPTURE_STOCK) statut=Statut::DISPONIBLE;
    }
    void setDescription(const string& d){ description=d; }
    void setNote(double n){
        if(n<0||n>5) throw invalid_argument("Note hors [0,5]");
        note=n;
    }
    void setEnPromotion(bool b){ enPromotion=b; }
    void setPrixOriginal(double p){ prixOriginal=p; }
    void setStatut(Statut s){ statut=s; }

    void archiver(){ statut=Statut::ARCHIVE; stock=0; }

    void activerPromotion(double pct){
        if(pct<=0||pct>=100) throw invalid_argument("Pourcentage invalide");
        prixOriginal=prix;
        prix*=(1.0-pct/100.0);
        enPromotion=true;
    }
    void desactiverPromotion(){ prix=prixOriginal; enPromotion=false; }

    void ajouterCommentaire(const string& auteur,const string& texte,int etoiles){
        if(etoiles<1||etoiles>5) throw invalid_argument("Étoiles entre 1 et 5");
        commentaires.emplace_back(auteur,texte,etoiles);
        double total=0;
        for(const auto& c:commentaires) total+=c.etoiles;
        note=total/commentaires.size();
    }

    // sauvegarde des commentaires dans la DB 
    void sauvegarderCommentaireDansDB(Database& db,
                                      const string& auteur,
                                      const string& texte,
                                      int etoiles) {
        db.execute(
            "INSERT INTO commentaires (id_produit, auteur, texte, etoiles) VALUES ("
            + to_string(id) + ", "
            + db.safe(auteur) + ", "
            + db.safe(texte) + ", "
            + to_string(etoiles) + ")"
        );
        db.execute(
            "UPDATE produits SET note = "
            "(SELECT AVG(etoiles) FROM commentaires WHERE id_produit = " + to_string(id) + ") "
            "WHERE id = " + to_string(id)
        );
        ajouterCommentaire(auteur, texte, etoiles);
    }

    // ── Synchronise le stock dans la DB ─────────────────────
    void mettreAJourStockDB(Database& db) {
        db.execute(
            "UPDATE produits SET stock = " + to_string(stock)
            + ", statut = " + db.safe(statutToString(statut))
            + " WHERE id = " + to_string(id)
        );
    }

    bool estDisponible() const {
        return statut==Statut::DISPONIBLE && stock>0;
    }

    bool acheter(int quantite=1){
        if(quantite<=0) throw invalid_argument("Quantité doit être > 0");
        if(statut==Statut::ARCHIVE) throw runtime_error("Produit archivé");
        if(stock<quantite) return false;
        stock-=quantite;
        if(stock==0) statut=Statut::RUPTURE_STOCK;
        return true;
    }

    void reapprovisionner(int quantite){
        if(quantite<=0) throw invalid_argument("Quantité > 0 requise");
        stock+=quantite;
        if(statut==Statut::RUPTURE_STOCK) statut=Statut::DISPONIBLE;
    }

    void afficherCourt() const {
        cout << fixed << setprecision(2)
             << "[" << id << "] " << nom
             << " | " << prix << " MAD"
             << " | Stock: " << stock
             << " | ★ " << note
             << " | " << statutToString(statut);
        if(enPromotion) cout << " PROMO";
        cout << "\n";
    }

    void afficher() const {
        cout << fixed << setprecision(2)
             << "┌────────────────────────────────────\n"
             << "│ [" << id << "] " << nom << "  (" << categorie << ")\n"
             << "├────────────────────────────────────\n";
        if(enPromotion)
            cout << "│ Prix    : " << prix << " MAD (était " << prixOriginal << " MAD) PROMO\n";
        else
            cout << "│ Prix    : " << prix << " MAD\n";
        cout << "│ Stock   : " << stock << " unité(s)\n"
             << "│ Note    : " << note << " / 5.0\n"
             << "│ Statut  : " << statutToString(statut) << "\n"
             << "├────────────────────────────────────\n";
        afficherDetails();
        cout << "└────────────────────────────────────\n";
    }

    string toString() const {
        return "["+to_string(id)+"] "+nom+" — "+to_string((int)prix)+" MAD";
    }

    virtual double    appliquerRemise(double pct) = 0;
    virtual void      afficherDetails()     const = 0;
    virtual Categorie getCategorieEnum()    const = 0;

    bool operator==(const Produit& a) const { return id==a.id; }
    bool operator<(const Produit& a)  const { return prix<a.prix; }

    friend ostream& operator<<(ostream& os,const Produit& p){
        os << fixed << setprecision(2)
           << "[" << p.id << "] " << p.nom
           << " | " << p.prix << " MAD"
           << " | Stock: " << p.stock
           << " | " << statutToString(p.statut);
        return os;
    }
};



//  SOUS-CLASSES de Produit :

class Vetement : public Produit {
protected:
    Taille  taille;
    Couleur couleur;
    string  matiere, marque;
    vector<Taille>  taillesDisponibles;
    vector<Couleur> couleursDisponibles;

public:
    Vetement(int id,const string& nom,double prix,int stock,
             Taille taille,Couleur couleur,
             const string& matiere,const string& marque)
        : Produit(id,nom,prix,stock,"Vêtement"),
          taille(taille),couleur(couleur),
          matiere(matiere),marque(marque)
    {
        taillesDisponibles  = {Taille::XS,Taille::S,Taille::M,
                               Taille::L,Taille::XL,Taille::XXL};
        couleursDisponibles = {Couleur::ROUGE,Couleur::BLEU,Couleur::VERT,
                               Couleur::NOIR,Couleur::BLANC,Couleur::GRIS,
                               Couleur::JAUNE,Couleur::ROSE,Couleur::ORANGE,
                               Couleur::VIOLET,Couleur::MARRON,Couleur::BEIGE};
    }
    virtual ~Vetement(){}

    Categorie getCategorieEnum() const override { return Categorie::VETEMENT; }
    Taille  getTaille()    const { return taille; }
    Couleur getCouleur()   const { return couleur; }
    string  getMarque()    const { return marque; }
    string  getTailleStr() const { return tailleToString(taille); }
    string  getCouleurStr()const { return couleurToString(couleur); }

    void setTaille(Taille t){ taille=t; }
    void setCouleur(Couleur c){ couleur=c; }

    bool choisirTaille(){
        cout << "\n=== Tailles disponibles pour : " << nom << " ===\n";
        for(size_t i=0;i<taillesDisponibles.size();++i)
            cout << "  [" << i+1 << "] " << tailleToString(taillesDisponibles[i]) << "\n";
        cout << "  Votre choix : ";
        int c; cin>>c;
        if(c<1||c>(int)taillesDisponibles.size()){cout<<"Choix invalide.\n";return false;}
        taille=taillesDisponibles[c-1];
        cout << "Taille : " << tailleToString(taille) << "\n";
        return true;
    }

    bool choisirCouleur(){
        cout << "\n=== Couleurs disponibles pour : " << nom << " ===\n";
        for(size_t i=0;i<couleursDisponibles.size();++i)
            cout << "  [" << i+1 << "] " << couleurToString(couleursDisponibles[i]) << "\n";
        cout << "  Votre choix : ";
        int c; cin>>c;
        if(c<1||c>(int)couleursDisponibles.size()){cout<<"Choix invalide.\n";return false;}
        couleur=couleursDisponibles[c-1];
        cout << "Couleur : " << couleurToString(couleur) << "\n";
        return true;
    }

    double appliquerRemise(double pct) override {
        if(pct<0||pct>100) throw invalid_argument("Pourcentage invalide");
        prix*=(1.0-pct/100.0);
        return prix;
    }

    void afficherDetails() const override {
        cout << "│ Taille  : " << tailleToString(taille)
             << "   Couleur : " << couleurToString(couleur) << "\n"
             << "│ Matière : " << matiere
             << "   Marque  : " << marque << "\n";
    }
};


class Electronique : public Produit {
protected:
    string marque;
    int    garantieMois;
    double puissanceWatts, tensionVolts;

public:
    Electronique(int id,const string& nom,double prix,int stock,
                 const string& marque,int garantie,
                 double puissance=0,double tension=220)
        : Produit(id,nom,prix,stock,"Électronique"),
          marque(marque),garantieMois(garantie),
          puissanceWatts(puissance),tensionVolts(tension){}

    virtual ~Electronique(){}
    Categorie getCategorieEnum() const override { return Categorie::ELECTRONIQUE; }
    string getMarque()   const { return marque; }
    int    getGarantie() const { return garantieMois; }

    double appliquerRemise(double pct) override {
        prix*=(1.0-pct/100.0); return prix;
    }
    void afficherDetails() const override {
        cout << "│  Marque   : " << marque
             << "  Garantie : " << garantieMois << " mois\n"
             << "│  Puissance: " << puissanceWatts
             << " W   Tension: " << tensionVolts << " V\n";
    }
};


class Ordinateur : public Electronique {
private:
    string cpu,gpu,typeStockage,typeOrdi;
    int    stockageGo;

public:
    Ordinateur(int id,const string& nom,double prix,int stock,
               const string& marque,int garantie,
               const string& cpu,const string& gpu,
               int stockageGo,const string& typeStockage,
               const string& typeOrdi)
        : Electronique(id,nom,prix,stock,marque,garantie),
          cpu(cpu),gpu(gpu),stockageGo(stockageGo),
          typeStockage(typeStockage),typeOrdi(typeOrdi)
    { categorie="Ordinateur"; }

    Categorie getCategorieEnum() const override { return Categorie::ELECTRONIQUE; }
    void afficherDetails() const override {
        Electronique::afficherDetails();
        cout << "│  CPU: " << cpu << "  GPU: " << gpu << "\n"
             << "│  Stockage: " << stockageGo << " Go ("
             << typeStockage << ")  Type: " << typeOrdi << "\n";
    }
};


class Alimentation : public Produit {
private:
    string dateExpiration,allergenes,paysOrigine;
    double poidsKg;
    int    caloriesPer100g;
    bool   bio;

public:
    Alimentation(int id,const string& nom,double prix,int stock,
                 const string& dateExp,double poids,int calories,
                 bool bio,const string& allergenes="",
                 const string& pays="Maroc")
        : Produit(id,nom,prix,stock,"Alimentation"),
          dateExpiration(dateExp),poidsKg(poids),
          caloriesPer100g(calories),bio(bio),
          allergenes(allergenes),paysOrigine(pays){}

    Categorie getCategorieEnum() const override { return Categorie::ALIMENTATION; }
    double appliquerRemise(double pct) override { prix*=(1.0-pct/100.0); return prix; }
    void afficherDetails() const override {
        cout << "│  Expiration : " << dateExpiration
             << "  Poids: " << poidsKg << " kg\n"
             << "│  Calories   : " << caloriesPer100g
             << "/100g  Bio: " << (bio?"Oui":"Non") << "\n"
             << "│  Allergènes : " << (allergenes.empty()?"Aucun":allergenes) << "\n"
             << "│  Origine    : " << paysOrigine << "\n";
    }
};


class Livre : public Produit {
private:
    string auteur,isbn,editeur,langue,genre;
    int    nombrePages;

public:
    Livre(int id,const string& nom,double prix,int stock,
          const string& auteur,const string& isbn,
          int pages,const string& editeur,
          const string& langue="Anglais",
          const string& genre="Roman")
        : Produit(id,nom,prix,stock,"Livre"),
          auteur(auteur),isbn(isbn),nombrePages(pages),
          editeur(editeur),langue(langue),genre(genre){}

    Categorie getCategorieEnum() const override { return Categorie::LIVRE; }
    double appliquerRemise(double pct) override { prix*=(1.0-pct/100.0); return prix; }
    void afficherDetails() const override {
        cout << "│  Auteur : " << auteur  << "  Éditeur : " << editeur << "\n"
             << "│  ISBN   : " << isbn   << "  Pages   : " << nombrePages << "\n"
             << "│  Langue : " << langue << "  Genre   : " << genre << "\n";
    }
};


class SportFitness : public Produit {
private:
    string sport,marque,materiau,niveauUtilisateur;
    double poidsKg;

public:
    SportFitness(int id,const string& nom,double prix,int stock,
                 const string& sport,const string& marque,
                 const string& materiau="",double poids=0,
                 const string& niveau="Tous niveaux")
        : Produit(id,nom,prix,stock,"Sport & Fitness"),
          sport(sport),marque(marque),materiau(materiau),
          poidsKg(poids),niveauUtilisateur(niveau){}

    Categorie getCategorieEnum() const override { return Categorie::SPORT_FITNESS; }
    double appliquerRemise(double pct) override { prix*=(1.0-pct/100.0); return prix; }
    void afficherDetails() const override {
        cout << "│  Sport    : " << sport    << "  Marque  : " << marque << "\n"
             << "│  Matériau : " << materiau << "  Poids   : " << poidsKg << " kg\n"
             << "│  Niveau   : " << niveauUtilisateur << "\n";
    }
};


class MaisonDecor : public Produit {
private:
    string piece,style,matiere,dimensions,marque;

public:
    MaisonDecor(int id,const string& nom,double prix,int stock,
                const string& piece,const string& style,
                const string& matiere="",const string& dim="",
                const string& marque="")
        : Produit(id,nom,prix,stock,"Maison"),
          piece(piece),style(style),matiere(matiere),
          dimensions(dim),marque(marque){}

    Categorie getCategorieEnum() const override { return Categorie::MAISON; }
    double appliquerRemise(double pct) override { prix*=(1.0-pct/100.0); return prix; }
    void afficherDetails() const override {
        cout << "│  Pièce      : " << piece     << "  Style : " << style << "\n"
             << "│  Matière    : " << matiere   << "\n"
             << "│  Dimensions : " << dimensions << "\n"
             << "│  Marque     : " << marque    << "\n";
    }
};


class Ticket : public Produit {
private:
    string evenement,lieu,dateEvenement,typePlace;
    int    placesRestantes;

public:
    Ticket(int id,const string& nom,double prix,int stock,
           const string& evenement,const string& lieu,
           const string& date,const string& typePlace="Standard")
        : Produit(id,nom,prix,stock,"Ticket"),
          evenement(evenement),lieu(lieu),
          dateEvenement(date),typePlace(typePlace),
          placesRestantes(stock){}

    Categorie getCategorieEnum() const override { return Categorie::TICKETS; }
    double appliquerRemise(double pct) override { prix*=(1.0-pct/100.0); return prix; }
    void afficherDetails() const override {
        cout << "│  Événement : " << evenement     << "\n"
             << "│  Lieu      : " << lieu          << "\n"
             << "│  Date      : " << dateEvenement << "\n"
             << "│  Place     : " << typePlace     << "\n"
             << "│  Restantes : " << placesRestantes << "\n";
    }
};


class Paiement {
public:
    enum class Mode { CARTE, PAYPAL };

private:
    Mode   mode;
    bool   valide;
    double montant;
    string reference;
    string numeroCarte,titulaire,dateExpiration,cvv;
    string emailPaypal;

public:
    Paiement()                   : mode(Mode::CARTE),valide(false),montant(0){}
    Paiement(Mode m,double mont) : mode(m),valide(false),montant(mont){
        reference="REF"+to_string(rand()%900000+100000);
    }

    bool payerParCarte(const string& num,const string& tit,
                       const string& exp,const string& c){
        if(num.length()!=16){ cout<<"  Numéro de carte invalide (16 chiffres).\n"; return false; }
        numeroCarte=num; titulaire=tit; dateExpiration=exp; cvv=c;
        valide=true;
        cout << "\n  Paiement par carte confirmé.\n  Référence : " << reference << "\n";
        return true;
    }

    bool payerParPaypal(const string& email,const string&){
        if(email.find('@')==string::npos){ cout<<"  Email PayPal invalide.\n"; return false; }
        emailPaypal=email; valide=true;
        cout << "\n  Paiement PayPal confirmé.\n  Référence : " << reference << "\n";
        return true;
    }

    bool effectuerPaiement(){
        cout << "\nMODE DE PAIEMENT : \n"
             << "  Montant : " << fixed << setprecision(2) << montant << " MAD\n\n"
             << "  1. Carte Bancaire\n  2. PayPal\n  Choix : ";
        int choix; cin>>choix;
        if(choix==1){
            mode=Mode::CARTE;
            string num,tit,exp,c;
            cout<<"\n  Numéro (16 chiffres) : "; cin>>num;
            cin.ignore();
            cout<<"  Titulaire            : "; getline(cin,tit);
            cout<<"  Expiration (MM/AA)   : "; cin>>exp;
            cout<<"  CVV                  : "; cin>>c;
            return payerParCarte(num,tit,exp,c);
        }else if(choix==2){
            mode=Mode::PAYPAL;
            string em,pwd;
            cout<<"\n  Email PayPal : "; cin>>em;
            cout<<"  Mot de passe : "; cin>>pwd;
            return payerParPaypal(em,pwd);
        }
        cout<<"Choix invalide.\n"; return false;
    }

    bool   estValide()    const { return valide; }
    double getMontant()   const { return montant; }
    string getReference() const { return reference; }
    Mode   getMode()      const { return mode; }

    string getModeStr() const {
        return mode==Mode::CARTE ? "Carte" : "PayPal";
    }
};



struct ArticlePanier {
    Produit* produit;
    int      quantite;
    string   tailleChoisie, couleurChoisie;

    ArticlePanier(Produit* p,int q,
                  const string& t="",const string& c="")
        : produit(p),quantite(q),
          tailleChoisie(t),couleurChoisie(c){}

    double sousTotal() const { return produit->getPrix()*quantite; }

    void afficher() const {
        cout << fixed << setprecision(2)
             << "  [" << produit->getId() << "] " << produit->getNom();
        if(!tailleChoisie.empty())  cout<<"  Taille: "  <<tailleChoisie;
        if(!couleurChoisie.empty()) cout<<"  Couleur: " <<couleurChoisie;
        cout << "\n  Qty: " << quantite
             << "  x  " << produit->getPrix() << " MAD"
             << "  =  " << sousTotal() << " MAD\n";
    }
};


class Panier { // class panier (Nouiri)
protected:
    int                   id;        // id dans la table paniers
    int                   idClient;
    string                nomClient;
    vector<ArticlePanier> articles;
    string                codePromo;
    double                remisePromo;

public:
    Panier(int id,int idClient,const string& nomClient)
        : id(id),idClient(idClient),nomClient(nomClient),remisePromo(0){}

    virtual ~Panier(){}

    void chargerDepuisDB(Database& db,
                         const vector<Produit*>& tousProduits) {
        // Récupère les infos du panier (code promo, remise)
        auto row = db.fetchOne(
            "SELECT id, code_promo, remise_promo FROM paniers WHERE id_client = "
            + to_string(idClient)
        );
        if(row.empty()) return;
        id          = stoi(row[0]);
        codePromo   = row[1];
        remisePromo = stod(row[2]);

        auto lignes = db.fetchAll(
            "SELECT id_produit, quantite, taille_choisie, couleur_choisie "
            "FROM articles_panier WHERE id_panier = " + row[0]
        );
        articles.clear();
        for(auto& l : lignes){
            int idP = stoi(l[0]);
            // Cherche le produit dans la liste fournie par le catalogue
            for(auto* p : tousProduits){
                if(p->getId()==idP){
                    articles.emplace_back(p, stoi(l[1]), l[2], l[3]);
                    break;
                }
            }
        }
    }

    void ajouterArticle(Database& db, Produit* p, int quantite,
                        const string& taille="",
                        const string& couleur="") {
        // Mise à jour en mémoire
        for(auto& a : articles){
            if(a.produit->getId()==p->getId()){
                a.quantite+=quantite;
                // Mise à jour en DB
                db.execute(
                    "UPDATE articles_panier SET quantite = " + to_string(a.quantite)
                    + " WHERE id_panier = " + to_string(id)
                    + " AND id_produit = " + to_string(p->getId())
                );
                cout<<"  Quantité mise à jour : "<<a.quantite<<"x "<<p->getNom()<<"\n";
                return;
            }
        }
        articles.emplace_back(p,quantite,taille,couleur);
        db.execute(
            "INSERT INTO articles_panier (id_panier, id_produit, quantite, taille_choisie, couleur_choisie) "
            "VALUES (" + to_string(id) + ", " + to_string(p->getId())
            + ", " + to_string(quantite)
            + ", " + db.safe(taille)
            + ", " + db.safe(couleur) + ")"
        );
        cout<<"  "<<quantite<<"x "<<p->getNom()<<" ajouté au panier.\n";
    }

    void supprimerArticle(Database& db, int idProduit){
        for(auto it=articles.begin();it!=articles.end();++it){
            if(it->produit->getId()==idProduit){
                cout<<"  "<<it->produit->getNom()<<" retiré du panier.\n";
                articles.erase(it);
                db.execute(
                    "DELETE FROM articles_panier WHERE id_panier = "
                    + to_string(id) + " AND id_produit = " + to_string(idProduit)
                );
                return;
            }
        }
        cout<<"  Produit #"<<idProduit<<" introuvable dans le panier.\n";
    }

    void appliquerCodePromo(Database& db,const string& code,double pct){
        codePromo=code; remisePromo=pct;
        db.execute(
            "UPDATE paniers SET code_promo = " + db.safe(code)
            + ", remise_promo = " + to_string(pct)
            + " WHERE id = " + to_string(id)
        );
        cout<<"  Code \""<<code<<"\" appliqué : "<<pct<<"% de réduction !\n";
    }

    void vider(Database& db){
        articles.clear(); codePromo=""; remisePromo=0;
        db.execute("DELETE FROM articles_panier WHERE id_panier = " + to_string(id));
        db.execute("UPDATE paniers SET code_promo = '', remise_promo = 0 WHERE id = " + to_string(id));
    }

    virtual double calculerTotal() const {
        double t=0;
        for(const auto& a:articles) t+=a.sousTotal();
        return t;
    }
    double calculerTotalApresPromo() const {
        double t=calculerTotal();
        return t - t*(remisePromo/100.0);
    }

    bool estVide() const { return articles.empty(); }

    void afficher() const {
        cout << fixed << setprecision(2)
             << "\nVotre panier : \n";
        if(articles.empty()){ cout<<"  Le panier est vide.\n\n"; return; }
        for(const auto& a:articles){ a.afficher(); cout<<"\n"; }
        cout << "─────────────────────────────────\n"
             << "  Sous-total : " << calculerTotal() << " MAD\n";
        if(remisePromo>0)
            cout << "  Promo ("<<codePromo<<") : -"
                 << calculerTotal()*remisePromo/100.0
                 << " MAD ("<<remisePromo<<"%)\n"
                 << "  TOTAL après promo : " << calculerTotalApresPromo() << " MAD\n";
        cout << "─────────────────────────────────\n\n";
    }

    int    getId()          const { return id; }
    int    getIdClient()    const { return idClient; }
    string getCodePromo()   const { return codePromo; }
    double getRemisePromo() const { return remisePromo; }
    const  vector<ArticlePanier>& getArticles() const { return articles; }
};



//  Class Commande (OULGHAZI)

class Commande {
private:
    int      idCommande;
    string   dateCommande, adresseLivraison;
    double   fraisLivraison;
    Paiement* paiement;
    vector<Produit*> produits;
    vector<int>      quantites;

public:
    Commande(int id,const string& date,const string& adresse)
        : idCommande(id),dateCommande(date),
          adresseLivraison(adresse),
          fraisLivraison(25.0),paiement(nullptr){}

    ~Commande(){ delete paiement; }

    void ajouterProduit(Produit* p,int q){ produits.push_back(p); quantites.push_back(q); }

    double calculerTotal() const {
        double t=0;
        for(size_t i=0;i<produits.size();++i)
            t+=produits[i]->getPrix()*quantites[i];
        return t;
    }
    double calculerTotalAvecLivraison() const { return calculerTotal()+fraisLivraison; }

    void afficherCommande() const {
        cout << fixed << setprecision(2)
             << "\n===== Commande #"<<idCommande<<" =====\n"
             << "Date       : "<<dateCommande<<"\n"
             << "Adresse    : "<<adresseLivraison<<"\n--- Produits ---\n";
        for(size_t i=0;i<produits.size();++i)
            cout<<"  - "<<produits[i]->getNom()
                <<" x"<<quantites[i]
                <<" = "<<produits[i]->getPrix()*quantites[i]<<" MAD\n";
        cout<<"─────────────────────\n"
            <<"Sous-total  : "<<calculerTotal()<<" MAD\n"
            <<"Livraison   : "<<fraisLivraison<<" MAD\n"
            <<"TOTAL       : "<<calculerTotalAvecLivraison()<<" MAD\n";
    }

    // Sauvegarde la commande complète dans la DB et retourne son id
    int sauvegarderDansDB(Database& db, int idClient) {
        db.execute(
            "INSERT INTO commandes (id_client, adresse_livraison, statut_commande) VALUES ("
            + to_string(idClient) + ", "
            + db.safe(adresseLivraison) + ","
            + db.safe("Livrée") + ")"
        );
        int idCmd = db.lastInsertId();
        idCommande = idCmd;

        for(size_t i=0;i<produits.size();++i){
            db.execute(
                "INSERT INTO commande_produits (id_commande, id_produit, quantite, prix_unitaire) "
                "VALUES (" + to_string(idCmd)
                + ", " + to_string(produits[i]->getId())
                + ", " + to_string(quantites[i])
                + ", " + to_string(produits[i]->getPrix()) + ")"
            );
            // Décrémente le stock dans la DB
            produits[i]->acheter(quantites[i]);
            produits[i]->mettreAJourStockDB(db);
        }

        if(paiement && paiement->estValide()){
            db.execute(
                "INSERT INTO paiements (id_commande, mode, montant, reference, valide) VALUES ("
                + to_string(idCmd) + ", "
                + db.safe(paiement->getModeStr()) + ", "
                + to_string(paiement->getMontant()) + ", "
                + db.safe(paiement->getReference()) + ", TRUE)"
            );
        }
        return idCmd;
    }

    bool   estVide()        const { return produits.empty(); }
    int    getId()          const { return idCommande; }
    string getDate()        const { return dateCommande; }
    double getFrais()       const { return fraisLivraison; }
    string getAdresse()     const { return adresseLivraison; }
    void   setPaiement(Paiement* p){ paiement=p; }
    void   setAdresse(const string& a){ adresseLivraison=a; }
};


// class personne (ELHARCHAOUI) 
//classe abstraite 
class Personne {
protected:
    int    identifiant;
    string prenom,nom,email,telephone,motDePasse;
    bool   estConnecte;

public:
    Personne()
        : identifiant(0),prenom(""),nom(""),email(""),
          telephone(""),motDePasse(""),estConnecte(false){}

    Personne(int id,const string& p,const string& n,
             const string& em,const string& tel,const string& pwd)
        : identifiant(id),prenom(p),nom(n),email(em),
          telephone(tel),motDePasse(pwd),estConnecte(false){}

    virtual ~Personne(){
        cout<<"Personne "<<getNomComplet()<<" détruite.\n";
    }

    int    getId()         const { return identifiant; }
    string getPrenom()     const { return prenom; }
    string getNom()        const { return nom; }
    string getNomComplet() const { return prenom+" "+nom; }
    string getEmail()      const { return email; }
    string getTelephone()  const { return telephone; }
    bool   connecte()      const { return estConnecte; }

    void setPrenom(const string& p){ prenom=p; }
    void setNom(const string& n)   { nom=n; }
    void setEmail(const string& e) {
        if(e.find('@')!=string::npos) email=e;
        else cout<<"Email invalide.\n";
    }
    void setTelephone(const string& t){ telephone=t; }

    // Connexion vérifiée en DB
    bool seConnecter(Database& db,const string& em,const string& pwd){
        auto row = db.fetchOne(
            "SELECT id, prenom, nom, telephone, mot_de_passe FROM personnes "
            "WHERE email = " + db.safe(em)
            + " AND mot_de_passe = " + db.safe(pwd)
        );
        if(row.empty()){ cout<<"Email ou mot de passe incorrect.\n"; return false; }
        identifiant = stoi(row[0]);
        prenom      = row[1];
        nom         = row[2];
        telephone   = row[3];
        motDePasse  = row[4];
        email       = em;
        estConnecte = true;
        db.execute("UPDATE personnes SET est_connecte = TRUE WHERE id = " + to_string(identifiant));
        cout<<"Bienvenue, "<<getNomComplet()<<"!\n";
        return true;
    }

    void seDeconnecter(Database& db){
        db.execute("UPDATE personnes SET est_connecte = FALSE WHERE id = " + to_string(identifiant));
        estConnecte=false;
        cout<<"\nDéconnecté. À bientôt, "<<prenom<<"!\n";
    }

    // Inscription en DB : retourne le nouvel id ou -1 si erreur
    int sInscrire(Database& db,
                  const string& p,const string& n,
                  const string& em,const string& tel,
                  const string& pwd,const string& role){
        auto existing = db.fetchOne(
            "SELECT id FROM personnes WHERE email = " + db.safe(em)
        );
        if(!existing.empty()){ cout<<"Email déjà utilisé.\n"; return -1; }
        db.execute(
            "INSERT INTO personnes (prenom, nom, email, telephone, mot_de_passe, role) "
            "VALUES (" + db.safe(p)   + ", " + db.safe(n)
                       + ", " + db.safe(em)  + ", " + db.safe(tel)
                       + ", " + db.safe(pwd) + ", " + db.safe(role) + ")"
        );
        int newId=db.lastInsertId();
        identifiant=newId;
        prenom=p; nom=n; email=em; telephone=tel; motDePasse=pwd;
        cout<<"Compte créé pour "<<getNomComplet()<<"!\n";
        return newId;
    }

    virtual void afficherRole()  = 0;
    virtual void tableauDeBord() = 0;

    friend ostream& operator<<(ostream& os,const Personne& p){
        os<<"Personne #"<<p.identifiant<<" | "<<p.getNomComplet()
          <<" | "<<p.email
          <<" | "<<(p.estConnecte?"En ligne":"Hors ligne");
        return os;
    }
};



//  CATALOGUE (charge tous les produits depuis la DB)

class Catalogue {
private:
    vector<Produit*> produits;

public:
    ~Catalogue(){ for(auto* p:produits) delete p; }

    // ── Chargement complet depuis MySQL ──────────────────────
    void chargerDepuisDB(Database& db){
        produits.clear();

        // ---- Livres ----
        auto livres = db.fetchAll(
            "SELECT p.id, p.nom, p.prix, p.stock, p.note, p.statut, "
            "       p.en_promotion, p.prix_original, p.description, "
            "       l.auteur, l.isbn, l.editeur, l.langue "
            "FROM produits p JOIN livres l ON p.id = l.id_produit "
            "WHERE p.statut != 'Archivé'"
        );
        for(auto& r:livres){
            auto* lv = new Livre(stoi(r[0]),r[1],stod(r[2]),stoi(r[3]),
                                 r[9],r[10],0,r[11],r[12]);
            lv->setNote(stod(r[4]));
            lv->setStatut(stringToStatut(r[5]));
            lv->setEnPromotion(r[6]=="1");
            lv->setPrixOriginal(stod(r[7]));
            lv->setDescription(r[8]);
            produits.push_back(lv);
        }

        auto vets = db.fetchAll(
            "SELECT p.id, p.nom, p.prix, p.stock, p.note, p.statut, "
            "       p.en_promotion, p.prix_original, p.description, "
            "       v.taille, v.couleur, v.matiere, v.marque "
            "FROM produits p JOIN vetements v ON p.id = v.id_produit "
            "WHERE p.statut != 'Archivé'"
        );
        for(auto& r:vets){
            Taille  t = stringToTaille(r[9]);
            Couleur c;
            try{ c = stringToCouleur(r[10]); }catch(...){ c=Couleur::NOIR; }
            auto* vt = new Vetement(stoi(r[0]),r[1],stod(r[2]),stoi(r[3]),
                                    t,c,r[11],r[12]);
            vt->setNote(stod(r[4]));
            vt->setStatut(stringToStatut(r[5]));
            vt->setEnPromotion(r[6]=="1");
            vt->setPrixOriginal(stod(r[7]));
            vt->setDescription(r[8]);
            produits.push_back(vt);
        }

        // ---- Électronique (sans ordinateurs) ----
        auto elecs = db.fetchAll(
            "SELECT p.id, p.nom, p.prix, p.stock, p.note, p.statut, "
            "       p.en_promotion, p.prix_original, p.description, "
            "       e.marque, e.garantie_mois, e.puissance_watts, e.tension_volts "
            "FROM produits p "
            "JOIN electroniques e ON p.id = e.id_produit "
            "LEFT JOIN ordinateurs o ON p.id = o.id_produit "
            "WHERE o.id_produit IS NULL AND p.statut != 'Archivé'"
        );
        for(auto& r:elecs){
            auto* el = new Electronique(stoi(r[0]),r[1],stod(r[2]),stoi(r[3]),
                                        r[9],stoi(r[10]),stod(r[11]),stod(r[12]));
            el->setNote(stod(r[4]));
            el->setStatut(stringToStatut(r[5]));
            el->setEnPromotion(r[6]=="1");
            el->setPrixOriginal(stod(r[7]));
            el->setDescription(r[8]);
            produits.push_back(el);
        }

        // ---- Ordinateurs ----
        auto ordi = db.fetchAll(
            "SELECT p.id, p.nom, p.prix, p.stock, p.note, p.statut, "
            "       p.en_promotion, p.prix_original, p.description, "
            "       e.marque, e.garantie_mois, "
            "       o.cpu, o.gpu, o.stockage_go, o.type_stockage, o.type_ordi "
            "FROM produits p "
            "JOIN electroniques e ON p.id = e.id_produit "
            "JOIN ordinateurs   o ON p.id = o.id_produit "
            "WHERE p.statut != 'Archivé'"
        );
        for(auto& r:ordi){
            auto* or2 = new Ordinateur(stoi(r[0]),r[1],stod(r[2]),stoi(r[3]),
                                       r[9],stoi(r[10]),
                                       r[11],r[12],stoi(r[13]),r[14],r[15]);
            or2->setNote(stod(r[4]));
            or2->setStatut(stringToStatut(r[5]));
            or2->setEnPromotion(r[6]=="1");
            or2->setPrixOriginal(stod(r[7]));
            or2->setDescription(r[8]);
            produits.push_back(or2);
        }

        // ---- Alimentation ----
        auto alim = db.fetchAll(
            "SELECT p.id, p.nom, p.prix, p.stock, p.note, p.statut, "
            "       p.en_promotion, p.prix_original, p.description, "
            "       a.date_expiration, a.poids_kg, a.calories_per_100g, "
            "       a.bio, a.allergenes, a.pays_origine "
            "FROM produits p JOIN alimentations a ON p.id = a.id_produit "
            "WHERE p.statut != 'Archivé'"
        );
        for(auto& r:alim){
            auto* al = new Alimentation(stoi(r[0]),r[1],stod(r[2]),stoi(r[3]),
                                        r[9],stod(r[10]),stoi(r[11]),
                                        r[12]=="1",r[13],r[14]);
            al->setNote(stod(r[4]));
            al->setStatut(stringToStatut(r[5]));
            al->setEnPromotion(r[6]=="1");
            al->setPrixOriginal(stod(r[7]));
            al->setDescription(r[8]);
            produits.push_back(al);
        }

        // ---- Sport & Fitness ----
        auto sport = db.fetchAll(
            "SELECT p.id, p.nom, p.prix, p.stock, p.note, p.statut, "
            "       p.en_promotion, p.prix_original, p.description, "
            "       s.sport, s.marque, s.materiau, s.poids_kg, s.niveau_utilisateur "
            "FROM produits p JOIN sport_fitness s ON p.id = s.id_produit "
            "WHERE p.statut != 'Archivé'"
        );
        for(auto& r:sport){
            auto* sp = new SportFitness(stoi(r[0]),r[1],stod(r[2]),stoi(r[3]),
                                        r[9],r[10],r[11],stod(r[12]),r[13]);
            sp->setNote(stod(r[4]));
            sp->setStatut(stringToStatut(r[5]));
            sp->setEnPromotion(r[6]=="1");
            sp->setPrixOriginal(stod(r[7]));
            sp->setDescription(r[8]);
            produits.push_back(sp);
        }

        // ---- Maison ----
        auto maison = db.fetchAll(
            "SELECT p.id, p.nom, p.prix, p.stock, p.note, p.statut, "
            "       p.en_promotion, p.prix_original, p.description, "
            "       m.piece, m.style, m.matiere, m.dimensions, m.marque "
            "FROM produits p JOIN maison_decor m ON p.id = m.id_produit "
            "WHERE p.statut != 'Archivé'"
        );
        for(auto& r:maison){
            auto* md = new MaisonDecor(stoi(r[0]),r[1],stod(r[2]),stoi(r[3]),
                                       r[9],r[10],r[11],r[12],r[13]);
            md->setNote(stod(r[4]));
            md->setStatut(stringToStatut(r[5]));
            md->setEnPromotion(r[6]=="1");
            md->setPrixOriginal(stod(r[7]));
            md->setDescription(r[8]);
            produits.push_back(md);
        }

        // ---- Tickets ----
        auto tickets = db.fetchAll(
            "SELECT p.id, p.nom, p.prix, p.stock, p.note, p.statut, "
            "       p.en_promotion, p.prix_original, p.description, "
            "       t.evenement, t.lieu, t.date_evenement, t.type_place "
            "FROM produits p JOIN tickets t ON p.id = t.id_produit "
            "WHERE p.statut != 'Archivé'"
        );
        for(auto& r:tickets){
            auto* tk = new Ticket(stoi(r[0]),r[1],stod(r[2]),stoi(r[3]),
                                  r[9],r[10],r[11],r[12]);
            tk->setNote(stod(r[4]));
            tk->setStatut(stringToStatut(r[5]));
            tk->setEnPromotion(r[6]=="1");
            tk->setPrixOriginal(stod(r[7]));
            tk->setDescription(r[8]);
            produits.push_back(tk);
        }

        cout << produits.size() << " produit(s) chargé(s) depuis la base de données.\n";
    }

    // Accesseurs
    void afficherTous() const {
        cout<<"\n Notre catalogue \n";
        for(const auto* p:produits) p->afficherCourt();
        cout<<"\n";
    }

    vector<Produit*> rechercherParNom(const string& mc) const {
        vector<Produit*> r;
        string m=mc;
        transform(m.begin(),m.end(),m.begin(),::tolower);
        for(auto* p:produits){
            string nm=p->getNom();
            transform(nm.begin(),nm.end(),nm.begin(),::tolower);
            if(nm.find(m)!=string::npos) r.push_back(p);
        }
        return r;
    }

    vector<Produit*> filtrerParCategorie(Categorie cat) const {
        if(cat==Categorie::TOUS) return produits;
        vector<Produit*> r;
        for(auto* p:produits)
            if(p->getCategorieEnum()==cat) r.push_back(p);
        return r;
    }

    vector<Produit*> filtrerParPrix(double pmin,double pmax) const {
        vector<Produit*> r;
        for(auto* p:produits)
            if(p->getPrix()>=pmin && p->getPrix()<=pmax) r.push_back(p);
        return r;
    }

    vector<Produit*> filtrerPromotions() const {
        vector<Produit*> r;
        for(auto* p:produits) if(p->isEnPromotion()) r.push_back(p);
        return r;
    }

    Produit* trouverParId(int id) const {
        for(auto* p:produits) if(p->getId()==id) return p;
        return nullptr;
    }

    const vector<Produit*>& getProduits() const { return produits; }

    static void afficherResultats(const vector<Produit*>& res,
                                  const string& titre="Résultats"){
        cout<<"\n--- "<<titre<<" ("<<res.size()<<" produit(s)) ---\n";
        if(res.empty()){ cout<<"  Aucun produit trouvé.\n\n"; return; }
        for(const auto* p:res) p->afficherCourt();
        cout<<"\n";
    }

    int getNbProduits() const { return (int)produits.size(); }
};


// utilisation de l'heritage entre les classes Personne et Client / Personne et vendeur 
class Client : public Personne {
private:
    string  adresse;
    Panier* panier;

    static int prochainIdCmd;

public:
    Client(int id,const string& p,const string& n,
           const string& em,const string& tel,const string& pwd)
        : Personne(id,p,n,em,tel,pwd), adresse("")
    {
        // Le panier sera initialisé après connexion (on a besoin de l'id DB)
        panier = new Panier(0, id, getNomComplet());
    }

    ~Client(){
        delete panier;
        cout<<"Client "<<getNomComplet()<<" détruit.\n";
    }

    Panier* getPanier() const { return panier; }
    string  getAdresse()const { return adresse; }
    void    setAdresse(const string& a){ adresse=a; }

    // Charge l'adresse depuis la DB après connexion
    void chargerProfil(Database& db){
        auto row = db.fetchOne(
            "SELECT adresse FROM clients WHERE id_personne = " + to_string(identifiant)
        );
        if(!row.empty()) adresse = row[0];
    }

    void afficherRole() override {
        cout<<"Rôle : CLIENT | Adresse : "<<adresse<<"\n";
    }

    void tableauDeBord() override {
        cout<<"\n══════════════════════════════════\n"
            <<"  Bienvenue, "<<getNomComplet()<<"\n"
            <<"══════════════════════════════════\n"
            <<"  1. Parcourir le catalogue\n"
            <<"  2. Rechercher un produit\n"
            <<"  3. Filtrer par catégorie\n"
            <<"  4. Filtrer par prix\n"
            <<"  5. Voir mon panier\n"
            <<"  6. Ajouter au panier\n"
            <<"  7. Passer la commande\n"
            <<"  8. Historique des commandes\n"
            <<"  9. Se déconnecter\n"
            <<"══════════════════════════════════\n";
    }

    void ajouterAuPanier(Database& db, Produit* p, int quantite,
                         const string& taille="",const string& couleur=""){
        if(!estConnecte){ cout<<"Connectez-vous d'abord.\n"; return; }
        if(!p->estDisponible()){ cout<<"Produit indisponible.\n"; return; }
        if(p->getStock()<quantite){
            cout<<"Stock insuffisant ("<<p->getStock()<<" restants).\n"; return;
        }
        panier->ajouterArticle(db, p, quantite, taille, couleur);
    }

    void retirerDuPanier(Database& db, int idProduit){
        panier->supprimerArticle(db, idProduit);
    }

    void appliquerCodePromo(Database& db,const string& code){
        // Codes promo hardcodés (même logique que server.js)
        static const map<string,double> CODES = {
            {"promo123", 20.0},
            {"ilovelinux", 50.0}
        };
        string c = code;
        transform(c.begin(),c.end(),c.begin(),::tolower);
        auto it = CODES.find(c);
        if(it==CODES.end()){ cout<<"Code promo invalide.\n"; return; }
        panier->appliquerCodePromo(db, c, it->second);
    }

    bool passerCommande(Database& db){
        if(!estConnecte){ cout<<"Connectez-vous d'abord.\n"; return false; }
        if(panier->estVide()){ cout<<"Votre panier est vide.\n"; return false; }

        panier->afficher();

        cout<<"  Adresse de livraison : ";
        cin.ignore(); getline(cin, adresse);

        // Sauvegarde de l'adresse dans la DB
        db.execute(
            "UPDATE clients SET adresse = " + db.safe(adresse)
            + " WHERE id_personne = " + to_string(identifiant)
        );

        double total = panier->calculerTotalApresPromo() + 25.0;
        cout<<"\n  Frais de livraison : 25.00 MAD\n"
            <<"  ─────────────────────\n"
            <<"  TOTAL FINAL : "<<fixed<<setprecision(2)<<total<<" MAD\n";

        Paiement* pmt = new Paiement(Paiement::Mode::CARTE, total);
        if(!pmt->effectuerPaiement()){
            delete pmt;
            cout<<"Paiement échoué. Commande annulée.\n";
            return false;
        }

        Commande commande(0, "2025-01-01", adresse);
        for(const auto& art : panier->getArticles())
            commande.ajouterProduit(art.produit, art.quantite);
        commande.setPaiement(pmt);

        int idCmd = commande.sauvegarderDansDB(db, identifiant);

        cout<<" Commande #"<<idCmd<<" enregistrée !\n"
            <<"  Nom       : "<<getNomComplet()<<"\n"
            <<"  Email     : "<<email<<"\n"
            <<"  Adresse   : "<<adresse<<"\n"
            <<"  Total     : "<<total<<" MAD\n"
            <<"  Réf.      : "<<pmt->getReference()<<"\n"
            <<"  Merci pour votre commande !\n\n";

        panier->vider(db);
        return true;
    }

    void afficherHistorique(Database& db) const {
        cout<<"\n--- Historique des commandes ---\n";
        auto commandes = db.fetchAll(
            "SELECT id, date_commande, adresse_livraison, frais_livraison, statut_commande "
            "FROM commandes WHERE id_client = " + to_string(identifiant)
            + " ORDER BY date_commande DESC"
        );
        if(commandes.empty()){ cout<<"  Aucune commande.\n"; return; }
        for(auto& c : commandes){
            cout<<"\n  Commande #"<<c[0]
                <<" | "<<c[1]<<" | "<<c[4]<<"\n"
                <<"  Adresse : "<<c[2]<<"\n";
            auto lignes = db.fetchAll(
                "SELECT p.nom, cp.quantite, cp.prix_unitaire "
                "FROM commande_produits cp "
                "JOIN produits p ON cp.id_produit = p.id "
                "WHERE cp.id_commande = " + c[0]
            );
            double total=0;
            for(auto& l : lignes){
                double sous = stod(l[1])*stod(l[2]);
                total+=sous;
                cout<<"    - "<<l[0]<<" x"<<l[1]
                    <<" = "<<fixed<<setprecision(2)<<sous<<" MAD\n";
            }
            cout<<"  Livraison : "<<c[3]<<" MAD\n"
                <<"  TOTAL     : "<<total+stod(c[3])<<" MAD\n";
        }
    }
};

int Client::prochainIdCmd = 1000;


// class vendeur 
class Vendeur : public Personne {
private:
    string nomBoutique;
    double chiffreAffaires;

public:
    Vendeur(int id,const string& p,const string& n,
            const string& em,const string& tel,
            const string& pwd,const string& boutique)
        : Personne(id,p,n,em,tel,pwd),
          nomBoutique(boutique),chiffreAffaires(0){}

    ~Vendeur(){ cout<<"Vendeur "<<getNomComplet()<<" détruit.\n"; }

    void afficherRole() override {
        cout<<"Rôle : VENDEUR | Boutique : "<<nomBoutique<<"\n";
    }

    void tableauDeBord() override {
        cout<<"\n  Bienvenue, "<<getNomComplet()<<"\n"
            <<"  Boutique : "<<nomBoutique<<"\n\n"
            <<"  1. Voir mes produits\n"
            <<"  2. Chiffre d'affaires\n"
            <<"  3. Se déconnecter\n"
            <<"══════════════════════════════════\n";
    }

    void afficherProduitsDB(Database& db) const {
        cout<<"\n--- Produits de "<<nomBoutique<<" ---\n";
        auto rows = db.fetchAll(
            "SELECT p.id, p.nom, p.prix, p.stock FROM produits p "
            "JOIN vendeur_produits vp ON p.id = vp.id_produit "
            "WHERE vp.id_vendeur = " + to_string(identifiant)
        );
        if(rows.empty()){ cout<<"  Aucun produit.\n"; return; }
        for(auto& r:rows)
            cout<<"  [" << r[0]<<"] "<<r[1]
                <<" | "<<r[2]<<" MAD | Stock: "<<r[3]<<"\n";
    }

    void chargerChiffreAffaires(Database& db){
        auto row = db.fetchOne(
            "SELECT chiffre_affaires FROM vendeurs WHERE id_personne = "
            + to_string(identifiant)
        );
        if(!row.empty()) chiffreAffaires=stod(row[0]);
    }
};


void menuVendeur(Vendeur* v, Database& db){
    bool actif=true;
    while(actif){
        v->tableauDeBord();
        cout<<"  Votre choix : ";
        int choix; cin>>choix;
        switch(choix){
            case 1: v->afficherProduitsDB(db); break;
            case 2:
                v->chargerChiffreAffaires(db);
                cout<<"  CA enregistré en DB.\n";
                break;
            case 3: v->seDeconnecter(db); actif=false; break;
            default: cout<<"  Choix invalide.\n";
        }
    }
}




int main() {
    Database db;
    Catalogue catalogue;
    catalogue.chargerDepuisDB(db);

    httplib::Server svr;

    svr.new_task_queue = [] { return new httplib::ThreadPool(1); };
    svr.new_task_queue = [] { return new httplib::ThreadPool(1); };
    svr.set_mount_point("/uploads", "./uploads");
    svr.set_mount_point("/music", ".");

    // ── CORS helper ────────────────────────────────
    auto setCORS = [](httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    };

    // Handle OPTIONS preflight for all routes
    svr.Options(".*", [&](const httplib::Request&, httplib::Response& res) {
        setCORS(res);
        res.set_content("", "text/plain");
    });

    // ══════════════════════════════════════════════
    //  PING
    // ══════════════════════════════════════════════
    svr.Get("/ping", [&](const httplib::Request&, httplib::Response& res) {
        setCORS(res);
        res.set_content("pong", "text/plain");
    });

    // ══════════════════════════════════════════════
    //  PRODUITS
    // ══════════════════════════════════════════════

    // GET /api/produits
    svr.Get("/api/produits", [&](const httplib::Request& req,
                                  httplib::Response& res) {
        setCORS(res);
        try {
            string sql = "SELECT * FROM vue_catalogue WHERE 1=1";
            vector<string> params;

            if (req.has_param("categorie")) {
                sql += " AND categorie = '" + db.escape(req.get_param_value("categorie")) + "'";
            }
            if (req.has_param("nom")) {
                sql += " AND nom LIKE '%" + db.escape(req.get_param_value("nom")) + "%'";
            }
            if (req.has_param("promotion") && req.get_param_value("promotion") == "true") {
                sql += " AND en_promotion = TRUE";
            }

            auto rows = db.fetchAll(sql);
            json result = json::array();
            for (auto& r : rows) {
                json j;
                j["id"]           = stoi(r[0]);
                j["nom"]          = r[1];
                j["prix"]         = stod(r[2]);
                j["prix_original"]= stod(r[3]);
                j["description"]  = r[4];
                j["stock"]        = stoi(r[5]);
                j["note"]         = stod(r[6]);
                j["categorie"]    = r[7];
                j["statut"]       = r[8];
                j["en_promotion"] = r[9] == "1";
                j["image_url"]    = r[10];
                result.push_back(j);
            }
            res.set_content(result.dump(), "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // GET /api/promotions
    svr.Get("/api/promotions", [&](const httplib::Request&,
                                    httplib::Response& res) {
        setCORS(res);
        try {
            auto rows = db.fetchAll("SELECT * FROM vue_promotions");
            json result = json::array();
            for (auto& r : rows) {
                json j;
                j["id"]            = stoi(r[0]);
                j["nom"]           = r[1];
                j["prix"]          = stod(r[2]);
                j["prix_original"] = stod(r[3]);
                j["reduction_pct"] = stod(r[4]);
                j["categorie"]     = r[5];
                j["image_url"]     = r[6];
                result.push_back(j);
            }
            res.set_content(result.dump(), "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // GET /api/produits/:id
    svr.Get(R"(/api/produits/(\d+))", [&](const httplib::Request& req,
                                           httplib::Response& res) {
        setCORS(res);
        try {
            string id = req.matches[1];
            auto rows = db.fetchAll("SELECT * FROM vue_catalogue WHERE id = " + id);
            if (rows.empty()) {
                res.status = 404;
                res.set_content("{\"error\":\"Produit non trouvé\"}", "application/json");
                return;
            }
            auto& r = rows[0];
            json j;
            j["id"]           = stoi(r[0]);
            j["nom"]          = r[1];
            j["prix"]         = stod(r[2]);
            j["prix_original"]= stod(r[3]);
            j["description"]  = r[4];
            j["stock"]        = stoi(r[5]);
            j["note"]         = stod(r[6]);
            j["categorie"]    = r[7];
            j["statut"]       = r[8];
            j["en_promotion"] = r[9] == "1";
            j["image_url"]    = r[10];

            // Comments
            auto comms = db.fetchAll(
                "SELECT auteur, texte, etoiles, created_at FROM commentaires "
                "WHERE id_produit = " + id + " ORDER BY created_at DESC"
            );
            json commentaires = json::array();
            for (auto& c : comms) {
                json cj;
                cj["auteur"]     = c[0];
                cj["texte"]      = c[1];
                cj["etoiles"]    = stoi(c[2]);
                cj["created_at"] = c[3];
                commentaires.push_back(cj);
            }
            j["commentaires"] = commentaires;

            res.set_content(j.dump(), "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // GET /api/produits/:id/images
    svr.Get(R"(/api/produits/(\d+)/images)", [&](const httplib::Request& req,
                                                   httplib::Response& res) {
        setCORS(res);
        try {
            string id = req.matches[1];
            auto rows = db.fetchAll(
                "SELECT id, image_url, ordre FROM produit_images "
                "WHERE id_produit = " + id + " ORDER BY ordre ASC"
            );
            json result = json::array();
            for (auto& r : rows) {
                json j;
                j["id"]        = stoi(r[0]);
                j["image_url"] = r[1];
                j["ordre"]     = stoi(r[2]);
                result.push_back(j);
            }
            res.set_content(result.dump(), "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // ══════════════════════════════════════════════
    //  AUTH
    // ══════════════════════════════════════════════

    // POST /api/auth/register
    svr.Post("/api/auth/register", [&](const httplib::Request& req,
                                        httplib::Response& res) {
        setCORS(res);
        try {
            auto body       = json::parse(req.body);
            string prenom   = body["prenom"];
            string nom      = body["nom"];
            string email    = body["email"];
            string tel      = body.value("telephone", "");
            string pwd      = body["mot_de_passe"];
            string role     = body["role"];

            // Check email unique
            auto existing = db.fetchOne(
                "SELECT id FROM personnes WHERE email = " + db.safe(email)
            );
            if (!existing.empty()) {
                res.status = 409;
                res.set_content("{\"error\":\"Email déjà utilisé\"}", "application/json");
                return;
            }

            db.execute(
                "INSERT INTO personnes (prenom, nom, email, telephone, mot_de_passe, role) "
                "VALUES (" + db.safe(prenom) + ", " + db.safe(nom) + ", "
                + db.safe(email) + ", " + db.safe(tel) + ", "
                + db.safe(pwd) + ", " + db.safe(role) + ")"
            );
            int newId = db.lastInsertId();

            if (role == "client") {
                db.execute("INSERT INTO clients (id_personne) VALUES (" + to_string(newId) + ")");
                db.execute("INSERT INTO paniers (id_client) VALUES (" + to_string(newId) + ")");
            } else {
                string boutique = body.value("nom_boutique", "Ma Boutique");
                db.execute(
                    "INSERT INTO vendeurs (id_personne, nom_boutique) VALUES ("
                    + to_string(newId) + ", " + db.safe(boutique) + ")"
                );
            }

            json response;
            response["message"]    = "Compte créé avec succès";
            response["id_personne"]= newId;
            response["role"]       = role;
            res.status = 201;
            res.set_content(response.dump(), "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // POST /api/auth/login
    svr.Post("/api/auth/login", [&](const httplib::Request& req,
                                     httplib::Response& res) {
        setCORS(res);
        try {
            auto body    = json::parse(req.body);
            string email = body["email"];
            string pwd   = body["mot_de_passe"];

            auto row = db.fetchOne(
                "SELECT id, prenom, nom, telephone, role FROM personnes "
                "WHERE email = " + db.safe(email) +
                " AND mot_de_passe = " + db.safe(pwd)
            );

            if (row.empty()) {
                res.status = 401;
                res.set_content("{\"error\":\"Email ou mot de passe incorrect\"}", "application/json");
                return;
            }

            db.execute("UPDATE personnes SET est_connecte = TRUE WHERE id = " + row[0]);

            json user;
            user["id"]        = stoi(row[0]);
            user["prenom"]    = row[1];
            user["nom"]       = row[2];
            user["telephone"] = row[3];
            user["role"]      = row[4];
            user["email"]     = email;

            json response;
            response["message"] = "Connexion réussie";
            response["user"]    = user;
            res.set_content(response.dump(), "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // POST /api/auth/logout
    svr.Post("/api/auth/logout", [&](const httplib::Request& req,
                                      httplib::Response& res) {
        setCORS(res);
        try {
            auto body = json::parse(req.body);
            string id = to_string((int)body["id_personne"]);
            db.execute("UPDATE personnes SET est_connecte = FALSE WHERE id = " + id);
            res.set_content("{\"message\":\"Déconnecté\"}", "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // ══════════════════════════════════════════════
    //  PANIER
    // ══════════════════════════════════════════════

    // GET /api/panier/:id_client
    svr.Get(R"(/api/panier/(\d+))", [&](const httplib::Request& req,
                                         httplib::Response& res) {
        setCORS(res);
        try {
            string idClient = req.matches[1];
            auto panierRow = db.fetchOne(
                "SELECT id, code_promo, remise_promo FROM paniers WHERE id_client = " + idClient
            );
            if (panierRow.empty()) {
                res.status = 404;
                res.set_content("{\"error\":\"Panier non trouvé\"}", "application/json");
                return;
            }

            json panier;
            panier["id"]          = stoi(panierRow[0]);
            panier["code_promo"]  = panierRow[1];
            panier["remise_promo"]= stod(panierRow[2]);

            auto articles = db.fetchAll(
                "SELECT ap.id_produit, ap.quantite, ap.taille_choisie, "
                "ap.couleur_choisie, p.nom, p.prix, p.image_url, p.categorie "
                "FROM articles_panier ap "
                "JOIN produits p ON ap.id_produit = p.id "
                "WHERE ap.id_panier = " + panierRow[0]
            );

            json artJson = json::array();
            for (auto& a : articles) {
                json j;
                j["id_produit"]     = stoi(a[0]);
                j["quantite"]       = stoi(a[1]);
                j["taille_choisie"] = a[2];
                j["couleur_choisie"]= a[3];
                j["nom"]            = a[4];
                j["prix"]           = stod(a[5]);
                j["image_url"]      = a[6];
                j["categorie"]      = a[7];
                artJson.push_back(j);
            }

            json response;
            response["panier"]   = panier;
            response["articles"] = artJson;
            res.set_content(response.dump(), "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // POST /api/panier/ajouter
    svr.Post("/api/panier/ajouter", [&](const httplib::Request& req,
                                         httplib::Response& res) {
        setCORS(res);
        try {
            auto body       = json::parse(req.body);
            int idClient    = body["id_client"];
            int idProduit   = body["id_produit"];
            int quantite    = body.value("quantite", 1);
            string taille   = body.value("taille_choisie", "");
            string couleur  = body.value("couleur_choisie", "");

            auto panierRow = db.fetchOne(
                "SELECT id FROM paniers WHERE id_client = " + to_string(idClient)
            );
            if (panierRow.empty()) {
                res.status = 404;
                res.set_content("{\"error\":\"Panier non trouvé\"}", "application/json");
                return;
            }
            string idPanier = panierRow[0];

            auto existing = db.fetchOne(
                "SELECT quantite FROM articles_panier WHERE id_panier = " + idPanier +
                " AND id_produit = " + to_string(idProduit)
            );

            if (!existing.empty()) {
                int newQty = stoi(existing[0]) + quantite;
                db.execute(
                    "UPDATE articles_panier SET quantite = " + to_string(newQty) +
                    " WHERE id_panier = " + idPanier +
                    " AND id_produit = " + to_string(idProduit)
                );
            } else {
                db.execute(
                    "INSERT INTO articles_panier "
                    "(id_panier, id_produit, quantite, taille_choisie, couleur_choisie) "
                    "VALUES (" + idPanier + ", " + to_string(idProduit) + ", "
                    + to_string(quantite) + ", " + db.safe(taille) + ", " + db.safe(couleur) + ")"
                );
            }
            res.set_content("{\"message\":\"Produit ajouté au panier\"}", "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // PUT /api/panier/quantite
    svr.Put("/api/panier/quantite", [&](const httplib::Request& req,
                                         httplib::Response& res) {
        setCORS(res);
        try {
            auto body     = json::parse(req.body);
            int idClient  = body["id_client"];
            int idProduit = body["id_produit"];
            int quantite  = body["quantite"];

            auto panierRow = db.fetchOne(
                "SELECT id FROM paniers WHERE id_client = " + to_string(idClient)
            );
            if (panierRow.empty()) {
                res.status = 404;
                res.set_content("{\"error\":\"Panier non trouvé\"}", "application/json");
                return;
            }

            db.execute(
                "UPDATE articles_panier SET quantite = " + to_string(quantite) +
                " WHERE id_panier = " + panierRow[0] +
                " AND id_produit = " + to_string(idProduit)
            );
            res.set_content("{\"message\":\"Quantité mise à jour\"}", "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // DELETE /api/panier/supprimer
    svr.Delete("/api/panier/supprimer", [&](const httplib::Request& req,
                                             httplib::Response& res) {
        setCORS(res);
        try {
            auto body     = json::parse(req.body);
            int idClient  = body["id_client"];
            int idProduit = body["id_produit"];

            auto panierRow = db.fetchOne(
                "SELECT id FROM paniers WHERE id_client = " + to_string(idClient)
            );
            if (panierRow.empty()) {
                res.status = 404;
                res.set_content("{\"error\":\"Panier non trouvé\"}", "application/json");
                return;
            }

            db.execute(
                "DELETE FROM articles_panier WHERE id_panier = " + panierRow[0] +
                " AND id_produit = " + to_string(idProduit)
            );
            res.set_content("{\"message\":\"Produit retiré du panier\"}", "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // POST /api/panier/promo
    svr.Post("/api/panier/promo", [&](const httplib::Request& req,
                                       httplib::Response& res) {
        setCORS(res);
        try {
            auto body    = json::parse(req.body);
            int idClient = body["id_client"];
            string code  = body["code"];

            // Same promo codes as before
            map<string, double> CODES = {
                {"promo123",   20.0},
                {"ilovelinux", 50.0}
            };

            transform(code.begin(), code.end(), code.begin(), ::tolower);
            auto it = CODES.find(code);
            if (it == CODES.end()) {
                res.status = 400;
                res.set_content("{\"error\":\"Code promo invalide\"}", "application/json");
                return;
            }

            auto panierRow = db.fetchOne(
                "SELECT id FROM paniers WHERE id_client = " + to_string(idClient)
            );
            db.execute(
                "UPDATE paniers SET code_promo = " + db.safe(code) +
                ", remise_promo = " + to_string(it->second) +
                " WHERE id = " + panierRow[0]
            );

            json response;
            response["message"] = "Code appliqué : " + to_string((int)it->second) + "% de réduction";
            response["remise"]  = it->second;
            res.set_content(response.dump(), "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // DELETE /api/panier/promo
    svr.Delete("/api/panier/promo", [&](const httplib::Request& req,
                                         httplib::Response& res) {
        setCORS(res);
        try {
            auto body    = json::parse(req.body);
            int idClient = body["id_client"];

            auto panierRow = db.fetchOne(
                "SELECT id FROM paniers WHERE id_client = " + to_string(idClient)
            );
            db.execute(
                "UPDATE paniers SET code_promo = '', remise_promo = 0 WHERE id = " + panierRow[0]
            );
            res.set_content("{\"message\":\"Code promo supprimé\"}", "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // ══════════════════════════════════════════════
    //  COMMANDES
    // ══════════════════════════════════════════════

    // POST /api/commandes
    svr.Post("/api/commandes", [&](const httplib::Request& req,
                                    httplib::Response& res) {
        setCORS(res);
        try {
            auto body      = json::parse(req.body);
            int idClient   = body["id_client"];
            string adresse = body["adresse_livraison"];

            auto panierRow = db.fetchOne(
                "SELECT id FROM paniers WHERE id_client = " + to_string(idClient)
            );
            if (panierRow.empty()) {
                res.status = 404;
                res.set_content("{\"error\":\"Panier non trouvé\"}", "application/json");
                return;
            }

            auto articles = db.fetchAll(
                "SELECT ap.id_produit, ap.quantite, p.prix FROM articles_panier ap "
                "JOIN produits p ON ap.id_produit = p.id "
                "WHERE ap.id_panier = " + panierRow[0]
            );

            if (articles.empty()) {
                res.status = 400;
                res.set_content("{\"error\":\"Le panier est vide\"}", "application/json");
                return;
            }

            // Use your Commande class!
            Commande commande(0, "now", adresse);
            for (auto& a : articles) {
                Produit* p = catalogue.trouverParId(stoi(a[0]));
                if (p) commande.ajouterProduit(p, stoi(a[1]));
            }

            int idCmd = commande.sauvegarderDansDB(db, idClient);

            // Clear cart
            db.execute("DELETE FROM articles_panier WHERE id_panier = " + panierRow[0]);
            db.execute("UPDATE paniers SET code_promo='', remise_promo=0 WHERE id = " + panierRow[0]);

            // Reload catalogue (stock changed)
            catalogue.chargerDepuisDB(db);

            json response;
            response["message"]     = "Commande passée avec succès";
            response["id_commande"] = idCmd;
            res.status = 201;
            res.set_content(response.dump(), "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // GET /api/commandes/:id_client
    svr.Get(R"(/api/commandes/(\d+))", [&](const httplib::Request& req,
                                            httplib::Response& res) {
        setCORS(res);
        try {
            string idClient = req.matches[1];
            auto rows = db.fetchAll(
                "SELECT id, date_commande, adresse_livraison, frais_livraison, statut_commande "
                "FROM commandes WHERE id_client = " + idClient +
                " ORDER BY date_commande DESC"
            );
            json result = json::array();
            for (auto& r : rows) {
                json j;
                j["id"]                = stoi(r[0]);
                j["date_commande"]     = r[1];
                j["adresse_livraison"] = r[2];
                j["frais_livraison"]   = stod(r[3]);
                j["statut_commande"]   = r[4];
                result.push_back(j);
            }
            res.set_content(result.dump(), "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // GET /api/commandes/detail/:id_commande
    svr.Get(R"(/api/commandes/detail/(\d+))", [&](const httplib::Request& req,
                                                    httplib::Response& res) {
        setCORS(res);
        try {
            string idCmd = req.matches[1];
            auto cmdRow = db.fetchOne(
                "SELECT id, date_commande, adresse_livraison, frais_livraison, statut_commande "
                "FROM commandes WHERE id = " + idCmd
            );
            if (cmdRow.empty()) {
                res.status = 404;
                res.set_content("{\"error\":\"Commande non trouvée\"}", "application/json");
                return;
            }

            auto produits = db.fetchAll(
                "SELECT cp.id_produit, p.nom, cp.quantite, cp.prix_unitaire, p.image_url "
                "FROM commande_produits cp "
                "JOIN produits p ON cp.id_produit = p.id "
                "WHERE cp.id_commande = " + idCmd
            );

            json prods = json::array();
            double total = 0;
            for (auto& p : produits) {
                json j;
                j["id_produit"]   = stoi(p[0]);
                j["nom"]          = p[1];
                j["quantite"]     = stoi(p[2]);
                j["prix_unitaire"]= stod(p[3]);
                j["image_url"]    = p[4];
                total += stod(p[3]) * stoi(p[2]);
                prods.push_back(j);
            }

            double frais = stod(cmdRow[3]);
            json response;
            response["commande"]["id"]                = stoi(cmdRow[0]);
            response["commande"]["date_commande"]     = cmdRow[1];
            response["commande"]["adresse_livraison"] = cmdRow[2];
            response["commande"]["statut_commande"]   = cmdRow[4];
            response["produits"]                      = prods;
            response["total_produits"]                = total;
            response["frais_livraison"]               = frais;
            response["total_final"]                   = total + frais;
            res.set_content(response.dump(), "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // ══════════════════════════════════════════════
    //  VENDEUR
    // ══════════════════════════════════════════════

    // GET /api/vendeur/:id/produits
    svr.Get(R"(/api/vendeur/(\d+)/produits)", [&](const httplib::Request& req,
                                                    httplib::Response& res) {
        setCORS(res);
        try {
            string idVendeur = req.matches[1];
            auto rows = db.fetchAll(
                "SELECT p.id, p.nom, p.prix, p.stock, p.categorie, p.statut, p.image_url "
                "FROM produits p "
                "JOIN vendeur_produits vp ON p.id = vp.id_produit "
                "WHERE vp.id_vendeur = " + idVendeur
            );
            json result = json::array();
            for (auto& r : rows) {
                json j;
                j["id"]        = stoi(r[0]);
                j["nom"]       = r[1];
                j["prix"]      = stod(r[2]);
                j["stock"]     = stoi(r[3]);
                j["categorie"] = r[4];
                j["statut"]    = r[5];
                j["image_url"] = r[6];
                result.push_back(j);
            }
            res.set_content(result.dump(), "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });
    
    // ══════════════════════════════════════════════
    //  COMMENTAIRES
    // ══════════════════════════════════════════════
    
    // POST /api/commentaires
    svr.Post("/api/commentaires", [&](const httplib::Request& req,
                                       httplib::Response& res) {
        setCORS(res);
        try {
            auto body      = json::parse(req.body);
            int idProduit  = body["id_produit"];
            string auteur  = body["auteur"];
            string texte   = body["texte"];
            int etoiles    = body["etoiles"];

            Produit* p = catalogue.trouverParId(idProduit);
            if (!p) {
                res.status = 404;
                res.set_content("{\"error\":\"Produit non trouvé\"}", "application/json");
                return;
            }

            // Uses your existing class method!
            p->sauvegarderCommentaireDansDB(db, auteur, texte, etoiles);

            res.status = 201;
            res.set_content("{\"message\":\"Commentaire ajouté\"}", "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });
        // ══════════════════════════════════════════════
    //  VENDEUR — Création de produit
    // ══════════════════════════════════════════════

    // POST /api/vendeur/:id/produits
    svr.Post(R"(/api/vendeur/(\d+)/produits)", [&](const httplib::Request& req,
                                                      httplib::Response& res) {
        setCORS(res);
        try {
            string idVendeur = req.matches[1];
            auto body = json::parse(req.body);
            
            string nom        = body["nom"];
            string description= body.value("description", "");
            double prixVendeur= body["prix"];
            int    stock      = body.value("stock", 0);
            string categorie  = body["categorie"];
            string imageUrl   = body.value("image_url", "");
            
            // Appliquer la marge de 20% pour eShop
            double prixFinal = round(prixVendeur * 1.2 * 100.0) / 100.0;
            
            // Insérer dans produits
            db.execute(
                "INSERT INTO produits (nom, prix, prix_original, description, stock, categorie, statut, image_url) "
                "VALUES (" + db.safe(nom) + ", " + to_string(prixFinal) + ", " + to_string(prixFinal) + ", "
                + db.safe(description) + ", " + to_string(stock) + ", " + db.safe(categorie) + ", "
                + db.safe("Disponible") + ", " + db.safe(imageUrl) + ")"
            );
            int newId = db.lastInsertId();
            
            // Lier au vendeur
            db.execute(
                "INSERT INTO vendeur_produits (id_vendeur, id_produit) VALUES ("
                + idVendeur + ", " + to_string(newId) + ")"
            );
            
            // Insérer dans la sous-table selon la catégorie
            if (categorie == "Livre") {
                string auteur = body.value("auteur", "");
                string isbn   = body.value("isbn", "");
                string editeur= body.value("editeur", "");
                string langue = body.value("langue", "Anglais");
                db.execute(
                    "INSERT INTO livres (id_produit, auteur, isbn, editeur, langue) VALUES ("
                    + to_string(newId) + ", " + db.safe(auteur) + ", " + db.safe(isbn) + ", "
                    + db.safe(editeur) + ", " + db.safe(langue) + ")"
                );
            }
            else if (categorie == "Vêtement") {
                string taille  = body.value("taille", "M");
                string couleur = body.value("couleur", "Noir");
                string matiere = body.value("matiere", "");
                string marque  = body.value("marque", "");
                db.execute(
                    "INSERT INTO vetements (id_produit, taille, couleur, matiere, marque) VALUES ("
                    + to_string(newId) + ", " + db.safe(taille) + ", " + db.safe(couleur) + ", "
                    + db.safe(matiere) + ", " + db.safe(marque) + ")"
                );
            }
            else if (categorie == "Électronique" || categorie == "Ordinateur") {
                string marque = body.value("marque", "");
                int garantie  = body.value("garantie_mois", 12);
                double puissance = body.value("puissance_watts", 0.0);
                db.execute(
                    "INSERT INTO electroniques (id_produit, marque, garantie_mois, puissance_watts, tension_volts) VALUES ("
                    + to_string(newId) + ", " + db.safe(marque) + ", " + to_string(garantie) + ", "
                    + to_string(puissance) + ", 220)"
                );
                if (categorie == "Ordinateur") {
                    string cpu = body.value("cpu", "");
                    string gpu = body.value("gpu", "");
                    int stockage = body.value("stockage_go", 0);
                    db.execute(
                        "INSERT INTO ordinateurs (id_produit, cpu, gpu, stockage_go, type_stockage, type_ordi) VALUES ("
                        + to_string(newId) + ", " + db.safe(cpu) + ", " + db.safe(gpu) + ", "
                        + to_string(stockage) + ", 'SSD', 'Gaming')"
                    );
                }
            }
            else if (categorie == "Alimentation") {
                string dateExp = body.value("date_expiration", "2026-12-31");
                double poids   = body.value("poids_kg", 0.0);
                int calories   = body.value("calories_per_100g", 0);
                bool bio       = body.value("bio", false);
                db.execute(
                    "INSERT INTO alimentations (id_produit, date_expiration, poids_kg, calories_per_100g, bio, allergenes, pays_origine) VALUES ("
                    + to_string(newId) + ", " + db.safe(dateExp) + ", " + to_string(poids) + ", "
                    + to_string(calories) + ", " + (bio ? "TRUE" : "FALSE") + ", '', 'Maroc')"
                );
            }
            else if (categorie == "Sport & Fitness") {
                string sport = body.value("sport", "");
                string marque= body.value("marque", "");
                db.execute(
                    "INSERT INTO sport_fitness (id_produit, sport, marque) VALUES ("
                    + to_string(newId) + ", " + db.safe(sport) + ", " + db.safe(marque) + ")"
                );
            }
            else if (categorie == "Maison") {
                string piece = body.value("piece", "");
                string style = body.value("style", "");
                db.execute(
                    "INSERT INTO maison_decor (id_produit, piece, style) VALUES ("
                    + to_string(newId) + ", " + db.safe(piece) + ", " + db.safe(style) + ")"
                );
            }
            else if (categorie == "Ticket") {
                string evenement = body.value("evenement", "");
                string lieu      = body.value("lieu", "");
                string dateEvt   = body.value("date_evenement", "");
                db.execute(
                    "INSERT INTO tickets (id_produit, evenement, lieu, date_evenement) VALUES ("
                    + to_string(newId) + ", " + db.safe(evenement) + ", " + db.safe(lieu) + ", "
                    + db.safe(dateEvt) + ")"
                );
            }
            
            // Ajouter l'image dans produit_images si fournie
            if (!imageUrl.empty()) {
                db.execute(
                    "INSERT INTO produit_images (id_produit, image_url, ordre) VALUES ("
                    + to_string(newId) + ", " + db.safe(imageUrl) + ", 1)"
                );
                db.execute(
                    "UPDATE produits SET image_url = " + db.safe(imageUrl) + " WHERE id = " + to_string(newId)
                );
            }
            
            // Recharger le catalogue
            catalogue.chargerDepuisDB(db);
            
            json response;
            response["message"]     = "Produit créé avec succès";
            response["id_produit"]  = newId;
            response["prix_vendeur"]= prixVendeur;
            response["prix_final"]  = prixFinal;
            res.status = 201;
            res.set_content(response.dump(), "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // GET /api/vendeur/:id/dashboard
    svr.Get(R"(/api/vendeur/(\d+)/dashboard)", [&](const httplib::Request& req,
                                                       httplib::Response& res) {
        setCORS(res);
        try {
            string idVendeur = req.matches[1];
            
            auto vendeur = db.fetchOne(
                "SELECT p.prenom, p.nom, v.nom_boutique, v.chiffre_affaires "
                "FROM personnes p JOIN vendeurs v ON p.id = v.id_personne "
                "WHERE v.id_personne = " + idVendeur
            );
            
            auto nbProds = db.fetchOne(
                "SELECT COUNT(*) FROM vendeur_produits WHERE id_vendeur = " + idVendeur
            );
            
            auto rows = db.fetchAll(
                "SELECT p.id, p.nom, p.prix, p.stock, p.categorie, p.statut, p.image_url, p.created_at "
                "FROM produits p JOIN vendeur_produits vp ON p.id = vp.id_produit "
                "WHERE vp.id_vendeur = " + idVendeur + " ORDER BY p.created_at DESC"
            );
            
            json produits = json::array();
            double caTotal = 0;
            for (auto& r : rows) {
                json j;
                j["id"]         = stoi(r[0]);
                j["nom"]        = r[1];
                double prixFinal = stod(r[2]);
                j["prix_final"] = prixFinal;
                j["prix_vendeur"]= round((prixFinal / 1.2) * 100.0) / 100.0;
                j["stock"]      = stoi(r[3]);
                j["categorie"]  = r[4];
                j["statut"]     = r[5];
                j["image_url"]  = r[6];
                j["date"]       = r[7];
                produits.push_back(j);
                caTotal += prixFinal * stod(r[3]);
            }
            
            json response;
            response["vendeur"]["prenom"]       = vendeur.empty() ? "" : vendeur[0];
            response["vendeur"]["nom"]          = vendeur.empty() ? "" : vendeur[1];
            response["vendeur"]["boutique"]     = vendeur.empty() ? "" : vendeur[2];
            response["vendeur"]["ca_enregistre"]= vendeur.empty() ? 0.0 : stod(vendeur[3]);
            response["vendeur"]["ca_calcule"]   = round(caTotal * 100.0) / 100.0;
            response["nb_produits"]             = nbProds.empty() ? 0 : stoi(nbProds[0]);
            response["produits"]                = produits;
            
            res.set_content(response.dump(), "application/json");
        } catch (exception& e) {
            res.status = 500;
            res.set_content("{\"error\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });
    // ══════════════════════════════════════════════
    //  START
    // ══════════════════════════════════════════════
    cout << "C++ eShop server running on http://localhost:3000\n";
    if (!svr.listen("127.0.0.1", 3000)) {
        cerr << "Failed to start server on port 3000\n";
        return 1;
    }
}