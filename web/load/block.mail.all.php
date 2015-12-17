<?php
if ($res->user->id != $res->membre->id) {
  throw new Exception("Not Authorized action");
}
//ne pas supprimer les commentaire de cette page pous l'instant tant qu'on n'est pas sur que ça fonctionne correctement
//$res->mails = Mail::all(array('conditions' => array('referer_id = ?', $res->user->id), 'group' => 'user_id', 'order' => 'created_at desc'))->asArray();
$sql_to_get_last_conversation = "
SELECT * FROM mails uc WHERE  (uc.user_id =".$res->user->id." OR uc.referer_id =".$res->user->id.")
 AND not exists (
   SELECT 1 FROM mails uc2 WHERE uc2.created_at > uc.created_at AND
     (
       (uc.user_id = uc2.user_id AND uc.referer_id = uc2.referer_id) OR
       (uc.user_id = uc2.referer_id AND uc.referer_id = uc2.user_id)
     )
  ) ORDER BY created_at desc";

//throw new Exception( '<pre>'.print_r($res, true).'</pre>');
$res->mails = Mail::find_by_sql($sql_to_get_last_conversation)->asArray();
$res->canComposeMail = $res->user->getPotientialUserNetwork()->count;
$res->useTemplate();
?>

<?php
/*
affiche le dernier message entre 2 et 4

SELECT *
FROM `mails`
WHERE
created_at = (SELECT MAX(created_at)  FROM `mails`
WHERE (
referer_id =2
OR referer_id =4
)
AND (
user_id =2
OR user_id =4
) )

*/

?>