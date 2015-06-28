<?php
	require_once('database.php');

	try{
		if(!$db){
		      echo $db->lastErrorMsg();
		   } else {
		      // echo "Opened database successfully";
		   }

		$results = $db->query('select * from wheel where id = 1');
		$wheel = ($results->fetch(PDO::FETCH_ASSOC));

		date_default_timezone_set("UTC");
		// echo date("Y-m-d H:i:s", time());

		// Default angle value
		$angle = -1;

		// Current angle set by device
		if(isset($_GET['angle'])){
			$angle = $_GET['angle'];
			// echo "Device's angle: ". $angle;
		}

		// Set tray last angle from device

		$wheelSql = "UPDATE wheel SET last_angle = " . $angle . " WHERE id = 1";

		$db->query($wheelSql);

		// Save device info to DB
		$sql = "INSERT INTO connections (date_time, user_ip, status) VALUES ('" . time() . "', '" . $_SERVER['REMOTE_ADDR'] ."', 1)";
		$db->query($sql);

		// Create response as JSON
		if(isset($_GET['json'])){
			$response = array(
				success => true,
	            "current_angle" => $wheel["current_angle"],
	            "tray_connected" => $wheel["tray_connected"],
	            "user_connected" => $wheel["user_connected"],
	            "last_angle" => $wheel["last_angle"]
			);

			echo json_encode($response);
		}else{
			echo $wheel["current_angle"];
		}

	}catch(Exception $e){
		// echo 'false';
		// echo $e->getMessage();

		if(isset($_GET['json'])){

			$responseError = array(
				"success" => false,
				"message" => $e->getMessage()
			);

			echo json_encode($responseError);
		}else{
			echo false;
		}
		die();
	}
?>
