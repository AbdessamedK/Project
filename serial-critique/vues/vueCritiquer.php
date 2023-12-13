<h2>Ajout d'une critique</h2>

<form method="post" action="#">
	<label for="nomSerie">Nom de la série : </label>
	<select name="nomSerie">
            <?php
                // use a while loop to fetch data
                // from the $all_categories variable
                // and individually display as an option
                while ($series = mysqli_fetch_array(
                        $all_series,MYSQLI_ASSOC)):;
            ?>
                <option value="<?php echo $series["nomSerie"];
                    // The value we usually set is the primary key
                ?>">
                    <?php echo $series["nomSerie"];
                        // To show the category name to the user
                    ?>
                </option>
            <?php
                endwhile;
                // While loop must be terminated
            ?>
        </select>
        <br>
	<br/><br/>
	<label for="texte">Votre Critique : </label>
	<br/><br/>
	DATE : 
	<?php
	echo $today
	?>
	<br/>

	<label for="pseudo">Votre pseudo :</label>
	<input type="text" name="pseudo" id="pseudo" required />
	<br/>

	<label for="texte">Votre Critique :</label>
	<input type="text" name="texte" id="texte" required />
	<br/><br/>

	<input type="submit" name="boutonValider" value="Ajouter"/>
</form>

<?php if(isset($message)) { ?>
	<p style="background-color: yellow;"><?= $message ?></p>
<?php } ?>

