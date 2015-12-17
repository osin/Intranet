<?php

class Dir {

    /** If you want to copy an entire directory recursively 
   *
   * @param type $source
   * @param type $destination 
   */
  function copyDirectory($source, $destination) {
    if (is_dir($source)) {
      @mkdir($destination);
      $directory = dir($source);
      while (FALSE !== ( $readdirectory = $directory->read() )) {
        if ($readdirectory == '.' || $readdirectory == '..') {
          continue;
        }
        $PathDir = $source . '/' . $readdirectory;
        if (is_dir($PathDir)) {
          copy_directory($PathDir, $destination . '/' . $readdirectory);
          continue;
        }
        copy($PathDir, $destination . '/' . $readdirectory);
      }

      $directory->close();
    } else {
      copy($source, $destination);
    }
  }
  
  
  public static function listFiles($Directory, $Entries = Array()) {
    $MyDirectory = opendir($Directory) or die('Erreur');
    while ($Entry = @readdir($MyDirectory)) {
      if (is_dir($Directory . '/' . $Entry) && $Entry != '.' && $Entry != '..' && $Entry != 'lib')
        array_merge($Entries, listFiles($Directory . '/' . $Entry));
      else {
        if ($Entry != '.' && $Entry != '..')
          $Entries[] = $Directory . '/' . $Entry;
      }
      return $Entries;
    }
  }

  /** If you want to include type of file contains in the dire.
   * you can define more types to include in the function declaration
   * @param type $Directory
   * @param type $type 
   */
  public static function includeDirectory($Directory, $type = 'inc') {
    $MyDirectory = opendir($Directory) or die('Erreur');
    while ($Entry = @readdir($MyDirectory)) {
      if (is_dir($Directory . '/' . $Entry) && $Entry != '.' && $Entry != '..' && $Entry != 'lib')
        IncludeDirectory($Directory . '/' . $Entry, $type);
      else {
        if ($Entry != '.' && $Entry != '..') {
          switch ($type) {
            case 'inc':
              require_once $Directory . '/' . $Entry;
              break;
            case 'js':
              print "<script type='text/javascript' src='" . $Directory . '/' . $Entry . "'></script>\n";
              break;
            case 'css':
              print "<link rel='stylesheet' type='text/css' media='screen' href='" . $Directory . '/' . $Entry . "' />\n";
              break;
            default:
              break;
          }
        }
      }
    }
    closedir($MyDirectory);
  }



  function mkdir_recursive($path, $check_is_parent_dir = false) {
    if (sugar_is_dir($path, 'instance')) {
      return(true);
    }
    if (sugar_is_file($path, 'instance')) {
      if (!empty($GLOBALS['log'])) {
        $GLOBALS['log']->fatal("ERROR: mkdir_recursive(): argument $path is already a file.");
      }
      return false;
    }

    //make variables with file paths
    $pathcmp = $path = rtrim(clean_path($path), '/');
    $basecmp = $base = rtrim(clean_path(getcwd()), '/');
    if (is_windows()) {
      //make path variable lower case for comparison in windows
      $pathcmp = strtolower($path);
      $basecmp = strtolower($base);
    }

    if ($basecmp == $pathcmp) {
      return true;
    }
    $base .= "/";
    if (strncmp($pathcmp, $basecmp, strlen($basecmp)) == 0) {
      /* strip current path prefix */
      $path = substr($path, strlen($base));
    }
    $thePath = '';
    $dirStructure = explode("/", $path);
    if ($dirStructure[0] == '') {
      // absolute path
      $base = '/';
      array_shift($dirStructure);
    }
    if (is_windows()) {
      if (strlen($dirStructure[0]) == 2 && $dirStructure[0][1] == ':') {
        /* C: prefix */
        $base = array_shift($dirStructure) . "\\";
      } elseif ($dirStructure[0][0] . $dirStructure[0][1] == "\\\\") {
        /* UNC absolute path */
        $base = array_shift($dirStructure) . "\\" . array_shift($dirStructure) . "\\"; // we won't try to mkdir UNC share name
      }
    }
    foreach ($dirStructure as $dirPath) {
      $thePath .= $dirPath . "/";
      $mkPath = $base . $thePath;

      if (!is_dir($mkPath)) {
        if (!sugar_mkdir($mkPath))
          return false;
      }
    }
    return true;
  }

  function rmdir_recursive($path) {
    if (is_file($path)) {
      return( unlink($path) );
    }
    if (!is_dir($path)) {
      if (!empty($GLOBALS['log'])) {
        $GLOBALS['log']->fatal("ERROR: rmdir_recursive(): argument $path is not a file or a dir.");
      }
      return false;
    }

    $status = true;

    $d = dir($path);

    while (($f = $d->read()) !== false) {
      if ($f == "." || $f == "..") {
        continue;
      }
      $status &= rmdir_recursive("$path/$f");
    }
    $d->close();
    $rmOk = @rmdir($path);
    if ($rmOk === FALSE) {
      $GLOBALS['log']->error("ERROR: Unable to remove directory $path");
    }
    return( $status );
  }

