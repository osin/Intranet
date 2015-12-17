<?php

class String {

  // if you want to modify later the css used to prettify the code it's here
  static $recognizer_code = '<code>';
  static $recognizer_code_closure = '</code>';
  static $replacement_code = '<pre class="prettyprint linenums">';
  static $replacement_code_closure = '</pre>';

  /** Convert a string in correct accent equivalent in html
   *
   * @param type $in
   * @return type 
   */
  public static function accentToHTML($string) {
    $search = explode(',', 'À,Á,Â,Ã,Ä,Å,Ç,È,É,Ê,Ë,Ì,Í,Î,
                            Ï,Ò,Ó,Ô,Õ,Ö,Ù,Ú,Û,Ü,Ý,à,á,â,ã,
                            ä,å,ç,è,é,ê,ë,ì,í,î,ï,ð,ò,ó,ô,õ,ö,
                            ù,ú,û,ü,ý,ÿ,«,»,’');
    $replace = explode(',', '&Agrave;,&Aacute;,&Acirc;,&Atilde;,&Auml;,A,&Ccedil;,&Egrave;,&Eacute;,&Ecirc;,&Euml;,&Igrave;,&Iacute;,&Icirc;,
                                &Iuml;,&Ograve;,&Oacute;,&Ocirc;,&Otilde;,&Ouml;,&Ugrave;,&Uacute;,&Ucirc;,&Uuml;,&Yacute;,&agrave;,&aacute;,&acirc;,&atilde;,
                                &auml;,a,&ccedil;,&egrave;,&eacute;,&ecirc;,&euml;,&igrave;,&iacute;,&icirc;,&iuml;,o,&ograve;,&oacute;,&ocirc;,&otilde;,&ouml;,
                                &ugrave;,&uacute;,&ucirc;,&uuml;,&yacute;,&yuml;,&laquo;,&raquo;,\'');
    $out = str_replace($search, $replace, $string);
    return $out;
  }

  /** add http 
   *
   * @param type $url
   * @return type 
   */
  public static function add_http($url) {
    if (!preg_match("@://@i", $url)) {
      $scheme = "http";
      if (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] === 'on') {
        $scheme = 'https';
      }

      return "{$scheme}://{$url}";
    }

    return $url;
  }

  /**
   * This function will take a number and system_id and format
   * @param	$url URL containing host to append port
   * @param	$port the port number - if '' is passed, no change to url
   * @return	$resulturl the new URL with the port appended to the host
   */
  public static function appendPortToHost($url, $port) {
    $resulturl = $url;

    // if no port, don't change the url
    if ($port != '') {
      $split = explode("/", $url);
      //check if it starts with http, in case they didn't include that in url
      if (str_begin($url, 'http')) {
        //third index ($split[2]) will be the host
        $split[2] .= ":" . $port;
      } else { // otherwise assumed to start with host name
        //first index ($split[0]) will be the host
        $split[0] .= ":" . $port;
      }

      $resulturl = implode("/", $split);
    }

    return $resulturl;
  }

  /**
   * Very cool algorithm for sorting multi-dimensional arrays.  Found at http://us2.php.net/manual/en/function.array-multisort.php
   * Syntax: $new_array = array_csort($array [, 'col1' [, SORT_FLAG [, SORT_FLAG]]]...);
   * Explanation: $array is the array you want to sort, 'col1' is the name of the column
   * you want to sort, SORT_FLAGS are : SORT_ASC, SORT_DESC, SORT_REGULAR, SORT_NUMERIC, SORT_STRING
   * you can repeat the 'col',FLAG,FLAG, as often you want, the highest prioritiy is given to
   * the first - so the array is sorted by the last given column first, then the one before ...
   * Example: $array = array_csort($array,'town','age',SORT_DESC,'name');
   */
  public static function array_csort() {
    $args = func_get_args();
    $marray = array_shift($args);
    $i = 0;

    $msortline = "return(array_multisort(";
    foreach ($args as $arg) {
      $i++;
      if (is_string($arg)) {
        foreach ($marray as $row) {
          $sortarr[$i][] = $row[$arg];
        }
      } else {
        $sortarr[$i] = $arg;
      }
      $msortline .= "\$sortarr[" . $i . "],";
    }
    $msortline .= "\$marray));";

    eval($msortline);
    return $marray;
  }

// recursive function to count the number of levels within an array
  public static function array_depth($array, $depth_count = -1, $depth_array = array()) {
    $depth_count++;
    if (is_array($array)) {
      foreach ($array as $key => $value) {
        $depth_array[] = array_depth($value, $depth_count);
      }
    } else {
      return $depth_count;
    }
    foreach ($depth_array as $value) {
      $depth_count = $value > $depth_count ? $value : $depth_count;
    }
    return $depth_count;
  }

