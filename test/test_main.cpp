#include <unity.h>
#include "BitcoinParser.h"

void test_valid_payload_parses_price()
{
    float price = 0;
    bool ok = parseBitcoinPrice("{\"bitcoin\":{\"eur\":54321.12}}", price);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 54321.12, price);
}

void test_missing_field_fails()
{
    float price = 0;
    bool ok = parseBitcoinPrice("{\"bitcoin\":{\"usd\":54321.12}}", price);

    TEST_ASSERT_FALSE(ok);
}

void test_malformed_json_fails()
{
    float price = 0;
    bool ok = parseBitcoinPrice("{not valid json", price);

    TEST_ASSERT_FALSE(ok);
}

void test_empty_payload_fails()
{
    float price = 0;
    bool ok = parseBitcoinPrice("", price);

    TEST_ASSERT_FALSE(ok);
}

void setup()
{
    delay(2000); // daj vremena serial monitoru na real hardwareu
    UNITY_BEGIN();
    RUN_TEST(test_valid_payload_parses_price);
    RUN_TEST(test_missing_field_fails);
    RUN_TEST(test_malformed_json_fails);
    RUN_TEST(test_empty_payload_fails);
    UNITY_END();
}

void loop()
{
}