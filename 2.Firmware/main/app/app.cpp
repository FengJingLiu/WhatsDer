#include "app.hpp"
#include "recoder.hpp"
#include "display.hpp"
#include "request.hpp"

void app::app_init()
{
    recoder::Recoder::getInstance();
    request::Websocket::getInstance();
    display::display_init();
}