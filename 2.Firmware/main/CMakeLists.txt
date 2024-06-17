set(GUI_DIR ./gui)

# file(GLOB_RECURSE GUI_DIR_SOURCES ${GUI_DIR}/*.c)
file(GLOB_RECURSE MAIN_DIR_SOURCES ./*.c)

idf_component_register(SRCS ${MAIN_DIR_SOURCES}
                       INCLUDE_DIRS "./common" "./app" "./gui" "./hal" "./port"
                       PRIV_REQUIRES nvs_flash esp_wifi esp_http_client json
)
