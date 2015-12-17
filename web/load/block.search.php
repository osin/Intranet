<?php

if (!isset($search) && !is_array($search) && !isset($model) && !class_exists($model))
  exit;

$s = ActiveRecord\Connection::instance();

$model = new $model;
$columns = $model->table()->columns;
foreach ($search as $filter) {
  if (!isset($columns[$filter['field']]))
    continue;
  switch ($filter['type']) {
    case 'text':
      if (isset($filter['class'])) {
        $class = $filter['class'];
      }
      elseif (isset($columns[$filter['field']]->lenght) && ($columns[$filter[$field]]->type != 2)) {
        if ($columns[$filter['field']]->lenght < 12)
          $class = "small";
        elseif ($columns[$filter['field']]->lenght < 19)
          $class = "small";
        else
          $class = "large";
      }else
        $class = "small";
      $form[] = '<input type="text" placeholder="' . $filter['label'] . '" class="input-' . $class . ' search-query" style="margin-right: 10px">';
      break;
    case 'list':
      if (strpos($filter['field'], '_id')) {
        $join_model = ucfirst(substr($filter['field'], 0, strlen($filter['field']) - 3));
        $values = $join_model::all()->asName(true);
        $str = ' '.$join_model . ' <select class="input-small" name="' . $filter['field'] . '"><option value="">--</option>';
        foreach ($values as $key => $value)
          $str.= '<option value="' . $key . '">' . $value . '</option>';
        $str.='</select>';
        $form[$filter['field']] = $str;
      } else {
        $query = $s->query('SELECT DISTINCT ' . $filter['field'] . ' FROM ' . $model->table_name());
        $str = ' '.ucfirst($filter['field']) . ' <select class="input-small" name="' . $filter['field'] . '"><option value="">--</option>';
        while (($row = $query->fetch(PDO::FETCH_ASSOC)))
          $str.= '<option value="' . $row[$filter['field']] . '">' . $row[$filter['field']] . '</option>';
        $str.='</select>';
        $form[$filter['field']] = $str;
      }
      break;
    default:
      break;
  }
  $res->form = $form;
}
$res->useTemplate();
?>