    public static function br2nl($str) {
    $regex = "#<[^>]+br.+?>#i";
    preg_match_all($regex, $str, $matches);

    foreach ($matches[0] as $match) {
      $str = str_replace($match, "<br>", $str);
    }

    $brs = array('<br>', '<br/>', '<br />');
    $str = str_replace("\r\n", "\n", $str); // make from windows-returns, *nix-returns
    $str = str_replace("\n\r", "\n", $str); // make from windows-returns, *nix-returns
    $str = str_replace("\r", "\n", $str); // make from windows-returns, *nix-returns
    $str = str_ireplace($brs, "\n", $str); // to retrieve it

    return $str;
  }
  
  /** resume a string to alphabetic words
   *
   * @param type $str
   * @param type $filter
   * @param type $dieOnBadData
   * @return boolean 
   */
  public static function clean_string($str, $filter = "STANDARD", $dieOnBadData = true) {
    $filters = Array(
        "STANDARD" => '#[^A-Z0-9\-_\.\@]#i',
        "STANDARDSPACE" => '#[^A-Z0-9\-_\.\@\ ]#i',
        "FILE" => '#[^A-Z0-9\-_\.]#i',
        "NUMBER" => '#[^0-9\-]#i',
        "SQL_COLUMN_LIST" => '#[^A-Z0-9\(\),_\.]#i',
        "PATH_NO_URL" => '#://#i',
        "SAFED_GET" => '#[^A-Z0-9\@\=\&\?\.\/\-_~+]#i', /* range of allowed characters in a GET string */
        "UNIFIED_SEARCH" => "#[\\x00]#", /* cn: bug 3356 & 9236 - MBCS search strings */
        "AUTO_INCREMENT" => '#[^0-9\-,\ ]#i',
        "ALPHANUM" => '#[^A-Z0-9\-]#i',
    );

    if (preg_match($filters[$filter], $str)) {
      return false;
    } else {
      return $str;
    }
  }
  
  public static function link2HtmlLink($str){
    return preg_replace('/<a(.*?)href="(.*?)"(.*?)>/','<a$1href="javascript:alert(\'Test\');"$3>',$str);
  }
  
  /** Normalize a String by removing accent and styles
   *
   * @param type $str
   * @return type 
   */
  public static function normalize($str) {
    $s = strtolower($s);
    $str = self::stripAccents($str);
    $str = preg_replace('/(\s|\W)/u', '', $str);
    return $str;
  }

 /**
   * Converts localized date format string to jscalendar format
   */
  public static function parse_calendardate($local_format) {
    preg_match('/\(?([^-]{1})[^-]*-([^-]{1})[^-]*-([^-]{1})[^-]*\)/', $local_format, $matches);
    $calendar_format = "%" . $matches[1] . "-%" . $matches[2] . "-%" . $matches[3];
    return str_replace(array("y", "ￄ1�7", "a", "j"), array("Y", "Y", "Y", "d"), $calendar_format);
  }
  /** Generate a random password
   *
   * @param type $length
   * @return string 
   */
  public static function password($length = 8) {
    $seed = 'abcdefhikmnrstuwxz' . '2456789';
    $ret = '';
    for ($i = 1; $i <= $length; $i++) {
      $ret .= $seed[mt_rand(0, strlen($seed) - 1)];
    }
    return $ret;
  }

