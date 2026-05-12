const mysql = require('mysql2');
const express = require('express');
const app = express();

const connection = mysql.createConnection({
  host: 'localhost',
  user: 'root',      
  password: 'PASSWORD_DIAL_MYSQL', 
  database: 'eShop_DB' 
});

connection.connect((err) => {
  if (err) throw err;
  console.log('Connecté à la base de données locale eShop_DB !');
});

app.listen(3000, () => {
  console.log('Serveur Node.js tourne sur le port 3000');
});
