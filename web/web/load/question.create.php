<?php
include '../../inc/init.inc';
$list = array();
foreach (Tag::all()->asArray() as $Tag) {
    $list[] = "&quot; <span class='label'>" . $Tag->name . "</span> &quot;";
}
if (count($list) > 0)
    $res->tagList = implode(',', $list);
$res->useTemplate();