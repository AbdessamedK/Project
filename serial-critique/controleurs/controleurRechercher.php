<?php 
$connexion = getConnexionBD(); // connexion à la BD

if(isset($_POST['boutonValider'])) {
	
	$valeur = mysqli_real_escape_string($connexion, trim($_POST['valeur']));
	$table	= mysqli_real_escape_string($connexion, $_POST['champRech']);
	$resultats = search($connexion, $table, $valeur);
	if(count($resultats) == 0) {
		$message = "Aucun résultat pour cette valeur !";
	}
}
?>
