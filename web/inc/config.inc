<?php
// <editor-fold defaultstate="collapsed" desc="set global variable">
if (!defined('Environment'))
  define("Environment", $_SERVER['SERVER_NAME']);

if (!defined("SITE_PATH"))
  Environment == 'development' ? define("SITE_PATH", 'http://'.$_SERVER['HTTP_HOST'].'/') : define("SITE_PATH", 'http://'.$_SERVER['HTTP_HOST'].'/');

if (!defined('RESSOURCES_PATH'))
  define("RESSOURCES_PATH", SITE_PATH.'inc/ressources/');

if (!defined('DEFAULT_TEMPLATE'))
  define("DEFAULT_TEMPLATE", "web");

if (!defined('MODEL_PATH'))
  define("MODEL_PATH", '../../inc/model/');

if (!defined('SERVICE_PATH'))
  define("SERVICE_PATH", '../../service/');

if (!defined('SNIPPET_PATH'))
  define("SNIPPET_PATH", '../inc/snippets/');

if (!defined('TEMPLATE_PATH'))
  define("TEMPLATE_PATH", '../../inc/template/');

if (!defined('VIEW_PATH'))
  define("VIEW_PATH", '../view/');

if (!defined('VAR_PATH'))
  define("VAR_PATH", sys_get_temp_dir() . '/');

if (!defined('PWD_HASH'))
  define("PWD_HASH", 'wotoogwebsite');

if (!defined('SiteName'))
  define("SiteName", 'Intranet');


/* Here you can define which scope of the site is for admin_user.
 * Please add scope separated by '|'
 */
define("ADMIN_SCOPE", '|back|private|');
// </editor-fold>

//add here all your database environment
switch(Environment){
    default:
        define('DB_DRIVER', 'mysql');
        define('DB_SERVER', $_ENV['HOSTNAME']);
        define('DB_USERNAME', $_ENV['DATABASE_ENV_MYSQL_USER']);
        define('DB_PASSWORD', $_ENV['DATABASE_ENV_MYSQL_PASSWORD']);
        define('DB_DATABASE', $_ENV['DATABASE_ENV_MYSQL_DATABASE']);
        define('DB_CHARSET', 'utf8');
        break;
}