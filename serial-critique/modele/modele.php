<?php 

$today = date('Y-m-d G:i:s');

// connexion à la BD, retourne un lien de connexion
function getConnexionBD() {
	$connexion = mysqli_connect(SERVEUR, UTILISATRICE, MOTDEPASSE, BDD);
	if (mysqli_connect_errno()) {
	    printf("Échec de la connexion : %s\n", mysqli_connect_error());
	    exit();
	}
	mysqli_query($connexion,'SET NAMES UTF8'); // noms en UTF8
	return $connexion;
}

// déconnexion de la BD
function deconnectBD($connexion) {
	mysqli_close($connexion);
}

// nombre d'instances d'une table $nomTable
function countInstances($connexion, $nomTable) {
	$requete = "SELECT COUNT(*) AS nb FROM $nomTable";
	$res = mysqli_query($connexion, $requete);
	if($res != FALSE) {
		$row = mysqli_fetch_assoc($res);
		return $row['nb'];
	}
	return -1;  // valeur négative si erreur de requête (ex, $nomTable contient une valeur qui n'est pas une table)
}

// retourne les instances d'une table $nomTable
function getInstances($connexion, $nomTable) {
	$requete = "SELECT * FROM $nomTable";
	$res = mysqli_query($connexion, $requete);
	$instances = mysqli_fetch_all($res, MYSQLI_ASSOC);
	return $instances;
}

// retourne les instances d'épisodes numérotés 1 et 2 
function getEpisodesPrepared($connexion) {
	$requete = "SELECT titre FROM Episodes WHERE numero = ?";
	$stmt = mysqli_prepare($connexion, $requete);
	if($stmt == false) {
		return null;
	}
	mysqli_stmt_bind_param($stmt, "i", $numEpisode); // lier la variable $var au paramètre de la requête
	// $var à 1 pour afficher les épisodes numérotés 1
	$numEpisode = 1;
	mysqli_stmt_execute($stmt); // exécution de la requête
	$episodes1 = mysqli_stmt_get_result($stmt);  // récupération des tuples résultats dans la variable $episodes1

	// $var à 2 pour afficher les épisodes numérotés 2
	$numEpisode = 2;
	mysqli_stmt_execute($stmt); // pas besoin de lier, exécution directe de la requête
	$episodes2 = mysqli_stmt_get_result($stmt);  // récupération des tuples résultats dans la variable $episodes1
	return array("episodes1" => $episodes1, "episodes2" => $episodes2);
}

// retourne les informations sur la série nommée $nomSerie
function getSeriesByName($connexion, $nomSerie) {
	$nomSerie = mysqli_real_escape_string($connexion, $nomSerie); // sécurisation de $nomSerie
	$requete = "SELECT * FROM Series WHERE nomSerie = '". $nomSerie . "'";
	$res = mysqli_query($connexion, $requete);
	$series = mysqli_fetch_all($res, MYSQLI_ASSOC);
	return $series;
}


// insère une nouvelle série nommée $nomSerie
function insertSerie($connexion, $nomSerie) {
	$nomSerie = mysqli_real_escape_string($connexion, $nomSerie); // au cas où $nomSerie provient d'un formulaire
	$requete = "INSERT INTO Series VALUES ('". $nomSerie . "')";
	$res = mysqli_query($connexion, $requete);
	return $res;
}

// insere une critique
function insertCritique($connexion, $texte, $pseudo, $dateCritique, $nomSerie) {
	$requete = "INSERT INTO Critiques (dateCritique, pseudo, texte, nomSerie) VALUES ('$dateCritique', '$pseudo', '$texte', '$nomSerie')";
	$res = mysqli_query($connexion, $requete);
	return $res;
}


function search($connexion, $table, $valeur) {
	$valeur = mysqli_real_escape_string($connexion, $valeur); // au cas où $valeur provient d'un formulaire
	if($table == 'Series')
		$requete = 'SELECT * FROM Series WHERE nomSerie LIKE \'%'.$valeur.'%\';';
	else  // $table == 'Actrices'
		$requete = 'SELECT * FROM Actrices WHERE nom LIKE \'%'.$valeur.'%\' OR prenom LIKE \'%'.$valeur.'%\';';
	$res = mysqli_query($connexion, $requete);
	$instances = mysqli_fetch_all($res, MYSQLI_ASSOC);
	return $instances;
}




$con = getConnexionBD();
$sql = "SELECT * FROM Series";
$all_series = mysqli_query($con, $sql);

?>