  /** return the string prettified
   *
   * @return string 
   */
  public static function prettify($string) {
    return str_replace(array(self::$recognizer_code, self::$recognizer_code_closure), array(self::$replacement_code, self::$replacement_code_closure), $string);
  }

  /** Usefull to generate long word password based
   *
   * @param type $syllables
   * @param type $use_prefix
   * @return string 
   */
  public static function randWord($syllables = 3, $use_prefix = false) {
    /**
     * Generer un mot facilement retenable. Utilisé pour générer des coupons
     */
    $prefix = array('aero', 'anti', 'auto', 'bi', 'bio', 'cine', 'deca', 'demo', 'dyna', 'eco', 'ergo', 'geo', 'gyno', 'hypo', 'kilo',
        'mega', 'tera', 'mini', 'nano', 'duo', 'fly', 'auto', 'kado', 'deco',);
    $suffix = array('dom', 'ity', 'ment', 'sion', 'ness',
        'ence', 'er', 'ist', 'tion', 'or', 'yes');
    $vowels = array('a', 'o', 'e', 'i', 'y', 'u', 'ou', 'oo');
    $consonants = array('w', 'r', 't', 'p', 's', 'd', 'f', 'g', 'h', 'j',
        'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm', 'qu');

    $password = $use_prefix ? ae_arr($prefix) : '';
    $password_suffix = ae_arr($suffix);

    for ($i = 0; $i < $syllables; $i++) {
      // on choisi une consonne aleatoire
      $doubles = array('n', 'm', 't', 's');
      $c = ae_arr($consonants);
      if (in_array($c, $doubles) && ($i != 0)) { // 
        if (rand(0, 2) == 1) // 33% probabilité
          $c .= $c;
      }
      $password .= $c;
      // puis une voyelle
      $password .= ae_arr($vowels);
      if ($i == $syllables - 1) // si le suffix commencent par une voyelle, on met une consonne
        if (in_array($password_suffix[0], $vowels))
          $password .= ae_arr($consonants);
    }
    // pour finir on ajoute un suffix
    $password .= $password_suffix;
    return $password;
  }

  /** Reduce a string to a specified lenght
   *
   * @param type $chaine
   * @param type $nbchars
   * @param type $end
   * @return string 
   */
  public static function shorten($chaine, $nbchars, $end = "...") {
    $lg_max = $nbchars;

    if (strlen($chaine) > $lg_max) {
      $chaine = substr($chaine, 0, $lg_max);
      $last_space = strrpos(strip_tags($chaine), " ");
      $chaine = substr($chaine, 0, $last_space) . $end;
    }
    return $chaine;
  }

  public static function stripAccents($str) {
    $patterns = array('/[àáâãäåÀÁÂÃÄÅ]/u', '/[èéêëěÈÉÊË]/u', '/[ìíîïÌÍÎÏ]/u',
        '/[òóôõöøÒÓÔÕÖØ]/u', '/[ùúûüůÙÚÛÜŮ]/u', '/[çčÇ]/u', '/[ñňÑŇ]/u',
        '/[ŕřŔŘ]/u', '/[ðď]/u', '/[ýÿÝ]/u', '/[þÞ]/u', '/[šßŠ]/u');
    $replacements = array('a', 'e', 'i', 'o', 'u', 'c', 'n', 'r', 'd', 'y', 'b', 's');
    $str = preg_replace($patterns, $replacements, $str);
    return $str;
  }

    /** Convert an url in correct format
   *
   * @param type $str
   * @return string 
   */
  public static function urlify($str) {
    $str = trim($str);
    if (empty($str))
      return '';
    $str = self::stripAccents($str);
    $str = strtolower($str);
    $str = preg_replace('/&.+?;/', '', $str);
    $str = preg_replace(array('/(\s|[^\w-]|-|,)+/u'), array('-'), $str);
    $str = trim($str, '-');
    $str = urlencode($str);
    return $str;
  }
}

?>