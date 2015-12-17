<?php
include "../../inc/init.inc";
if (isset($_GET['conversation']) && $_GET['conversation'] != 0 && Follower::isFriend($res->user->id, $_GET['conversation'])) {
  $filename = $res->user->id > $_GET['conversation'] ? 'conversations/'.$_GET['conversation'] . "-" . $res->user->id . ".xml" : 'conversations/'.$res->user->id . "-" . $_GET['conversation'] . ".xml";
  if (!Conversation::exists(array('conditions' => array('filename' => $filename)))) {
    $attributes = array(
        "user_id" => $res->user->id,
        "filename" => $filename
    );
    $conversation = Conversation::create($attributes);
  }else
    $conversation = Conversation::find_by_filename($filename);
  if ($conversation == null) {
    throw new Exception('can not init conversation', 504);
  }
  $xmlfile = VAR_PATH.$conversation->filename; 

// <editor-fold defaultstate="collapsed" desc="creation d'un message">
  if (isset($_GET['msg'])) {
    $msg = $_GET['msg'];
    $userChat = isset($_GET['userChat']) ? $_GET['userChat'] : "Invité";

    //Load it using simpleXML
    $doc = new DOMDocument;
    $doc->load($xmlfile);

    //Add a conversation item
    $item = $doc->createElement("item", $msg);

    //Add the sender's name as an attribute
    $sender = $doc->createAttribute("sender");
    $sender->appendChild($doc->createTextNode($userChat));

    //Add another attribute for time on which the message was added
    $time = $doc->createAttribute("time");
    $time->appendChild($doc->createTextNode(date("H:i", time())));

    //Put it together
    $item->appendChild($sender);
    $item->appendChild($time);

    //Add $item to the xml document
    $doc->documentElement->appendChild($item);
    $doc->save($xmlfile);
  }
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="To read the messages, we need to load the XML document for reading">
  $xmldoc = simplexml_load_file($xmlfile);

  $totalElements=count($xmldoc->item);
//Here the number of messages to show
  $elementsToShow = 20;

  if (isset($_GET['count']) && $_GET['count'] != "") {
    echo $totalElements;
  }elseif(isset($_GET['k']) && $_GET['k'] != $totalElements){
//The Emoticon fonction can't follow the update fonction... find a way to not refresh always but just refresh when messages is posted
//Here we just iterate through all <item> nodes printing out what we find there
    for($i = $totalElements-$elementsToShow < 0 ? 0 : $totalElements-$elementsToShow; $i<$totalElements; $i++){
      echo "<div class=\"msg-wrapper\"><div class=\"me\"><strong>".$xmldoc->item[$i]['sender']."</strong> <em>(".$xmldoc->item[$i]['time'].")</em> : ".(string)$xmldoc->item[$i]."</div></div>";
    }
  }
// </editor-fold>
}
?>