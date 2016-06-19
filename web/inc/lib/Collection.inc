<?php

class Collection {

  private $items;
  private $attributes;

  public function __construct() {
    $this->items = array();
    $this->attributes = array();
    $this->attributes['count'] = 0;
    $this->attributes['isfixedSize'] = false;
    $this->attributes['fixedSize'] = 0;
    $this->attributes['isReadOnly'] = false;
  }

  public function __get($var) {
    if (key_exists  ($var, (array) $this->attributes)) {
      return $this->attributes[$var];
    } else {
      throw new Exception("The property {$var} does not exist", 0);
    }
  }

  public function __set($var, $value) {
    if (key_exists($var, (array) $this->attributes)) {
      $this->attributes[$var] = $value;
    } else {
      throw new Exception("The property {$var} cannot be set as it does not exist", 0);
    }
  }

  /** Return the collection as an array
   * 
   * @return array
   */
  public function asArray() {
    return $this->items;
  }

  /** Usefull to get by asType -asID, -asString...
   * 
   * @param type $method
   * @param type $args
   */
  public function __call($method, $args) { {
      if (count($args)>1)
        throw new Exception('Not proposal using of this function, use only a bool argument) '.(count($args)));
      elseif (strpos($method, 'as') === 0) {
        $field = strtolower(str_replace('as', '', $method));
        $conditions[] = $field;
        if(isset($args[0])) $conditions[] = $args[0];
//        echo '<pre>'.print_r($field, true).'</pre>';
//        echo '<pre>'.print_r($conditions, true).'</pre>';
        return call_user_func_array(array($this, 'asType'), $conditions);
      }else
        throw new Exception('Not proposal using of this function, use only a bool argument) '.(count($args)));
    }
  }

  /**
   * Return an array of id contains in the collection
   * 
   * @return Array
   */
  public function asType($property = 'id', $associate = false) {
    $ids = array();
    if ($this->count == 0) {
      return;
    }
    if ($associate) {
      foreach ($this->items as $object)
        $ids[$object->id] = $object->read_attribute($property);
    } else {
      foreach ($this->items as $object)
        $ids[] = $object->read_attribute($property);
    }
    return $ids;
  }

  /** Add an object to the collectip,
   * 
   * @param type $item
   * @throws Exception
   */
  public function add($item) {
    if ($this->isfixedSize) {
      if ($this->count < $this->fixedSize) {
        $this->items[] = $item;
        $this->count += 1;
      } else {
        throw new Exception("Cannot not add more items to collection. Max size is {$this->fixedSize}", 0);
      }
    } else if ($this->isReadOnly) {
      throw new Exception("Cannot add item to a read only collection", 0);
    } else {
      $this->items[] = $item;
      $this->count += 1;
    }
  }

  /** Add a set of objects to the collections
   * 
   * @param array $items
   */
  public function addRange(array $items) {
    foreach ($items as $item) {
      self::Add($item);
    }
  }

  /** Check if an object is contain inside the collecction
   * 
   * @param type $item
   * @return boolean
   */
  public function contains($item) {
    foreach ($this->items as $i) {
      if ($i == $item) {
        return true;
      }
    }
    return false;
  }

  /** Return the first object of the collection
   * 
   * @return null
   */
  public function first() {
    if (count($this->items) > 0) {
      return $this->items[0];
    }
    else
      return null;
  }

  /** Get object at the index position of an elements
   * 
   * @param type $index
   * @return boolean
   */
  public function get($index) {
    if (key_exists($index, $this->items)) {
      return $this->items[$index];
    }
    return false;
  }

  /** Return the index of the object located in the collection
   * if >=0 the object is in the collection
   * if -1 the object is not in the collections
   * @param type $item
   * @param type $startIndex
   * @return type
   */
  public function IndexOf($item, $startIndex = 0) {
    for ($i = $startIndex; $i < $this->count; $i++) {
      if ($this->items[$i] == $item) {
        return $i;
        break;
      }
    }
    return -1;
  }

  /** Return the last object of the collection
   * 
   * @return null
   */
  public function last() {
    if ($this->count >= 1) {
      return $this->items[count($this->items) - 1];
    }
    else
      return null;
  }

  /** Return the last position of an object inside the collection.
   * If -1, the object is not in the collection
   * 
   * @param type $item
   * @return int
   */
  public function lastIndexOf($item) {
    $lastIndex = -1;

    for ($i = 0; $i < $this->count; $i++) {
      if ($this->items[$i] == $item) {
        $lastIndex = $i;
      }
    }
    return $lastIndex;
  }

  /** Insert an object at an specified position in the collection
   * 
   * @param type $index
   * @param type $item
   * @throws Exception
   */
  public function insert($index, $item) {
    if ($this->isfixedSize) {
      if ($index < $this->fixedSize) {
        $this->items[$index] = $item;
        $this->count += 1;
      } else {
        throw new Exception("Cannot insert item at {$index}. Max size is {$this->fixedSize}", 0);
      }
    } else if ($this->isReadOnly) {
      throw new Exception("Cannot insert an item into a read only collection", 0);
    } else {
      $this->items[$index] = $item;
      $this->count += 1;
    }
  }

  /** Remove the object in the collection
   * Return -1 if the object doesn't exist
   * 
   * @param type $item
   */
  public function remove($item) {
    $index = self::IndexOf($item);
    self::RemoveAt($index);
  }

  /** Remove an object at a specified position
   *  Return -1 if the object or the position don't exist
   * @param type $index
   * @throws Exception
   */
  public function removeAt($index) {
    if (!$this->isReadOnly) {
      if (key_exists($index, $this->items)) {
        unset($this->items[$index]);
        $this->count -= 1;
      } else {
        throw new Exception("Index out of range. The index {$index} is out of range of the collection", 0);
      }
    } else {
      throw new Exception("Cannot remove item from read only collection", 0);
    }
  }

  /** Remove a set of object starting and ending at specified position
   * 
   * @param type $startIndex
   * @param type $endIndex
   */
  public function removeRange($startIndex, $endIndex) {
    for ($i = $startIndex; $i < $endIndex; $i++) {
      self::RemoveAt($i);
    }
  }

  /** Sort in alphabetic order object
   * 
   */
  public function sort() {
    sort($this->items, SORT_STRING);
  }

}

?>