<?php
include '../../inc/init.inc';
error_reporting(E_ALL  & ~E_NOTICE & ~E_WARNING);
//ini_set('display_errors', 0);

$url = urldecode($_GET['url']);
$url = Request::checkValues($url);

$return_array = array();

$base_url = substr($url,0, strpos($url, "/",8));
$relative_url = substr($url,0, strrpos($url, "/")+1);


// Get Data
$cc = new cURL(false);

$string = $cc->get($url);
$string = str_replace(array("\n","\r","\t",'</span>','</div>'), '', $string);

$string = preg_replace('/(<(div|span)\s[^>]+\s?>)/',  '', $string);
if (mb_detect_encoding($string, "UTF-8") != "UTF-8") 
	$string = utf8_encode($string);

// Parse Title
$nodes = Request::extract_tags( $string, 'title' );
$return_array['title'] = trim($nodes[0]['contents']);

// Parse Base
$base_override = false; 
$base_regex = '/<base[^>]*'.'href=[\"|\'](.*)[\"|\']/Ui';
preg_match_all($base_regex, $string, $base_match, PREG_PATTERN_ORDER);
if(strlen($base_match[1][0]) > 0)
{
	$base_url = $base_match[1][0];
	$base_override = true; 
}

// Parse Description
$return_array['description'] = '';
$nodes = Request::extract_tags( $string, 'meta' );
foreach($nodes as $node)
{
	if (strtolower($node['attributes']['name']) == 'description')
		$return_array['description'] = trim($node['attributes']['content']);
}

// Parse Images
$images_array = Request::extract_tags( $string, 'img' );
$images = array();
for ($i=0;$i<=sizeof($images_array);$i++)
{
	$img = trim(@$images_array[$i]['attributes']['src']);
	$width = preg_replace("/[^0-9.]/", '', $images_array[$i]['attributes']['width']);
	$height = preg_replace("/[^0-9.]/", '', $images_array[$i]['attributes']['height']);
	
	$ext = trim(pathinfo($img, PATHINFO_EXTENSION));
	
	if($img && $ext != 'gif') 
	{
		if (substr($img,0,7) == 'http://')
			;
		else	if (substr($img,0,1) == '/' || $base_override)
			$img = $base_url . $img;
		else 
			$img = $relative_url . $img;
		
		if ($width == '' && $height == '')
		{
			$details = @getimagesize($img);
			
			if(is_array($details))
			{
				list($width, $height, $type, $attr) = $details;
			} 
		}
		$width = intval($width);
		$height = intval($height);
		
		
		if ($width > 199 || $height > 199 )
		{
			if (
				(($width > 0 && $height > 0 && (($width / $height) < 3) && (($width / $height) > .2)) 
					|| ($width > 0 && $height == 0 && $width < 700) 
					|| ($width == 0 && $height > 0 && $height < 700)
				) 
				&& strpos($img, 'logo') === false )
			{
				$images[] = array("img" => $img, "width" => $width, "height" => $height, 'area' =>  ($width * $height),'offset' => $images_array[$i]['offset']);
			}
		}
		
	}
}
$return_array['images'] = array_values(($images));
$return_array['total_images'] = count($return_array['images']); 

header('Cache-Control: no-cache, must-revalidate');
header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
header('Content-type: application/json');

echo json_encode($return_array);
?>
