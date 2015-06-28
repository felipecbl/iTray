<?php
	require_once('database.php');

	try{
		if(!$db){
		      echo $db->lastErrorMsg();
		   } else {
		      // echo "Opened database successfully";
		   }

		$angle = $_POST['angle'];
		$results = $db->query('UPDATE wheel set current_angle = '.$angle.' WHERE id = 1');
		echo $angle;

	}catch(Exception $e){
		// echo 'false';
		echo $e->getMessage();
		die();
	}
?>
