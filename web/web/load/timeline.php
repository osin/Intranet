<?php
include '../../inc/init.inc';

if (isset($id) && $id != "" && $id>0)
{
    try{
        $res->membre = User::find($id);
    }catch (Exception $e){
        $res->membre = $res->user;
    }
}
else $res->membre = $res->user;
$res->block = isset($block) ? $block : "timeline";
$res->follow_status = Follower::isFollow($res->user->id, $res->membre->id) > 0 ? array("class" => "btn btn-primary", "text" => "Abonné"): array("class" => "btn", "text" => "S'abonner");
if (isset($mailSuccess) && $mailSuccess == true ) $alert = array('success' => "Le  message est correctement envoyé");
elseif (isset($mailSuccess) && $mailSuccess == false ) $alert = array('error' => "Mail pas envoyé");
if (isset($feedSuccess) && $feedSuccess == true ) $alert = array('success' => "Timeline mis à jour");
elseif (isset($feedSuccess) && $feedSuccess == false ) $alert = array('error' => "Erreur avec l'Event");
$res->nbEvents = $res->membre->getEvents()->count;
$res->nbUnreadMsg = Mail::count(array('conditions' => array('status' => 1, 'referer_id'=>$res->user->id)));
$res->useTemplate('Mon Profil', isset($alert)? $alert : null);
?>