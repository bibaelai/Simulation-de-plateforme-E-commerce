-- MySQL dump 10.13  Distrib 8.0.45, for Win64 (x86_64)
--
-- Host: localhost    Database: eshop
-- ------------------------------------------------------
-- Server version	8.0.45

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `alimentations`
--

DROP TABLE IF EXISTS `alimentations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `alimentations` (
  `produit_id` int unsigned NOT NULL,
  `date_expiration` date NOT NULL,
  `poids_kg` decimal(6,3) NOT NULL,
  `calories_per_100g` int NOT NULL DEFAULT '0',
  `bio` tinyint(1) NOT NULL DEFAULT '0',
  `allergenes` text COLLATE utf8mb4_unicode_ci,
  `pays_origine` varchar(80) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'Maroc',
  PRIMARY KEY (`produit_id`),
  CONSTRAINT `fk_alimentation_produit` FOREIGN KEY (`produit_id`) REFERENCES `produits` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `alimentations`
--

LOCK TABLES `alimentations` WRITE;
/*!40000 ALTER TABLE `alimentations` DISABLE KEYS */;
INSERT INTO `alimentations` VALUES (1,'2025-12-01',0.200,0,0,'Aucun','Maroc'),(2,'2025-11-15',0.800,0,0,'Coton','Turquie'),(3,'2026-05-20',0.180,0,0,'Aucun','Corée du Sud'),(4,'2026-01-10',1.200,0,0,'Aucun','USA'),(5,'2026-12-31',0.750,884,1,'Aucun','Maroc'),(6,'2027-01-01',0.300,0,0,'Papier','France'),(7,'2025-06-15',0.450,0,0,'Aucun','Vietnam'),(8,'2025-09-20',1.500,0,0,'Latex','Allemagne'),(9,'2026-03-10',25.000,0,0,'Aucun','Suède'),(10,'2025-07-01',0.100,0,0,'Aucun','Maroc'),(11,'2025-12-25',2.400,0,0,'Aucun','Chine'),(12,'2026-04-18',0.160,0,0,'Aucun','USA'),(13,'2025-10-30',2.100,0,0,'Aucun','USA');
/*!40000 ALTER TABLE `alimentations` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `articles_panier`
--

DROP TABLE IF EXISTS `articles_panier`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `articles_panier` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `panier_id` int unsigned NOT NULL,
  `produit_id` int unsigned NOT NULL,
  `quantite` int NOT NULL DEFAULT '1',
  `taille_choisie` varchar(10) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `couleur_choisie` varchar(30) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uq_panier_produit` (`panier_id`,`produit_id`),
  KEY `fk_article_produit` (`produit_id`),
  CONSTRAINT `fk_article_panier` FOREIGN KEY (`panier_id`) REFERENCES `paniers` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_article_produit` FOREIGN KEY (`produit_id`) REFERENCES `produits` (`id`) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT `articles_panier_chk_1` CHECK ((`quantite` > 0))
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `articles_panier`
--

LOCK TABLES `articles_panier` WRITE;
/*!40000 ALTER TABLE `articles_panier` DISABLE KEYS */;
/*!40000 ALTER TABLE `articles_panier` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `campagnes_marketing`
--

DROP TABLE IF EXISTS `campagnes_marketing`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `campagnes_marketing` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `nom_campagne` varchar(100) COLLATE utf8mb4_unicode_ci NOT NULL,
  `budget_alloue` decimal(10,2) DEFAULT NULL,
  `date_debut` date DEFAULT NULL,
  `date_fin` date DEFAULT NULL,
  `canal` enum('Social Media','Email','Search Engine','TV') COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `clics_generes` int DEFAULT '0',
  `conversions_ventes` int DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `campagnes_marketing`
--

LOCK TABLES `campagnes_marketing` WRITE;
/*!40000 ALTER TABLE `campagnes_marketing` DISABLE KEYS */;
INSERT INTO `campagnes_marketing` VALUES (1,'Soldes Ramadan',5000.00,'2026-03-01','2026-04-01','Social Media',12500,450),(2,'Back to School',3500.00,'2026-08-15','2026-09-15','Search Engine',8900,210),(3,'Promo Black Friday',12000.00,'2026-11-20','2026-11-30','Social Media',45000,1800),(4,'Lancement iPhone',8000.00,'2026-05-10','2026-06-10','Email',5600,120),(5,'Flash Summer',2500.00,'2026-07-01','2026-07-15','Social Media',15000,300),(6,'New Year Expo',6000.00,'2026-12-25','2027-01-05','TV',2500,50),(7,'Eco-Friendly Week',1500.00,'2026-04-20','2026-04-27','Email',3200,95),(8,'Influencer Collab',4000.00,'2026-06-01','2026-06-30','Social Media',22000,550);
/*!40000 ALTER TABLE `campagnes_marketing` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `clients`
--

DROP TABLE IF EXISTS `clients`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `clients` (
  `personne_id` int unsigned NOT NULL,
  `adresse` text COLLATE utf8mb4_unicode_ci,
  `est_prime` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`personne_id`),
  CONSTRAINT `fk_client_personne` FOREIGN KEY (`personne_id`) REFERENCES `personnes` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `clients`
--

LOCK TABLES `clients` WRITE;
/*!40000 ALTER TABLE `clients` DISABLE KEYS */;
/*!40000 ALTER TABLE `clients` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `commandes`
--

DROP TABLE IF EXISTS `commandes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `commandes` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `client_id` int unsigned NOT NULL,
  `date_commande` date NOT NULL,
  `adresse_livraison` text COLLATE utf8mb4_unicode_ci NOT NULL,
  `etat` enum('En attente','Confirmée','En cours de livraison','Livrée','Annulée') COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'En attente',
  `frais_livraison` decimal(8,2) NOT NULL DEFAULT '25.00',
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `fk_commande_client` (`client_id`),
  CONSTRAINT `fk_commande_client` FOREIGN KEY (`client_id`) REFERENCES `clients` (`personne_id`) ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `commandes`
