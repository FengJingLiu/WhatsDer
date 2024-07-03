#include "cpp_test.h"
#include "esp_log.h"

class CppTest
{
public:
    void print1(){
        ESP_LOGI("cpp_test", "CppTest::print1");
    }
};

extern "C" void cpp_test1(void)
{
    CppTest ct;
    ct.print1();
    ESP_LOGI("cpp_test", "cpp_test1");
}
