#include "request.hpp"
#include <cstdio>

namespace request {
    void base_test()
    {
        char* str = nullptr;
        void* msg_ptr = heap_caps_malloc(sizeof(Message), MALLOC_CAP_SPIRAM);
        Message* msg = new (msg_ptr)Message(USER_ROLE);
        const char* c = "asd";
        while (true) {
            msg->update_message_content(c, 3);
            str = msg->get_message_content();
            printf("%s", str);
        }
        const char* e = "E";
        msg->done_message_content(e, 1);
        printf("%s", msg->get_message_content());
    }
}
