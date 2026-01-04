#include <gtest/gtest.h>
#include "json/api.hpp"
#include "json/builder.hpp"
#include <string_view> // Penting!

using namespace json;
using namespace std::string_view_literals; // Ini solusinya!

// Fixture buat setup Arena di setiap test
class JsonTest : public ::testing::Test {
protected:
    Arena arena;
};

TEST_F(JsonTest, ParsePrimitives) {
    // Tambahin 'sv' di belakang string biar jadi string_view eksplisit
    auto res_null = parse("null"sv, arena);
    ASSERT_TRUE(res_null);
    EXPECT_TRUE((*res_null)->is_null());

    auto res_bool = parse("true"sv, arena);
    ASSERT_TRUE(res_bool);
    EXPECT_TRUE((*res_bool)->as_bool().value());
    
    auto res_num = parse("123.45"sv, arena);
    ASSERT_TRUE(res_num);
    EXPECT_DOUBLE_EQ((*res_num)->as_number().value(), 123.45);
}

TEST_F(JsonTest, ParseStringUnescape) {
    // Raw string literal + sv suffix
    auto res = parse(R"("Hello\nWorld")"sv, arena);
    ASSERT_TRUE(res);
    EXPECT_EQ((*res)->as_string().value(), "Hello\nWorld");
}

TEST_F(JsonTest, ParseArray) {
    auto res = parse("[1, 2, 3]"sv, arena);
    ASSERT_TRUE(res);
    auto arr = (*res)->as_array();
    ASSERT_TRUE(arr);
    EXPECT_EQ(arr->size, 3);
    // Note: array access might need dereference checks depending on your overload implementation
    // Assuming operator[] returns expected<Node*, ...>
    auto elem0 = (*arr.value()[0]).as_number();
    EXPECT_DOUBLE_EQ(elem0.value(), 1.0);
}

TEST_F(JsonTest, ParseObject) {
    auto res = parse(R"({"key": "value"})"sv, arena);
    ASSERT_TRUE(res);
    
    auto root = *res;
    auto val = (*root)["key"]; // Operator[] on Node* might need dereference first if it returns wrapper
    ASSERT_TRUE(val);
    EXPECT_EQ((*val)->as_string().value(), "value");
}

TEST_F(JsonTest, BuilderAPI) {
    auto builder = build_object(arena);
    builder.add("id", 1).add("active", true);
    Node* root = builder.build();

    EXPECT_TRUE(root->is_object());
    
    auto id_res = (*root)["id"];
    EXPECT_DOUBLE_EQ((*id_res)->as_number().value(), 1.0);
}

TEST_F(JsonTest, ErrorHandling) {
    auto res = parse("{ invalid json }"sv, arena);
    EXPECT_FALSE(res);
    // Sesuaikan ErrorCode dengan yang ada di header kamu
    // EXPECT_EQ(res.error().code, ErrorCode::InvalidString); 
}