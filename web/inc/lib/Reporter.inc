<?php

/**
 * @name Extract Builder
 * @category class
 * @author Osin
 * @version 17 octobre 2011
 * @desc
 * <p>Les flags servent à referencer les options disponibles dans l'UI
 * de générations de flux. A chaque fois qu'un flag est crée, il doit
 * être referencer ici selon le model qu'il nécessite, le flag, et la
 * description qui lui correspond. Il sera alors disponible dans l'UI. </p>
 * @see lib/uad/back/extract.build.inc
 * */
abstract class Reporter {
  /*
   * Afficher des messages utiles pour le suivi et le debuging
   */

  public static $debug = false;
// <editor-fold defaultstate="collapsed" desc="vars">
  public static $flags = array(
      'user' => array("created_at" => "date d'inscription au site",
                      "last_connection" => "dernière date de connection enregistréer",
          ),
  );
  public static $numeric = array('int',
      'float',
      'double',
      'real',
      'decimal');
  public static $text = array('char',
      'blob',
      'text',
      'enum',
      'set');
  public static $time = array('date',
      'time',
      'year');
  public static $tips = array(
      'id' => "référence unique",
      'user_id' => "propriétaire de l'objet",
  );

// </editor-fold>

  public static function build($extract, $fields = null, $output = null, $file = false) {
    /*
     * construction du flux à partir de cette fonction
     */
    try {

      if ($fields == null)
        $fields = self::getFields($extract->code);
      if ($output == null)
        $output = $extract->format;

      $items = self::getItemsFromScope($extract);
      $flux = self::writeBegin($output, $extract);
      $nodeName = $extract->childXMLName != '' ? $extract->childXMLName : Model::name($extract->scope);
      if (!is_null($items) && !empty($items) && $items->count() > 0) {
        foreach ($items as $key => $item) {
          /**
           * recupération du compteur, ou recupération de la valeur, arrondi si necessaire, 
           * et affichage dans le format défini pour chaque field de chaque objet
           */
          foreach ($fields as $keyf => $field) {
            $value = null;
            if ($field['flag'] == '{counter}')
              $value = $key + 1;
            else
              $value = self::getParameters($item, $field['flag'], $extract->is_planified);
            if (is_numeric($value)) {
              $value = round($value, 2);
            }
            switch ($output) {
              case Extract::FORMAT_XML:
                if ($keyf == 0)
                  $flux .= "<" . $nodeName . ">";
                $flux .= self::fieldToXML($field['name'], $value);
                if ($keyf + 1 == count($fields))
                  $flux .= "</" . $nodeName . ">";
                break;
              case Extract::FORMAT_HTML:
                if ($keyf == 0)
                  $flux .= '<tr>';
                $flux .= self::fieldToHTML($field['name'], $value);
                if ($keyf + 1 == count($fields))
                  $flux .= '</tr>';
                break;
              case Extract::FORMAT_CSV:
                $flux .= self::fieldToCSV($field['name'], $value);
                if ($keyf + 1 == count($fields))
                  $flux .= " \n";
                else
                  $flux .= '|';
                break;
              default:
                $flux = "incorrect output type specified";
                break;
            }
          }
          $flux = str_replace(array('\n', '\t', '\r', chr(10), chr(13)), ' ', $flux);
          $flux = str_replace('&', '&amp;', $flux);

          //ecriture du fichier pas à pas si fichier fourni
          if ($file && $key == 0) {
            $file = fopen('app/uad/files/Extract-' . trim($extract->id . ' - ' . $extract->filename . '.' . Extract::$format[$output]), "w");
            fputs($file, $flux);
            $flux = "";
          }
        }
        $flux .= self::writeEnd($output, $extract);
        if ($file) {
          fputs($file, $flux);
          fclose($file);
          return 1;
        }
        return $flux;
      }
    } catch (Exception $e) {
      return ('Error on build :' . $e);
    }
  }

