<?php  

/**
 * Logging Class
 *
 * @package		Ivke Framework
 * @subpackage	Libraries
 * @category	Logging
 * @author		Ivan Stojmenovic Ivke
 */

class Log {

	protected $_level   = null;
	protected $_path    = null;
	protected $_status  = true;
	
	public $date_format = null;
	public $file_ext    = null;
	public $write_mode  = null;
	public $log_prefix  = null;
	public $log_error   = null;
	
	const DEB        = 'DEBUG';
	const ERR        = 'ERROR';
	const INF        = 'INFO';
	const NOT        = 'NOTICE';
	const UNK        = 'UNKNOW_MODE';

	// --------------------------------------------------------------------

	/**
	 * Log class constructor
	 *
	 * This function is automaticly called.
	 * 
	 * @author Ivan Stojmenovic
	 */	
	 
	public function __construct()
	{
		if ($this->get_status() == true) {
		
			// Configuration 

			$this->set_path('logs/');                  // Directory name <full path>
			$this->set_date_format('Y-m-d');           // Date format
			$this->set_extension('.txt');              // Log file extension
			$this->set_mode('a');                      // Writig mode
			$this->set_prefix('log_');				   // File name prefix example(log_2012-06-26)
		}
		
		Log::log_message('debug','Log class initialized');
	}

	// --------------------------------------------------------------------

	/**
	 * Levels
	 *
	 * This function is used to check what level is used.
	 *
	 * @param  $mode|string
	 * @return Text
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */	
	
	public function level($mode = null)
	{
		if (is_numeric($mode) && isset($mode)) {
			switch($mode) {
				case 1:
					return $this->set_level_mode(Log::DEB);
				break;
				case 2:
					return $this->set_level_mode(Log::ERR);
				break;
				case 3:
					return $this->set_level_mode(Log::INF);
				break;
				case 4:
					return $this->set_level_mode(Log::NOT);
				break;
			default:
				return $this->set_level_mode(Log::UNK);				
			}
		}
		if (is_string($mode) && isset($mode)) {
			switch($mode) {
				case 'debug':
					return $this->set_level_mode(Log::DEB);
				break;
				case 'error':
					return $this->set_level_mode(Log::ERR);
				break;
				case 'info':
					return $this->set_level_mode(Log::INF);
				break;
				case 'notice':
					return $this->set_level_mode(Log::NOT);
				break;
			default:
				return $this->set_level_mode(Log::UNK);
				
			}			
		}
	}
	
	
	// --------------------------------------------------------------------

	/**
	 * Get Path 
	 *
	 * This function is used to return the directory path name.
	 * 
	 * @return Text
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */	

	public function get_path()
	{
		return $this->_path;
	}
	
	// --------------------------------------------------------------------

	/**
	 * Set Path 
	 *
	 * This function is used to set new directory path name.
	 * 
	 * @param $path|string
	 * @return Text
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */	
	 
	public function set_path($path)
	{
		return $this->_path = $path;
	}
	
	// --------------------------------------------------------------------

	/**
	 * Get Date Format
	 *
	 * This function is used to get date format.
	 * 
	 * @return Text
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */	
	 
	public function get_date_format()
	{
		return $this->date_format;
	}
	
	// --------------------------------------------------------------------

	/**
	 * Set Date Format
	 *
	 * This function is used to set new date format.
	 * 
	 * @param $format|string
	 * @return Text
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */	
	 
	public function set_date_format($format)
	{
		return $this->date_format = $format;
	}
	
	// --------------------------------------------------------------------

	/**
	 * Get Extension
	 *
	 * This function is used to get file extension.
	 * 
	 * @return Text
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */	
	 
	public function get_extension()
	{
		return $this->file_ext;
	}
	
	// --------------------------------------------------------------------

	/**
	 * Set extension
	 *
	 * This function is used to set new file extension.
	 * 
	 * @param $extension|string
	 * @return Text
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */	
	 
	public function set_extension($extension)
	{
		return $this->file_ext = $extension;
	}
	
	// --------------------------------------------------------------------

	/**
	 * Get Mode
	 *
	 * This function is used to get fopen mode.
	 * 
	 * @return Text
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */	
	 
	public function get_mode()
	{
		return $this->write_mode;
	}
	
	// --------------------------------------------------------------------

	/**
	 * Set Mode
	 *
	 * This function is used to set new fopen mode.
	 * 
	 * @param $mode|string
	 * @return Text
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */	
	 
	public function set_mode($mode)
	{
		return $this->write_mode = $mode;
	}
	
	// --------------------------------------------------------------------

	/**
	 * Get Prefix
	 *
	 * This function is used to get filename prefix.
	 * Example: log_
	 *
	 * @return Text
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */	
	 
	public function get_prefix()
	{
		return $this->log_prefix;
	}

	// --------------------------------------------------------------------

	/**
	 * Set Prefix
	 *
	 * This function is used to set new filename prefix.
	 * 
	 * @param $prefix|string
	 * @return Text
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */	
	 
	public function set_prefix($prefix)
	{
		return $this->log_prefix = $prefix;
	}

	// --------------------------------------------------------------------

	/**
	 * Get Status
	 *
	 * This function is used to check if is class established.
	 *
	 * @return bool
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */		
	
	public function get_status()
	{
		return $this->_status;
	}
	
	// --------------------------------------------------------------------

	/**
	 * Set Status
	 *
	 * This function is used to set new status.
	 * TRUE is established. FALSE is disabled.
	 *
	 * @param  $status|string
	 * @return Text
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */	
	 
	public function set_status($status)
	{
		return $this->_status = $status;
	}
	
	// --------------------------------------------------------------------

	/**
	 * Get Level Mode
	 *
	 * This function is used to see what level mode is used.
	 *
	 * @return Text or integer
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */		
	 
	public function get_level_mode()
	{
		return $this->_level;
	}
	
	// --------------------------------------------------------------------

	/**
	 * Set Level Mode
	 *
	 * This function is used to set new level mode.
	 *
	 * @param  $mode|string
	 * @return Text
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */	
	public function set_level_mode($mode)
	{
		return $this->_level = $mode;
	}
	
	// --------------------------------------------------------------------

	/**
	 * Log Message
	 *
	 * This function is used to write new message in file.
	 *
	 * @param  $id|string or int
	 * @param  $message|string 
	 * @return bool
	 * @access public
	 *
	 * @author Ivan Stojmenovic
	 */	
	
	public function log_message($id, $message)
	{
		if ($this->get_status() == false) {
			return FALSE;
		}	

		if (!is_dir($this->get_path())) {
			return FALSE;
		}
		if ($this->get_status() == true) {
		
			$path = $this->get_path();

			$prefix       = $this->get_prefix();
			
			$date_input   = date('Y-m-d H:i:s');
					
			$file_name    = $path.$prefix.date($this->get_date_format()).$this->get_extension();	

			$fopen = fopen($file_name, $this->get_mode());
			
			if (!file_exists($file_name)){
				return false;
			}
			
			$template = '['.$date_input.']--'.strtoupper($id).'--'.$message."\r\n";
			
			fwrite($fopen, $template);
			
			fclose($fopen);
		}		
	}
	



}

?>