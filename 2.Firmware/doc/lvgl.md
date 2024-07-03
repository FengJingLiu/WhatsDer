### Input device events[](https://docs.lvgl.io/master/overview/event.html#input-device-events "Link to this heading")

* [LV_EVENT_PRESSED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t16LV_EVENT_PRESSEDE "LV_EVENT_PRESSED"): The object has been pressed
* [LV_EVENT_PRESSING](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t17LV_EVENT_PRESSINGE "LV_EVENT_PRESSING"): The object is being pressed (called continuously while pressing)
* [LV_EVENT_PRESS_LOST](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t19LV_EVENT_PRESS_LOSTE "LV_EVENT_PRESS_LOST"): The object is still being pressed but slid cursor/finger off of the object
* [LV_EVENT_SHORT_CLICKED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t22LV_EVENT_SHORT_CLICKEDE "LV_EVENT_SHORT_CLICKED"): The object was pressed for a short period of time, then released it. Not called if scrolled.
* [LV_EVENT_LONG_PRESSED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t21LV_EVENT_LONG_PRESSEDE "LV_EVENT_LONG_PRESSED"): Object has been pressed for at least long_press_time. Not called if scrolled.
* [LV_EVENT_LONG_PRESSED_REPEAT](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t28LV_EVENT_LONG_PRESSED_REPEATE "LV_EVENT_LONG_PRESSED_REPEAT"): Called after long_press_time in every long_press_repeat_time ms. Not called if scrolled.
* [LV_EVENT_CLICKED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t16LV_EVENT_CLICKEDE "LV_EVENT_CLICKED"): Called on release if not scrolled (regardless to long press)
* [LV_EVENT_RELEASED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t17LV_EVENT_RELEASEDE "LV_EVENT_RELEASED"): Called in every cases when the object has been released
* [LV_EVENT_SCROLL_BEGIN](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t21LV_EVENT_SCROLL_BEGINE "LV_EVENT_SCROLL_BEGIN"): Scrolling begins. The event parameter is a pointer to the animation of the scroll. Can be modified
* [LV_EVENT_SCROLL_THROW_BEGIN](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t27LV_EVENT_SCROLL_THROW_BEGINE "LV_EVENT_SCROLL_THROW_BEGIN"):
* [LV_EVENT_SCROLL_END](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t19LV_EVENT_SCROLL_ENDE "LV_EVENT_SCROLL_END"): Scrolling ends
* [LV_EVENT_SCROLL](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t15LV_EVENT_SCROLLE "LV_EVENT_SCROLL"): Scrolling
* [LV_EVENT_GESTURE](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t16LV_EVENT_GESTUREE "LV_EVENT_GESTURE"): A gesture is detected. Get the gesture with lv_indev_get_gesture_dir(lv_indev_active());
* [LV_EVENT_KEY](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t12LV_EVENT_KEYE "LV_EVENT_KEY"): A key is sent to the object. Get the key with lv_indev_get_key(lv_indev_active());
* [LV_EVENT_FOCUSED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t16LV_EVENT_FOCUSEDE "LV_EVENT_FOCUSED"): The object is focused
* [LV_EVENT_DEFOCUSED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t18LV_EVENT_DEFOCUSEDE "LV_EVENT_DEFOCUSED"): The object is defocused
* [LV_EVENT_LEAVE](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t14LV_EVENT_LEAVEE "LV_EVENT_LEAVE"): The object is defocused but still selected
* [LV_EVENT_HIT_TEST](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t17LV_EVENT_HIT_TESTE "LV_EVENT_HIT_TEST"): Perform advanced hit-testing
* [LV_EVENT_INDEV_RESET](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t20LV_EVENT_INDEV_RESETE "LV_EVENT_INDEV_RESET"): Indev has been reset

### Drawing events[](https://docs.lvgl.io/master/overview/event.html#drawing-events "Link to this heading")

* [LV_EVENT_COVER_CHECK](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t20LV_EVENT_COVER_CHECKE "LV_EVENT_COVER_CHECK"): Check if the object fully covers an area. The event parameter is lv_cover_check_info_t *.
* [LV_EVENT_REFR_EXT_DRAW_SIZE](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t27LV_EVENT_REFR_EXT_DRAW_SIZEE "LV_EVENT_REFR_EXT_DRAW_SIZE"): Get the required extra draw area around the object (e.g. for shadow). The event parameter is int32_t * to store the size.
* [LV_EVENT_DRAW_MAIN_BEGIN](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t24LV_EVENT_DRAW_MAIN_BEGINE "LV_EVENT_DRAW_MAIN_BEGIN"): Starting the main drawing phase
* [LV_EVENT_DRAW_MAIN](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t18LV_EVENT_DRAW_MAINE "LV_EVENT_DRAW_MAIN"): Perform the main drawing
* [LV_EVENT_DRAW_MAIN_END](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t22LV_EVENT_DRAW_MAIN_ENDE "LV_EVENT_DRAW_MAIN_END"): Finishing the main drawing phase
* [LV_EVENT_DRAW_POST_BEGIN](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t24LV_EVENT_DRAW_POST_BEGINE "LV_EVENT_DRAW_POST_BEGIN"): Starting the post draw phase (when all children are drawn)
* [LV_EVENT_DRAW_POST](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t18LV_EVENT_DRAW_POSTE "LV_EVENT_DRAW_POST"): Perform the post draw phase (when all children are drawn)
* [LV_EVENT_DRAW_POST_END](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t22LV_EVENT_DRAW_POST_ENDE "LV_EVENT_DRAW_POST_END"): Finishing the post draw phase (when all children are drawn)
* [LV_EVENT_DRAW_TASK_ADDED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t24LV_EVENT_DRAW_TASK_ADDEDE "LV_EVENT_DRAW_TASK_ADDED"): Adding a draw task

