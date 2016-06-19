<?php
if (is_file('./inc/config.inc')){
    header('location: ./web/load/');
}
echo "Please fill information from inc/config.inc"
?>