CREATE DATABASE IF NOT EXISTS eShop_db;

USE eShop_db;

CREATE TABLE genres (
id   INT          AUTO_INCREMENT PRIMARY KEY,
nom  VARCHAR(50)  NOT NULL UNIQUE
);

CREATE TABLE produits (
id             INT            AUTO_INCREMENT PRIMARY KEY,
nom            VARCHAR(150)   NOT NULL,
prix           DECIMAL(10,2)  NOT NULL CHECK (prix >= 0),
prix_original  DECIMAL(10,2)  NOT NULL,
description    TEXT,
stock          INT            NOT NULL DEFAULT 0 CHECK (stock >= 0),
note           DECIMAL(3,2)   DEFAULT 0.00 CHECK (note BETWEEN 0 AND 5),
categorie      ENUM(
'Vêtement','Électronique','Alimentation',
'Livre','Sport & Fitness','Maison','Ticket','Ordinateur'
) NOT NULL,
statut         ENUM('Disponible','Rupture de stock','Archivé')
NOT NULL DEFAULT 'Disponible',
en_promotion   BOOLEAN        NOT NULL DEFAULT FALSE,
image_url      VARCHAR(255)   DEFAULT NULL,
created_at     TIMESTAMP      DEFAULT CURRENT_TIMESTAMP,
updated_at     TIMESTAMP      DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

CREATE TABLE livres (
id_produit   INT          PRIMARY KEY,
auteur       VARCHAR(100) NOT NULL,
isbn         VARCHAR(20),
editeur      VARCHAR(100),
langue       VARCHAR(30)  DEFAULT 'Anglais',
FOREIGN KEY (id_produit) REFERENCES produits(id) ON DELETE CASCADE
);

CREATE TABLE livre_genres (
id_livre INT NOT NULL,
id_genre INT NOT NULL,
PRIMARY KEY (id_livre, id_genre),
FOREIGN KEY (id_livre) REFERENCES livres(id_produit) ON DELETE CASCADE,
FOREIGN KEY (id_genre) REFERENCES genres(id)         ON DELETE CASCADE
);

CREATE TABLE vetements (
id_produit INT         PRIMARY KEY,
taille     ENUM('XS','S','M','L','XL','XXL') NOT NULL DEFAULT 'M',
couleur    ENUM('Rouge','Bleu','Vert','Noir','Blanc','Gris',
'Jaune','Rose','Orange','Violet','Marron','Beige')
NOT NULL DEFAULT 'Noir',
matiere    VARCHAR(80),
marque     VARCHAR(80),
FOREIGN KEY (id_produit) REFERENCES produits(id) ON DELETE CASCADE
);

CREATE TABLE electroniques (
id_produit       INT         PRIMARY KEY,
marque           VARCHAR(80) NOT NULL,
garantie_mois    INT         NOT NULL DEFAULT 12,
puissance_watts  DECIMAL(8,2) DEFAULT 0,
tension_volts    DECIMAL(6,2) DEFAULT 220,
FOREIGN KEY (id_produit) REFERENCES produits(id) ON DELETE CASCADE
);

CREATE TABLE ordinateurs (
id_produit      INT         PRIMARY KEY,
cpu             VARCHAR(100),
gpu             VARCHAR(100),
stockage_go     INT,
type_stockage   VARCHAR(20),
type_ordi       VARCHAR(30),
FOREIGN KEY (id_produit) REFERENCES electroniques(id_produit) ON DELETE CASCADE
);

CREATE TABLE alimentations (
id_produit        INT         PRIMARY KEY,
date_expiration   DATE        NOT NULL,
poids_kg          DECIMAL(6,3),
calories_per_100g INT,
bio               BOOLEAN     DEFAULT FALSE,
allergenes        VARCHAR(200) DEFAULT '',
pays_origine      VARCHAR(60)  DEFAULT 'Maroc',
FOREIGN KEY (id_produit) REFERENCES produits(id) ON DELETE CASCADE
);

CREATE TABLE sport_fitness (
id_produit         INT         PRIMARY KEY,
sport              VARCHAR(80) NOT NULL,
marque             VARCHAR(80),
materiau           VARCHAR(80),
poids_kg           DECIMAL(6,3) DEFAULT 0,
niveau_utilisateur VARCHAR(50)  DEFAULT 'Tous niveaux',
FOREIGN KEY (id_produit) REFERENCES produits(id) ON DELETE CASCADE
);

CREATE TABLE maison_decor (
id_produit  INT         PRIMARY KEY,
piece       VARCHAR(80),
style       VARCHAR(80),
matiere     VARCHAR(80),
dimensions  VARCHAR(80),
marque      VARCHAR(80),
FOREIGN KEY (id_produit) REFERENCES produits(id) ON DELETE CASCADE
);

CREATE TABLE tickets (
id_produit       INT          PRIMARY KEY,
evenement        VARCHAR(150) NOT NULL,
lieu             VARCHAR(150),
date_evenement   DATE         NOT NULL,
type_place       VARCHAR(50)  DEFAULT 'Standard',
places_restantes INT          DEFAULT 0,
FOREIGN KEY (id_produit) REFERENCES produits(id) ON DELETE CASCADE
);

CREATE TABLE commentaires (
id          INT          AUTO_INCREMENT PRIMARY KEY,
id_produit  INT          NOT NULL,
auteur      VARCHAR(100) NOT NULL,
texte       TEXT         NOT NULL,
etoiles     TINYINT      NOT NULL CHECK (etoiles BETWEEN 1 AND 5),
created_at  TIMESTAMP    DEFAULT CURRENT_TIMESTAMP,
FOREIGN KEY (id_produit) REFERENCES produits(id) ON DELETE CASCADE
);

CREATE TABLE personnes (
id           INT          AUTO_INCREMENT PRIMARY KEY,
prenom       VARCHAR(60)  NOT NULL,
nom          VARCHAR(60)  NOT NULL,
email        VARCHAR(120) NOT NULL UNIQUE,
telephone    VARCHAR(15),
mot_de_passe VARCHAR(255) NOT NULL,
role         ENUM('client','vendeur') NOT NULL,
est_connecte BOOLEAN      DEFAULT FALSE,
created_at   TIMESTAMP    DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE clients (
id_personne INT         PRIMARY KEY,
adresse     VARCHAR(250),
FOREIGN KEY (id_personne) REFERENCES personnes(id) ON DELETE CASCADE
);

CREATE TABLE vendeurs (
id_personne      INT            PRIMARY KEY,
nom_boutique     VARCHAR(120)   NOT NULL,
chiffre_affaires DECIMAL(12,2)  DEFAULT 0.00,
FOREIGN KEY (id_personne) REFERENCES personnes(id) ON DELETE CASCADE
);

CREATE TABLE vendeur_produits (
id_vendeur  INT NOT NULL,
id_produit  INT NOT NULL,
PRIMARY KEY (id_vendeur, id_produit),
FOREIGN KEY (id_vendeur) REFERENCES vendeurs(id_personne) ON DELETE CASCADE,
FOREIGN KEY (id_produit) REFERENCES produits(id)          ON DELETE CASCADE
);

CREATE TABLE paniers (
id           INT          AUTO_INCREMENT PRIMARY KEY,
id_client    INT          NOT NULL UNIQUE,
code_promo   VARCHAR(30)  DEFAULT '',
remise_promo DECIMAL(5,2) DEFAULT 0.00,
updated_at   TIMESTAMP    DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
FOREIGN KEY (id_client) REFERENCES clients(id_personne) ON DELETE CASCADE
);

CREATE TABLE articles_panier (
id_panier       INT NOT NULL,
id_produit      INT NOT NULL,
quantite        INT NOT NULL DEFAULT 1 CHECK (quantite > 0),
taille_choisie  VARCHAR(10) DEFAULT '',
couleur_choisie VARCHAR(30) DEFAULT '',
PRIMARY KEY (id_panier, id_produit),
FOREIGN KEY (id_panier)  REFERENCES paniers(id)  ON DELETE CASCADE,
FOREIGN KEY (id_produit) REFERENCES produits(id) ON DELETE CASCADE
);

CREATE TABLE commandes (
id                INT            AUTO_INCREMENT PRIMARY KEY,
id_client         INT            NOT NULL,
date_commande     DATETIME       NOT NULL DEFAULT CURRENT_TIMESTAMP,
adresse_livraison VARCHAR(250)   NOT NULL,
frais_livraison   DECIMAL(8,2)   NOT NULL DEFAULT 25.00,
statut_commande   ENUM('En attente','Confirmée','Expédiée','Livrée','Annulée')
NOT NULL DEFAULT 'En attente',
FOREIGN KEY (id_client) REFERENCES clients(id_personne)
);

CREATE TABLE commande_produits (
id_commande   INT NOT NULL,
id_produit    INT NOT NULL,
quantite      INT NOT NULL DEFAULT 1,
prix_unitaire DECIMAL(10,2) NOT NULL,
PRIMARY KEY (id_commande, id_produit),
FOREIGN KEY (id_commande) REFERENCES commandes(id)   ON DELETE CASCADE,
FOREIGN KEY (id_produit)  REFERENCES produits(id)
);

CREATE TABLE paiements (
id                  INT          AUTO_INCREMENT PRIMARY KEY,
id_commande         INT          NOT NULL UNIQUE,
mode                ENUM('Carte','PayPal') NOT NULL,
montant             DECIMAL(10,2) NOT NULL,
reference           VARCHAR(20)  NOT NULL UNIQUE,
valide              BOOLEAN      NOT NULL DEFAULT FALSE,
numero_carte_masque VARCHAR(19)  DEFAULT NULL,
titulaire           VARCHAR(100) DEFAULT NULL,
date_expiration     VARCHAR(5)   DEFAULT NULL,
email_paypal        VARCHAR(120) DEFAULT NULL,
created_at          TIMESTAMP    DEFAULT CURRENT_TIMESTAMP,
FOREIGN KEY (id_commande) REFERENCES commandes(id) ON DELETE CASCADE
);

CREATE OR REPLACE VIEW vue_catalogue AS
SELECT
[p.id](http://p.id/), p.nom, p.prix, p.prix_original, p.description,
p.stock, p.note, p.categorie, p.statut, p.en_promotion, p.image_url,
GROUP_CONCAT(DISTINCT g.nom ORDER BY g.nom SEPARATOR ', ') AS genres
FROM produits p
LEFT JOIN livres       l  ON [p.id](http://p.id/) = l.id_produit
LEFT JOIN livre_genres lg ON l.id_produit = lg.id_livre
LEFT JOIN genres       g  ON lg.id_genre = [g.id](http://g.id/)
GROUP BY [p.id](http://p.id/);

CREATE OR REPLACE VIEW vue_livres AS
SELECT
[p.id](http://p.id/), p.nom AS titre, l.auteur, l.isbn, l.editeur, l.langue,
p.prix, p.stock, p.statut, p.image_url,
GROUP_CONCAT(g.nom ORDER BY g.nom SEPARATOR ', ') AS genres
FROM produits p
JOIN livres       l  ON [p.id](http://p.id/) = l.id_produit
LEFT JOIN livre_genres lg ON l.id_produit = lg.id_livre
LEFT JOIN genres       g  ON lg.id_genre  = [g.id](http://g.id/)
GROUP BY [p.id](http://p.id/);

CREATE OR REPLACE VIEW vue_promotions AS
SELECT id, nom, prix, prix_original,
ROUND((1 - prix/prix_original) * 100, 0) AS reduction_pct,
categorie, image_url
FROM produits
WHERE en_promotion = TRUE AND statut = 'Disponible';

INSERT INTO genres (nom) VALUES
('Roman'),
('Horror'),
('Fantasy'),
('Self Development');

INSERT INTO produits (nom, prix, prix_original, description, stock, categorie, statut, image_url) VALUES
('Silver',                              89.00,  89.00, 'Un roman d''horreur signé Ira Levin.',                                    15, 'Livre', 'Disponible', '/images/livres/silver.jpg'),
('The Mountain Is You',                 99.00,  99.00, 'Un guide de développement personnel par Brianna Wiest.',                  20, 'Livre', 'Disponible', '/images/livres/mountain_is_you.jpg'),
('The Miracle Morning',                 85.00,  85.00, 'La routine matinale qui transforme votre vie.',                           18, 'Livre', 'Disponible', '/images/livres/miracle_morning.jpg'),
('The Subtle Art of Not Giving a Fuck', 95.00,  95.00, 'Un guide anti-conformiste pour une belle vie.',                           25, 'Livre', 'Disponible', '/images/livres/subtle_art.jpg'),
('The Sky Readers',                     75.00,  75.00, 'Roman de Sue Murray.',                                                    10, 'Livre', 'Disponible', '/images/livres/sky_readers.jpg'),
('The Black Night',                     70.00,  70.00, 'Roman de Denise Kirby.',                                                  12, 'Livre', 'Disponible', '/images/livres/black_night.jpg'),
('The Key',                             70.00,  70.00, 'Roman de Denise Kirby.',                                                  12, 'Livre', 'Disponible', '/images/livres/the_key.jpg'),
('Red Waters Rising',                   90.00,  90.00, 'Roman fantastique de Laura Anne Gilman.',                                  8, 'Livre', 'Disponible', '/images/livres/red_waters.jpg'),
('Ikigai',                              95.00,  95.00, 'Le secret japonais d''une vie longue et heureuse.',                       30, 'Livre', 'Disponible', '/images/livres/ikigai.jpg'),
('Think Before It''s Too Late',         80.00,  80.00, 'Réflexions sur la pensée créative par Edward de Bono.',                  14, 'Livre', 'Disponible', '/images/livres/think_before.jpg'),
('The Scarlet Plague',                  65.00,  65.00, 'Roman post-apocalyptique de Jack London.',                                 9, 'Livre', 'Disponible', '/images/livres/scarlet_plague.jpg'),
('Girl in Pieces',                     105.00, 105.00, 'Roman émouvant de Kathleen Glasgow.',                                     11, 'Livre', 'Disponible', '/images/livres/girl_in_pieces.jpg'),
('Not the End of the World',            92.00,  92.00, 'Un regard optimiste sur l''avenir par Hannah Ritchie.',                   16, 'Livre', 'Disponible', '/images/livres/not_end.jpg'),
('All That''s Left to Say',             88.00,  88.00, 'Un roman touchant par Emery Lord.',                                       13, 'Livre', 'Disponible', '/images/livres/all_thats_left.jpg'),
('A Terrible Kindness',                 90.00,  90.00, 'Roman de Jo Browning Wroe.',                                              10, 'Livre', 'Disponible', '/images/livres/terrible_kindness.jpg'),
('How to Be Human',                     85.00,  85.00, 'Un guide humoristique par Ruby Wax.',                                     17, 'Livre', 'Disponible', '/images/livres/how_to_be_human.jpg'),
('Brave Girl Eating',                   80.00,  80.00, 'Roman de Harriet Brown.',                                                  9, 'Livre', 'Disponible', '/images/livres/brave_girl.jpg'),
('The Language of Kindness',            88.00,  88.00, 'Roman de Christie Watson.',                                               14, 'Livre', 'Disponible', '/images/livres/language_kindness.jpg');

INSERT INTO livres (id_produit, auteur, isbn, editeur, langue) VALUES
(1,  'Ira Levin',          '978-0385333481', 'Random House',           'Anglais'),
(2,  'Brianna Wiest',      '978-1949759228', 'Thought Catalog Books',  'Anglais'),
(3,  'Hal Elrod',          '978-0979019777', 'Hal Elrod International','Anglais'),
(4,  'Mark Manson',        '978-0062457714', 'HarperOne',              'Anglais'),
(5,  'Sue Murray',         NULL,             NULL,                     'Anglais'),
(6,  'Denise Kirby',       NULL,             NULL,                     'Anglais'),
(7,  'Denise Kirby',       NULL,             NULL,                     'Anglais'),
(8,  'Laura Anne Gilman',  NULL,             NULL,                     'Anglais'),
(9,  'Hector Garcia',      '978-0143130727', 'Penguin Books',          'Anglais'),
(10, 'Edward de Bono',     NULL,             NULL,                     'Anglais'),
(11, 'Jack London',        '978-0486483580', 'Dover Publications',     'Anglais'),
(12, 'Kathleen Glasgow',   '978-1101934715', 'Delacorte Press',        'Anglais'),
(13, 'Hannah Ritchie',     NULL,             'Penguin Books',          'Anglais'),
(14, 'Emery Lord',         NULL,             NULL,                     'Anglais'),
(15, 'Jo Browning Wroe',   NULL,             NULL,                     'Anglais'),
(16, 'Ruby Wax',           NULL,             NULL,                     'Anglais'),
(17, 'Harriet Brown',      NULL,             NULL,                     'Anglais'),
(18, 'Christie Watson',    '978-0525559016', 'Viking',                 'Anglais');

INSERT INTO livre_genres VALUES
(1, 2), (1, 1),
(2, 4),
(3, 4),
(4, 4),
(5, 1),
(6, 1),
(7, 1),
(8, 1),
(9, 4),
(10, 4),
(11, 3),
(12, 1),
(13, 4),
(14, 1),
(15, 1),
(16, 4), (16, 1),
(17, 1),
(18, 1);

INSERT INTO produits (nom, prix, prix_original, description, stock, categorie, statut, image_url) VALUES
('T-Shirt Basique Noir',   129.00, 129.00, 'T-shirt 100% coton, coupe droite.',          50, 'Vêtement', 'Disponible', '/images/vetements/tshirt_noir.jpg'),
('Jean Slim Bleu',         349.00, 349.00, 'Jean slim stretch, coupe moderne.',           30, 'Vêtement', 'Disponible', '/images/vetements/jean_slim.jpg'),
('Hoodie Gris Oversize',   299.00, 299.00, 'Sweat à capuche oversize en coton brossé.',  25, 'Vêtement', 'Disponible', '/images/vetements/hoodie_gris.jpg'),
('Robe Fleurie Été',       249.00, 249.00, 'Robe légère imprimé fleurs, taille S-XL.',   20, 'Vêtement', 'Disponible', '/images/vetements/robe_fleurie.jpg');

INSERT INTO vetements VALUES
(19, 'M', 'Noir', 'Coton',   'Zara'),
(20, 'L', 'Bleu', 'Denim',   'Levi''s'),
(21, 'L', 'Gris', 'Coton',   'H&M'),
(22, 'S', 'Rose', 'Viscose', 'Mango');

INSERT INTO produits (nom, prix, prix_original, description, stock, categorie, statut, image_url) VALUES
('Écouteurs Bluetooth Pro',   599.00, 599.00, 'Écouteurs sans fil avec réduction de bruit.',  40, 'Électronique', 'Disponible', '/images/electronique/ecouteurs.jpg'),
('Chargeur Rapide 65W USB-C', 199.00, 199.00, 'Charge rapide compatible tous appareils.',      60, 'Électronique', 'Disponible', '/images/electronique/chargeur.jpg');

INSERT INTO electroniques VALUES
(23, 'Sony',  24, 5,   5),
(24, 'Anker', 12, 65, 220);

INSERT INTO produits (nom, prix, prix_original, description, stock, categorie, statut, image_url) VALUES
('Laptop Gaming Asus TUF', 8999.00, 9999.00, 'PC gaming 15" — RTX 4060, 16 Go RAM, 512 Go SSD.', 10, 'Ordinateur', 'Disponible', '/images/electronique/asus_tuf.jpg');

INSERT INTO electroniques VALUES (25, 'Asus', 24, 200, 220);
INSERT INTO ordinateurs  VALUES (25, 'Intel Core i7-13700H', 'NVIDIA RTX 4060', 512, 'SSD NVMe', 'Gaming');

INSERT INTO produits (nom, prix, prix_original, description, stock, categorie, statut, image_url) VALUES
('Ballon de Football Taille 5', 199.00, 199.00, 'Ballon officiel taille 5, toutes surfaces.',         35, 'Sport & Fitness', 'Disponible', '/images/sport/ballon.jpg'),
('Tapis de Yoga Antidérapant',  249.00, 249.00, 'Tapis 6mm, matière écologique, 183x61cm.',            28, 'Sport & Fitness', 'Disponible', '/images/sport/tapis_yoga.jpg'),
('Haltères 2x5 kg',             329.00, 329.00, 'Paire d''haltères en caoutchouc, prise ergonomique.', 20, 'Sport & Fitness', 'Disponible', '/images/sport/halteres.jpg');

INSERT INTO sport_fitness VALUES
(26, 'Football',    'Adidas',   'PU synthétique', 0.43, 'Tous niveaux'),
(27, 'Yoga',        'LifeFit',  'TPE écologique', 1.20, 'Tous niveaux'),
(28, 'Musculation', 'Decathlon','Caoutchouc',     10.0, 'Tous niveaux');

INSERT INTO produits (nom, prix, prix_original, description, stock, categorie, statut, image_url) VALUES
('Lampe de Bureau LED',   349.00, 349.00, 'Lampe LED tactile, 3 températures, USB.', 22, 'Maison', 'Disponible', '/images/maison/lampe_bureau.jpg'),
('Bougie Parfumée Vanille', 89.00, 89.00, 'Bougie artisanale 200g, 40h de combustion.', 45, 'Maison', 'Disponible', '/images/maison/bougie.jpg');

INSERT INTO maison_decor VALUES
(29, 'Bureau', 'Moderne', 'Métal/Plastique', '35x15x45 cm', 'Philips'),
(30, 'Salon',  'Cosy',    'Cire de soja',    '8x8 cm',      'L''Artisan');

INSERT INTO produits (nom, prix, prix_original, description, stock, categorie, statut, image_url) VALUES
('Amandes Bio 500g',         79.00,  79.00, 'Amandes nature premium, origine Espagne.',      40, 'Alimentation', 'Disponible', '/images/alim/amandes.jpg'),
('Huile d''Argan Culinaire', 95.00,  95.00, 'Huile d''argan pure, pressée à froid, 250ml.', 30, 'Alimentation', 'Disponible', '/images/alim/argan.jpg');

INSERT INTO alimentations VALUES
(31, '2026-06-30', 0.500, 579, TRUE,  'Fruits à coque', 'Espagne'),
(32, '2026-12-31', 0.250, 884, FALSE, 'Aucun',           'Maroc');

INSERT INTO produits (nom, prix, prix_original, description, stock, categorie, statut, image_url) VALUES
('Ticket Concert Gnawa Fes 2025', 350.00, 350.00, 'Festival de Musique Sacrée du Monde, Fès.',  200, 'Ticket', 'Disponible', '/images/tickets/gnawa.jpg'),
('Ticket Match Raja vs Wydad',    150.00, 150.00, 'Classico marocain — Tribune Centrale.',       500, 'Ticket', 'Disponible', '/images/tickets/derby.jpg');

INSERT INTO tickets VALUES
(33, 'Festival de Musique Sacrée du Monde', 'Fès, Maroc',       '2025-06-20', 'Pelouse',  200),
(34, 'Raja CA vs Wydad AC',                 'Stade Mohammed V', '2025-07-05', 'Tribune',  500);

INSERT INTO produits (nom, prix, prix_original, description, stock, categorie, statut, image_url) VALUES
('Lait entier 1L',
8.50, 8.50,
'Lait entier pasteurisé, riche en calcium et en vitamines. Conditionné en bouteille plastique hermétique. Idéal pour le petit-déjeuner, les recettes sucrées et salées.',
200, 'Alimentation', 'Disponible', '/images/alim/lait_entier.jpg'),

('Pain de campagne 400g',
3.50, 3.50,
'Pain artisanal à la mie dense et à la croûte dorée, fabriqué avec de la farine complète et du levain naturel. Sans additifs ni conservateurs.',
150, 'Alimentation', 'Disponible', '/images/alim/pain_campagne.jpg'),

('Huile d''olive extra vierge 500ml',
65.00, 65.00,
'Huile d''olive pressée à froid, première extraction, d''origine marocaine (région de Meknès). Goût fruité et léger avec une belle couleur dorée. Idéale pour la cuisine et la salade.',
80, 'Alimentation', 'Disponible', '/images/alim/huile_olive.jpg'),

('Miel de thym bio 250g',
95.00, 95.00,
'Miel pur 100 % naturel récolté dans les montagnes du Haut Atlas. Goût légèrement épicé avec des arômes floraux. Non chauffé, non filtré, certifié bio.',
60, 'Alimentation', 'Disponible', '/images/alim/miel_thym.jpg'),

('Yaourt nature pack x6',
25.00, 25.00,
'Yaourt brassé au lait entier, sans colorants ni arômes artificiels. Texture crémeuse et goût doux. Pack de 6 pots de 125g. À conserver au réfrigérateur.',
120, 'Alimentation', 'Disponible', '/images/alim/yaourt_nature.jpg'),

('Jus d''orange 100% pur 1L',
22.00, 22.00,
'Jus d''orange fraîchement pressé, sans sucres ajoutés ni conservateurs. Riche en vitamine C. Fabriqué à partir d''oranges cultivées dans la région de Souss-Massa.',
100, 'Alimentation', 'Disponible', '/images/alim/jus_orange.jpg'),

('Oeufs fermiers boîte x12',
35.00, 35.00,
'Oeufs frais de poules élevées en plein air, nourries sans OGM. Coquille solide, jaune bien coloré. Catégorie A. Date de ponte indiquée sur chaque oeuf.',
90, 'Alimentation', 'Disponible', '/images/alim/oeufs_fermiers.jpg'),

('Fromage Kiri pack x8 portions',
38.00, 38.00,
'Fromage fondu à tartiner doux et crémeux, enrichi en calcium. Parfait pour les tartines, les sandwichs ou en collation. Pack de 8 portions individuelles de 17.5g.',
110, 'Alimentation', 'Disponible', '/images/alim/fromage_kiri.jpg');

INSERT INTO alimentations (id_produit, date_expiration, poids_kg, calories_per_100g, bio, allergenes, pays_origine) VALUES
(35, '2025-08-01', 1.000,  61, FALSE, 'Lait',        'Maroc'),
(36, '2025-06-20', 0.400, 245, FALSE, 'Gluten, blé', 'Maroc'),
(37, '2026-12-31', 0.500, 884, FALSE, 'Aucun',       'Maroc'),
(38, '2027-01-01', 0.250, 304, TRUE,  'Aucun',       'Maroc'),
(39, '2025-06-25', 0.750,  61, FALSE, 'Lait',        'Maroc'),
(40, '2025-06-18', 1.000,  45, FALSE, 'Aucun',       'Maroc'),
(41, '2025-07-15', 0.720, 155, FALSE, 'Aucun',       'Maroc'),
(42, '2025-10-01', 0.140, 248, FALSE, 'Lait',        'France');

INSERT INTO produits (nom, prix, prix_original, description, stock, categorie, statut, image_url) VALUES
('Clé USB SanDisk Ultra 64 Go',
120.00, 120.00,
'Clé USB 3.1 avec vitesse de lecture jusqu''à 130 Mo/s. Idéale pour stocker photos, vidéos et documents. Compacte et légère, avec capuchon de protection. Compatible Windows, Mac et Linux.',
75, 'Électronique', 'Disponible', '/images/electronique/cle_usb_sandisk64.jpg'),

('Clé USB Kingston DataTraveler 128 Go',
180.00, 180.00,
'Clé USB 3.2 Gen 1 haute capacité, vitesse de lecture 100 Mo/s et écriture 15 Mo/s. Design robuste avec capuchon. Parfaite pour transporter de grandes quantités de données au quotidien.',
50, 'Électronique', 'Disponible', '/images/electronique/cle_usb_kingston128.jpg'),

('TV Samsung 55" 4K UHD Smart TV',
5500.00, 5500.00,
'Téléviseur LED 55 pouces résolution 4K Ultra HD (3840x2160). Processeur Crystal 4K, HDR, Tizen OS avec accès Netflix/YouTube. 3 ports HDMI, 2 USB, Wi-Fi intégré, télécommande vocale incluse.',
15, 'Électronique', 'Disponible', '/images/electronique/tv_samsung_55.jpg'),

('TV Hisense 43" Full HD Smart TV',
2800.00, 2800.00,
'Téléviseur LED 43 pouces Full HD (1920x1080). VIDAA Smart OS, Dolby Audio, Micro Dimming. 2 ports HDMI, 1 USB, Wi-Fi et Bluetooth intégrés. Idéal pour chambre ou bureau.',
20, 'Électronique', 'Disponible', '/images/electronique/tv_hisense_43.jpg'),

('iPhone 15 128 Go Noir',
12000.00, 12000.00,
'Smartphone Apple iPhone 15 — écran Super Retina XDR 6.1" OLED, puce A16 Bionic, appareil photo 48 Mpx avec zoom optique, port USB-C, autonomie jusqu''à 20h. iOS 17. Coloris : Noir minuit.',
20, 'Électronique', 'Disponible', '/images/electronique/iphone15_noir.jpg'),

('Samsung Galaxy A55 5G 128 Go',
4500.00, 4500.00,
'Smartphone Samsung Android — écran Super AMOLED 6.6" 120Hz, processeur Exynos 1480, triple caméra 50+12+5 Mpx, batterie 5000 mAh avec charge rapide 25W, 8 Go RAM, résistance IP67.',
30, 'Électronique', 'Disponible', '/images/electronique/samsung_a55.jpg'),

('Chargeur rapide USB-C 65W GaN',
199.00, 199.00,
'Chargeur mural compact à technologie GaN (nitrure de gallium). Compatible Power Delivery 3.0 et Quick Charge 4+. Charge un laptop, smartphone et tablette simultanément (2 ports USB-C + 1 USB-A). Certifié CE.',
60, 'Électronique', 'Disponible', '/images/electronique/chargeur_65w.jpg'),

('Chargeur sans fil Qi 15W',
149.00, 149.00,
'Chargeur à induction 15W compatible Qi. Charge rapide pour iPhone 12 et plus, Samsung Galaxy S10+. Surface antidérapante, indicateur LED de charge. Câble USB-C inclus. Compatible avec la plupart des coques fines.',
45, 'Électronique', 'Disponible', '/images/electronique/chargeur_sansfil.jpg');

INSERT INTO electroniques (id_produit, marque, garantie_mois, puissance_watts, tension_volts) VALUES
(43, 'SanDisk',  12,   2.5,   5),
(44, 'Kingston', 12,   2.5,   5),
(45, 'Samsung',  24, 130,   220),
(46, 'Hisense',  24,  80,   220),
(47, 'Apple',    12,  20,     5),
(48, 'Samsung',  12,  25,     5),
(49, 'Anker',    12,  65,   220),
(50, 'Belkin',   12,  15,     5);

INSERT INTO produits (nom, prix, prix_original, description, stock, categorie, statut, image_url) VALUES
('Canapé 3 places velours bleu nuit',
3500.00, 3500.00,
'Canapé 3 places en velours doux bleu nuit avec pieds dorés en métal. Structure en bois massif, assise et dossier moelleux en mousse haute résilience. Dimensions : 210x85x90 cm. Facilement démontable pour transport.',
8, 'Maison', 'Disponible', '/images/maison/canape_velours_bleu.jpg'),

('Tapis berbère fait main 200x300cm',
1800.00, 1800.00,
'Tapis traditionnel marocain tissé à la main par des artisanes de Midelt. Laine naturelle mérinos, motifs géométriques berbères authentiques en noir et blanc. Résistant, doux sous les pieds, anti-dérapant.',
12, 'Maison', 'Disponible', '/images/maison/tapis_berbere.jpg'),

('Tapis Shaggy poils longs 160x230cm',
950.00, 950.00,
'Tapis moderne à poils longs (4 cm) ultra doux, coloris gris clair. Parfait pour salon ou chambre, apporte chaleur et confort. Facile à entretenir, lavable en machine. Matière : 100% microfibre polyester.',
18, 'Maison', 'Disponible', '/images/maison/tapis_shaggy_gris.jpg'),

('Rideau occultant thermique — lot x2',
280.00, 280.00,
'Lot de 2 panneaux de rideau occultant 140x260 cm. Bloque 99% de la lumière, réduit les bruits extérieurs et l''échange thermique. Coloris anthracite. Oeillets dorés, facile à poser. Lavable à 30°C.',
35, 'Maison', 'Disponible', '/images/maison/rideau_occultant.jpg'),

('Table basse bois massif naturel',
1200.00, 1200.00,
'Table basse rectangulaire en bois de thuya massif, finition naturelle huilée. Plateau épais 4 cm, pieds trapézoïdaux stables. Dimensions : 120x60x42 cm. Fabrication artisanale marocaine. Chaque pièce est unique.',
10, 'Maison', 'Disponible', '/images/maison/table_basse_thuya.jpg'),

('Étagère bibliothèque 5 niveaux blanc',
890.00, 890.00,
'Bibliothèque modulable 5 étagères en MDF blanc mat avec renforts métalliques. Dimensions : 80x180x25 cm. Charge max par étagère : 20 kg. Montage facile, quincaillerie et notice incluses. Style scandinave minimaliste.',
22, 'Maison', 'Disponible', '/images/maison/etagere_blanc.jpg');

INSERT INTO maison_decor (id_produit, piece, style, matiere, dimensions, marque) VALUES
(51, 'Salon',   'Contemporain', 'Velours, bois, métal',    '210x85x90 cm',   'Atmosphera'),
(52, 'Salon',   'Traditionnel', 'Laine mérinos naturelle', '200x300 cm',     'Artisanat Midelt'),
(53, 'Salon',   'Moderne',      'Microfibre polyester',    '160x230 cm',     'Nattiot'),
(54, 'Chambre', 'Minimaliste',  'Tissu polyester doublé',  '140x260 cm (x2)','Beautex'),
(55, 'Salon',   'Artisanal',    'Thuya massif huilé',      '120x60x42 cm',   'Artisanat Essaouira'),
(56, 'Bureau',  'Scandinave',   'MDF, métal',              '80x180x25 cm',   'Woodman');

INSERT INTO produits (nom, prix, prix_original, description, stock, categorie, statut, image_url) VALUES
('Ticket Coupe du Monde 2030 — Tribune VIP',
2500.00, 2500.00,
'Ticket officiel pour un match de groupe de la Coupe du Monde FIFA 2030, co-organisée par le Maroc, l''Espagne et le Portugal. Place en tribune VIP avec accès salon hospitalité, vue panoramique garantie et siège numéroté. Le stade et la date seront confirmés à l''attribution.',
200, 'Ticket', 'Disponible', '/images/tickets/cm2030_vip.jpg'),

('Ticket Coupe du Monde 2030 — Tribune Standard',
800.00, 800.00,
'Ticket officiel pour un match de groupe de la Coupe du Monde FIFA 2030 au Maroc. Place en tribune standard avec siège numéroté et accès aux espaces communs. Le stade et la date exacte seront confirmés par email à l''attribution des rencontres.',
1000, 'Ticket', 'Disponible', '/images/tickets/cm2030_standard.jpg'),

('Ticket Coupe du Monde 2030 — Pelouse Debout',
400.00, 400.00,
'Ticket officiel catégorie pelouse (zone debout) pour un match de groupe de la Coupe du Monde FIFA 2030 au Maroc. Ambiance garantie dans le virage réservé aux supporters. Placement libre dans la zone.',
2000, 'Ticket', 'Disponible', '/images/tickets/cm2030_pelouse.jpg'),

('Concert Oum — Scène nationale Casablanca',
350.00, 350.00,
'Ticket pour le concert live d''Oum, la voix d''or de la soul marocaine. Une soirée exceptionnelle mêlant gnawa, jazz et musiques du monde à la Scène nationale Mohamed VI de Casablanca. Placement assis numéroté. Ouverture des portes à 20h, début du concert à 21h.',
600, 'Ticket', 'Disponible', '/images/tickets/concert_oum.jpg'),

('Concert Oum — Carré Or (1ères rangées)',
600.00, 600.00,
'Ticket Carré Or pour le concert d''Oum à la Scène nationale de Casablanca. Accès aux 10 premières rangées face à la scène, expérience immersive au plus près de l''artiste. Offre limitée. Placement assis numéroté.',
100, 'Ticket', 'Disponible', '/images/tickets/concert_oum_gold.jpg'),

('Concert Sarand Ismail — Théâtre Mohammed V Rabat',
250.00, 250.00,
'Ticket pour la soirée musicale de Sarand Ismail au Théâtre Mohammed V de Rabat. Une performance live envoûtante mêlant chant oriental, musiques andalouses et sonorités contemporaines. Placement assis. Portes ouvertes à 19h30, concert à 20h30.',
500, 'Ticket', 'Disponible', '/images/tickets/concert_sarand_ismail.jpg');

INSERT INTO tickets (id_produit, evenement, lieu, date_evenement, type_place, places_restantes) VALUES
(57, 'Coupe du Monde FIFA 2030',    'Stade Hassan II, Casablanca (à confirmer)', '2030-06-15', 'Tribune VIP',      200),
(58, 'Coupe du Monde FIFA 2030',    'Stade Hassan II, Casablanca (à confirmer)', '2030-06-15', 'Tribune Standard', 1000),
(59, 'Coupe du Monde FIFA 2030',    'Stade Hassan II, Casablanca (à confirmer)', '2030-06-15', 'Pelouse Debout',   2000),
(60, 'Concert Oum — Soul du Maroc', 'Scène nationale Mohamed VI, Casablanca',    '2025-09-20', 'Assis numéroté',   600),
(61, 'Concert Oum — Carré Or',      'Scène nationale Mohamed VI, Casablanca',    '2025-09-20', 'Carré Or',         100),
(62, 'Concert Sarand Ismail',       'Théâtre Mohammed V, Rabat',                 '2025-10-11', 'Assis numéroté',   500);

UPDATE produits SET prix_original = 5500.00,  prix = 4950.00,  en_promotion = TRUE WHERE id = 45;
UPDATE produits SET prix_original = 3500.00,  prix = 2975.00,  en_promotion = TRUE WHERE id = 51;
UPDATE produits SET prix_original = 1800.00,  prix = 1584.00,  en_promotion = TRUE WHERE id = 52;
UPDATE produits SET prix_original = 12000.00, prix = 11400.00, en_promotion = TRUE WHERE id = 47;


CREATE TABLE IF NOT EXISTS produit_images (
  id          INT AUTO_INCREMENT PRIMARY KEY,
  id_produit  INT NOT NULL,
  image_url   VARCHAR(255) NOT NULL,
  ordre       TINYINT DEFAULT 1,  -- 1 = main picture
  FOREIGN KEY (id_produit) REFERENCES produits(id) ON DELETE CASCADE
);


INSERT INTO produit_images (id_produit, image_url, ordre) VALUES
(1,  '/uploads/produits/1.jpg',    1),
(2,  '/uploads/produits/2.jpeg',   1),
(3,  '/uploads/produits/3.jpg',    1),
(4,  '/uploads/produits/4.jpeg',   1),
(5,  '/uploads/produits/5.jpeg',   1),
(6,  '/uploads/produits/6.jpeg',   1),
(7,  '/uploads/produits/7.jpeg',   1),
(8,  '/uploads/produits/8.jpg',    1),
(9,  '/uploads/produits/9.jpeg',   1),
(10, '/uploads/produits/10.jpeg',  1),
(11, '/uploads/produits/11.jpeg',  1),
(12, '/uploads/produits/12.jpeg',  1),
(13, '/uploads/produits/13.png',   1),
(14, '/uploads/produits/14.jpg',   1),
(15, '/uploads/produits/15.jpeg',  1),
(16, '/uploads/produits/16.jpeg',  1),
(17, '/uploads/produits/17.jpg',   1),
(18, '/uploads/produits/18.jpeg',  1),
(19, '/uploads/produits/19.jpeg',  1),
(20, '/uploads/produits/20.jpeg',  1),
(21, '/uploads/produits/21.jpeg',  1),
(22, '/uploads/produits/22.jpeg',  1),
(23, '/uploads/produits/23.jpeg',  1),
(24, '/uploads/produits/24.jpeg',  1),
(25, '/uploads/produits/25.jpeg',  1),
(26, '/uploads/produits/26.jpeg',  1),
(27, '/uploads/produits/27.jpeg',  1),
(28, '/uploads/produits/28.jpeg',  1),
(29, '/uploads/produits/29.jpeg',  1),
(30, '/uploads/produits/30.jpeg',  1),
(31, '/uploads/produits/31.jpeg',  1),
(32, '/uploads/produits/32.jpeg',  1),
(33, '/uploads/produits/33.jpeg',  1),
(34, '/uploads/produits/34.jpeg',  1),
(35, '/uploads/produits/35.jpeg',  1),
(36, '/uploads/produits/36.jpeg',  1),
(37, '/uploads/produits/37.jpeg',  1),
(38, '/uploads/produits/38.png',   1),
(39, '/uploads/produits/39.jpeg',  1),
(40, '/uploads/produits/40.jpeg',  1),
(41, '/uploads/produits/41.jpeg',  1),
(42, '/uploads/produits/42.jpeg',  1),
(43, '/uploads/produits/43.jpeg',  1),
(44, '/uploads/produits/44.jpeg',  1),
(45, '/uploads/produits/45.jpeg',  1),
(46, '/uploads/produits/46.jpeg',  1),
(47, '/uploads/produits/47.jpeg',  1),
(48, '/uploads/produits/48.jpeg',  1),
(49, '/uploads/produits/49.jpeg',  1),
(50, '/uploads/produits/50.jpeg',  1),
(51, '/uploads/produits/51.png',   1),
(52, '/uploads/produits/52.png',   1),
(53, '/uploads/produits/53.jpeg',  1),
(54, '/uploads/produits/54.jpeg',  1),
(55, '/uploads/produits/55.jpeg',  1),
(56, '/uploads/produits/56.jpeg',  1),
(57, '/uploads/produits/57.jpeg',  1),
(58, '/uploads/produits/58.jpeg',  1),
(59, '/uploads/produits/59.jpeg',  1),
(60, '/uploads/produits/60.jpeg',  1),
(61, '/uploads/produits/61.jpeg',  1),
(62, '/uploads/produits/62.jpeg',  1);

INSERT INTO produit_images (id_produit, image_url, ordre) VALUES
(19, '/uploads/produits/19-2.jpeg', 2),
(19, '/uploads/produits/19-3.jpeg', 3),
(20, '/uploads/produits/20-2.jpeg', 2),
(20, '/uploads/produits/20-3.jpeg', 3),
(20, '/uploads/produits/20-4.jpeg', 4),
(21, '/uploads/produits/21-2.jpeg', 2),
(21, '/uploads/produits/21-3.jpeg', 3),
(23, '/uploads/produits/23-2.jpeg', 2),
(23, '/uploads/produits/23-3.jpeg', 3),
(24, '/uploads/produits/24-2.jpeg', 2),
(25, '/uploads/produits/25-2.jpeg', 2),
(25, '/uploads/produits/25-3.jpeg', 3),
(26, '/uploads/produits/26-2.jpeg', 2),
(26, '/uploads/produits/26-3.jpeg', 3),
(27, '/uploads/produits/27-2.jpeg', 2),
(27, '/uploads/produits/27-3.jpeg', 3),
(28, '/uploads/produits/28-2.jpeg', 2),
(43, '/uploads/produits/43-2.jpeg', 2),
(44, '/uploads/produits/44-2.jpeg', 2),
(44, '/uploads/produits/44-3.jpeg', 3),
(45, '/uploads/produits/45-2.jpeg', 2),
(46, '/uploads/produits/46-2.jpeg', 2),
(47, '/uploads/produits/47-2.jpeg', 2),
(47, '/uploads/produits/47-3.jpeg', 3),
(48, '/uploads/produits/48-2.jpeg', 2),
(49, '/uploads/produits/49-2.jpeg', 2),
(49, '/uploads/produits/49-3.jpeg', 3),
(50, '/uploads/produits/50-2.jpeg', 2),
(50, '/uploads/produits/50-3.jpeg', 3);

UPDATE produits p
JOIN produit_images pi ON p.id = pi.id_produit AND pi.ordre = 1
SET p.image_url = pi.image_url;
CREATE OR REPLACE VIEW vue_promotions AS
SELECT id, nom, prix, prix_original,
ROUND((1 - prix/prix_original) * 100, 0) AS reduction_pct,
categorie, image_url
FROM produits
WHERE en_promotion = TRUE AND statut = 'Disponible';
INSERT INTO clients (id_personne) 
SELECT id_personne FROM vendeurs 
WHERE id_personne NOT IN (SELECT id_personne FROM clients);

INSERT INTO paniers (id_client) 
SELECT id_personne FROM clients c
WHERE id_personne NOT IN (SELECT id_client FROM paniers);

INSERT INTO produit_images (id_produit, image_url, ordre) VALUES
(57, '/uploads/produits/57.png', 1),
(58, '/uploads/produits/58.png', 1),
(59, '/uploads/produits/59.png', 1),
(60, '/uploads/produits/60.png', 1),
(61, '/uploads/produits/61.png', 1),
(62, '/uploads/produits/62.png', 1);