### Special events[](https://docs.lvgl.io/master/overview/event.html#special-events "Link to this heading")

* [LV_EVENT_VALUE_CHANGED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t22LV_EVENT_VALUE_CHANGEDE "LV_EVENT_VALUE_CHANGED"): The object's value has changed (i.e. slider moved)
* [LV_EVENT_INSERT](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t15LV_EVENT_INSERTE "LV_EVENT_INSERT"): A text is inserted to the object. The event data is char * being inserted.
* [LV_EVENT_REFRESH](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t16LV_EVENT_REFRESHE "LV_EVENT_REFRESH"): Notify the object to refresh something on it (for the user)
* [LV_EVENT_READY](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t14LV_EVENT_READYE "LV_EVENT_READY"): A process has finished
* [LV_EVENT_CANCEL](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t15LV_EVENT_CANCELE "LV_EVENT_CANCEL"): A process has been cancelled

### Other events[](https://docs.lvgl.io/master/overview/event.html#other-events "Link to this heading")

* [LV_EVENT_CREATE](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t15LV_EVENT_CREATEE "LV_EVENT_CREATE"): Object is being created
* [LV_EVENT_DELETE](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t15LV_EVENT_DELETEE "LV_EVENT_DELETE"): Object is being deleted
* [LV_EVENT_CHILD_CHANGED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t22LV_EVENT_CHILD_CHANGEDE "LV_EVENT_CHILD_CHANGED"): Child was removed, added, or its size, position were changed
* [LV_EVENT_CHILD_CREATED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t22LV_EVENT_CHILD_CREATEDE "LV_EVENT_CHILD_CREATED"): Child was created, always bubbles up to all parents
* [LV_EVENT_CHILD_DELETED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t22LV_EVENT_CHILD_DELETEDE "LV_EVENT_CHILD_DELETED"): Child was deleted, always bubbles up to all parents
* [LV_EVENT_SCREEN_UNLOAD_START](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t28LV_EVENT_SCREEN_UNLOAD_STARTE "LV_EVENT_SCREEN_UNLOAD_START"): A screen unload started, fired immediately when scr_load is called
* [LV_EVENT_SCREEN_LOAD_START](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t26LV_EVENT_SCREEN_LOAD_STARTE "LV_EVENT_SCREEN_LOAD_START"): A screen load started, fired when the screen change delay is expired
* [LV_EVENT_SCREEN_LOADED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t22LV_EVENT_SCREEN_LOADEDE "LV_EVENT_SCREEN_LOADED"): A screen was loaded
* [LV_EVENT_SCREEN_UNLOADED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t24LV_EVENT_SCREEN_UNLOADEDE "LV_EVENT_SCREEN_UNLOADED"): A screen was unloaded
* [LV_EVENT_SIZE_CHANGED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t21LV_EVENT_SIZE_CHANGEDE "LV_EVENT_SIZE_CHANGED"): Object coordinates/size have changed
* [LV_EVENT_STYLE_CHANGED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t22LV_EVENT_STYLE_CHANGEDE "LV_EVENT_STYLE_CHANGED"): Object's style has changed
* [LV_EVENT_LAYOUT_CHANGED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t23LV_EVENT_LAYOUT_CHANGEDE "LV_EVENT_LAYOUT_CHANGED"): The children position has changed due to a layout recalculation
* [LV_EVENT_GET_SELF_SIZE](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t22LV_EVENT_GET_SELF_SIZEE "LV_EVENT_GET_SELF_SIZE"): Get the internal size of a widget

### Display events[](https://docs.lvgl.io/master/overview/event.html#display-events "Link to this heading")

* [LV_EVENT_INVALIDATE_AREA](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t24LV_EVENT_INVALIDATE_AREAE "LV_EVENT_INVALIDATE_AREA")
* [LV_EVENT_RESOLUTION_CHANGED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t27LV_EVENT_RESOLUTION_CHANGEDE "LV_EVENT_RESOLUTION_CHANGED")
* [LV_EVENT_COLOR_FORMAT_CHANGED](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t29LV_EVENT_COLOR_FORMAT_CHANGEDE "LV_EVENT_COLOR_FORMAT_CHANGED")
* [LV_EVENT_REFR_REQUEST](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t21LV_EVENT_REFR_REQUESTE "LV_EVENT_REFR_REQUEST")
* [LV_EVENT_REFR_START](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t19LV_EVENT_REFR_STARTE "LV_EVENT_REFR_START")
* [LV_EVENT_REFR_READY](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t19LV_EVENT_REFR_READYE "LV_EVENT_REFR_READY")
* [LV_EVENT_RENDER_START](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t21LV_EVENT_RENDER_STARTE "LV_EVENT_RENDER_START")
* [LV_EVENT_RENDER_READY](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t21LV_EVENT_RENDER_READYE "LV_EVENT_RENDER_READY")
* [LV_EVENT_FLUSH_START](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t20LV_EVENT_FLUSH_STARTE "LV_EVENT_FLUSH_START")
* [LV_EVENT_FLUSH_FINISH](https://docs.lvgl.io/master/API/misc/lv_event.html#_CPPv4N15lv_event_code_t21LV_EVENT_FLUSH_FINISHE "LV_EVENT_FLUSH_FINISH")