  public static function buildOpts($filters = null, $sqlTable = null) {
    /*
     * opts pour la fonction collect des items du scope
     */
    if ($filters == null)
      return;
    foreach ($filters as $filter) {
      if ($filter['name'] == 'in') {
        $value = substr($filter['flag'], 1, strlen($filter['flag']) - 2);
        if (ereg('=', $value)) {
          $values = explode('=', $value);
          $value = $values[1];
          $scope = $values[0];
        }
        $filterExtract = Extract::find($value);
        $extracts = self::getItemsFromScope($filterExtract);
        $ids = array();
        foreach ($extracts as $extract)
          $ids[] = $extract->id;
        isset($scope) ? $opt[$scope] = $ids : $opt[Model::name($filterExtract->scope) . '_id'] = $ids;
      } elseif ($filter['name'] == 'offset') {
        $value = substr($filter['flag'], 1, strlen($filter['flag']) - 2);
        $opts['offset'] = $value;
      } elseif ($filter['name'] == 'limit') {
        $value = substr($filter['flag'], 1, strlen($filter['flag']) - 2);
        $opts['limit'] = $value;
      } elseif ($filter['name'] == 'include') {
        $value = substr($filter['flag'], 1, strlen($filter['flag']) - 2);
        $opts['include'] = $value;
      } elseif ($filter['name'] == 'order') {
        $value = substr($filter['flag'], 1, strlen($filter['flag']) - 2);
        $values = explode('=', $value);
        $opts['order'] = array($values[0] => $values[1]);
      } elseif ($filter['name'] == 'unique') {
        $value = substr($filter['flag'], 1, strlen($filter['flag']) - 2);
        $values = S('dbo')->asArray('SELECT id FROM ' . $sqlTable . ' GROUP BY ' . $value);
        $ids = array();
        foreach ($values as $value) {
          $ids[] = $value['id'];
        }
        $opt['id'] = $ids;
      } else {
        $value = substr($filter['flag'], 1, strlen($filter['flag']) - 2);
        if (strpbrk('=', $value) && strpbrk('<>!', $value))
          $operator = '|' . substr($value, 0, 2) . '|';
        else
          $operator = '|' . substr($value, 0, 1) . '|';
        $value = substr($value, strlen($operator) - 2);
        //si on passe plusieurs argument dans le filtres (operator OU)
        $values = explode('+', $value);
        if (count($values) > 1) {
          $value = array();
          foreach ($values as $val) {
            $value[] = $val;
          }
        }
        $opt[$operator . $filter['name']] = $value;
      }
    }
    $opts['metas'] = array('created_at', 'visit_id');
    $opts['filters'] = array($opt);
    return $opts;
  }

  public static function fieldToCSV($name, $value) {
    /*
     * converti un champs pour une exploitation en csv
     */
    return $value;
  }

  public static function fieldToHTML($name, $value) {
    /*
     * converti un champs pour une exploitation en html
     */
    return '<td>' . $value . '</td>';
  }

  public static function fieldToXML($name, $value) {
    /*
     * converti un champs pour une exploitation en xml
     */
    return '<' . $name . '>' . $value . '</' . $name . '>';
  }

  public static function getFields($encapsulate = null) {
    /*
     * Extrait les valeurs codés dans le contenu d'un extract et les retournes sous formes d'un array(name, value)
     */
    if (!isset($encapsulate)) {
      return;
    }
    try {
      $array = explode('|', $encapsulate);
      $fields = array();
      foreach ($array as $value) {
        $args = explode(':', $value);
        if ($args[0] != "") { // Retire les mal formations du code (| content |)
          $field['name'] = $args[0];
          $field['flag'] = strtolower($args[1]);
          $fields[] = $field;
        }
      }
      return $fields;
    } catch (Exception $e) {
      throw new Exception('Extract->code not well formed');
      return 0;
    }
  }

  public static function getFieldType($fieldType) {
    /*
     * renvoie le type de champs spécifié en bdd
     */
    foreach (self::$numeric as $value)
      if (strstr($fieldType, $value))
        return "numeric";
    foreach (self::$text as $value)
      if (strstr($fieldType, $value))
        return "text";
    foreach (self::$time as $value)
      if (strstr($fieldType, $value))
        return "time";
    return 0;
  }

  public static function getFiltersByFieldType($field) {
    /*
     * renvoie une liste d'opérateur > < ! = en fonction du type de champs
     */
    $type = self::getFieldType($field['Type']);
    switch ($type) {
      case 'numeric':
        return array('=', '!=', '>', '<', '>=', '<=');
        break;
      case 'text':
        return array('=', '%%');
        break;
      case 'time':
        return array('=', '!=', '>', '<', '>=', '<=');
        break;
      default:
        break;
    }
  }

  public static function getFormatedDate($item, $field) {
    /*


     * prend un field du genre String Date: {field=format} et le renvoie dans un format
     */
    $args = explode("=", $field);
    $year = substr($item->$args[0], 0, 4);
    $month = substr($item->$args[0], 5, 2);
    $day = substr($item->$args[0], 8, 2);
    $date = substr($item->$args[0], 0, 10);
    $time = substr($item->$args[0], 11, 8);
    $tim = substr($item->$args[0], 11, 5);
    switch ($args[1]) {
      case "date":
        return $date;
      case "datef":
        return $day . "-" . $month . "-" . $year;
      case "day":
        return $day;
      case "month":
        return $month;
      case "month-year":
        return $month . "-" . $year;
      case "tim":
        return $tim;
      case "time":
        return $time;
      case "year":
        return $year;
      case "year-month":
        return $year . "-" . $month;
      default:
        return $args[0];
    }
  }