  function findTextFiles($the_dir, $the_array) {
    if (!is_dir($the_dir)) {
      return $the_array;
    }
    $d = dir($the_dir);
    while (false !== ($f = $d->read())) {
      if ($f == "." || $f == "..") {
        continue;
      }
      if (is_dir("$the_dir/$f")) {
        // i think depth first is ok, given our cvs repo structure -Bob.
        $the_array = findTextFiles("$the_dir/$f", $the_array);
      } else {
        switch (mime_content_type("$the_dir/$f")) {
          // we take action on these cases
          case "text/html":
          case "text/plain":
            array_push($the_array, "$the_dir/$f");
            break;
          // we consciously skip these types
          case "application/pdf":
          case "application/x-zip":
          case "image/gif":
          case "image/jpeg":
          case "image/png":
          case "text/rtf":
            break;
          default:
            $GLOBALS['log']->info("no type handler for $the_dir/$f with mime_content_type: " . mime_content_type("$the_dir/$f") . "\n");
        }
      }
    }
    return( $the_array );
  }

  /** Lists files in directories giving the possibility to include and exlude directory
   *
   * @param type $the_dir
   * @param type $the_array
   * @param type $include_dirs
   * @param type $ext
   * @param type $exclude_dir
   * @return type 
   */
  function findAllFiles($the_dir, $the_array, $include_dirs = false, $ext = '', $exclude_dir = '') {
    // jchi  #24296
    if (!empty($exclude_dir)) {
      $exclude_dir = is_array($exclude_dir) ? $exclude_dir : array($exclude_dir);
      foreach ($exclude_dir as $ex_dir) {
        if ($the_dir == $ex_dir) {
          return $the_array;
        }
      }
    }
    $the_dir = rtrim($the_dir, "/\\");
    //end
    if (!is_dir($the_dir)) {
      return $the_array;
    }
    $d = dir($the_dir);
    while (false !== ($f = $d->read())) {
      if ($f == "." || $f == "..") {
        continue;
      }

      if (is_dir("$the_dir/$f")) {
        // jchi  #24296
        if (!empty($exclude_dir)) {
          //loop through array to compare directories..
          foreach ($exclude_dir as $ex_dir) {
            if ("$the_dir/$f" == $ex_dir) {
              continue 2;
            }
          }
        }
        //end

        if ($include_dirs) {
          $the_array[] = clean_path("$the_dir/$f");
        }
        $the_array = findAllFiles("$the_dir/$f", $the_array, $include_dirs, $ext);
      } else {
        if (empty($ext) || preg_match('/' . $ext . '$/i', $f)) {
          $the_array[] = "$the_dir/$f";
        }
      }
    }
    rsort($the_array);
    return $the_array;
  }

  function findAllFilesRelative($the_dir, $the_array) {
    if (!is_dir($the_dir)) {
      return $the_array;
    }
    $original_dir = getCwd();
    if (is_dir($the_dir)) {
      chdir($the_dir);
      $the_array = findAllFiles(".", $the_array);
      if (is_dir($original_dir)) {
        chdir($original_dir);
      }
    }
    return( $the_array );
  }

  /*
   * Obtain an array of files that have been modified after the $date_modified
   *
   * @param the_dir			the directory to begin the search
   * @param the_array			array to hold the results
   * @param date_modified		the date to use when searching for files that have
   * 							been modified
   * @param filter			optional regular expression filter
   *
   * return					an array containing all of the files that have been
   * 							modified since date_modified
   */

  function findAllTouchedFiles($the_dir, $the_array, $date_modified, $filter = '') {
    if (!is_dir($the_dir)) {
      return $the_array;
    }
    $d = dir($the_dir);
    while (false !== ($f = $d->read())) {
      if ($f == "." || $f == "..") {
        continue;
      }
      if (is_dir("$the_dir/$f")) {
        // i think depth first is ok, given our cvs repo structure -Bob.
        $the_array = findAllTouchedFiles("$the_dir/$f", $the_array, $date_modified, $filter);
      } else {
        $file_date = date('Y-m-d H:i:s', strtotime(date('Y-m-d H:i:s', filemtime("$the_dir/$f"))) - date('Z'));

        if (strtotime($file_date) > strtotime($date_modified) && (empty($filter) || preg_match($filter, $f))) {
          array_push($the_array, "$the_dir/$f");
        }
      }
    }
    return $the_array;
  }

}

?>
