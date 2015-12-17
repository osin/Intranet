<?php

class Log extends ActiveRecord\Model {
  /** This function is usefull to Log every Object passing by the model
   *
   * @param ActiveRecord\Model $object
   * @param String $type 
   */

  /** Call the Event class to put a new event in the timeline
   * 
   */
  function after_create() {
    
  }

  /** We directly construct the object saved by the log data
   *
   * @return ActiveRecord Object or Null
   */
  function constructLogObject() {
    if (!isset($this->object))
      return;
    return json_decode($this->object);
  }

  /** We create here a log directly, this function is called by the Model Hook,
   * After the log creation an event is created for the timeline
   *
   * @param type $object
   * @param type $type 
   * param type is usually update/create/delete
   */
  static function commit($object, $type) {
    $argsLog['model'] = get_class($object);
    $argsLog['user_id'] = isset($_SESSION['lbi_user']) ? $_SESSION['lbi_user']->id : 0;
    $argsLog['record'] = $object->id;
    $argsEvent = $argsLog;
    $argsLog['type'] = $type;
    $argsLog['is_readable'] = self::is_unAllow(get_class($object), $type) == true ? 0 : 1;
    $argsLog['object'] = $object->to_json();
    $log = self::create($argsLog);
    if ($argsLog['is_readable'] && !isset($object->log_id)) {
      $argsEvent['privacy'] = 'public';
      $argsEvent['log_id'] = $log->id;
      $event = Event::create($argsEvent);
    }
  }

  /** Here to know which confidentiality parameter to pass for the event creating
   *
   * @param type $model
   * @param type $property optionnal
   * Some model accept only to read a property instead of entire model.
   * @return type 
   */
  static function is_unAllow($model, $type) {
    if (isset(Event::$unAllowClass[$model]['type']))
      return in_array($type, Event::$unAllowClass[$model]['type']);
    return in_array($model, Event::$unAllowClass);
  }

  /** We directly retrieve the object used by the log record find by model><record
   *
   * @return ActiveRecord Object or Null
   */
  function getObject() {
    if (!isset($this->model) && !isset($this->record))
      return;
    return call_user_func_array(array('\\' . $this->model, 'find'), array($this->record));
  }

}

?>