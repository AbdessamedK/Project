<?php 
if(isset($_POST['boutonValider'])) { // formulaire soumis

	$nomSerie = $_POST['nomSerie']; // recuperation de la valeur saisie
	$verification = getSeriesByName($connexion, $nomSerie);

	if($verification == FALSE || count($verification) == 0) { // pas de série avec ce nom, insertion
		$insertion = insertSerie($connexion, $nomSerie);
		if($insertion == TRUE) {
			$message = "La série $nomSerie a bien été ajoutée !";
		}
		else {
			$message = "Erreur lors de l'insertion de la série $nomSerie.";
		}
	}
	else {
		$message = "Une série existe déjà avec ce nom ($nomSerie).";
	}
}

?>
