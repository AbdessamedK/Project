<?php 
if(isset($_POST['boutonValider'])) { // formulaire soumis

	$texte = $_POST['texte']; // recuperation de la valeur saisie
	$pseudo = $_POST['pseudo'];
	$dateCritique = $today;
	$nomSerie = $_POST['nomSerie'];
	$insertion = insertCritique($connexion, $texte, $pseudo, $dateCritique, $nomSerie);

}

?>
