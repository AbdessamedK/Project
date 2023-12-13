<!--
ce fichier est vide, il faudra y mettre du code pour que la page d'accueil soit en MVC
-->
	<?php
		$nb = countInstances($connexion, "Series");
		if($nb <= 0)
			$message = "Aucune série n'a été trouvée dans la base de données !";
		else
			$message = "Actuellement $nb séries dans la base.";

	?>




