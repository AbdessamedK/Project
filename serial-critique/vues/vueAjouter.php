<h2>Ajout d'une série</h2>

<form method="post" action="#">
	<label for="nomSerie">Nom de la série : </label>
	<input type="text" name="nomSerie" id="nomSerie" placeholder="Orange is the new black" required />
	<br/><br/>
	<input type="submit" name="boutonValider" value="Ajouter"/>
</form>

<?php if(isset($message)) { ?>
	<p style="background-color: yellow;"><?= $message ?></p>
<?php } ?>

