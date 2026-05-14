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
CREATE TABLE IF NOT EXISTS personnes (
    id            INT          UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    prenom        VARCHAR(50)  NOT NULL,
    nom           VARCHAR(50)  NOT NULL,
    email         VARCHAR(100) NOT NULL UNIQUE,
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
CREATE TABLE IF NOT EXISTS  clients (
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
CREATE TABLE IF NOT EXISTS vendeurs (
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
CREATE TABLE IF NOT EXISTS produits (
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
CREATE TABLE IF NOT EXISTS vetements (
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
CREATE TABLE IF NOT EXISTS electroniques (
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
CREATE TABLE IF NOT EXISTS alimentations (
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
CREATE TABLE IF NOT EXISTS livres (
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
CREATE TABLE IF NOT EXISTS sport_fitness (
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
CREATE TABLE IF NOT EXISTS maison_decor (
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
CREATE TABLE IF NOT EXISTS tickets (
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
CREATE TABLE IF NOT EXISTS commentaires (
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
CREATE TABLE IF NOT EXISTS commandes (
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
CREATE TABLE IF NOT EXISTS lignes_commande (
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
CREATE TABLE IF NOT EXISTS paiements (
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
CREATE TABLE IF NOT EXISTS paniers (
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
CREATE TABLE IF NOT EXISTS articles_panier (
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
SET SQL_SAFE_UPDATES = 0;
DELETE FROM produits;
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
  (10, 'Concert Gnawa Festival',     150.00, 150.00,500, 0.00, 'Ticket',         0),
  (11, 'Acer Nitro V', 8500.00, 8500.00, 15, 4.20, 'Electronique', 0),
  (12, 'Google Pixel 8', 7500.00, 8000.00, 12, 4.80, 'Electronique', 1),
  (13, 'Razer Blade 14', 25000.00, 25000.00, 5, 5.00, 'Electronique', 0);

INSERT INTO vetements (produit_id, taille, couleur, matiere, marque, est_neuf)
VALUES
  (1, 'M', 'Blanc', '100% Coton', 'Zara', 1),
  (2, 'L', 'Bleu', 'Denim', "Levi's", 1),
  (3, 'S', 'Noir', 'Polyester', 'Nike', 2),
  (3, 'M', 'Noir', 'Polyester', 'Nike', 3),
  (3, 'XL', 'Gris', 'Coton', 'Nike', 1),
  (4, 'L', 'Rouge', 'Coton Bio', 'H&M', 1),
  (4, 'S', 'Rose', 'Coton Bio', 'H&M', 2),
  (5, 'M', 'Vert', 'Lin', 'Zara', 1),
  (5, 'L', 'Marron', 'Cuir', 'Zara', 1),
  (6, 'XS', 'Jaune', 'Soie', 'Mango', 1),
  (6, 'M', 'Orange', 'Viscose', 'Mango', 2),
  (7, 'L', 'Beige', 'Laine', 'Uniqlo', 1),
  (7, 'XL', 'Kaki', 'Coton', 'Uniqlo', 3),
  (8, 'M', 'Violet', 'Polyamide', 'Decathlon', 4),
  (9, 'S', 'Turquoise', 'Coton', 'Celio', 1),
  (9, 'L', 'Blanc', 'Lin', 'Celio', 1),
  (10, 'M', 'Bordeaux', 'Laine', 'Levi\'s', 2),
  (10, 'XL', 'Bleu Foncé', 'Denim', 'Levi\'s', 1);
ALTER TABLE electroniques MODIFY camera_mp VARCHAR(50);
ALTER TABLE electroniques MODIFY sous_type VARCHAR(50);
INSERT INTO electroniques 
(produit_id, marque, garantie_mois, puissance_watts, tension_volts, sous_type, ram_go, os, processeur, camera_mp, support_5g, taille_ecran, cpu, gpu, stockage_go, type_stockage, type_ordi) 
VALUES 
(1, 'Samsung', 24, 25.0, 220.0, 'Smartphone', 12, 'Android', 'Snapdragon 8 Gen 3', 200, 1, 6.8, 'Octa-core', 'Adreno 750', 256, 'UFS 4.0', 'Mobile'),
(2, 'Apple', 12, 20.0, 220.0, 'Smartphone', 8, 'iOS', 'A17 Pro', 48, 1, 6.1, 'Hexa-core', 'Apple GPU', 128, 'NVMe', 'Mobile'),
(3, 'Dell', 36, 65.0, 19.5, 'Ordinateur', 16, 'Windows 11', 'Intel i7', 2, 0, 15.6, 'i7-13700H', 'RTX 4060', 512, 'SSD', 'Laptop'),
(4, 'HP', 24, 45.0, 19.0, 'Ordinateur', 8, 'Windows 11', 'AMD Ryzen 5', 1, 0, 14.0, 'Ryzen 5600U', 'Radeon Graphics', 512, 'SSD', 'Laptop'),
(5, 'Lenovo', 24, 65.0, 20.0, 'Ordinateur', 16, 'Windows 11', 'Intel i5', 1, 1, 14.0, 'i5-12450H', 'Intel Graphics', 512, 'SSD', 'Laptop'),
(6, 'Asus', 24, 240.0, 20.0, 'Ordinateur', 32, 'Windows 11', 'Intel i9', 2, 1, 16.0, 'i9-14900HX', 'RTX 4080', 1024, 'SSD', 'Laptop'),
(7, 'Xiaomi', 12, 120.0, 220.0, 'Smartphone', 12, 'Android', 'Dimensity 9200', 50, 1, 6.6, 'Octa-core', 'Immortalis', 512, 'UFS', 'Mobile'),
(8, 'Sony', 24, 350.0, 220.0, 'Console', 16, 'Custom', 'Zen 2', 0, 0, 0.0, 'Custom CPU', 'Custom RDNA', 825, 'SSD', 'Console'),
(9, 'Huawei', 12, 66.0, 220.0, 'Smartphone', 8, 'HarmonyOS', 'Kirin 9000', 50, 1, 6.8, 'Octa-core', 'Maleoon', 256, 'UFS', 'Mobile'),
(10, 'MSI', 36, 180.0, 19.5, 'Ordinateur', 16, 'Windows 11', 'Intel i7', 1, 1, 17.3, 'i7-13620H', 'RTX 4050', 1024, 'SSD', 'Laptop'),
(11, 'Acer', 24, 90.0, 19.0, 'Ordinateur', 8, 'Windows 11', 'Intel i5', 1, 0, 15.6, 'i5-11400H', 'RTX 3050', 512, 'SSD', 'Laptop'),
(12, 'Google', 24, 30.0, 220.0, 'Smartphone', 8, 'Android', 'Tensor G3', 50, 1, 6.2, 'Nona-core', 'Titan M2', 128, 'UFS', 'Mobile'),
(13, 'Razer', 12, 230.0, 20.0, 'Ordinateur', 32, 'Windows 11', 'Intel i9', 2, 1, 14.0, 'i9-13950HX', 'RTX 4090', 1024, 'SSD', 'Laptop');
use eshop;
INSERT INTO alimentations (produit_id, date_expiration, poids_kg, calories_per_100g, bio, allergenes, pays_origine) VALUES 
(1, '2025-12-01', 0.20, 0, 0, 'Aucun', 'Maroc'),
(2, '2025-11-15', 0.80, 0, 0, 'Coton', 'Turquie'),
(3, '2026-05-20', 0.18, 0, 0, 'Aucun', 'Corée du Sud'),
(4, '2026-01-10', 1.20, 0, 0, 'Aucun', 'USA'),
(5, '2026-12-31', 0.75, 884, 1, 'Aucun', 'Maroc'), 
(6, '2027-01-01', 0.30, 0, 0, 'Papier', 'France'),
(7, '2025-06-15', 0.45, 0, 0, 'Aucun', 'Vietnam'),
(8, '2025-09-20', 1.50, 0, 0, 'Latex', 'Allemagne'),
(9, '2026-03-10', 25.0, 0, 0, 'Aucun', 'Suède'),
(10, '2025-07-01', 0.10, 0, 0, 'Aucun', 'Maroc'),
(11, '2025-12-25', 2.40, 0, 0, 'Aucun', 'Chine'),
(12, '2026-04-18', 0.16, 0, 0, 'Aucun', 'USA'),
(13, '2025-10-30', 2.10, 0, 0, 'Aucun', 'USA');

INSERT INTO livres (produit_id, auteur, isbn, nombre_pages, editeur, langue, annee_publication, genre) VALUES 
(1, 'Auteur Un', '978-1-11', 150, 'Editeur A', 'Français', 2024, 'Mode'),
(2, 'Auteur Deux', '978-2-22', 200, 'Editeur B', 'Anglais', 2023, 'Style'),
(3, 'Tech Expert', '978-3-33', 120, 'Samsung Pub', 'Arabe', 2025, 'Guide'),
(4, 'Apple Team', '978-4-44', 95, 'Apple Press', 'Français', 2024, 'Manuel'),
(5, 'Chef Bio', '978-5-55', 180, 'Maroc Edit', 'Arabe', 2022, 'Cuisine'),
(6, 'Antoine de Saint-Exupéry', '978-2-07-040850-4', 96, 'Gallimard', 'Français', 1943, 'Roman'), -- كتابك
(7, 'Sport Pro', '978-7-77', 60, 'Fitness Mag', 'Anglais', 2023, 'Sport'),
(8, 'Zen Master', '978-8-88', 210, 'Life Co', 'Français', 2021, 'Santé'),
(9, 'Deco Home', '978-9-99', 300, 'IKEA Pub', 'Français', 2020, 'Déco'),
(10, 'Music Team', '978-10-0', 45, 'Culture Press', 'Arabe', 2024, 'Culture'),
(11, 'Gamer Dev', '978-11-1', 85, 'Gaming Edit', 'Français', 2023, 'Tech'),
(12, 'Google AI', '978-12-2', 40, 'Google Pub', 'Anglais', 2024, 'Informatique'),
(13, 'Razer Lab', '978-13-3', 70, 'Blade Press', 'Français', 2023, 'Hardware');
SET SQL_SAFE_UPDATES = 0;
DELETE FROM sport_fitness;
SET SQL_SAFE_UPDATES = 1;
INSERT INTO sport_fitness (produit_id, sport, marque, materiau, poids_kg, niveau_utilisateur) VALUES 
(1, 'Football', 'Nike', 'Synthétique', 0.450, 'Amateur'),
(2, 'Yoga', 'Adidas', 'Caoutchouc', 1.200, 'Tous niveaux'),
(3, 'Tennis', 'Wilson', 'Graphite', 0.300, 'Professionnel'),
(4, 'Basketball', 'Spalding', 'Cuir', 0.600, 'Avancé'),
(5, 'Fitness', 'Decathlon', 'Fonte', 5.000, 'Débutant'),
(6, 'Natation', 'Speedo', 'Silicone', 0.050, 'Amateur'),
(7, 'Cyclisme', 'Shimano', 'Aluminium', 0.250, 'Avancé'),
(8, 'Boxe', 'Everlast', 'Cuir', 0.800, 'Tous niveaux'),
(9, 'Running', 'Puma', 'Mesh', 0.280, 'Amateur'),
(10, 'Golf', 'Titleist', 'Titane', 0.400, 'Professionnel'),
(11, 'Camping', 'Quechua', 'Polyester', 2.500, 'Débutant'),
(12, 'Musculation', 'Domyos', 'Acier', 10.000, 'Avancé'),
(13, 'Pilates', 'Reebok', 'Mousse', 0.900, 'Tous niveaux');  
SET SQL_SAFE_UPDATES = 0;
DELETE FROM maison_decor;
SET SQL_SAFE_UPDATES = 1;
INSERT INTO maison_decor (produit_id, piece, style, matiere, dimensions, marque) VALUES 
(1, 'Salon', 'Moderne', 'Coton', 'M', 'Zara Home'),
(2, 'Chambre', 'Classique', 'Denim', 'L', 'Levi\'s Casa'),
(3, 'Bureau', 'Futuriste', 'Métal', '15 inch', 'Samsung Design'),
(4, 'Bureau', 'Minimaliste', 'Aluminium', '13 inch', 'Apple Style'),
(5, 'Cuisine', 'Rustique', 'Verre', '750ml', 'Maroc Tradition'),
(6, 'Bibliothèque', 'Vintage', 'Papier', 'A5', 'Gallimard Deco'),
(7, 'Jardin', 'Sportif', 'Cuir', 'Taille 5', 'Nike Garden'),
(8, 'Salon', 'Zen', 'Caoutchouc', '180x60cm', 'Adidas Home'),
(9, 'Salon', 'Scandinave', 'Tissu', '3 Places', 'IKEA'),
(10, 'Studio', 'Artistique', 'Bois', '100x50cm', 'Festival Art'),
(11, 'Bureau', 'Gaming', 'Simili-cuir', 'Large', 'Razer Home'),
(12, 'Bureau', 'Ergonomique', 'Bois/Acier', '160x80cm', 'Logitech Desk'),
(13, 'Chambre', 'Ambiance', 'Plastique', 'Small', 'Philips Hue');

INSERT INTO tickets  (produit_id, evenement, lieu, date_evenement, type_place, places_restantes)
VALUES
  (1, 'Mawazine Festival', 'Rabat', '2025-05-16', 'VIP', 1200),
(1, 'Mawazine Festival', 'Rabat', '2025-05-16', 'Standard', 400),
(1, 'Mawazine Festival', 'Rabat', '2025-05-16', 'Gold', 800),
(2, 'Jazzablanca', 'Casablanca', '2025-06-10', 'VIP', 900),
(2, 'Jazzablanca', 'Casablanca', '2025-06-10', 'Standard', 300),
(3, 'Timgad Festival', 'Algérie', '2025-07-05', 'Standard', 250),
(4, 'Carthage Festival', 'Tunis', '2025-08-01', 'VIP', 750),
(4, 'Carthage Festival', 'Tunis', '2025-08-01', 'Standard', 280),
(5, 'Fes Sacred Music', 'Fès', '2025-05-30', 'VIP', 600),
(5, 'Fes Sacred Music', 'Fès', '2025-05-30', 'Standard', 200),
(6, 'Tanjazz', 'Tanger', '2025-09-15', 'Standard', 220),
(7, 'Atlas Electronic', 'Marrakech', '2025-11-10', 'VIP', 550),
(7, 'Atlas Electronic', 'Marrakech', '2025-11-10', 'Standard', 180),
(8, 'Timitar Festival', 'Agadir', '2025-07-18', 'Standard', 150),
(9, 'Boujloud Festival', 'Agadir', '2025-12-25', 'Standard', 100),
(10, 'Gnawa World Music Festival', 'Essaouira', '2025-06-20', 'Standard', 500),
(10, 'Gnawa World Music Festival', 'Essaouira', '2025-06-20', 'VIP', 1000),
(10, 'Gnawa World Music Festival', 'Essaouira', '2025-06-20', 'Gold', 750),
(11, 'L’Boulevard Festival', 'Casablanca', '2025-07-25', 'Standard', 150),
(11, 'L’Boulevard Festival', 'Casablanca', '2025-07-25', 'VIP', 350),
(12, 'Awrach Festival', 'Marrakech', '2025-10-05', 'Standard', 120),
(13, 'Hob Festival', 'Rabat', '2025-09-20', 'Standard', 180),
(14, 'Mogador Music Festival', 'Essaouira', '2025-04-10', 'Standard', 250),
(15, 'Casablanca Festival', 'Casablanca', '2025-06-30', 'VIP', 500),
(15, 'Casablanca Festival', 'Casablanca', '2025-06-30', 'Standard', 150);


INSERT INTO commentaires (produit_id, auteur, texte, etoiles) VALUES
  (1, 'Ahmed', 'Tres confortable, je recommande !', 5),
(1, 'Sara', 'Bonne qualite pour le prix.', 4),
(2, 'Karim', 'Coupe parfaite.', 5),
(3, 'Nadia', 'Excellent smartphone, camera incroyable.', 5),
(3, 'Youssef', 'La batterie tient bien la journee.', 4),
(3, 'Fatima', 'Ecran superbe mais un peu cher.', 4),
(4, 'Hassan', 'Livre tres interessant, je l\'ai adore.', 5),
(4, 'Leila', 'Histoire captivante, je recommande.', 5),
(5, 'Omar', 'Tres bonne qualite pour ce prix.', 4),
(5, 'Meryem', 'Conforme a la description.', 4),
(6, 'Samir', 'Service client reactif et professionnel.', 5),
(6, 'Nadia', 'Livraison rapide, bien emballe.', 5),
(7, 'Karim', 'Produit conforme, satisfait.', 4),
(8, 'Sara', 'Rapport qualite-prix excellent.', 5),
(8, 'Ahmed', 'Je l\'utilise tous les jours, tres pratique.', 5),
(9, 'Leila', 'Design magnifique, mais fragile.', 3),
(10, 'Youssef', 'Fonctionne parfaitement bien.', 5),
(10, 'Fatima', 'Un peu difficile a utiliser au debut.', 3);

-- ============================================================
-- SYSTEME LOGISTIQUE, FINANCE ET SAV
-- Note : 'tickets' a été remplacé par 'support_reclamations' 
-- pour éviter le conflit avec leur table de produits.
-- ============================================================

USE eshop_db;

-- ############################################################
-- 1. CRÉATION DES TABLES (STRUCTURE)
-- ############################################################

-- Gestion des Entrepôts
CREATE TABLE IF NOT EXISTS entrepots (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    nom_entrepot VARCHAR(100) NOT NULL,
    ville VARCHAR(50) NOT NULL,
    adresse TEXT,
    capacite_max INT DEFAULT 10000
    ) ENGINE=InnoDB;

-- Stock par Entrepôt
CREATE TABLE IF NOT EXISTS stock_entrepot (
    entrepot_id INT UNSIGNED NOT NULL,
    produit_id INT UNSIGNED NOT NULL,
    quantite_dispo INT NOT NULL DEFAULT 0,
    PRIMARY KEY (entrepot_id, produit_id),
    FOREIGN KEY (entrepot_id) REFERENCES entrepots(id) ON DELETE CASCADE,
    FOREIGN KEY (produit_id) REFERENCES produits(id) ON DELETE CASCADE
) ENGINE=InnoDB;

-- Sociétés de Transport (Logistique)
CREATE TABLE IF NOT EXISTS transporteurs (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    nom_societe VARCHAR(100) NOT NULL,
    telephone VARCHAR(15),
    tarif_base DECIMAL(8,2) DEFAULT 30.00,
    delai_estime_jours INT DEFAULT 3
) ENGINE=InnoDB;

-- Suivi de Livraison (Tracking System)
CREATE TABLE IF NOT EXISTS suivis_livraison (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    commande_id INT UNSIGNED NOT NULL UNIQUE,
    transporteur_id INT UNSIGNED,
    code_suivi VARCHAR(50) UNIQUE,
    etape_actuelle ENUM('Preparation', 'En entrepot', 'En transit', 'En cours de livraison', 'Livré') DEFAULT 'Preparation',
    date_livraison_prevue DATE,
    FOREIGN KEY (commande_id) REFERENCES commandes(id) ON DELETE CASCADE,
    FOREIGN KEY (transporteur_id) REFERENCES transporteurs(id)
) ENGINE=InnoDB;

-- Coupons Promotionnels Avancés
CREATE TABLE IF NOT EXISTS coupons_avance (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    code VARCHAR(20) NOT NULL UNIQUE,
    pourcentage_remise DECIMAL(5,2) NOT NULL,
    date_fin DATETIME,
    usage_max INT DEFAULT 100,
    usage_actuel INT DEFAULT 0
) ENGINE=InnoDB;

-- Gestion des Remboursements (Finance)
CREATE TABLE IF NOT EXISTS remboursements (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    paiement_id INT UNSIGNED NOT NULL,
    montant_rembourse DECIMAL(10,2) NOT NULL,
    motif TEXT,
    date_remboursement DATETIME DEFAULT CURRENT_TIMESTAMP,
    statut_remboursement ENUM('Demande', 'Approuvé', 'Effectué', 'Rejeté') DEFAULT 'Demande',
    FOREIGN KEY (paiement_id) REFERENCES paiements(id) ON DELETE CASCADE
) ENGINE=InnoDB;

-- SAV : Réclamations Clients (Différent de tickets de concert)
CREATE TABLE IF NOT EXISTS support_reclamations (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    client_id INT UNSIGNED NOT NULL,
    sujet VARCHAR(150) NOT NULL,
    message_client TEXT NOT NULL,
    priorite ENUM('Basse', 'Moyenne', 'Haute') DEFAULT 'Moyenne',
    statut_ticket ENUM('Ouvert', 'En cours', 'Résolu', 'Fermé') DEFAULT 'Ouvert',
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (client_id) REFERENCES clients(personne_id) ON DELETE CASCADE
) ENGINE=InnoDB;


--  Historique des Prix (Price Tracking)
CREATE TABLE IF NOT EXISTS historique_prix (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    produit_id INT UNSIGNED NOT NULL,
    ancien_prix DECIMAL(10,2) NOT NULL,
    nouveau_prix DECIMAL(10,2) NOT NULL,
    date_changement DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (produit_id) REFERENCES produits(id) ON DELETE CASCADE
) ENGINE=InnoDB;

-- Liste d'envies (Wishlist)
CREATE TABLE IF NOT EXISTS favoris (
    client_id INT UNSIGNED NOT NULL,
    produit_id INT UNSIGNED NOT NULL,
    date_ajout DATETIME DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (client_id, produit_id),
    FOREIGN KEY (client_id) REFERENCES clients(personne_id) ON DELETE CASCADE,
    FOREIGN KEY (produit_id) REFERENCES produits(id) ON DELETE CASCADE
) ENGINE=InnoDB;


-- ############################################################
--  INSERTIONS MASSIVES (DATA POPULATION)
-- ############################################################

-- Entrepôts
INSERT INTO entrepots (nom_entrepot, ville, adresse) VALUES 
('Stock Central Casa', 'Casablanca', 'Tit Mellil'),
('Base Tanger Med', 'Tanger', 'Zone Franche Export'),
('Logistique Souss', 'Agadir', 'Ait Melloul'),
('Entrepôt Bahja', 'Marrakech', 'Sidi Ghanem'),
('Entrepôt Atlas', 'Fès', 'Zone Industrielle Doukkarat'),
('Storage Rabat', 'Rabat', 'Technopolis Rabat-Shore'),
('Dépôt Oriental', 'Oujda', 'Zone Franche d\'Oujda'),
('Centre Logistique', 'Meknès', 'Route de Fès, Km 5'),
('Entrepôt Sahel', 'El Jadida', 'Industrielle El Jadida'),
('Stock Nord', 'Tétouan', 'Mghogha Industrial Park'),
('Entrepôt Soleil', 'Beni Mellal', ' Industrielle Beni Mellal'),
('Base Sud', 'Laâyoune', ' Industrielle Laâyoune'),
('Logistique Centre', 'Settat', ' Industrielle Settat'),
('Entrepôt Oasis', 'Ouarzazate', 'Route de Marrakech'),
('Storage Atlas', 'Ifrane', 'Parc Industriel Ifrane'),
('Dépôt Méditerranée', 'Nador', ' Franche Nador'),
('Entrepôt Royal', 'Kénitra', 'Atlantic '),
('Base Atlantique', 'Safi', ' Industrielle Safi');

-- Stock Massif
INSERT INTO stock_entrepot (entrepot_id, produit_id, quantite_dispo) VALUES 
(1, 1, 300), (1, 2, 100), (1, 3, 50), (1, 5, 200),
(2, 4, 15), (2, 6, 500), (2, 7, 120), (2, 10, 50),
(3, 1, 100), (3, 5, 600), (3, 8, 150), (3, 9, 20),
(4, 1, 40), (4, 3, 10), (4, 6, 100), (4, 10, 400);

-- Transporteurs
INSERT INTO transporteurs (nom_societe, telephone, tarif_base, delai_estime_jours) VALUES 
('Aramex Morocco', '0522112233', 35.00, 2),
('Amana Express', '0802006060', 25.00, 3),
('DHL International', '0522445566', 70.00, 1),
('Ghazala Messagerie', '0537778899', 20.00, 5),
('CatLogistics', '0522009988', 30.00, 2),
('MDS Transports', '0801112223', 28.00, 3),
('UpS Maroc', '0522778899', 55.00, 2),
('Express Cargo', '0533334455', 22.00, 4),
('City Logistique', '0522990011', 18.00, 5),
('FedEx Morocco', '0522667788', 80.00, 1),
('Trans Azur', '0533221144', 15.00, 6),
('Rapid Deliv', '0800556677', 32.00, 2),
('Maroc Logistique', '0522334455', 40.00, 3),
('Speed Cargo', '0800778899', 45.00, 2);

-- Coupons Divers
INSERT INTO coupons_avance (code, pourcentage_remise, date_fin, usage_max, usage_actuel) VALUES 
('MAROC2026', 20.00, '2026-12-31', 1000, 450),
('SOLDE_MAI', 15.00, '2026-05-31', 200, 190),
('VIP_EXCLUSIVE', 40.00, '2026-12-31', 20, 5),
('LIVRAISON_OFFERTE', 100.00, '2026-06-30', 500, 320);

-- Wishlist Bien Remplie
INSERT INTO favoris (client_id, produit_id) VALUES 
(1, 3), (1, 4), (1, 6), (1, 7), (1, 9), (1, 10);

-- Réclamations SAV
INSERT INTO support_reclamations (client_id, sujet, message_client, priorite, statut_ticket) VALUES 
(1, 'Retard Logistique', 'Le colis est bloqué au centre de tri.', 'Haute', 'En cours'),
(1, 'Question Produit', 'Le Samsung est-il double SIM ?', 'Basse', 'Résolu'),
(1, 'Promo Invalide', 'Le code MAROC2026 ne passe pas.', 'Moyenne', 'Ouvert');

-- Historique Prix
INSERT INTO historique_prix (produit_id, ancien_prix, nouveau_prix) VALUES 
(1, 150.00, 103.20),
(3, 9999.00, 8999.00),
(4, 25000.00, 22000.00),
(10, 200.00, 150.00);

-- ############################################################
-- 3. VUE DE SYNTHÈSE (ADMIN DASHBOARD)
-- ############################################################

CREATE OR REPLACE VIEW v_suivi_premium AS
SELECT 
    c.id AS 'N_Commande',
    p.nom AS 'Nom_Client',
    t.nom_societe AS 'Livreur',
    s.code_suivi AS 'Tracking_ID',
    s.etape_actuelle AS 'Etape',
    s.date_livraison_prevue AS 'Prevu_le',
    DATEDIFF(s.date_livraison_prevue, CURDATE()) AS 'Jours_Restants'
FROM commandes c
JOIN clients cl ON c.client_id = cl.personne_id
JOIN personnes p ON cl.personne_id = p.id
LEFT JOIN suivis_livraison s ON c.id = s.commande_id
LEFT JOIN transporteurs t ON s.transporteur_id = t.id;

-- ============================================================
--  EXTENSIONS STRATÉGIQUES : ANALYTICS ET EXPANSION
-- ============================================================

USE eshop_db;

-- TABLE : fournisseurs (Suppliers Management)
-- Rôle : Gérer les entreprises qui nous vendent les produits (B2B)
CREATE TABLE fournisseurs (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    nom_societe VARCHAR(100) NOT NULL,
    contact_nom VARCHAR(50),
    email VARCHAR(100),
    telephone VARCHAR(20),
    ville VARCHAR(50),
    categorie_fournie VARCHAR(100), -- Ex: 'Électronique', 'Textile'
    note_fiabilite TINYINT DEFAULT 5 -- Note de 1 à 5
) ENGINE=InnoDB;

--  TABLE : campagnes_marketing (Marketing Campaigns)
-- Rôle : Suivre les publicités (Facebook Ads, Google Ads) et leur impact
CREATE TABLE campagnes_marketing (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    nom_campagne VARCHAR(100) NOT NULL,
    budget_alloue DECIMAL(10,2),
    date_debut DATE,
    date_fin DATE,
    canal ENUM('Social Media', 'Email', 'Search Engine', 'TV'),
    clics_generes INT DEFAULT 0,
    conversions_ventes INT DEFAULT 0
) ENGINE=InnoDB;

-- TABLE : taxes_regionales (Regional Tax Management)
-- Rôle : Gérer la TVA et taxes selon la ville ou le pays (Expansion internationale)
CREATE TABLE taxes_regionales (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    nom_zone VARCHAR(50) NOT NULL, -- Ex: 'Maroc-Zone-Nord', 'Europe'
    taux_tva DECIMAL(5,2) NOT NULL,
    taxe_douane DECIMAL(5,2) DEFAULT 0.00,
    est_active TINYINT(1) DEFAULT 1
) ENGINE=InnoDB;

-- TABLE : logs_activite_admin (Admin Audit Logs)
-- Rôle : Tracer qui (Staff) a modifié quoi dans la base de données (Sécurité)
CREATE TABLE logs_activite_admin (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    admin_id INT UNSIGNED,
    action_type VARCHAR(50), -- Ex: 'UPDATE_PRICE', 'DELETE_USER'
    table_affectee VARCHAR(50),
    date_action DATETIME DEFAULT CURRENT_TIMESTAMP,
    details_action TEXT
) ENGINE=InnoDB;

-- TABLE : abonnements_premium (Subscription Model)
-- Rôle : Gérer les clients VIP qui paient un abonnement mensuel (Comme Amazon Prime)
CREATE TABLE abonnements_premium (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    client_id INT UNSIGNED NOT NULL,
    type_plan ENUM('Mensuel', 'Annuel') DEFAULT 'Mensuel',
    date_debut DATE,
    date_fin DATE,
    avantages TEXT, -- Ex: 'Livraison gratuite, Remises -20%'
    statut_paiement ENUM('Actif', 'Expire', 'Annule') DEFAULT 'Actif',
    CONSTRAINT fk_premium_client FOREIGN KEY (client_id) REFERENCES clients(personne_id) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================================
-- 📥 INSERTIONS MASSIVES (8 LIGNES PAR TABLE)
-- ============================================================

-- Fournisseurs
INSERT INTO fournisseurs (nom_societe, contact_nom, email, telephone, ville, categorie_fournie) VALUES 
('TechDistri SA', 'Anas Alami', 'contact@techdistri.ma', '0522114477', 'Casablanca', 'Électronique'),
('Maroc Textile', 'Sanaa Bennani', 's.bennani@textile.ma', '0539102030', 'Tanger', 'Mode'),
('AgroExport Souss', 'Omar Souss', 'omar@agroexport.ma', '0528556677', 'Agadir', 'Alimentation'),
('Global Furniture', 'David Levy', 'info@globalfurniture.com', '+1202555012', 'New York', 'Maison'),
('Livreurs d''Orient', 'Yassine Jaber', 'yassine@orient.ma', '0524334455', 'Marrakech', 'Livres'),
('Beauty Pro', 'Hind Karami', 'hind@beautypro.ma', '0522889900', 'Casablanca', 'Beauté'),
('Sport Equip', 'Karim Sport', 'karim@sportequip.ma', '0537001122', 'Rabat', 'Sport'),
('EuroParts', 'Jean Dupont', 'j.dupont@europarts.eu', '+331405060', 'Paris', 'Auto');

-- Campagnes Marketing
INSERT INTO campagnes_marketing (nom_campagne, budget_alloue, date_debut, date_fin, canal, clics_generes, conversions_ventes) VALUES 
('Soldes Ramadan', 5000.00, '2026-03-01', '2026-04-01', 'Social Media', 12500, 450),
('Back to School', 3500.00, '2026-08-15', '2026-09-15', 'Search Engine', 8900, 210),
('Promo Black Friday', 12000.00, '2026-11-20', '2026-11-30', 'Social Media', 45000, 1800),
('Lancement iPhone', 8000.00, '2026-05-10', '2026-06-10', 'Email', 5600, 120),
('Flash Summer', 2500.00, '2026-07-01', '2026-07-15', 'Social Media', 15000, 300),
('New Year Expo', 6000.00, '2026-12-25', '2027-01-05', 'TV', 2500, 50),
('Eco-Friendly Week', 1500.00, '2026-04-20', '2026-04-27', 'Email', 3200, 95),
('Influencer Collab', 4000.00, '2026-06-01', '2026-06-30', 'Social Media', 22000, 550);

-- Taxes Régionales
INSERT INTO taxes_regionales (nom_zone, taux_tva, taxe_douane) VALUES 
('Maroc - TVA Standard', 20.00, 0.00),
('Maroc - TVA Réduite', 7.00, 0.00),
('Zone Europe', 20.00, 15.00),
('USA / Canada', 10.00, 20.00),
('Zone Afrique Ouest', 18.00, 5.00),
('Zone Moyen Orient', 5.00, 10.00),
('Exonéré (Export)', 0.00, 0.00),
('Produits Médicaux', 7.00, 2.00);

-- Logs Admin
INSERT INTO logs_activite_admin (admin_id, action_type, table_affectee, details_action) VALUES 
(1, 'UPDATE', 'produits', 'Changement de prix MacBook Pro'),
(1, 'DELETE', 'avis', 'Suppression commentaire inapproprié'),
(2, 'INSERT', 'stocks', 'Réception de 500 unités iPhone'),
(3, 'UPDATE', 'clients', 'Changement de statut en Premium (ID 45)'),
(1, 'BAN', 'utilisateurs', 'Bannissement pour tentative de fraude'),
(2, 'EXPORT', 'ventes', 'Exportation du rapport financier Avril'),
(1, 'UPDATE', 'config', 'Modification des frais de port (Tanger)'),
(3, 'INSERT', 'coupons', 'Création code promo EID2026');

-- Abonnements Premium
INSERT INTO abonnements_premium (client_id, type_plan, date_debut, date_fin, avantages) VALUES 
(1, 'Mensuel', '2026-01-01', '2026-02-01', 'Livraison Gratuite'),
(2, 'Annuel', '2026-01-10', '2027-01-10', 'Remise -15% sur tout'),
(3, 'Annuel', '2026-02-15', '2027-02-15', 'Accès anticipé aux ventes'),
(4, 'Mensuel', '2026-03-01', '2026-04-01', 'Support client prioritaire 24/7'),
(5, 'Annuel', '2026-04-20', '2027-04-20', 'Cadeau surprise chaque mois'),
(6, 'Mensuel', '2026-05-01', '2026-06-01', 'Retours gratuits illimités'),
(7, 'Annuel', '2026-01-05', '2027-01-05', 'Accès Lounge VIP'),
(8, 'Mensuel', '2026-05-05', '2026-06-05', 'Doublement des points fidélité');

