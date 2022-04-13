<?php
    //echo $_GET["firstname"];
    //setcookie("FirstName", $_GET["firstname"]);
    //setcookie("LastName", $_GET["lastname"]);
    
    if(count($_COOKIE) > 0) {
    echo "Cookies are enabled. <br>";
    } else {
    echo "Cookies are disabled. <br>";
    }

?>