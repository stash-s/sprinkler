#include <Arduino.h>
#include <unity.h>

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void convert_string_to_int(void)
{
    String foo("137");
    long foo_int = foo.toInt();
    TEST_ASSERT_EQUAL(137, foo_int);
}

void setup()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(convert_string_to_int);
}

void loop()
{

    UNITY_END();
}