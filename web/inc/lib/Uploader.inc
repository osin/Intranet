<?php 
/**
 * Uploader to transfer file... can resize image (need for use with AmazonS3
 * Works on PHP 5
 */
class uploader{
	/**
	 * Array, The file object as $_FILES['element'].
	 * String, file location.
	 */
	public $source;
	/**
	 * Destination file location as folder.
	 */
	public $destDir;
	/**
	 * Directory for Resized images.
	 */
	public $resizeDir;
	/**
	 * Directory for Cropped images.
	 */
	public $cropDir;
	/**
	 * stores information for uploading file
	 */
	private $info = '';
	/**
	 * Handles the error when it occurs.
	 */
	private $errorMsg = '';
	/**
	 * new width for resizing and cropping.
	 */
	public $newWidth;
	/**
	 * new height for resizing and cropping.
	 */
	public $newHeight;
	/**
	 * TOP postion to cropping image.
	 */
	public $top = 0;
	/**
	 * LEFT position for cropping image.
	 */
	public $left = 0;
	/**
	 * JPG quality (0 - 100). used for image resizing or cropping.
	 */
	public $quality = 60;
	/**
	 * Boolean: Auto name, to prevent file name duplicate.
	 */
	public $autoName = true;
	/**
	 * Handle new generated file name for Auto File name.
	 */
	public $fileName;
	
	public function __construct(){
		//nothing
	}
	/**
	 * Uploads the file to the server.
	 * @param Array $_FILES[]
	 */
	public function upload($source){
		if($source != ""){
			$this->source = $source;
		}
		if(is_array($this->source)){
			if($this->fileExists()){
				return false;
			}
			return $this->copyFile();
		} else {
			if(preg_match('|^http(s)?://[a-z0-9-]+(.[a-z0-9-]+)*(:[0-9]+)?(/.*)?$|i', $this->source)){
				$this->copyExternalFile();
			} else {
				return $this->source;
			}
		}
	}
	/**
	 * return the error messages.
	 * @return Text messages.
	 */
	public function getError(){
		return $this->errorMsg;
	}
	/**
	 * Get uploading information.
	 */
	public function getInfo(){
		return $this->info;
	}
	/**
	 * Copy the uploaded file to destination.
	 */
	private function copyFile(){
		if(!$this->isWritable()){
			$this->errorMsg .= '<div>Error, the directory: ('.$this->destDir.') is not writable. Please fix the permission to be able to upload.</div>';
			return false;
		}
		if($this->autoName==true){
			$ext = explode('.',$this->source['name']);
			$this->fileName = date("U").'.'. $ext[(sizeof($ext)-1)];
		} else {
			$this->fileName = $this->source['name'];
		}
		if(copy($this->source['tmp_name'],$this->destDir . $this->fileName)){
			// Done.
			$this->info .= '<div>file was uploaded successfully.</div>';
		} else {
			$this->errorMsg .= '<div>Error, the file was not uploaded correctly because of an internal error. Please try again, if you see this message again, please contact web admin.</div>';
		}
	}
	
