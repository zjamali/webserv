<?php
   if(isset($_FILES['file'])){
      $errors= array();
      $file_name = $_FILES['file']['name'];
      $file_tmp =$_FILES['file']['tmp_name'];
      if(empty($errors)==true){
         move_uploaded_file($file_tmp,"../upload/".$file_name);
         echo "the file with name $file_name uploaded successfuly";
      }else{
         print_r($errors);
      }
   }
?>