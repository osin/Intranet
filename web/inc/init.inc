<?php
// <editor-fold defaultstate="collapsed" desc="inclusion des classes de models">

require 'config.inc';

setlocale(LC_ALL, 'fr_FR');
//if (strtolower($_SERVER['REQUEST_URI']) == strtolower(SITE_PATH))
//  header("location:" . SITE_PATH . 'load');

$connections = array(
    Environment => DB_DRIVER.'://'.DB_USERNAME.':'.DB_PASSWORD.'@'.DB_SERVER.'/'.DB_DATABASE.'?charset='.DB_CHARSET,
);

require_once 'initialize.inc';
ActiveRecord\Config::initialize(function($cfg) use ($connections) {
          $cfg->set_model_directory(MODEL_PATH);
          $cfg->set_connections($connections);
          $cfg->set_date_format("Y-m-d H:i:s");
          $cfg->set_default_connection(Environment);
        });
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="definition de res GET et POST (hors block)">
$res = Template::instance('template');
/*
 * Passage des posts et get en tant que variable
 */
if (is_array($_POST)) {
  foreach ($_POST as $key => $var) {
    global $$key;
    $$key = $var;
  }
}
if (is_array($_GET)) {
  foreach ($_GET as $key => $var) {
    global $$key;
    $$key = $var;
  }
}

// <editor-fold defaultstate="collapsed" desc="session">
/*
 * to use test controlleur and test view, prefix the name by dev.*
 */
session_name('Wotoog');
session_start();
if (isset($_SESSION['Wotoog']) || isset($res->user)) {
  $res->user = $_SESSION['Wotoog'];
//  if(!$res->user->is_admin > 0)
//@Todo: fix BUG here
//    if(str_word_count('|'.$res->getScope().'|', ADMIN_SCOPE) == 0)
//      $res->load(DEFAULT_TEMPLATE.'/index');
}else {
  if (!(stripos($_SERVER['SCRIPT_NAME'], "index.php")) && !stripos($_SERVER['SCRIPT_NAME'], "suscribe.php") && !stripos($_SERVER['SCRIPT_FILENAME'], "service/") && !stripos($_SERVER['SCRIPT_FILENAME'], "dev.")) {
    $res->user = null;
    $redirect = explode('/', $_SERVER['SCRIPT_NAME']);
    $redirect = $redirect[count($redirect) - 1];
    $redirect = stripos($_SERVER['SCRIPT_NAME'], ".php") ? substr($redirect, 0, strlen($redirect) - 4) : $redirect;
    $args = isset($_GET) ? $_GET : null;
    $res->load('web/index', array('signin' => 1, 'redirect' => $redirect, 'args' => $args));
  }
}
// </editor-fold>
?>