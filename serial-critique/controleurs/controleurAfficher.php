<?php 
$message = "";

// recupération des séries
$series = getInstances($connexion, "Series");
if($series == null || count($series) == 0) {
	$message .= "Aucune série n'a été trouvée dans la base de données !";
}


/*
** À vous de jouer : lister les critiques en vous inspirant du code ci-dessus.
** Vous pourrez plus tard améliorer le code en affichant chaque série avec les
** critiques qui la concernent !
*/

// recupération des actrices
$actrices = getInstances($connexion, "Actrices");
if($actrices == null || count($actrices) == 0) {
	$message .= "Aucune actrice n'a été trouvée dans la base de données !";
}

// recupération des critiques
$critiques = getInstances($connexion, "Critiques");
if($critiques == null || count($series) == 0) {
	$message .= "Aucune critiques n'a été trouvée dans la base de données !";
}

// recupération des épisodes numérotés 1 et 2 avec une requête préparée
$tabEpisodes = getEpisodesPrepared($connexion);
if($tabEpisodes == null) {
	$message .= "Aucun épisode n'a été trouvé dans la base de données !";
}
else {
	$episodes1 = $tabEpisodes["episodes1"];
	$episodes2 = $tabEpisodes["episodes2"];
}

?>
