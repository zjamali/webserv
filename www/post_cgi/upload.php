<?php
var_dump($_FILES);
foreach ($_FILES as $key => $value)
{         
  if ((isset($_FILES[$key]['tmp_name']))) {
    $name = $_FILES[$key]['name'];
    echo $name;
    $dir = './../upload/';
    move_uploaded_file($_FILES[$key]['tmp_name'], $dir . "$name");
    echo "  L'image à été uploadé !";
  }
    else
        echo "L'image n'as pas été uploadé";}

?>