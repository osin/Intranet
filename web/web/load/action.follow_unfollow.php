<?php
include '../../inc/init.inc';
if (isset($userToFollow))
  $res->user->setFollowUser($userToFollow);
if (isset($userToUnfollow)) {
  $res->user->setUnfollowUser($userToUnfollow);
}
exit;
?>
