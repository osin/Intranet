<?php

class Template
{

    private static $instances = array();

    /**
     * Static method for retrieving the template.
     * in this method we defin also where is located ressources because the location of these ressource depends on the template
     *
     * @return object
     */
    final public static function instance()
    {
        if (!isset(self::$instances['Template']))
            self::$instances['Template'] = new Template();
        self::$instances['Template']->setTemplate(self::getScope());
         define ("CSS_PATH", RESSOURCES_PATH.self::$instances['Template']->template.'/css/');
         define ("JS_PATH", RESSOURCES_PATH.self::$instances['Template']->template.'/js/');
         define ("IMG_PATH", RESSOURCES_PATH.self::$instances['Template']->template.'/img/');
        return self::$instances['Template'];
    }

    final private function __clone()
    {

    }

    final protected function get_called_class()
    {
        $backtrace = debug_backtrace();
        return get_class($backtrace[2]['object']);
    }

    public function __construct()
    {
      $this->template = self::getScope();
        $this->template = "default";
    }

    /** Sert à charger la vue d'un controleur
     *
     * Vous pouvez mettre nul ici pour ne pas décorer le template comma valeur à begin. La vue sera chargé si elle existe.
     * @param string title donne un nom au titre de la page
     * @param layout utiliser ou non le template de la page
     * @param string/array alert vous pouvez envoyer un message d'information directement en remplissant alert.
     * Vous pouvez configurer alert comme un tableau de
     * block => warning
     * success => succes
     * error => erreur ou danger
     * info [ce qui est choisi par défaut] => info ou notification
     */
    function useTemplate($title = 'Home', $infos = "")
    {
      $this->setTemplate(file_exists(TEMPLATE_PATH . $this->template . "/header.tpl") ? $this->template : "default");
        foreach (get_object_vars($this) as $key => $var) {
            global $$key;
            $$key = $var;
        }
        $res = $this;
        if (isset($this->child_url) && $this->child_url != NULL) {
            //chargement via get d'un block
            if (is_file(VIEW_PATH . '/' . $this->child_url . '.view')) {
                include VIEW_PATH . $this->child_url . '.view';
            }
            $this->child_url = NULL;
        } else {
                include(TEMPLATE_PATH . $this->template . "/header.tpl");
            $paths = explode('/', $_SERVER['SCRIPT_NAME']);
            $script = $paths[count($paths) - 1];
            $script = str_replace('.php', '.view', $script);

            //Ne fonctionne pas dans la notion de sous dossier
            if (is_file(VIEW_PATH . $script)) {
                include VIEW_PATH . $script;
                if (!stripos($_SERVER['SCRIPT_NAME'], "index.php") && !stripos($_SERVER['SCRIPT_NAME'], "suscribe.php"))
                    $res->get("block.right");
            }
                include(TEMPLATE_PATH . $this->template . "/footer.tpl");
        }
    }

    /** load a page
     *
     * @param type $action
     * @param type $args
     */
    function load($action, $args = array())
    {
       if(stripos($action, '.php'))
          header("location:" . $action);
       header("location:" . self::asLink($action, $args));
    }

    /** load the content of a page inside a page
     *
     * @global type $key
     * @global type $key
     * @global type $key
     * @global type $res
     * @param type $action
     * @param type $args
     */
    function get($action, $args = array())
    {
        if (count(explode('/', $action)) > 1) {
            if (count(explode('http', $action))) {
                return count(explode('php', $action)) > 1 ? file_get_contents($action . '?' . http_build_query($args)) : file_get_contents($action . '.php?' . http_build_query($args));
            } else
                return count(explode('php', $action)) ? file_get_contents(SITE_PATH . $action . '?' . http_build_query($args)) : file_get_contents(SITE_PATH . $action . '.php?' . http_build_query($args));
        }
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
        foreach ($args as $key => $arg) {
            global $$key;
            $$key = $arg;
        }
        global $res;
        $res = $this;
        $res->child_url = $action;
        if ((is_file('./' . $action . '.php'))) {
            include './' . $action . '.php';
        } elseif (is_file('../view/' . $action . '.view')) {
            include self::getCurrentViewPath() . $action . '.view';
        } else
            throw new Exception("Action is not found " . $action, "403");
    }