	/**
	 * Copy external file.
	 * @return boolean
	 */
	private function copyExternalFile(){
		$file_name = date("U").basename($this->source);
		if(copy($this->source,$this->destDir . $file_name)){
			// Done.
			$this->info .= '<div>file was uploaded successfully.</div>';
			return $file_name;
		} else {
			$this->errorMsg .= '<div>Error, the file was not uploaded correctly because of an internal error. Please try again, if you see this message again, please contact web admin.</div>';
			return false;
		}
	}
	/**
	 * Checks if the file was uploaded.
	 * @return boolean
	 */
	private function uploaded(){
		if($this->source['tmp_name']=="" || $this->source['error'] !=0){
			$this->errorMsg .= '<div>Error, file was not uploaded to the server. Please try again.</div>';
			return false;
		} else 
			return true;
	}
	/**
	 * Prepares the directory.
	 */
	private function preDir(){
		if($this->destDir!="" && substr($this->destDir, -1,1) != "/"){
			$this->destDir = $this->destDir . '/';
		}
		if($this->resizeDir!="" && substr($this->resizeDir, -1,1) != "/"){
			$this->destDir = $this->resizeDir . '/';
		}
		if($this->cropDir!="" && substr($this->cropDir, -1,1) != "/"){
			$this->destDir = $this->cropDir . '/';
		}
	}
	/**
	 * Check if the folder is writable or not.
	 * @return boolean
	 */
	private function isWritable(){
		$err = false;
		if(!is_writeable($this->destDir) && $this->destDir!=""){
			$this->errorMsg .= '<div>Error, the directory ('.$this->destDir.') is not writeable. File could not be uploaded.</div>';
			$err = true;
		}
		if(!is_writeable($this->resizeDir) && $this->resizeDir!=""){
			$this->errorMsg .= '<div>Error, the directory ('.$this->resizeDir.') is not writeable. File could not be resized.</div>';
			$err = true;
		}
		if(!is_writeable($this->cropDir) && $this->cropDir!=""){
			$this->errorMsg .= '<div>Error, the directory ('.$this->cropDir.') is not writeable. File could not be cropped.</div>';
			$err = true;
		}
		if($err == true){
			return false;
		} else {
			return true;
		}
	}
	/**
	 * Checks if the file exists on the server
	 * @return boolean
	 */
	private function fileExists(){
		$this->preDir();
		if(file_exists($this->destDir.$this->source)){
			$this->errorMsg .= '<div>Upload error because file already exists.</div>';
			return true;
		} else {
			return false;
		}
	}
	/**
	 * /586742130./8532 Crops image.
	 * @return Text fileName or False on error
	 */
	public function crop($file='',$width='',$height='',$top='',$left=''){
		if($file!=""){ $this->source = $file;}
		if ($width != '') $this->newWidth = $width;
		if ($height != '') $this->newHeight = $height;
		if ($top != '') $this->top = $top;
		if ($left != '') $this->left = $left;
		return $this->_resize_crop(true);
	}
	/**
	 * Resizes an image.
	 * @return Text fileName or False on error
	 */
	public function resize($file='',$width='',$height='',$fixed='width'){
		if($file!=""){ $this->source = $file; }
		if($width != '') $this->newWidth = $width;
		if($height != '') $this->newHeight = $height;
		return $this->_resize_crop(false,$fixed);
	}
	/**
	 * Get the Temp file location for the file.
	 * If the Source was a file location, it returns the same file location.
	 * @return Text Temp File Location
	 */
	private function getTemp(){
		if(is_array($this->source)){
			return $this->source['tmp_name'];
		} else {
			return $this->source;
		}
	}
	/**
	 * Get the File location.
	 * If the source was a file location, it returns the same file location.
	 * @return Text File Location
	 */
	private function getFile(){
		if(is_array($this->source)){
			return $this->source['name'];
		} else {
			return $this->source;
		}
	}
	/**
	 * Resize or crop- the image.
	 * @param boolean $crop
	 * @return Text fileName False on error
	 */
	private function _resize_crop ($crop,$fixed) {
		$ext = explode(".",$this->getFile());
		$ext = strtolower(end($ext));
		list($width, $height) = getimagesize($this->getTemp());
		if(!$crop){
			$ratio = $width/$height;
			if($fixed=="width"){
				if ($this->newWidth/$this->newHeight > $ratio) {
					$this->newWidth = $this->newHeight*$ratio;
				} else {
					$this->newHeight = $this->newWidth/$ratio;
				}
			} else {
				if ($this->newWidth/$this->newHeight > $ratio) {
					$this->newHeight = $this->newWidth*$ratio;
				} else {
					$this->newWidth = $this->newHeight/$ratio;
				}
			}
		}
		$normal  = imagecreatetruecolor($this->newWidth, $this->newHeight);
		if($ext == "jpg") {
			$src = imagecreatefromjpeg($this->getTemp());
		} else if($ext == "gif") {
            $src = imagecreatefromgif ($this->getTemp());
		} else if($ext == "png") {
            $src = imagecreatefrompng ($this->getTemp());
		}
		if($crop){
			$pre = $this->newWidth.'x'.$this->newHeight.'_crop_';
 			if(imagecopy($normal, $src, 0, 0, $this->top, $this->left, $this->newWidth, $this->newHeight)){
 				$this->info .= '<div>image was cropped and saved.</div>';
 			}
 			$dir = $this->cropDir;
		} else {
			$pre = $this->newWidth.'x'.$this->newHeight.'_';
			if(imagecopyresampled($normal, $src, 0, 0, 0, 0, $this->newWidth, $this->newHeight, $width, $height)){
				$this->info .= '<div>image was resized and saved.</div>';
			}
			$dir = $this->resizeDir;
		}
		if($ext == "jpg" || $ext == "jpeg") {
			imagejpeg($normal, $dir . $pre . $this->getFile(), $this->quality);
		} else if($ext == "gif") {
			imagegif ($normal, $dir . $pre . $this->getFile());
		} else if($ext == "png") {
			imagepng ($normal, $dir . $pre . $this->getFile(),0);
		}
		imagedestroy($src);
		return $src;
	}
}



/** How to use (no controler created for the moment so... )...
 *
<?php
$uploader = new uploader();
// Setting properties then Uploading the image
$uploader->source = $_FILES['field_image'];
$uploader->destDir = "images/";
$uploader->autoName = true; // Set false as default.
$uploader->upload();
// =====================
// Or you may use this too
$uploader->destDir = "images/";
$uploader->upload($_FILES['field_image']);
?>
Note: upload() returns the file name uploaded.

To get error messages, use this code:
<?php
echo $uploader->getError();
// This function will return all errors occured while uploading the image. But it's not printing it to print it where ever you want.
?>
And to get Information messages use this code:
<?php
echo $uploader->getInfo();
// This function will return all information to let the user print it where ever he wants.
?>

After uploading this image, we may resize it. To do that use the following code:
<?php
$uploader->newWidth = 75; // in Pexels.
$uploader->newHeight = 75;
$uploader->resizeDir = "images/resized/";
$uploader->resize();
?>
You may also use this syntax:
<?php
// $uploader->resize($file,$width,$height);
// $file could has a value of "" (nothing), in this case we use the last uploader file.
// if the $file has a String value (file url) it will resize this new file.
$uploader->resizeDir = "images/resized/";
$uploader->resize('',75,65); // Default function call. This will resize with ratio of Width
$uploader->resize('',75,65,'width'); // Same as above.
$uploader->resize('',75,65,'height'); // 'height' means the the ratio for resizing will be calculated by Height.
// Or you may upload and resize in the same line:
$uploader->resizeDir = "images/resized/";
$uploader->resize($uploader->upload(),75,65,'height');
?>

Crop this image after uploading it:
<?php
$uploader->cropDir = "images/cropped/";
$uploader->newWidth = 75;
$uploader->newHeight = 75;
$uploader->top = 20; // Default is ZERO.. This used to set the cropping top location from the original image.
$uploader->left = 40; // Default is ZERO.. This used to set the cropping left location from the original image.
$uploader->crop();
// You may also use this:
// $uploader->crop($file,$width,$height,$top,$left);
$uploader->cropDir = "images/cropped/";
$uploader->crop('',75,75,20,40);
?> 
 */
?>