  public static function getFormatedTime($item, $field) {
    /*
     * prend un field du genre INT time en seconde: {field=format} et le renvoie dans un format en heure, minute, jour, semaine, mois
     */
    $args = explode("=", $field);
    $min = $item->$args[0] / 60;
    $hour = $min / 60;
    $day = $hour / 24;
    $week = $day / 7;
    $month = $day / 30;
    $year = $day / 365;
    switch ($args[1]) {
      case "sec":
        return $args[1];
      case "min":
        return round($min) + 1;
      case "hour":
        return round($hour) + 1;
      case "day":
        return round($day) + 1;
      case "week":
        return round($week) + 1;
      case "month":
        return round($month) + 1;
      case "year":
        return round($year) + 1;
      default:
        return $args[0];
    }
  }

  public static function getItemsFromScope($extract) {
    /*
     * fonction collecte, renvoie un objet de type active record
     */
    $opts = self::buildOpts(self::getFields($extract->filters), Model::table($extract->scope));
    return ActiveRecord::collect(Model::name($extract->scope), $opts);
  }

  public static function getObject($item, &$flag) {
    /*
     * permet d'obtenir le derniers objet nécessaire et position le focus à l'entrée de celui ci.
     * le focus doit être position dans objet->champs ou objet1->objet2->champs
     * utilise la fonction getItem
     * utilise la fonction get_Object()
     * Si la fonction getObject renvoie une collection, certains opérations peuvent être effectuer directement de cette collection.
     * Item alors ne sera plus un objet.
     */

    $arg = substr($flag, 1, strlen($flag) - 2);
    $args = explode('->', $arg);
    $model = $args[0];
    $model_ref = $args[0] . '_id';
    if (isset($item->$model_ref)) {
      $item->$model_ref != 0 ? $item = call_user_func(array($item, 'get' . $model)) : $item = false;
    }
    else {
      try {
        $item = $item->getObject();
      } catch (Exception $e) {
        try {
          $item = call_user_func(array($item, 'get' . $model));
        } catch (Exception $e) {
          $item = false;
        }
      }
    }
    if (count($args) > 2 && $item != false) {
      array_shift($args);
      $flag = '{' . implode("->", $args) . '}';
      $item = @self::getObject($item, $flag);
    }
    $flag = '{' . $args[count($args) - 1] . '}';
    /*
     * Si c'et une collection on peut effectuer une opération directement dessus
     */
    if (is_object($item) && $item instanceof \Collection) {
      return self::getParametersCollectionValue($item, $args[count($args) - 1]);
    }
    return $item;
  }

  public static function getParameters($item, $flag, $silent_error = false) {
    /*
     * renvoie la valeur demande dans le code.
     * Utilise la valeur liés par l'objet selon:
     * le flag, le champs, la proriétés renseigne dans sys_item_meta
     */
    try {
      if (ereg('->', $flag)) {
        $item = @self::getObject(&$item, $flag);
        if (!is_object($item))
          return $item;
      }
      /*
       * On verifie que l'utilisateur soit autorisé à obtenir les modeles du flux
       */
      if (!self::hasRight($item)) {
        return ('prohibited');
      }
      switch ($flag) {
// </editor-fold>
        default:
          $args = substr($flag, 1, strlen($flag) - 2);
          if ($args == 'password') {
            //ne retourne pas les password
            return 'prohibited';
          }
          //exception pour les formats de date en 0000-00-00 00:00:00 ou 1970-01-01
          elseif ($item->$args == "0000-00-00 00:00:00" || $item->$args == "1970-01-01") {
            return '';
          } elseif (ereg('=', $args)) {
            //return une date ou un temps dans un format prédéfinie
            $tmp = explode("=", $args);
            if (is_numeric($item->$tmp[0]))
              return self::getFormatedTime($item, $args);
            else
              return self::getFormatedDate($item, $args);
          } elseif (isset($item->$args)) {
            //return la valeur directe du model
            if (strrchr($args, '_') == '_id'
                    && $args != 'record_id'
                    && Model::id(substr($args, 0, strlen($args) - 3)) != 0) {
              return self::getParametersModelValue($item, $args);
            } else
            //renvoie du champs
              return strip_tags($item->$args);
          }
          elseif ($args == 'note') {
            //return la note
            try {
              $var = $item->getNote();
              return $var;
            } catch (Exception $e) {
              return '';
            }
          } elseif ($item->getProperty($args)) {
            //return les propriétés de l'objets
            return strip_tags($item->getProperty($args));
          } elseif ($item->getMeta($args))
          //return les metas
            return $item->getMeta($args);
          else
            return 'unrecognized';
      }
    } catch (Exception $e) {
      return $silent_error == false ? $flag : '';
    }
  }

