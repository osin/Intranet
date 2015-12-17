<?php

include '../../inc/init.inc';
if ((!isset($title) || $title == "") && (!isset($question_id)) || (!isset($content) || $content == "")) {
  $res->load('questions', array());
} else {
    if (isset($title)) $args['title'] = $title;
    if (isset($question_id)) $args['question_id'] = $question_id;
    
    $args['content'] = $content;
    $args['user_id'] = $res->user->id;
    $question = Question::create($args);
    
    if (isset($tag_list_values)) {
      $tags = explode(',', $tag_list_values);
      foreach ($tags as $tagName) {
        $tagName = strtoupper($tagName);
        if (Tag::exists(array('name' => $tagName)))
          $tag = Tag::find_by_name($tagName);
        else
          $tag = Tag::create(array('name' => $tagName));
        Questions_Tag::create(
                        array(
                            'question_id' => $question->id,
                            'tag_id' => $tag->id,
                ));
      }
    }
}
$res->load('question', array('id'=> $question->id));
