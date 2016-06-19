<?php
include '../../inc/init.inc';
if((isset ($referer_name) ||isset($referer_id)) && isset($message_to_referer_name))
{

  if(!isset($referer_id) && isset($referer_name))
    preg_match('#\(([^)]+)\)#', $referer_name, $recever_id);
  elseif(isset($referer_id) && !isset($referer_name))
    $recever_id = array(0,$referer_id);

  try{
    if ($res->user->isFollow(User::find($recever_id[1])->id) > 0 || $res->user->isFollowed(User::find($recever_id[1])->id) > 0 ){
      $attributes = array('message' => $message_to_referer_name, 'referer_id' => User::find($recever_id[1])->id, 'user_id' => $res->user->id, 'status' => 1 );
      $mail = Mail::create($attributes);
      $mail->save();
      $res->load('timeline', array('id' => $res->user->id, 'mailSuccess' => true, 'block' => 'mail.all'));
    }
  }catch(Exception $e){
    $res->load('timeline', array('id' => $res->user->id, 'mailSuccess' => false, 'block' => 'mail.all'));
  }
}else
$res->load('timeline', array('id' => $res->user->id, 'block' => 'mail.all'));
?>