--

LOCK TABLES `commandes` WRITE;
/*!40000 ALTER TABLE `commandes` DISABLE KEYS */;
/*!40000 ALTER TABLE `commandes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `commentaires`
--

DROP TABLE IF EXISTS `commentaires`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `commentaires` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `produit_id` int unsigned NOT NULL,
  `auteur` varchar(100) COLLATE utf8mb4_unicode_ci NOT NULL,
  `texte` text COLLATE utf8mb4_unicode_ci NOT NULL,
  `etoiles` tinyint NOT NULL,
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `fk_commentaire_produit` (`produit_id`),
  CONSTRAINT `fk_commentaire_produit` FOREIGN KEY (`produit_id`) REFERENCES `produits` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `commentaires_chk_1` CHECK ((`etoiles` between 1 and 5))
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `commentaires`
--

LOCK TABLES `commentaires` WRITE;
/*!40000 ALTER TABLE `commentaires` DISABLE KEYS */;
/*!40000 ALTER TABLE `commentaires` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `coupons_avance`
--

DROP TABLE IF EXISTS `coupons_avance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `coupons_avance` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `code` varchar(20) COLLATE utf8mb4_unicode_ci NOT NULL,
  `pourcentage_remise` decimal(5,2) NOT NULL,
  `date_fin` datetime DEFAULT NULL,
  `usage_max` int DEFAULT '100',
  `usage_actuel` int DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `code` (`code`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `coupons_avance`
--

LOCK TABLES `coupons_avance` WRITE;
/*!40000 ALTER TABLE `coupons_avance` DISABLE KEYS */;
INSERT INTO `coupons_avance` VALUES (1,'MAROC2026',20.00,'2026-12-31 00:00:00',1000,450),(2,'SOLDE_MAI',15.00,'2026-05-31 00:00:00',200,190),(3,'VIP_EXCLUSIVE',40.00,'2026-12-31 00:00:00',20,5),(4,'LIVRAISON_OFFERTE',100.00,'2026-06-30 00:00:00',500,320);
/*!40000 ALTER TABLE `coupons_avance` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `electroniques`
--

DROP TABLE IF EXISTS `electroniques`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `electroniques` (
  `produit_id` int unsigned NOT NULL,
  `marque` varchar(100) COLLATE utf8mb4_unicode_ci NOT NULL,
  `garantie_mois` int NOT NULL DEFAULT '12',
  `puissance_watts` decimal(8,2) DEFAULT '0.00',
  `tension_volts` decimal(6,2) DEFAULT '220.00',
  `sous_type` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `ram_go` int DEFAULT NULL,
  `os` varchar(50) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `processeur` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `camera_mp` int DEFAULT NULL,
  `support_5g` tinyint(1) DEFAULT NULL,
  `taille_ecran` decimal(4,1) DEFAULT NULL,
  `cpu` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `gpu` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `stockage_go` int DEFAULT NULL,
  `type_stockage` varchar(20) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `type_ordi` varchar(30) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`produit_id`),
  CONSTRAINT `fk_electronique_produit` FOREIGN KEY (`produit_id`) REFERENCES `produits` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `electroniques`
--

LOCK TABLES `electroniques` WRITE;
/*!40000 ALTER TABLE `electroniques` DISABLE KEYS */;
INSERT INTO `electroniques` VALUES (1,'Samsung',24,25.00,220.00,'Smartphone',12,'Android','Snapdragon 8 Gen 3',200,1,6.8,'Octa-core','Adreno 750',256,'UFS 4.0','Mobile'),(2,'Apple',12,20.00,220.00,'Smartphone',8,'iOS','A17 Pro',48,1,6.1,'Hexa-core','Apple GPU',128,'NVMe','Mobile'),(3,'Dell',36,65.00,19.50,'Ordinateur',16,'Windows 11','Intel i7',2,0,15.6,'i7-13700H','RTX 4060',512,'SSD','Laptop'),(4,'HP',24,45.00,19.00,'Ordinateur',8,'Windows 11','AMD Ryzen 5',1,0,14.0,'Ryzen 5600U','Radeon Graphics',512,'SSD','Laptop'),(5,'Lenovo',24,65.00,20.00,'Ordinateur',16,'Windows 11','Intel i5',1,1,14.0,'i5-12450H','Intel Graphics',512,'SSD','Laptop'),(6,'Asus',24,240.00,20.00,'Ordinateur',32,'Windows 11','Intel i9',2,1,16.0,'i9-14900HX','RTX 4080',1024,'SSD','Laptop'),(7,'Xiaomi',12,120.00,220.00,'Smartphone',12,'Android','Dimensity 9200',50,1,6.6,'Octa-core','Immortalis',512,'UFS','Mobile'),(8,'Sony',24,350.00,220.00,'Console',16,'Custom','Zen 2',0,0,0.0,'Custom CPU','Custom RDNA',825,'SSD','Console'),(9,'Huawei',12,66.00,220.00,'Smartphone',8,'HarmonyOS','Kirin 9000',50,1,6.8,'Octa-core','Maleoon',256,'UFS','Mobile'),(10,'MSI',36,180.00,19.50,'Ordinateur',16,'Windows 11','Intel i7',1,1,17.3,'i7-13620H','RTX 4050',1024,'SSD','Laptop'),(11,'Acer',24,90.00,19.00,'Ordinateur',8,'Windows 11','Intel i5',1,0,15.6,'i5-11400H','RTX 3050',512,'SSD','Laptop'),(12,'Google',24,30.00,220.00,'Smartphone',8,'Android','Tensor G3',50,1,6.2,'Nona-core','Titan M2',128,'UFS','Mobile'),(13,'Razer',12,230.00,20.00,'Ordinateur',32,'Windows 11','Intel i9',2,1,14.0,'i9-13950HX','RTX 4090',1024,'SSD','Laptop');
/*!40000 ALTER TABLE `electroniques` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `entrepots`
--

DROP TABLE IF EXISTS `entrepots`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `entrepots` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `nom_entrepot` varchar(100) COLLATE utf8mb4_unicode_ci NOT NULL,
  `ville` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL,
  `adresse` text COLLATE utf8mb4_unicode_ci,
  `capacite_max` int DEFAULT '10000',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `entrepots`
--

LOCK TABLES `entrepots` WRITE;
/*!40000 ALTER TABLE `entrepots` DISABLE KEYS */;
INSERT INTO `entrepots` VALUES (1,'Stock Central Casa','Casablanca','Tit Mellil',10000),(2,'Base Tanger Med','Tanger','Zone Franche Export',10000),(3,'Logistique Souss','Agadir','Ait Melloul',10000),(4,'Entrepôt Bahja','Marrakech','Sidi Ghanem',10000);
/*!40000 ALTER TABLE `entrepots` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `favoris`
--

DROP TABLE IF EXISTS `favoris`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `favoris` (
  `client_id` int unsigned NOT NULL,
  `produit_id` int unsigned NOT NULL,
  `date_ajout` datetime DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`client_id`,`produit_id`),
  KEY `produit_id` (`produit_id`),
  CONSTRAINT `favoris_ibfk_1` FOREIGN KEY (`client_id`) REFERENCES `clients` (`personne_id`) ON DELETE CASCADE,
  CONSTRAINT `favoris_ibfk_2` FOREIGN KEY (`produit_id`) REFERENCES `produits` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `favoris`
--

LOCK TABLES `favoris` WRITE;
/*!40000 ALTER TABLE `favoris` DISABLE KEYS */;
/*!40000 ALTER TABLE `favoris` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fournisseurs`
--

DROP TABLE IF EXISTS `fournisseurs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `fournisseurs` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `nom_societe` varchar(100) COLLATE utf8mb4_unicode_ci NOT NULL,
  `contact_nom` varchar(50) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `email` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `telephone` varchar(20) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `ville` varchar(50) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `categorie_fournie` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `note_fiabilite` tinyint DEFAULT '5',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `fournisseurs`
--

LOCK TABLES `fournisseurs` WRITE;
/*!40000 ALTER TABLE `fournisseurs` DISABLE KEYS */;
INSERT INTO `fournisseurs` VALUES (1,'TechDistri SA','Anas Alami','contact@techdistri.ma','0522114477','Casablanca','Électronique',5),(2,'Maroc Textile','Sanaa Bennani','s.bennani@textile.ma','0539102030','Tanger','Mode',5),(3,'AgroExport Souss','Omar Souss','omar@agroexport.ma','0528556677','Agadir','Alimentation',5),(4,'Global Furniture','David Levy','info@globalfurniture.com','+1202555012','New York','Maison',5),(5,'Livreurs d\'Orient','Yassine Jaber','yassine@orient.ma','0524334455','Marrakech','Livres',5),(6,'Beauty Pro','Hind Karami','hind@beautypro.ma','0522889900','Casablanca','Beauté',5),(7,'Sport Equip','Karim Sport','karim@sportequip.ma','0537001122','Rabat','Sport',5),(8,'EuroParts','Jean Dupont','j.dupont@europarts.eu','+331405060','Paris','Auto',5);
/*!40000 ALTER TABLE `fournisseurs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `historique_prix`
--

DROP TABLE IF EXISTS `historique_prix`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `historique_prix` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `produit_id` int unsigned NOT NULL,
  `ancien_prix` decimal(10,2) NOT NULL,
  `nouveau_prix` decimal(10,2) NOT NULL,
  `date_changement` datetime DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `produit_id` (`produit_id`),
  CONSTRAINT `historique_prix_ibfk_1` FOREIGN KEY (`produit_id`) REFERENCES `produits` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `historique_prix`
--

LOCK TABLES `historique_prix` WRITE;
/*!40000 ALTER TABLE `historique_prix` DISABLE KEYS */;
/*!40000 ALTER TABLE `historique_prix` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `lignes_commande`
--

DROP TABLE IF EXISTS `lignes_commande`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `lignes_commande` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `commande_id` int unsigned NOT NULL,
  `produit_id` int unsigned NOT NULL,
  `quantite` int NOT NULL,
  `prix_unitaire` decimal(10,2) NOT NULL,
  `taille_choisie` varchar(10) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `couleur_choisie` varchar(30) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_ligne_commande` (`commande_id`),
  KEY `fk_ligne_produit` (`produit_id`),
  CONSTRAINT `fk_ligne_commande` FOREIGN KEY (`commande_id`) REFERENCES `commandes` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_ligne_produit` FOREIGN KEY (`produit_id`) REFERENCES `produits` (`id`) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT `lignes_commande_chk_1` CHECK ((`quantite` > 0))
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `lignes_commande`
--

LOCK TABLES `lignes_commande` WRITE;
/*!40000 ALTER TABLE `lignes_commande` DISABLE KEYS */;
/*!40000 ALTER TABLE `lignes_commande` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `livres`
--

DROP TABLE IF EXISTS `livres`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `livres` (
  `produit_id` int unsigned NOT NULL,
  `auteur` varchar(150) COLLATE utf8mb4_unicode_ci NOT NULL,
  `isbn` varchar(20) COLLATE utf8mb4_unicode_ci NOT NULL,
  `nombre_pages` int NOT NULL,
  `editeur` varchar(100) COLLATE utf8mb4_unicode_ci NOT NULL,
  `langue` varchar(30) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'Français',
  `annee_publication` year NOT NULL,
  `genre` varchar(80) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'Roman',
  PRIMARY KEY (`produit_id`),
  CONSTRAINT `fk_livre_produit` FOREIGN KEY (`produit_id`) REFERENCES `produits` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `livres`
--

LOCK TABLES `livres` WRITE;
/*!40000 ALTER TABLE `livres` DISABLE KEYS */;
INSERT INTO `livres` VALUES (1,'Auteur Un','978-1-11',150,'Editeur A','Français',2024,'Mode'),(2,'Auteur Deux','978-2-22',200,'Editeur B','Anglais',2023,'Style'),(3,'Tech Expert','978-3-33',120,'Samsung Pub','Arabe',2025,'Guide'),(4,'Apple Team','978-4-44',95,'Apple Press','Français',2024,'Manuel'),(5,'Chef Bio','978-5-55',180,'Maroc Edit','Arabe',2022,'Cuisine'),(6,'Antoine de Saint-Exupéry','978-2-07-040850-4',96,'Gallimard','Français',1943,'Roman'),(7,'Sport Pro','978-7-77',60,'Fitness Mag','Anglais',2023,'Sport'),(8,'Zen Master','978-8-88',210,'Life Co','Français',2021,'Santé'),(9,'Deco Home','978-9-99',300,'IKEA Pub','Français',2020,'Déco'),(10,'Music Team','978-10-0',45,'Culture Press','Arabe',2024,'Culture'),(11,'Gamer Dev','978-11-1',85,'Gaming Edit','Français',2023,'Tech'),(12,'Google AI','978-12-2',40,'Google Pub','Anglais',2024,'Informatique'),(13,'Razer Lab','978-13-3',70,'Blade Press','Français',2023,'Hardware');
/*!40000 ALTER TABLE `livres` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `logs_activite_admin`
--

DROP TABLE IF EXISTS `logs_activite_admin`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `logs_activite_admin` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `admin_id` int unsigned DEFAULT NULL,
  `action_type` varchar(50) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `table_affectee` varchar(50) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `date_action` datetime DEFAULT CURRENT_TIMESTAMP,
  `details_action` text COLLATE utf8mb4_unicode_ci,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `logs_activite_admin`
--

LOCK TABLES `logs_activite_admin` WRITE;
/*!40000 ALTER TABLE `logs_activite_admin` DISABLE KEYS */;
INSERT INTO `logs_activite_admin` VALUES (1,1,'UPDATE','produits','2026-05-10 02:47:52','Changement de prix MacBook Pro'),(2,1,'DELETE','avis','2026-05-10 02:47:52','Suppression commentaire inapproprié'),(3,2,'INSERT','stocks','2026-05-10 02:47:52','Réception de 500 unités iPhone'),(4,3,'UPDATE','clients','2026-05-10 02:47:52','Changement de statut en Premium (ID 45)'),(5,1,'BAN','utilisateurs','2026-05-10 02:47:52','Bannissement pour tentative de fraude'),(6,2,'EXPORT','ventes','2026-05-10 02:47:52','Exportation du rapport financier Avril'),(7,1,'UPDATE','config','2026-05-10 02:47:52','Modification des frais de port (Tanger)'),(8,3,'INSERT','coupons','2026-05-10 02:47:52','Création code promo EID2026');
/*!40000 ALTER TABLE `logs_activite_admin` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `maison_decor`
--

DROP TABLE IF EXISTS `maison_decor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `maison_decor` (
  `produit_id` int unsigned NOT NULL,
  `piece` varchar(80) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `style` varchar(80) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `matiere` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `dimensions` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `marque` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`produit_id`),
  CONSTRAINT `fk_maison_produit` FOREIGN KEY (`produit_id`) REFERENCES `produits` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `maison_decor`
--

LOCK TABLES `maison_decor` WRITE;
/*!40000 ALTER TABLE `maison_decor` DISABLE KEYS */;
INSERT INTO `maison_decor` VALUES (1,'Salon','Moderne','Coton','M','Zara Home'),(2,'Chambre','Classique','Denim','L','Levi\'s Casa'),(3,'Bureau','Futuriste','Métal','15 inch','Samsung Design'),(4,'Bureau','Minimaliste','Aluminium','13 inch','Apple Style'),(5,'Cuisine','Rustique','Verre','750ml','Maroc Tradition'),(6,'Bibliothèque','Vintage','Papier','A5','Gallimard Deco'),(7,'Jardin','Sportif','Cuir','Taille 5','Nike Garden'),(8,'Salon','Zen','Caoutchouc','180x60cm','Adidas Home'),(9,'Salon','Scandinave','Tissu','3 Places','IKEA'),(10,'Studio','Artistique','Bois','100x50cm','Festival Art'),(11,'Bureau','Gaming','Simili-cuir','Large','Razer Home'),(12,'Bureau','Ergonomique','Bois/Acier','160x80cm','Logitech Desk'),(13,'Chambre','Ambiance','Plastique','Small','Philips Hue');
/*!40000 ALTER TABLE `maison_decor` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `paiements`
--

DROP TABLE IF EXISTS `paiements`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `paiements` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `commande_id` int unsigned NOT NULL,
  `mode` enum('Carte','PayPal') COLLATE utf8mb4_unicode_ci NOT NULL,
  `montant` decimal(10,2) NOT NULL,
  `reference` varchar(20) COLLATE utf8mb4_unicode_ci NOT NULL,
  `est_valide` tinyint(1) NOT NULL DEFAULT '0',
  `titulaire` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `email_paypal` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `commande_id` (`commande_id`),
  UNIQUE KEY `reference` (`reference`),
  CONSTRAINT `fk_paiement_commande` FOREIGN KEY (`commande_id`) REFERENCES `commandes` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `paiements`
--

LOCK TABLES `paiements` WRITE;
/*!40000 ALTER TABLE `paiements` DISABLE KEYS */;
/*!40000 ALTER TABLE `paiements` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `produits`
--

DROP TABLE IF EXISTS `produits`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `produits` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `nom` varchar(150) COLLATE utf8mb4_unicode_ci NOT NULL,
  `prix` decimal(10,2) NOT NULL,
  `prix_original` decimal(10,2) NOT NULL,
  `stock` int NOT NULL DEFAULT '0',
  `note` decimal(3,2) NOT NULL DEFAULT '0.00',
  `categorie` enum('Vetement','Electronique','Alimentation','Livre','Sport & Fitness','Maison','Ticket') COLLATE utf8mb4_unicode_ci NOT NULL,
  `statut` enum('Disponible','Rupture de stock','Archive') COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'Disponible',
  `en_promotion` tinyint(1) NOT NULL DEFAULT '0',
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  CONSTRAINT `produits_chk_1` CHECK ((`prix` >= 0)),
  CONSTRAINT `produits_chk_2` CHECK ((`prix_original` >= 0)),
  CONSTRAINT `produits_chk_3` CHECK ((`stock` >= 0)),
  CONSTRAINT `produits_chk_4` CHECK ((`note` between 0 and 5))
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `produits`
--

LOCK TABLES `produits` WRITE;
/*!40000 ALTER TABLE `produits` DISABLE KEYS */;
INSERT INTO `produits` VALUES (1,'T-shirt Classic',103.20,129.00,50,4.50,'Vetement','Disponible',1,'2026-05-10 23:30:58','2026-05-10 23:30:58'),(2,'Jean Slim Fit',399.00,399.00,30,5.00,'Vetement','Disponible',0,'2026-05-10 23:30:58','2026-05-10 23:30:58'),(3,'Samsung Galaxy S25',8999.00,8999.00,20,5.00,'Electronique','Disponible',0,'2026-05-10 23:30:58','2026-05-10 23:30:58'),(4,'MacBook Pro M3',22000.00,22000.00,10,0.00,'Electronique','Disponible',0,'2026-05-10 23:30:58','2026-05-10 23:30:58'),(5,'Huile d\'olive extra vierge',45.00,45.00,500,0.00,'Alimentation','Disponible',0,'2026-05-10 23:30:58','2026-05-10 23:30:58'),(6,'Le Petit Prince',79.00,79.00,200,0.00,'Livre','Disponible',0,'2026-05-10 23:30:58','2026-05-10 23:30:58'),(7,'Ballon de Football Nike',199.00,199.00,100,0.00,'Sport & Fitness','Disponible',0,'2026-05-10 23:30:58','2026-05-10 23:30:58'),(8,'Tapis de Yoga Adidas',289.00,289.00,60,0.00,'Sport & Fitness','Disponible',0,'2026-05-10 23:30:58','2026-05-10 23:30:58'),(9,'Canape Scandinave 3 Places',3500.00,3500.00,5,0.00,'Maison','Disponible',0,'2026-05-10 23:30:58','2026-05-10 23:30:58'),(10,'Concert Gnawa Festival',150.00,150.00,500,0.00,'Ticket','Disponible',0,'2026-05-10 23:30:58','2026-05-10 23:30:58'),(11,'Acer Nitro V',8500.00,8500.00,15,4.20,'Electronique','Disponible',0,'2026-05-10 23:30:58','2026-05-10 23:30:58'),(12,'Google Pixel 8',7500.00,8000.00,12,4.80,'Electronique','Disponible',1,'2026-05-10 23:30:58','2026-05-10 23:30:58'),(13,'Razer Blade 14',25000.00,25000.00,5,5.00,'Electronique','Disponible',0,'2026-05-10 23:30:58','2026-05-10 23:30:58');
/*!40000 ALTER TABLE `produits` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `remboursements`
--

DROP TABLE IF EXISTS `remboursements`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `remboursements` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `paiement_id` int unsigned NOT NULL,
  `montant_rembourse` decimal(10,2) NOT NULL,
  `motif` text COLLATE utf8mb4_unicode_ci,
  `date_remboursement` datetime DEFAULT CURRENT_TIMESTAMP,
  `statut_remboursement` enum('Demande','Approuvé','Effectué','Rejeté') COLLATE utf8mb4_unicode_ci DEFAULT 'Demande',
  PRIMARY KEY (`id`),
  KEY `paiement_id` (`paiement_id`),
  CONSTRAINT `remboursements_ibfk_1` FOREIGN KEY (`paiement_id`) REFERENCES `paiements` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `remboursements`
--

LOCK TABLES `remboursements` WRITE;
/*!40000 ALTER TABLE `remboursements` DISABLE KEYS */;
/*!40000 ALTER TABLE `remboursements` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sport_fitness`
--

DROP TABLE IF EXISTS `sport_fitness`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `sport_fitness` (
  `produit_id` int unsigned NOT NULL,
  `sport` varchar(80) COLLATE utf8mb4_unicode_ci NOT NULL,
  `marque` varchar(100) COLLATE utf8mb4_unicode_ci NOT NULL,
  `materiau` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `poids_kg` decimal(6,3) DEFAULT '0.000',
  `niveau_utilisateur` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'Tous niveaux',
  PRIMARY KEY (`produit_id`),
  CONSTRAINT `fk_sport_produit` FOREIGN KEY (`produit_id`) REFERENCES `produits` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sport_fitness`
--

LOCK TABLES `sport_fitness` WRITE;
/*!40000 ALTER TABLE `sport_fitness` DISABLE KEYS */;
INSERT INTO `sport_fitness` VALUES (1,'Football','Nike','Synthétique',0.450,'Amateur'),(2,'Yoga','Adidas','Caoutchouc',1.200,'Tous niveaux'),(3,'Tennis','Wilson','Graphite',0.300,'Professionnel'),(4,'Basketball','Spalding','Cuir',0.600,'Avancé'),(5,'Fitness','Decathlon','Fonte',5.000,'Débutant'),(6,'Natation','Speedo','Silicone',0.050,'Amateur'),(7,'Cyclisme','Shimano','Aluminium',0.250,'Avancé'),(8,'Boxe','Everlast','Cuir',0.800,'Tous niveaux'),(9,'Running','Puma','Mesh',0.280,'Amateur'),(10,'Golf','Titleist','Titane',0.400,'Professionnel'),(11,'Camping','Quechua','Polyester',2.500,'Débutant'),(12,'Musculation','Domyos','Acier',10.000,'Avancé'),(13,'Pilates','Reebok','Mousse',0.900,'Tous niveaux');
/*!40000 ALTER TABLE `sport_fitness` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `stock_entrepot`
--

DROP TABLE IF EXISTS `stock_entrepot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stock_entrepot` (
  `entrepot_id` int unsigned NOT NULL,
  `produit_id` int unsigned NOT NULL,
  `quantite_dispo` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`entrepot_id`,`produit_id`),
  KEY `produit_id` (`produit_id`),
  CONSTRAINT `stock_entrepot_ibfk_1` FOREIGN KEY (`entrepot_id`) REFERENCES `entrepots` (`id`) ON DELETE CASCADE,
  CONSTRAINT `stock_entrepot_ibfk_2` FOREIGN KEY (`produit_id`) REFERENCES `produits` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `stock_entrepot`
--

LOCK TABLES `stock_entrepot` WRITE;
/*!40000 ALTER TABLE `stock_entrepot` DISABLE KEYS */;
/*!40000 ALTER TABLE `stock_entrepot` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `suivis_livraison`
--

DROP TABLE IF EXISTS `suivis_livraison`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `suivis_livraison` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `commande_id` int unsigned NOT NULL,
  `transporteur_id` int unsigned DEFAULT NULL,
  `code_suivi` varchar(50) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `etape_actuelle` enum('Preparation','En entrepot','En transit','En cours de livraison','Livré') COLLATE utf8mb4_unicode_ci DEFAULT 'Preparation',
  `date_livraison_prevue` date DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `commande_id` (`commande_id`),
  UNIQUE KEY `code_suivi` (`code_suivi`),
  KEY `transporteur_id` (`transporteur_id`),
  CONSTRAINT `suivis_livraison_ibfk_1` FOREIGN KEY (`commande_id`) REFERENCES `commandes` (`id`) ON DELETE CASCADE,
  CONSTRAINT `suivis_livraison_ibfk_2` FOREIGN KEY (`transporteur_id`) REFERENCES `transporteurs` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `suivis_livraison`
--

LOCK TABLES `suivis_livraison` WRITE;
/*!40000 ALTER TABLE `suivis_livraison` DISABLE KEYS */;
/*!40000 ALTER TABLE `suivis_livraison` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `support_reclamations`
--

DROP TABLE IF EXISTS `support_reclamations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `support_reclamations` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `client_id` int unsigned NOT NULL,
  `sujet` varchar(150) COLLATE utf8mb4_unicode_ci NOT NULL,
  `message_client` text COLLATE utf8mb4_unicode_ci NOT NULL,
  `priorite` enum('Basse','Moyenne','Haute') COLLATE utf8mb4_unicode_ci DEFAULT 'Moyenne',
  `statut_ticket` enum('Ouvert','En cours','Résolu','Fermé') COLLATE utf8mb4_unicode_ci DEFAULT 'Ouvert',
  `created_at` datetime DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `client_id` (`client_id`),
  CONSTRAINT `support_reclamations_ibfk_1` FOREIGN KEY (`client_id`) REFERENCES `clients` (`personne_id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `support_reclamations`
--

LOCK TABLES `support_reclamations` WRITE;
/*!40000 ALTER TABLE `support_reclamations` DISABLE KEYS */;
INSERT INTO `support_reclamations` VALUES (1,1,'Retard Logistique','Le colis est bloqué au centre de tri.','Haute','En cours','2026-05-10 02:44:31'),(2,1,'Question Produit','Le Samsung est-il double SIM ?','Basse','Résolu','2026-05-10 02:44:31'),(3,1,'Promo Invalide','Le code MAROC2026 ne passe pas.','Moyenne','Ouvert','2026-05-10 02:44:31');
/*!40000 ALTER TABLE `support_reclamations` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `taxes_regionales`
--

DROP TABLE IF EXISTS `taxes_regionales`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `taxes_regionales` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `nom_zone` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL,
  `taux_tva` decimal(5,2) NOT NULL,
  `taxe_douane` decimal(5,2) DEFAULT '0.00',
  `est_active` tinyint(1) DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `taxes_regionales`
--

LOCK TABLES `taxes_regionales` WRITE;
/*!40000 ALTER TABLE `taxes_regionales` DISABLE KEYS */;
INSERT INTO `taxes_regionales` VALUES (1,'Maroc - TVA Standard',20.00,0.00,1),(2,'Maroc - TVA Réduite',7.00,0.00,1),(3,'Zone Europe',20.00,15.00,1),(4,'USA / Canada',10.00,20.00,1),(5,'Zone Afrique Ouest',18.00,5.00,1),(6,'Zone Moyen Orient',5.00,10.00,1),(7,'Exonéré (Export)',0.00,0.00,1),(8,'Produits Médicaux',7.00,2.00,1);
/*!40000 ALTER TABLE `taxes_regionales` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tickets`
--

DROP TABLE IF EXISTS `tickets`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tickets` (
  `produit_id` int unsigned NOT NULL,
  `evenement` varchar(150) COLLATE utf8mb4_unicode_ci NOT NULL,
  `lieu` varchar(150) COLLATE utf8mb4_unicode_ci NOT NULL,
  `date_evenement` date NOT NULL,
  `type_place` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'Standard',
  `places_restantes` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`produit_id`),
  CONSTRAINT `fk_ticket_produit` FOREIGN KEY (`produit_id`) REFERENCES `produits` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tickets`
--

LOCK TABLES `tickets` WRITE;
/*!40000 ALTER TABLE `tickets` DISABLE KEYS */;
/*!40000 ALTER TABLE `tickets` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `transporteurs`
--

DROP TABLE IF EXISTS `transporteurs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `transporteurs` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `nom_societe` varchar(100) COLLATE utf8mb4_unicode_ci NOT NULL,
  `telephone` varchar(15) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `tarif_base` decimal(8,2) DEFAULT '30.00',
  `delai_estime_jours` int DEFAULT '3',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `transporteurs`
--

LOCK TABLES `transporteurs` WRITE;
/*!40000 ALTER TABLE `transporteurs` DISABLE KEYS */;
INSERT INTO `transporteurs` VALUES (1,'Aramex Morocco','0522112233',35.00,2),(2,'Amana Express','0802006060',25.00,3),(3,'DHL International','0522445566',70.00,1),(4,'Ghazala Messagerie','0537778899',20.00,5),(5,'CatLogistics','0522009988',30.00,2),(6,'Aramex Morocco','0522112233',35.00,2),(7,'Amana Express','0802006060',25.00,3),(8,'DHL International','0522445566',70.00,1),(9,'Ghazala Messagerie','0537778899',20.00,5),(10,'CatLogistics','0522009988',30.00,2);
/*!40000 ALTER TABLE `transporteurs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary view structure for view `v_commandes_detail`
--

DROP TABLE IF EXISTS `v_commandes_detail`;
/*!50001 DROP VIEW IF EXISTS `v_commandes_detail`*/;
SET @saved_cs_client     = @@character_set_client;
/*!50503 SET character_set_client = utf8mb4 */;
/*!50001 CREATE VIEW `v_commandes_detail` AS SELECT 
 1 AS `commande_id`,
 1 AS `date_commande`,
 1 AS `etat`,
 1 AS `frais_livraison`,
 1 AS `prenom`,
 1 AS `nom`,
 1 AS `email`,
 1 AS `sous_total`,
 1 AS `total`,
 1 AS `mode_paiement`,
 1 AS `ref_paiement`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_produits_complet`
--

DROP TABLE IF EXISTS `v_produits_complet`;
/*!50001 DROP VIEW IF EXISTS `v_produits_complet`*/;
SET @saved_cs_client     = @@character_set_client;
/*!50503 SET character_set_client = utf8mb4 */;
/*!50001 CREATE VIEW `v_produits_complet` AS SELECT 
 1 AS `id`,
 1 AS `nom`,
 1 AS `prix`,
 1 AS `prix_original`,
 1 AS `stock`,
 1 AS `note`,
 1 AS `categorie`,
 1 AS `statut`,
 1 AS `en_promotion`,
 1 AS `pct_promo`,
 1 AS `nb_avis`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_suivi_premium`
--

DROP TABLE IF EXISTS `v_suivi_premium`;
/*!50001 DROP VIEW IF EXISTS `v_suivi_premium`*/;
SET @saved_cs_client     = @@character_set_client;
/*!50503 SET character_set_client = utf8mb4 */;
/*!50001 CREATE VIEW `v_suivi_premium` AS SELECT 
 1 AS `N_Commande`,
 1 AS `Nom_Client`,
 1 AS `Livreur`,
 1 AS `Tracking_ID`,
 1 AS `Etape`,
 1 AS `Prevu_le`,
 1 AS `Jours_Restants`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `vendeurs`
--

DROP TABLE IF EXISTS `vendeurs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `vendeurs` (
  `personne_id` int unsigned NOT NULL,
  `nom_boutique` varchar(100) COLLATE utf8mb4_unicode_ci NOT NULL,
  `chiffre_affaires` decimal(12,2) NOT NULL DEFAULT '0.00',
  PRIMARY KEY (`personne_id`),
  CONSTRAINT `fk_vendeur_personne` FOREIGN KEY (`personne_id`) REFERENCES `personnes` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `vendeurs`
--

LOCK TABLES `vendeurs` WRITE;
/*!40000 ALTER TABLE `vendeurs` DISABLE KEYS */;
/*!40000 ALTER TABLE `vendeurs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `vetements`
--

DROP TABLE IF EXISTS `vetements`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `vetements` (
  `produit_id` int unsigned NOT NULL,
  `taille` enum('XS','S','M','L','XL','XXL') COLLATE utf8mb4_unicode_ci NOT NULL,
  `couleur` enum('Rouge','Bleu','Vert','Noir','Blanc','Gris','Jaune','Rose','Orange','Violet','Marron','Beige') COLLATE utf8mb4_unicode_ci NOT NULL,
  `matiere` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `marque` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `est_neuf` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`produit_id`),
  CONSTRAINT `fk_vetement_produit` FOREIGN KEY (`produit_id`) REFERENCES `produits` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `vetements`
--

LOCK TABLES `vetements` WRITE;
/*!40000 ALTER TABLE `vetements` DISABLE KEYS */;
/*!40000 ALTER TABLE `vetements` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Final view structure for view `v_commandes_detail`
--

/*!50001 DROP VIEW IF EXISTS `v_commandes_detail`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_commandes_detail` AS select `co`.`id` AS `commande_id`,`co`.`date_commande` AS `date_commande`,`co`.`etat` AS `etat`,`co`.`frais_livraison` AS `frais_livraison`,`pe`.`prenom` AS `prenom`,`pe`.`nom` AS `nom`,`pe`.`email` AS `email`,sum((`lc`.`quantite` * `lc`.`prix_unitaire`)) AS `sous_total`,(sum((`lc`.`quantite` * `lc`.`prix_unitaire`)) + `co`.`frais_livraison`) AS `total`,`pa`.`mode` AS `mode_paiement`,`pa`.`reference` AS `ref_paiement` from ((((`commandes` `co` join `clients` `cl` on((`cl`.`personne_id` = `co`.`client_id`))) join `personnes` `pe` on((`pe`.`id` = `cl`.`personne_id`))) left join `lignes_commande` `lc` on((`lc`.`commande_id` = `co`.`id`))) left join `paiements` `pa` on((`pa`.`commande_id` = `co`.`id`))) group by `co`.`id`,`pe`.`prenom`,`pe`.`nom`,`pe`.`email`,`co`.`date_commande`,`co`.`etat`,`co`.`frais_livraison`,`pa`.`mode`,`pa`.`reference` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_produits_complet`
--

/*!50001 DROP VIEW IF EXISTS `v_produits_complet`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_produits_complet` AS select `p`.`id` AS `id`,`p`.`nom` AS `nom`,`p`.`prix` AS `prix`,`p`.`prix_original` AS `prix_original`,`p`.`stock` AS `stock`,`p`.`note` AS `note`,`p`.`categorie` AS `categorie`,`p`.`statut` AS `statut`,`p`.`en_promotion` AS `en_promotion`,round(((1 - (`p`.`prix` / nullif(`p`.`prix_original`,0))) * 100),0) AS `pct_promo`,(select count(0) from `commentaires` `c` where (`c`.`produit_id` = `p`.`id`)) AS `nb_avis` from `produits` `p` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_suivi_premium`
--

/*!50001 DROP VIEW IF EXISTS `v_suivi_premium`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_suivi_premium` AS select `c`.`id` AS `N_Commande`,`p`.`nom` AS `Nom_Client`,`t`.`nom_societe` AS `Livreur`,`s`.`code_suivi` AS `Tracking_ID`,`s`.`etape_actuelle` AS `Etape`,`s`.`date_livraison_prevue` AS `Prevu_le`,(to_days(`s`.`date_livraison_prevue`) - to_days(curdate())) AS `Jours_Restants` from ((((`commandes` `c` join `clients` `cl` on((`c`.`client_id` = `cl`.`personne_id`))) join `personnes` `p` on((`cl`.`personne_id` = `p`.`id`))) left join `suivis_livraison` `s` on((`c`.`id` = `s`.`commande_id`))) left join `transporteurs` `t` on((`s`.`transporteur_id` = `t`.`id`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-05-12  5:22:53
