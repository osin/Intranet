<?php if (is_file('./inc/config.inc'))
// print_r($_SERVER);
header('location: ./web/load/');
?>
<!DOCTYPE html>
<html>
  <head>
    <title>Installation</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <link rel="stylesheet" type="text/css" href="./inc/ressources/default/css/bootstrap.css" media="screen" />
    <script type="text/javascript" src="./inc/ressources/default/js/jquery.min.js"></script>
    <script type="text/javascript" src="./inc/ressources/default/js/jquery.md5.js"></script>
  </head>
  <body>
    <div class="hero-unit">
      <h1>Welcome to the Labo Intranet</h1>
      <p>Use this script to start quickly and deploy your intranet.</p>
    </div>

    <div class="container">
	<div class="box padding10">
            <div class="alert alert-block">
              <a class="close" data-dismiss="alert" href="#">×</a>
              <h4 class="alert-warning">Important!</h4>
              Be sure to have permissions to write in path/inc in development configuration.
			  Be sure to NOT BE DENIED to write in path/ in environmemnt configuration.
            </div>
          </div>
      <div class="formulaire-block row-fluid">
        <form method="POST" action="setup.php" class="form-horizontal">

          <div class="page-header">
            <h2>Inform your company</h2>
          </div>

          <div class="box padding10 info">
            <div class="control-group">
              <label class="control-label" for="visibility">Entreprise</label>
              <div class="controls">
                <label class="radio inline">
                  <input type="radio" name="sexe" value="0" checked = "true"> Le
                </label>
                <label class="radio inline">
                  <input type="radio" name="sexe" value="1"> La
                </label>
                <input type="text" name="name"/>
              </div>
            </div>
          </div>

          <div class="box padding10">
            <div class="alert alert-info">
              <a class="close" data-dismiss="alert" href="#">×</a>
              <h4 class="alert-info">Important!</h4>
              Create your first superadmin user
            </div>
          </div>

          <div class="box">
            <div class="row-fluid">
              <div class="span4">
                <img alt="" class="thumbnail"id="thumbnail" src="" style="max-width: 300px;"/>&nbsp;
              </div>
              <div class="span8 text-right">
                <fieldset>
                  <legend>Fill your first user (will be an administrator)</legend>
                  <div class="control-group">
                    <label class="control-label" for="id_booster">ID Booster</label>
                    <div class="controls">
                      <input type="text" id="id_booster" class="input-large" name="id" placeholder="ID Booster" />
                    </div>
                  </div>


                  <div class="control-group">
                    <label class="control-label" for="password">Mot de passe</label>
                    <div class="controls">
                      <input type="text" id="password" class="input-large" name="password" placeholder="Mot de passe" />
                    </div>
                  </div>

                  <div class="control-group">
                    <label class="control-label" for="repassword">Répéter le mot de passe</label>
                    <div class="controls">
                      <input type="text" id="repassword" class="input-large" name="repassword" placeholder="Mot de passe" />
                    </div>
                  </div>

                  <div class="control-group">
                    <label class="control-label" for="email">Adresse Electronique</label>
                    <div class="controls">
                      <input class="input-large" name="email" placeholder="Adresse Electronique" id="email" size="16" type="text" onblur="document.getElementById('thumbnail').src='http://www.gravatar.com/avatar/'+$.md5($('#email').val())+'?s=300&d=mm';$('#picture').val($('#thumbnail').attr('src'));" />
                    </div>
                  </div>

                  <div class="control-group">
                    <label class="control-label" for="firstname">Prénom</label>
                    <div class="controls">
                      <input type="text" id="firstname" class="input-large" name="firstname" placeholder="Prénom" />
                    </div>
                  </div>

                  <div class="control-group">
                    <label class="control-label" for="lastname">Nom</label>
                    <div class="controls">
                      <input type="text" id="lastname" class="input-large" name="lastname" placeholder="Nom" />
                    </div>
                  </div>

     <div class="control-group">
                    <label class="control-label" for="promotion">Promotion</label>
                    <div class="controls">
                      <input type="text" id="promotion" class="input-large" name="promotion" placeholder="Promotion" />
                    </div>
                  </div>

                  <div class="control-group">
                    <label class="control-label" for="picture">Url (Adresse web) de votre image</label>
                    <div class="controls">
                      <input type="text" id="picture" class="input-large" name="picture" placeholder="Placer l'url de votre image de profil" onblur="document.getElementById('thumbnail').src=document.getElementById('picture').value">
                    </div>
                  </div>

              </div>
            </div>
          </div>

          <div class="page-header">
            <h2>Connect to your database</h2>
          </div>
<!-- connection à la base de données -->
          <div class="box">
            <div class="row-fluid">
              <div class="span4">
                <img alt="" class="thumbnail"id="thumbnail" src="" style="max-width: 300px;"/>&nbsp;
              </div>
              <div class="span8 text-right">
                <fieldset>
                  <legend>Fill your database configuration</legend>
                  <div class="control-group">
                    <label class="control-label" for="Database_Name">Database name</label>
                    <div class="controls">
                      <input type="text" id="id_booster" class="input-large" name="Database_Name" placeholder="Database Name" />
                    </div>
                  </div>


                  <div class="control-group">
                    <label class="control-label" for="DB_Login">Login</label>
                    <div class="controls">
                      <input type="text" id="repassword" class="input-large" name="DB_Login" placeholder="Login" />
                    </div>
                  </div>


                  <div class="control-group">
                    <label class="control-label" for="DB_password">Mot de passe</label>
                    <div class="controls">
                      <input type="text" id="password" class="input-large" name="DB_password" placeholder="Mot de passe" />
                    </div>
                  </div>


                  <div class="control-group">
                    <label class="control-label" for="DB_Host">Database hostname</label>
                    <div class="controls">
                      <input class="input-large" name="DB_Host" placeholder="" value="localhost" id="email"  />
                    </div>
                  </div>

                  <div class="control-group">
                    <label class="control-label" for="DB_Prefix">table prefix</label>
                    <div class="controls">
                      <input type="text" id="firstname" class="input-large" name="DB_Prefix" placeholder="li_" value="li_" />
                    </div>
                  </div>

              </div>
            </div>
          </div>


<input type="submit" value="Submit" />
        </form>
      </div>
    </div>
  </body>
</html>
