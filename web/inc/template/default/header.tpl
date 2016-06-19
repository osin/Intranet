<?php
$full = stripos($_SERVER['SCRIPT_NAME'], "index");
$full = $full > 0 ? $full : stripos($_SERVER['SCRIPT_NAME'], "suscribe");
?>
<!DOCTYPE html>
<html>
  <head>
    <title><?php echo Company::first()->name?> - <?php echo  $title ?></title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<!--    <script type="text/javascript" src="<?php echo  JS_PATH ?>jquery.cleditor.table.js"></script>-->
    <script type="text/javascript" src="<?php echo  JS_PATH ?>jquery.min.js"></script>
    <script type="text/javascript" src="<?php echo  JS_PATH ?>jquery.easing.js"></script>
    <script type="text/javascript" src="<?php echo  JS_PATH ?>bootstrap.js"></script>
    <script type="text/javascript" src="<?php echo  JS_PATH ?>bootstrap-datepicker.js"></script>
    <script type="text/javascript" src="<?php echo  JS_PATH ?>bootstrap-timepicker.js"></script>
    <script type="text/javascript" src="<?php echo  JS_PATH ?>jquery.emoticons.js"></script>
    <script type="text/javascript" src="<?php echo  JS_PATH ?>jquery.cleditor.js"></script>
    <script type="text/javascript" src="<?php echo  JS_PATH ?>jquery.cleditor.icon.js"></script>
    <script type="text/javascript" src="<?php echo  JS_PATH ?>jquery.cleditor.xhtml.js"></script>
    <script type="text/javascript" src="<?php echo  JS_PATH ?>jquery.prettify.js"></script>
    <script type="text/javascript" src="<?php echo  JS_PATH ?>jquery.md5.js"></script>
    <script type="text/javascript" src="<?php echo  JS_PATH ?>bootstrap-load-image.js"></script>
    <!-- Le styles -->
    <link rel="stylesheet" type="text/css" href="<?php echo  CSS_PATH ?>bootstrap.css" media="screen" />
    <link rel="stylesheet" type="text/css" href="<?php echo  CSS_PATH ?>bootstrap.responsive.css" media="screen" />
    <link rel="stylesheet" type="text/css" href="<?php echo  CSS_PATH ?>bootstrap-datepicker.css" media="screen" />
    <link rel="stylesheet" type="text/css" href="<?php echo  CSS_PATH ?>bootstrap-timepicker.css" media="screen" />
    <link rel="stylesheet" type="text/css" href="<?php echo  CSS_PATH ?>emoticons.css" media="screen" />
	<link rel="stylesheet" type="text/css" href="<?php echo  CSS_PATH ?>cleditor.css" media="screen" />
      <link rel="stylesheet" type="text/css" href="<?php echo  CSS_PATH ?>style.css?id=00021" media="screen" />
  </head>
  <body onload="prettyPrint()">
    <div id="background" class="lazyHeight">
      <div id="bg-user" class="lazyHeight">

      </div>
      <div id="bg-liner" class="lazyHeight">

      </div>      
    </div>
    <div id="doc" class="text-shadow">
      <div id="content">
        <div id="header">
          <?php echo  $this->get('block.header') ?>
        </div>
      </div>
      <div id="main" class="container">
        <?php
        if (is_array($infos)):
          foreach ($infos as $key => $info):
            ?>
            <div class="alert alert-<?php echo  $key ?>"><?php echo  $info ?></div>
            <?php
          endforeach;
        elseif ($infos != ""):
          ?>
          <div class="alert alert-info"><strong>Information! </strong> <?php echo  $infos ?></div>
        <?php endif; ?>
        <div id="main-wrapper" class="row<?php echo  $full > 0 ? "" : "-fluid" ?> show-grid">