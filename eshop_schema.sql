-- ============================================================
--  E-SHOP DATABASE SCHEMA
--  Mirrors the C++ class hierarchy
--  Engine: MySQL 8.0+  |  Charset: utf8mb4
-- ============================================================

CREATE DATABASE IF NOT EXISTS eshop
    CHARACTER SET utf8mb4
    COLLATE utf8mb4_unicode_ci;

USE eshop;

-- ============================================================
--  TABLE : personnes  (base class Personne)
-- ============================================================
CREATE TABLE personnes (
    id            INT          UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    prenom        VARCHAR(50)  NOT NULL,
    nom           VARCHAR(50)  NOT NULL,
    email         VARCHAR(100) NOT NULL UNIQUE,
    telephone     VARCHAR(10)  NOT NULL,
    mot_de_passe  VARCHAR(255) NOT NULL,     -- store hashed (bcrypt)
    role          ENUM('client','vendeur')   NOT NULL DEFAULT 'client',
    est_connecte  TINYINT(1)   NOT NULL DEFAULT 0,
    created_at    DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at    DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP
                               ON UPDATE CURRENT_TIMESTAMP,
    CONSTRAINT chk_telephone CHECK (
        telephone REGEXP '^0[67][0-9]{8}$'
    )
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : clients  (extends personnes)
-- ============================================================
CREATE TABLE clients (
    personne_id   INT UNSIGNED NOT NULL PRIMARY KEY,
    adresse       TEXT,
    est_prime     TINYINT(1)   NOT NULL DEFAULT 0,
    CONSTRAINT fk_client_personne
        FOREIGN KEY (personne_id) REFERENCES personnes(id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : vendeurs  (extends personnes)
-- ============================================================
CREATE TABLE vendeurs (
    personne_id      INT UNSIGNED NOT NULL PRIMARY KEY,
    nom_boutique     VARCHAR(100) NOT NULL,
    chiffre_affaires DECIMAL(12,2) NOT NULL DEFAULT 0.00,
    CONSTRAINT fk_vendeur_personne
        FOREIGN KEY (personne_id) REFERENCES personnes(id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : produits  (base class Produit — abstract fields)
-- ============================================================
CREATE TABLE produits (
    id             INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    nom            VARCHAR(150) NOT NULL,
    prix           DECIMAL(10,2) NOT NULL CHECK (prix >= 0),
    prix_original  DECIMAL(10,2) NOT NULL CHECK (prix_original >= 0),
    stock          INT          NOT NULL DEFAULT 0 CHECK (stock >= 0),
    note           DECIMAL(3,2) NOT NULL DEFAULT 0.00
                   CHECK (note BETWEEN 0 AND 5),
    categorie      ENUM(
                       'Vetement',
                       'Electronique',
                       'Alimentation',
                       'Livre',
                       'Sport & Fitness',
                       'Maison',
                       'Ticket'
                   ) NOT NULL,
    statut         ENUM('Disponible','Rupture de stock','Archive')
                   NOT NULL DEFAULT 'Disponible',
    en_promotion   TINYINT(1) NOT NULL DEFAULT 0,
    created_at     DATETIME   NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at     DATETIME   NOT NULL DEFAULT CURRENT_TIMESTAMP
                              ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : vetements  (extends produits)
-- ============================================================
CREATE TABLE vetements (
    produit_id  INT UNSIGNED NOT NULL PRIMARY KEY,
    taille      ENUM('XS','S','M','L','XL','XXL') NOT NULL,
    couleur     ENUM('Rouge','Bleu','Vert','Noir','Blanc','Gris',
                     'Jaune','Rose','Orange','Violet','Marron','Beige')
                NOT NULL,
    matiere     VARCHAR(100),
    marque      VARCHAR(100),
    est_neuf    TINYINT(1) NOT NULL DEFAULT 1,
    CONSTRAINT fk_vetement_produit
        FOREIGN KEY (produit_id) REFERENCES produits(id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : electroniques  (extends produits)
-- ============================================================
CREATE TABLE electroniques (
    produit_id      INT UNSIGNED NOT NULL PRIMARY KEY,
    marque          VARCHAR(100) NOT NULL,
    garantie_mois   INT          NOT NULL DEFAULT 12,
    puissance_watts DECIMAL(8,2) DEFAULT 0,
    tension_volts   DECIMAL(6,2) DEFAULT 220,
    sous_type       ENUM('Smartphone','Ordinateur','Autre')
                    NOT NULL DEFAULT 'Autre',
    -- Smartphone specific (nullable for non-smartphones)
    ram_go          INT,
    os              VARCHAR(50),
    processeur      VARCHAR(100),
    camera_mp       INT,
    support_5g      TINYINT(1),
    taille_ecran    DECIMAL(4,1),
    -- Ordinateur specific (nullable for non-computers)
    cpu             VARCHAR(100),
    gpu             VARCHAR(100),
    stockage_go     INT,
    type_stockage   VARCHAR(20),
    type_ordi       VARCHAR(30),
    CONSTRAINT fk_electronique_produit
        FOREIGN KEY (produit_id) REFERENCES produits(id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : alimentations  (extends produits)
-- ============================================================
CREATE TABLE alimentations (
    produit_id        INT UNSIGNED NOT NULL PRIMARY KEY,
    date_expiration   DATE        NOT NULL,
    poids_kg          DECIMAL(6,3) NOT NULL,
    calories_per_100g INT          NOT NULL DEFAULT 0,
    bio               TINYINT(1)   NOT NULL DEFAULT 0,
    allergenes        TEXT,
    pays_origine      VARCHAR(80)  NOT NULL DEFAULT 'Maroc',
    CONSTRAINT fk_alimentation_produit
        FOREIGN KEY (produit_id) REFERENCES produits(id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : livres  (extends produits)
-- ============================================================
CREATE TABLE livres (
    produit_id         INT UNSIGNED NOT NULL PRIMARY KEY,
    auteur             VARCHAR(150) NOT NULL,
    isbn               VARCHAR(20)  NOT NULL,
    nombre_pages       INT          NOT NULL,
    editeur            VARCHAR(100) NOT NULL,
    langue             VARCHAR(30)  NOT NULL DEFAULT 'Français',
    annee_publication  YEAR         NOT NULL,
    genre              VARCHAR(80)  NOT NULL DEFAULT 'Roman',
    CONSTRAINT fk_livre_produit
        FOREIGN KEY (produit_id) REFERENCES produits(id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : sport_fitness  (extends produits)
-- ============================================================
CREATE TABLE sport_fitness (
    produit_id         INT UNSIGNED NOT NULL PRIMARY KEY,
    sport              VARCHAR(80)  NOT NULL,
    marque             VARCHAR(100) NOT NULL,
    materiau           VARCHAR(100),
    poids_kg           DECIMAL(6,3) DEFAULT 0,
    niveau_utilisateur VARCHAR(50)  NOT NULL DEFAULT 'Tous niveaux',
    CONSTRAINT fk_sport_produit
        FOREIGN KEY (produit_id) REFERENCES produits(id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : maison_decor  (extends produits)
-- ============================================================
CREATE TABLE maison_decor (
    produit_id  INT UNSIGNED NOT NULL PRIMARY KEY,
    piece       VARCHAR(80),
    style       VARCHAR(80),
    matiere     VARCHAR(100),
    dimensions  VARCHAR(100),
    marque      VARCHAR(100),
    CONSTRAINT fk_maison_produit
        FOREIGN KEY (produit_id) REFERENCES produits(id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : tickets  (extends produits)
-- ============================================================
CREATE TABLE tickets (
    produit_id       INT UNSIGNED NOT NULL PRIMARY KEY,
    evenement        VARCHAR(150) NOT NULL,
    lieu             VARCHAR(150) NOT NULL,
    date_evenement   DATE         NOT NULL,
    type_place       VARCHAR(50)  NOT NULL DEFAULT 'Standard',
    places_restantes INT          NOT NULL DEFAULT 0,
    CONSTRAINT fk_ticket_produit
        FOREIGN KEY (produit_id) REFERENCES produits(id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : commentaires  (Commentaire struct)
-- ============================================================
CREATE TABLE commentaires (
    id          INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    produit_id  INT UNSIGNED NOT NULL,
    auteur      VARCHAR(100) NOT NULL,
    texte       TEXT         NOT NULL,
    etoiles     TINYINT      NOT NULL CHECK (etoiles BETWEEN 1 AND 5),
    created_at  DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_commentaire_produit
        FOREIGN KEY (produit_id) REFERENCES produits(id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : commandes  (Commande class)
-- ============================================================
CREATE TABLE commandes (
    id               INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    client_id        INT UNSIGNED NOT NULL,
    date_commande    DATE         NOT NULL,
    adresse_livraison TEXT        NOT NULL,
    etat             ENUM('En attente','Confirmée',
                          'En cours de livraison',
                          'Livrée','Annulée')
                     NOT NULL DEFAULT 'En attente',
    frais_livraison  DECIMAL(8,2) NOT NULL DEFAULT 25.00,
    created_at       DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at       DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP
                                  ON UPDATE CURRENT_TIMESTAMP,
    CONSTRAINT fk_commande_client
        FOREIGN KEY (client_id) REFERENCES clients(personne_id)
        ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : lignes_commande  (ArticlePanier → order line)
-- ============================================================
CREATE TABLE lignes_commande (
    id             INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    commande_id    INT UNSIGNED  NOT NULL,
    produit_id     INT UNSIGNED  NOT NULL,
    quantite       INT           NOT NULL CHECK (quantite > 0),
    prix_unitaire  DECIMAL(10,2) NOT NULL,   -- snapshot at order time
    taille_choisie VARCHAR(10),
    couleur_choisie VARCHAR(30),
    CONSTRAINT fk_ligne_commande
        FOREIGN KEY (commande_id) REFERENCES commandes(id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_ligne_produit
        FOREIGN KEY (produit_id) REFERENCES produits(id)
        ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : paiements  (Paiement class)
-- ============================================================
CREATE TABLE paiements (
    id           INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    commande_id  INT UNSIGNED  NOT NULL UNIQUE,
    mode         ENUM('Carte','PayPal') NOT NULL,
    montant      DECIMAL(10,2) NOT NULL,
    reference    VARCHAR(20)   NOT NULL UNIQUE,
    est_valide   TINYINT(1)    NOT NULL DEFAULT 0,
    -- Card info (store only masked number in production)
    titulaire    VARCHAR(100),
    -- PayPal info
    email_paypal VARCHAR(100),
    created_at   DATETIME      NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_paiement_commande
        FOREIGN KEY (commande_id) REFERENCES commandes(id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : paniers  (Panier class — persisted cart)
-- ============================================================
CREATE TABLE paniers (
    id            INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    client_id     INT UNSIGNED  NOT NULL UNIQUE,
    code_promo    VARCHAR(30)   DEFAULT NULL,
    remise_promo  DECIMAL(5,2)  NOT NULL DEFAULT 0.00,
    est_prime     TINYINT(1)    NOT NULL DEFAULT 0,
    updated_at    DATETIME      NOT NULL DEFAULT CURRENT_TIMESTAMP
                               ON UPDATE CURRENT_TIMESTAMP,
    CONSTRAINT fk_panier_client
        FOREIGN KEY (client_id) REFERENCES clients(personne_id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TABLE : articles_panier  (ArticlePanier struct)
-- ============================================================
CREATE TABLE articles_panier (
    id              INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    panier_id       INT UNSIGNED NOT NULL,
    produit_id      INT UNSIGNED NOT NULL,
    quantite        INT          NOT NULL DEFAULT 1 CHECK (quantite > 0),
    taille_choisie  VARCHAR(10),
    couleur_choisie VARCHAR(30),
    UNIQUE KEY uq_panier_produit (panier_id, produit_id),
    CONSTRAINT fk_article_panier
        FOREIGN KEY (panier_id) REFERENCES paniers(id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_article_produit
        FOREIGN KEY (produit_id) REFERENCES produits(id)
        ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB;

-- ============================================================
--  TRIGGER : auto-update produit.statut on stock change
-- ============================================================
DELIMITER $$

CREATE TRIGGER trg_stock_update_statut
BEFORE UPDATE ON produits
FOR EACH ROW
BEGIN
    IF NEW.statut != 'Archive' THEN
        IF NEW.stock = 0 THEN
            SET NEW.statut = 'Rupture de stock';
        ELSEIF NEW.stock > 0 THEN
            SET NEW.statut = 'Disponible';
        END IF;
    END IF;
END$$

-- ============================================================
--  TRIGGER : recalculate average rating after new comment
-- ============================================================
CREATE TRIGGER trg_update_note_after_insert
AFTER INSERT ON commentaires
FOR EACH ROW
BEGIN
    UPDATE produits p
    SET    note = (
        SELECT AVG(c.etoiles)
        FROM   commentaires c
        WHERE  c.produit_id = NEW.produit_id
    )
    WHERE p.id = NEW.produit_id;
END$$

DELIMITER ;

-- ============================================================
--  VIEW : produits_complet  (useful for the web frontend)
-- ============================================================
CREATE VIEW v_produits_complet AS
SELECT
    p.id,
    p.nom,
    p.prix,
    p.prix_original,
    p.stock,
    p.note,
    p.categorie,
    p.statut,
    p.en_promotion,
    ROUND((1 - p.prix / NULLIF(p.prix_original, 0)) * 100, 0) AS pct_promo,
    (SELECT COUNT(*) FROM commentaires c WHERE c.produit_id = p.id) AS nb_avis
FROM produits p;

-- ============================================================
--  VIEW : commandes_detail
-- ============================================================
CREATE VIEW v_commandes_detail AS
SELECT
    co.id            AS commande_id,
    co.date_commande,
    co.etat,
    co.frais_livraison,
    pe.prenom,
    pe.nom,
    pe.email,
    SUM(lc.quantite * lc.prix_unitaire) AS sous_total,
    SUM(lc.quantite * lc.prix_unitaire) + co.frais_livraison AS total,
    pa.mode          AS mode_paiement,
    pa.reference     AS ref_paiement
FROM      commandes     co
JOIN      clients       cl ON cl.personne_id = co.client_id
JOIN      personnes     pe ON pe.id          = cl.personne_id
LEFT JOIN lignes_commande lc ON lc.commande_id = co.id
LEFT JOIN paiements     pa ON pa.commande_id  = co.id
GROUP BY  co.id, pe.prenom, pe.nom, pe.email,
          co.date_commande, co.etat, co.frais_livraison,
          pa.mode, pa.reference;

-- ============================================================
--  STORED PROCEDURE : passer_commande
--  Wraps the full checkout in a transaction
-- ============================================================
DELIMITER $$

CREATE PROCEDURE passer_commande(
    IN  p_client_id      INT UNSIGNED,
    IN  p_adresse        TEXT,
    IN  p_mode_paiement  ENUM('Carte','PayPal'),
    IN  p_reference      VARCHAR(20),
    IN  p_titulaire      VARCHAR(100),
    IN  p_email_paypal   VARCHAR(100),
    OUT p_commande_id    INT UNSIGNED
)
BEGIN
    DECLARE v_total       DECIMAL(10,2);
    DECLARE v_remise      DECIMAL(5,2);
    DECLARE v_est_prime   TINYINT(1);
    DECLARE v_panier_id   INT UNSIGNED;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;

    START TRANSACTION;

    -- Get cart info
    SELECT id, remise_promo, est_prime
    INTO   v_panier_id, v_remise, v_est_prime
    FROM   paniers
    WHERE  client_id = p_client_id;

    -- Calculate cart total (applying prime discount first)
    SELECT SUM(
        pr.prix * ap.quantite *
        CASE WHEN v_est_prime = 1 THEN 0.85 ELSE 1 END *
        (1 - v_remise / 100)
    )
    INTO v_total
    FROM articles_panier ap
    JOIN produits pr ON pr.id = ap.produit_id
    WHERE ap.panier_id = v_panier_id;

    -- Create order
    INSERT INTO commandes (client_id, date_commande, adresse_livraison, etat)
    VALUES (p_client_id, CURDATE(), p_adresse, 'Confirmée');

    SET p_commande_id = LAST_INSERT_ID();

    -- Copy cart items to order lines + decrement stock
    INSERT INTO lignes_commande
        (commande_id, produit_id, quantite, prix_unitaire,
         taille_choisie, couleur_choisie)
    SELECT
        p_commande_id,
        ap.produit_id,
        ap.quantite,
        pr.prix,
        ap.taille_choisie,
        ap.couleur_choisie
    FROM articles_panier ap
    JOIN produits pr ON pr.id = ap.produit_id
    WHERE ap.panier_id = v_panier_id;

    -- Decrement stock
    UPDATE produits pr
    JOIN   articles_panier ap ON ap.produit_id = pr.id
    SET    pr.stock = pr.stock - ap.quantite
    WHERE  ap.panier_id = v_panier_id;

    -- Record payment
    INSERT INTO paiements
        (commande_id, mode, montant, reference, est_valide,
         titulaire, email_paypal)
    VALUES
        (p_commande_id, p_mode_paiement, v_total + 25,
         p_reference, 1, p_titulaire, p_email_paypal);

    -- Clear cart
    DELETE FROM articles_panier WHERE panier_id = v_panier_id;
    UPDATE paniers SET code_promo = NULL, remise_promo = 0
    WHERE  id = v_panier_id;

    COMMIT;
END$$

DELIMITER ;

-- ============================================================
--  SAMPLE DATA  (matches the C++ main() catalogue)
-- ============================================================

-- Products
INSERT INTO produits (id, nom, prix, prix_original, stock, note, categorie, en_promotion)
VALUES
  (1,  'T-shirt Classic',            103.20, 129.00, 50, 4.50, 'Vetement',       1),
  (2,  'Jean Slim Fit',              399.00, 399.00, 30, 5.00, 'Vetement',       0),
  (3,  'Samsung Galaxy S25',        8999.00,8999.00, 20, 5.00, 'Electronique',   0),
  (4,  'MacBook Pro M3',           22000.00,22000.00,10, 0.00, 'Electronique',   0),
  (5,  "Huile d'olive extra vierge",  45.00,  45.00,500, 0.00, 'Alimentation',   0),
  (6,  'Le Petit Prince',             79.00,  79.00,200, 0.00, 'Livre',          0),
  (7,  'Ballon de Football Nike',    199.00, 199.00,100, 0.00, 'Sport & Fitness',0),
  (8,  'Tapis de Yoga Adidas',       289.00, 289.00, 60, 0.00, 'Sport & Fitness',0),
  (9,  'Canape Scandinave 3 Places', 3500.00,3500.00, 5, 0.00, 'Maison',         0),
  (10, 'Concert Gnawa Festival',     150.00, 150.00,500, 0.00, 'Ticket',         0);

INSERT INTO vetements (produit_id, taille, couleur, matiere, marque, est_neuf)
VALUES
  (1, 'M', 'Blanc', '100% Coton', 'Zara',   1),
  (2, 'L', 'Bleu',  'Denim',      "Levi's", 1);

INSERT INTO electroniques
  (produit_id, marque, garantie_mois, sous_type,
   ram_go, os, processeur, camera_mp, support_5g, taille_ecran)
VALUES
  (3, 'Samsung', 24, 'Smartphone', 12, 'Android', 'Snapdragon 8', 200, 1, 6.7);

INSERT INTO electroniques
  (produit_id, marque, garantie_mois, sous_type,
   cpu, gpu, stockage_go, type_stockage, type_ordi)
VALUES
  (4, 'Apple', 12, 'Ordinateur', 'Apple M3', 'Apple M3 GPU', 512, 'SSD', 'Laptop');

INSERT INTO alimentations
  (produit_id, date_expiration, poids_kg, calories_per_100g, bio)
VALUES
  (5, '2026-12-31', 0.75, 0, 1);

INSERT INTO livres
  (produit_id, auteur, isbn, nombre_pages, editeur, langue, annee_publication, genre)
VALUES
  (6, 'Antoine de Saint-Exupery', '978-2-07-040850-4', 96,
   'Gallimard', 'Français', 1943, 'Roman');

INSERT INTO sport_fitness
  (produit_id, sport, marque, materiau, poids_kg, niveau_utilisateur)
VALUES
  (7, 'Football', 'Nike',   'Caoutchouc', 0.45, 'Tous niveaux'),
  (8, 'Yoga',     'Adidas', 'TPE',        1.20, 'Débutant');

INSERT INTO maison_decor
  (produit_id, piece, style, matiere, dimensions, marque)
VALUES
  (9, 'Salon', 'Scandinave', 'Tissu + Bois', '220x85x80 cm', 'IKEA');

INSERT INTO tickets
  (produit_id, evenement, lieu, date_evenement, type_place, places_restantes)
VALUES
  (10, 'Gnawa World Music Festival', 'Essaouira', '2025-06-20', 'Standard', 500);

INSERT INTO commentaires (produit_id, auteur, texte, etoiles) VALUES
  (1, 'Ahmed', 'Tres confortable, je recommande !', 5),
  (1, 'Sara',  'Bonne qualite pour le prix.',        4),
  (2, 'Karim', 'Coupe parfaite.',                    5),
  (3, 'Nadia', 'Excellent smartphone, camera incroyable.', 5);
