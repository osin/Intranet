<?php

function GeneratePagination($totalNumberOfItems, $numberOfItemsByPage, $currentPage) {

  $pagination = array();
  $pagination['End'] = ceil($totalNumberOfItems / $numberOfItemsByPage);
  $pagination['Current'] = (int) ($currentPage);
  $pagination['Previous'] = (((int) ($currentPage - 1)) >= 1) ? (int) ($currentPage - 1) : null;
  $pagination['Next'] = (((int) ($currentPage + 1)) <= $pagination['End']) ? (int) ($currentPage + 1) : (int) $pagination['End'];
  
  
  return $pagination;
}
$res->pagination = GeneratePagination($total, $limit, ($currentPage != null) ? $currentPage : (int) 1 );
$res->useTemplate();
?>
