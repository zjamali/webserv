<?php
    setcookie("FirstName", $_GET["firstname"], time() + (86400 * 30));
    setcookie("LastName", $_GET["lastname"], time() + (86400 * 30));
    if(count($_COOKIE) > 0) {
    echo "Cookies are enabled. <br>";
    } else {
    echo "Cookies are disabled. <br>";
    }
?>