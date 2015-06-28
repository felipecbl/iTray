<?php
	require_once('database.php');

	try{
		$results = $db->query('select * from wheel');


	}catch(Exception $e){
		echo $e->getMessage();
		die();
	}

	// $wheel = $results->fetchAll(PDO::FETCH_ASSOC)[0]; //first and only row
?>

<!DOCTYPE html PUBLIC"-//W3C//DTD XHTML 1.0 Strict//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta content="text/html;charset=utf-8" http-equiv="Content-Type">
	<meta content="utf-8" http-equiv="encoding">
	<title>Remote Rotating Tray</title>
	  <link rel="stylesheet" type="text/css" href="css/style.css" media="all">
</head>
<body>

<!-- <a href="#" id="test">Set current angle</a> -->
<div class="container">
	<input class="knob" data-displayInput=false data-width="100%" data-linecap=round data-displayPrevious=true data-displayPrevious=true data-cursor=true value=<? echo $wheel["current_angle"]; ?> >
</div>

<?php
	// echo 'Wheel on DB => Device Connected: '.$wheel["tray_connected"].', User connected: '.$wheel["user_connected"].', Last angle: '.$wheel["last_angle"].' Current angle: '.$wheel["current_angle"].' id: '.$wheel["id"];
?>
<script src="//code.jquery.com/jquery-1.11.3.min.js"></script>
<script src="js/jquery.knob.min.js"></script>

<script>
	(function(){

			function setAngle(angle){
				var data = {
						angle: angle
					};

				request = $.ajax({
					url: "setWheel.php",
					type: "post",
					data: data,
					success: function(response, textStatus, jqXHR){
				        console.log(response);
					},
					error: function(jqXHR, textStatus, errorThrown){
				        console.error("The following error occurred: " + textStatus, errorThrown );
					}
				});
			}

		$(".knob").knob({
			'min':0,
			'max':359,
		    change : function (value) {
		        //console.log("change : " + value);
		    },
		    release : function (value) {
		        //console.log(this.$.attr('value'));
		        console.log("release : " + value);
		        setAngle(value);
		    },
		    cancel : function () {
		        console.log("cancel : ", this);
		    },
		    /*format : function (value) {
		        return value + '%';
		    },*/
		    draw : function () {

		    }
		});

	})();
</script>

</body>
</html>
