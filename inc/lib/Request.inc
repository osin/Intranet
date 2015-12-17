<?php

class Request {

  public function __construct() {
    ;
  }

  function post_request($url, $data, $referer = '') {
    $curl_opts = array(
        CURLOPT_CONNECTTIMEOUT => 10,
        CURLOPT_RETURNTRANSFER => TRUE,
        CURLOPT_HEADER => TRUE,
        CURLOPT_TIMEOUT => 60,
        CURLOPT_USERAGENT => "Labo-Intranet Server ", // FIXME: add version
        CURLOPT_HTTPHEADER => array("Accept: application/json; charset=UTF-8"),
        CURLOPT_SSL_VERIFYHOST => 0,
        CURLOPT_SSL_VERIFYPEER => 0,
        CURLOPT_CUSTOMREQUEST, "POST",
        CURLOPT_URL, $url . "?amp" . http_build_query($data, null, '&'),
        CURLOPT_POST, true,
        CURLOPT_POSTFIELDS, http_build_query($data, null, '&'),
    );
    echo "DATA is : ";
    echo '<pre>' . print_r(http_build_query($data, null, '&'), true) . '</pre>';
    $ch = curl_init();
    curl_setopt_array($ch, $curl_opts);
    $result = curl_exec($ch);
    return $result;
  }

  static function sort_array_by_field($original, $field, $descending = false) {
    $sortArr = array();

    foreach ($original as $key => $value) {
      $sortArr[$key] = $value[$field];
    }

    if ($descending) {
      arsort($sortArr);
    } else {
      asort($sortArr);
    }

    $resultArr = array();
    foreach ($sortArr as $key => $value) {
      $resultArr[$key] = $original[$key];
    }

    return $resultArr;
  }

  /** retrieve the bad data given to the link
   * 
   */
  static function checkValues($value) {
    $value = trim($value);
    if (get_magic_quotes_gpc()) {
      $value = stripslashes($value);
    }
    $value = strtr($value, array_flip(get_html_translation_table(HTML_ENTITIES)));
    $value = strip_tags($value);
    $value = htmlspecialchars($value);
    return $value;
  }

  static function extract_tags($html, $tag, $selfclosing = null, $return_the_entire_tag = false, $charset = 'ISO-8859-1') {

    if (is_array($tag)) {
      $tag = implode('|', $tag);
    }

    //If the user didn't specify if $tag is a self-closing tag we try to auto-detect it
    //by checking against a list of known self-closing tags.
    $selfclosing_tags = array('area', 'base', 'basefont', 'br', 'hr', 'input', 'img', 'link', 'meta', 'col', 'param');
    if (is_null($selfclosing)) {
      $selfclosing = in_array($tag, $selfclosing_tags);
    }

    //The regexp is different for normal and self-closing tags because I can't figure out 
    //how to make a sufficiently robust unified one.
    if ($selfclosing) {
      $tag_pattern =
              '@<(?P<tag>' . $tag . ')			# <tag
			(?P<attributes>\s[^>]+)?		# attributes, if any
			\s*/?>					# /> or just >, being lenient here 
			@xsi';
    } else {
      $tag_pattern =
              '@<(?P<tag>' . $tag . ')			# <tag
			(?P<attributes>\s[^>]+)?		# attributes, if any
			\s*>					# >
			(?P<contents>.*?)			# tag contents
			</(?P=tag)>				# the closing </tag>
			@xsi';
    }

    $attribute_pattern =
            '@
		(?P<name>\w+)							# attribute name
		\s*=\s*
		(
			(?P<quote>[\"\'])(?P<value_quoted>.*?)(?P=quote)	# a quoted value
			|							# or
			(?P<value_unquoted>[^\s"\']+?)(?:\s+|$)			# an unquoted value (terminated by whitespace or EOF) 
		)
		@xsi';

    //Find all tags 
    if (!preg_match_all($tag_pattern, $html, $matches, PREG_SET_ORDER | PREG_OFFSET_CAPTURE)) {
      //Return an empty array if we didn't find anything
      return array();
    }

    $tags = array();
    foreach ($matches as $match) {

      //Parse tag attributes, if any
      $attributes = array();
      if (!empty($match['attributes'][0])) {

        if (preg_match_all($attribute_pattern, $match['attributes'][0], $attribute_data, PREG_SET_ORDER)) {
          //Turn the attribute data into a name->value array
          foreach ($attribute_data as $attr) {
            if (!empty($attr['value_quoted'])) {
              $value = $attr['value_quoted'];
            } else if (!empty($attr['value_unquoted'])) {
              $value = $attr['value_unquoted'];
            } else {
              $value = '';
            }

            //Passing the value through html_entity_decode is handy when you want
            //to extract link URLs or something like that. You might want to remove
            //or modify this call if it doesn't fit your situation.
            $value = html_entity_decode($value, ENT_QUOTES, $charset);

            $attributes[$attr['name']] = $value;
          }
        }
      }

      $tag = array(
          'tag_name' => $match['tag'][0],
          'offset' => $match[0][1],
          'contents' => !empty($match['contents']) ? $match['contents'][0] : '', //empty for self-closing tags
          'attributes' => $attributes,
      );
      if ($return_the_entire_tag) {
        $tag['full_tag'] = $match[0][0];
      }

      $tags[] = $tag;
    }

    return $tags;
  }

}

?>
