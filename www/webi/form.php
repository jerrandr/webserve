<?php

$nom       = $_GET['nom'] ;
$prenom    = $_GET['prenom']
?>
<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
<title>Résultat du formulaire</title>
<style>
body {
  margin: 0;
  min-height: 100vh;
  background: radial-gradient(circle at top left, #1a1a1a 0%, #0f0f0f 100%);
  display: flex;
  justify-content: center;
  align-items: center;
  font-family: "Segoe UI", Arial, sans-serif;
  color: #eee;
}
.result {
  background: rgba(30,30,30,0.85);
  border: 1px solid rgba(80,80,80,0.3);
  border-radius: 12px;
  padding: 40px;
  box-shadow: 0 4px 20px rgba(0,0,0,0.4), 0 0 20px rgba(30,60,90,0.15);
  backdrop-filter: blur(6px);
  width: 320px;
  text-align: center;
}
.result h1 {
  margin-bottom: 20px;
}
.result p {
  margin: 10px 0;
  font-size: 1.1em;
}
</style>
</head>
<body>
  <div class="result">
    <h1>Données reçues</h1>
    <p><strong>Nom :</strong> <?php echo $nom; ?></p>
    <p><strong>Prénom :</strong> <?php echo $prenom; ?></p>
  </div>
</body>
</html>
