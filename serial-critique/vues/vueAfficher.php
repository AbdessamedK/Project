<?php if(isset($message)) { ?>
	<p style="background-color: yellow;"><?= $message ?></p>
<?php } ?>
			
<h2>Liste des séries :</h2>
<ul>
<?php foreach($series as $serie) { ?>
	<li><?= $serie['nomSerie'] ?></li>
<?php } ?>
</ul>

<h2>Liste des actrices :</h2>
<ul>
<?php foreach($actrices as $actrice) { ?>
	<li><?= $actrice['prenom'] ?> <?= $actrice['nom'] ?> (#<?= $actrice['numINSEE'] ?>)</li>
<?php } ?>
</ul>

<h2>Liste des critiques :</h2>
<ul>
<?php foreach($critiques as $critique) { ?>
	<li><?= $critique['texte'] ?></li>
<?php } ?>
</ul>

<h2>Titre des épisodes numérotés 1 :</h2>
<ul>
<?php foreach($episodes1 as $episode) { ?>
	<li><?= $episode['titre'] ?></li>
<?php } ?>
</ul>

<h2>Titre des épisodes numérotés 2 :</h2>
<ul>
<?php foreach($episodes2 as $episode) { ?>
	<li><?= $episode['titre'] ?></li>
<?php } ?>
</ul>