  public static function getParametersCollectionValue($item, $arg) {
    /*
     * Cette fonction traite les collections qui sont renvoyés par getObjet
     * Liste des operations sur les collections nombre somme, moyenne, maximum, minimum
     */
    /*
     * Liste des operations sur les collections nombre somme, moyenne, maximum, minimum, list
     */
    switch ($arg) {
      case 'count':
        return $item->count();
      case 'list':
        $list = "";
        foreach ($item as $key => $value) {
          $list .= $value->id;
          if ($key + 1 != $item->count())
            $list .= ' - ';
        }return $list;
      default:
        if (ereg('=', $arg)) {
          //le premier argument est le champs sur lequel travaillé, le second est l'operation a effectuer
          $tmp = explode("=", $arg);
          switch ($tmp[1]) {
            case 'sum':
              return $item->sum($tmp[0]);
            case 'average':
              return $item->average($tmp[0]);
            case 'maximum':
              return $item->maximum($tmp[0]);
            case 'minimum':
              return $item->minimum($tmp[0]);
            default:
              break;
          }
        }
        return 'collection_found';
        break;
    }
  }

  public static function getParametersModelValue($item, $flag_id) {
    /*
     * par exemple pour produit->marque_id recupere l'objet marque ayant l'id du produit->marque_id
     */
    if ($item->$flag_id != 0) {
      $model = $classname = substr($flag_id, 0, strlen($flag_id) - 3);
      $opts['filters']['id'] = ($item->$flag_id);
      $collection = Model::collect($model, $opts);
      if ($collection->count() > 0)
        return $collection->first();
    }
    return '';
  }

  public static function getTips($field = null, $model = null) {
    /*
     * permet de recupérer une defition userfriendly pour l'UI
     */
    if (isset(self::$flags[$model][$field]))
      return self::$flags[$model][$field];
    elseif (isset(self::$tips[$field]))
      return self::$tips[$field];
    else
      return "No description found";
  }

  public static function hasRight($item) {
    /*
     * Dertermine les privilèges ou non sur l'exploitation d'un model
     */
//    $user = S('admin');
//    if ($user->policy_id == 1 || $user->policy_id == 10 || array_search(Model::id($item->model()), explode(',', Policy::getInstance('policy', $user->policy_id)->extract_rights))) {
      return 1;
//    }else
//      return 0;
  }

  public static function writeBegin($output, $extract) {
    /*
     * Ecris le debut d'un format de fichier
     */
    switch ($output) {
      case Extract::FORMAT_RSS:
        $rss = '<?xml version="1.0" encoding="UTF-8"?>
                <rss version="2.0">';
        return $rss;
      case Extract::FORMAT_XML:
        $xml = '<?xml version="1.0" encoding="UTF-8"?>';
        $xml .= $extract->parentXMLName != '' ? "<" . $extract->parentXMLName . ">" : "<" . Model::name($extract->scope) . "s>";
        return $xml;
        break;
      case Extract::FORMAT_CSV:
        $code = self::getFields($extract->code);
        foreach ($code as $key => $col) {
          $csv .= strtoupper($col['name']);
          if ($key + 1 < count($code))
            $csv .= '|';
        }
        $csv .= chr(10);
        return $csv;
        break;
      case Extract::FORMAT_HTML:

        $html = '<html>
                    <head><title="' . $extract->name . '"><meta http-equiv="content-type" content="text/html; charset=utf-8" /></head>
                  <body>
                    <table border="1"><thead>';
        $code = self::getFields($extract->code);
        foreach ($code as $col) {
          $html .= '<th>' . strtoupper($col['name']) . '</th>';
        }
        $html .= '</thead><tbody>';
        return $html;
      default:
        return "";
        break;
    }
  }

  public static function writeEnd($output, $extract) {
    /*
     * Ecris la fin d'un format de fichier
     */
    switch ($output) {
      case Extract::FORMAT_XML:
        return $extract->parentXMLName != '' ? "</" . $extract->parentXMLName . ">" : "</" . Model::name($extract->scope) . "s>";
        break;
      case Extract::FORMAT_HTML:
        return '</tbody></table></body></html>';
      default:
        return "";
        break;
    }
  }

}