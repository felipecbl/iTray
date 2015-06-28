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

		$response = array(
			success => true,
			angle => $angle,
			message => "Angle set successfully"
		);

		// echo $angle;
		echo json_encode($response);


	}catch(Exception $e){
		// echo 'false';
		// echo $e->getMessage();

		$response = array(
			success => true,
			message => $e->getMessage()
		);

		echo json_encode($response);
		die();
	}
?>
