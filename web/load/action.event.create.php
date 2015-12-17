<?php
include '../../inc/init.inc';
if (isset($event_link) && $event_link != "") {
  if (!isset($data)) {
    $data = file_get_contents(SITE_PATH."service/parse.link.php?url=".$event_link);
  }
  $data = json_decode($data);

  if ($data == null || count($data) == 0) {
    throw new Exception('Invalid parse' . print_r($data, true));
  }
  
  if($data->total_images > 0 && isset($cur_image) && $cur_image > 0){
    $data->image = $data->images[$cur_image-1];
  }
  $args['data'] = json_encode($data);
  $args['link'] = $event_link;
}
if(isset($event_compose) && !empty($event_compose)){
  $args['content'] = $event_compose;
}

if($args){
    $args['privacy'] = empty($res->user->privacy) ? User::$PRIVACY_PUBLIC : $res->user->privacy ;
    $args['user_id'] = $res->user->id;
    $event = Event::create($args);
    $res->load('timeline', array('createEvent' => true));
}

$res->load('timeline', array('createEvent' => false));