    /** Reload the page with arguments
     * @param $action
     * @param array $args
     */
    function reload($action, $args = array())
    {
        $this->load($_SERVER['SCRIPT_NAME'], $args);
    }

    /** Set the current templating theme
     * @param string $template
     */
    function setTemplate($template = "default")
    {
      $template = $template == DEFAULT_TEMPLATE ? DEFAULT_TEMPLATE : $this->template = $template;
    }

    /** Renvoie le lien vers la page en fournissant le nom de la page
     * Si la page se trouve dans un autre scope, prefixé ainsi {scope}/{page} l'argument à fournir
     * @static
     * @param string $page
     * @param array $args
     * @return string
     */
    public static function asLink($page = "", $args = array())
    {
        $scope = stripos($page, '/') ? strstr($page, '/', true) : self::getScope();
        $page = self::cleanScopeInGivenPage($page);
        $params = count($args) > 0 ? $args = '?' . http_build_query($args) : '';
        if (is_file('./../../' . $scope . '/load/' . $page . '.php'))
            return './../../' . $scope . '/load/' . $page . '.php' . $params;
        elseif (is_file('./../../' . $scope . '/view/' . $page . '.view'))
            return './../../' . $scope . '/view/' . $page . '.view';
        else throw new Exception("Sorry, don't know the generated link : ".$scope . '/load/' . $page . '.php');
    }

    //TODO: Fix redirection with the view or other dir than load. Work only when you call from an uri inside the projet (load/view)
    public static function asPublicLink($page = "", $args = array()){
        $scopePage = stripos($page, '/') ? strstr($page, '/', true) : self::getScope();
        $scope = self::getScope();
        $uri = self::getCurrentPublicPath();
        if (stripos($uri, $scopePage)){
            $page = self::cleanScopeInGivenPage($page);
            return $uri.$page.'.php';
        }
        else{
            $uri = str_replace($scope, $scopePage, self::getCurrentPublicPath());
            $page = self::cleanScopeInGivenPage($page);
            return $uri.$page.'.php';
        }

    }

  /*
   * In a link in type {scope}/{page}, remove the{scope} to the link
   */
    public static function cleanScopeInGivenPage($page){
        if (strstr($page, '/')) {
            $page = explode('/', $page);
            $page = end($page);
        }
        return $page;
    }

    /** Get the scope of the current page
     * @static
     * @return mixed
     */
    public static function getScope()
    {
        $paths = explode('/', $_SERVER['SCRIPT_NAME']);
        if (stripos($_SERVER['SCRIPT_NAME'], '.php') > 0) {
            array_pop($paths);
        }
        array_pop($paths);
        return end($paths);
    }

    /** Get the controller path of the current page
     * @static
     * @return array|string
     */
    public static function getCurrentControllerPath()
    {
        return self::getCurrentPath();
    }

    /** Get the current path
     * @static
     * @return string
     */
    public static function getCurrentPath()
    {
        $paths = explode('/', $_SERVER['SCRIPT_NAME']);
        if (stripos($_SERVER['SCRIPT_NAME'], '.php') > 0) {
            array_pop($paths);
        }
        $paths = implode('/', $paths) . '/';
        return $paths;
    }

    /** Get the current path from internet (with http, and accessible from outside)
     * @static
     * @return string
     */
    public static function getCurrentPublicPath()
    {
        $paths=$_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI'];
        if (stripos($paths, '.php') > 0) {
            $paths = explode('/', $paths);
            array_pop($paths);
            $paths = implode('/', $paths) . '/';
        }
        return 'http://'.$paths;
    }


    public static function getCurrentViewPath()
    {
        return '../view/';
    }

}