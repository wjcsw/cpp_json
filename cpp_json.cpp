// cpp_json.cpp: 定义应用程序的入口点。
//

#include "cpp_json.h"

int amount = 0;
double pass = 0;


#define  compare(expected, actual,error) do{\
	amount++;\
	if(expected==actual) pass++;\
	else {\
	 cout<< __FILE__ << ":" << __LINE__ << "  "<<error<<" expected: " << expected << ", actual: " << actual << endl;\
	}\
}while(0)

#define compare_type(expected, actual)  compare(expected, actual.type, "类型错误!")
#define compare_boolean(expected, actual)  compare(expected, actual.value.boolean, "值错误!")
#define compare_error(expected, actual) compare(expected, actual, "状态错误!")
#define compare_number(expected, actual) compare(expected, actual.value.number, "值错误!")
#define compare_string(expected, actual) compare(expected, actual.value.str, "值错误!")
#define compare_array_size(expected, actual) compare(expected, actual.value.array.size(), "大小错误!")
#define compare_object_size(expected, actual) compare(expected, actual.value.object.size(), "大小错误!")
#define compare_key(expected, actual, i) compare(expected, actual.value.object[i].first, "key错误!")

#define TEST_NUMBER(expected, actual, j) do{j.type = JSON_NULL;\
		json_parse(actual, j);\
		compare_type(JSON_NUMBER, j);\
		compare_number(expected, j);\
		json_free(j);\
	}while(0)

#define TEST_STRING(expected, actual, j) do{j.type = JSON_NULL;\
		json_parse(actual, j);\
		compare_type(JSON_STRING, j);\
		compare_string(expected, j);\
		json_free(j);\
	}while(0)


void test_string() {
	json j;

	TEST_STRING("", "\"\"",j);
	TEST_STRING("Hello", "\"Hello\"", j);
	TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"", j);
	TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"", j);
	TEST_STRING("\x24", "\"\\u0024\"", j);         /* Dollar sign U+0024 */
	TEST_STRING("\xC2\xA2", "\"\\u00A2\"", j);     /* Cents sign U+00A2 */
	TEST_STRING("\xE2\x82\xAC", "\"\\u20AC\"", j); /* Euro sign U+20AC */
	TEST_STRING("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\"", j);  /* G clef sign U+1D11E */
	TEST_STRING("\xF0\x9D\x84\x9E", "\"\\ud834\\udd1e\"", j);  /* G clef sign U+1D11E */

}

void test_null() {
	json j;
	j.type = JSON_BOOLEAN;
	json_parse("null", j);
	compare_type(JSON_NULL, j);
	json_free(j);
}

void test_boolean() {
	json j;
	j.type = JSON_NULL;
	json_parse("true", j);
	compare_type(JSON_BOOLEAN, j);
	compare_boolean(true, j);
	json_free(j);

	j.type = JSON_NULL;
	json_parse("FALSE", j);
	compare_type(JSON_BOOLEAN, j);
	compare_boolean(false, j);
	json_free(j);
}

void test_number() {
	json j;

	TEST_NUMBER(0.0, "0",j);
	TEST_NUMBER(0.0, "-0",j);
	TEST_NUMBER(0.0, "-0.0",j);
	TEST_NUMBER(1.0, "1",j);
	TEST_NUMBER(-1.0, "-1", j);
	TEST_NUMBER(1.5, "1.5", j);
	TEST_NUMBER(-1.5, "-1.5", j);
	TEST_NUMBER(3.1416, "3.1416", j);
	TEST_NUMBER(1E10, "1E10", j);
	TEST_NUMBER(1e10, "1e10", j);
	TEST_NUMBER(1E+10, "1E+10", j);
	TEST_NUMBER(1E-10, "1E-10", j);
	TEST_NUMBER(-1E10, "-1E10", j);
	TEST_NUMBER(-1e10, "-1e10", j);
	TEST_NUMBER(-1E+10, "-1E+10", j);
	TEST_NUMBER(-1E-10, "-1E-10", j);
	TEST_NUMBER(1.234E+10, "1.234E+10", j);
	TEST_NUMBER(1.234E-10, "1.234E-10", j);
	TEST_NUMBER(0.0, "1e-10000", j); /* must underflow */

	TEST_NUMBER(1.0000000000000002, "1.0000000000000002", j); /* the smallest number > 1 */
	TEST_NUMBER(4.9406564584124654e-324, "4.9406564584124654e-324", j); /* minimum denormal */
	TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324", j);
	TEST_NUMBER(2.2250738585072009e-308, "2.2250738585072009e-308", j);  /* Max subnormal double */
	TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308", j);
	TEST_NUMBER(2.2250738585072014e-308, "2.2250738585072014e-308", j);  /* Min normal positive double */
	TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308", j);
	TEST_NUMBER(1.7976931348623157e+308, "1.7976931348623157e+308", j);  /* Max double */
	TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308", j);
}

void test_array() {
	json j;

	j.type = JSON_NULL;
	json_parse("[ ]", j);
	compare_type(JSON_ARRAY, j);
	compare_array_size(0, j);
	json_free(j);

	j.type = JSON_NULL;
	json_parse("[\"sda\",2.3, null, false, true]", j);
	compare_type(JSON_ARRAY, j);
	compare_array_size(5,j);
	compare_string("sda", j.value.array[0]);
	compare_number(2.3, j.value.array[1]);
	compare_type(JSON_NULL, j.value.array[2]);
	compare_type(JSON_BOOLEAN, j.value.array[3]);
	compare_boolean(false, j.value.array[3]);
	compare_type(JSON_BOOLEAN, j.value.array[4]);
	compare_boolean(true, j.value.array[4]);
	json_free(j);

	j.type = JSON_NULL;
	json_parse("[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ] ]", j);
	compare_type(JSON_ARRAY, j);
	compare_array_size(4, j);
	for (size_t i = 0; i < 4; i++) {
		json t = j.value.array[i];
		compare_type(JSON_ARRAY, t);
		compare_array_size(i, t);
		for (size_t k = 0; k < i; k++) {
			json e = t.value.array[k];
			compare_type(JSON_NUMBER, e);
			compare_number(k, e);
		}
	}
	json_free(j);
}

void test_object() {
	json j;

	j.type = JSON_NULL;
	json_parse("{}", j);
	compare_type(JSON_OBJECT, j);
	compare_object_size(0, j);
	json_free(j);

	j.type = JSON_NULL;
	json_parse(" { "
        "\"n\" : null , "
        "\"f\" : false , "
        "\"t\" : true , "
        "\"i\" : 123 , "
        "\"s\" : \"abc\", "
        "\"a\" : [ 1, 2, 3 ],"
        "\"o\" : { \"1\" : 1, \"2\" : 2, \"3\" : 3 }"
		" } ", j);
	compare_type(JSON_OBJECT, j);
	compare_object_size(7, j);
	compare_key("n", j, 0);
	compare_type(JSON_NULL, j.value.object[0].second);
	compare_key("f", j, 1);
	compare_type(JSON_BOOLEAN, j.value.object[1].second);
	compare_boolean(false, j.value.object[1].second);
	compare_key("t", j, 2);
	compare_type(JSON_BOOLEAN, j.value.object[2].second);
	compare_boolean(true, j.value.object[2].second);
	compare_key("i", j, 3);
	compare_type(JSON_NUMBER, j.value.object[3].second);
	compare_number(123, j.value.object[3].second);
	compare_key("s", j, 4);
	compare_type(JSON_STRING, j.value.object[4].second);
	compare_string("abc", j.value.object[4].second);
	compare_key("a", j, 5);
	compare_type(JSON_ARRAY, j.value.object[5].second);
	compare_array_size(3, j.value.object[5].second);
	for (size_t k = 0; k < 3; k++) {
		json e = j.value.object[5].second.value.array[k];
		compare_type(JSON_NUMBER, e);
		compare_number(k+1, e);
	}
	compare_key("o", j, 6);
	compare_type(JSON_OBJECT, j.value.object[6].second);
	json e = j.value.object[6].second;
	compare_object_size(3, e);
	for (size_t k = 0; k < 3; k++) {
		string t;
		t.push_back(char('1' + k));
		compare_key(t, e, k);
		compare_number(k+1, e.value.object[k].second);
	}

	json_free(j);
}

void test_error() {
	json j;
	j.type = JSON_NULL;
	compare_error(PARSE_NO_WORD, json_parse("", j));

	compare_error(PARSE_WRONG_WORD, json_parse("tue", j));
	compare_error(PARSE_WRONG_WORD, json_parse("n x", j));
	/* invalid value in array */
	compare_error(PARSE_WRONG_WORD, json_parse("[1,]", j));
	compare_error(PARSE_WRONG_WORD, json_parse("[\"a\", nul]", j));

	compare_error(PARSE_EXTRA_WORD, json_parse("01", j));
	compare_error(PARSE_EXTRA_WORD, json_parse("1e01", j));
	compare_error(PARSE_EXTRA_WORD, json_parse("1u01", j));
	compare_error(PARSE_EXTRA_WORD, json_parse("FALSEx", j));
	compare_error(PARSE_EXTRA_WORD, json_parse("TRUE x", j));

	compare_error(PARSE_NO_DOUBLE_QUOTSTION, json_parse("\"TRUE x", j));
	compare_error(PARSE_NO_DOUBLE_QUOTSTION, json_parse("\"", j));

	compare_error(PARSE_INVALID_ESCAPE, json_parse("\"\\v\"", j));
	compare_error(PARSE_INVALID_ESCAPE, json_parse("\"\\'\"", j));
	compare_error(PARSE_INVALID_ESCAPE, json_parse("\"\\0\"", j));
	compare_error(PARSE_INVALID_ESCAPE, json_parse("\"\\x12\"", j));

	compare_error(PARSE_INVALID_UNICODE, json_parse("\"\\uDBFF\\uEFFF\"", j));
	compare_error(PARSE_INVALID_UNICODE, json_parse("\"\x01\"", j));
	compare_error(PARSE_INVALID_UNICODE, json_parse("\"\x1F\"", j));

	compare_error(PARSE_NO_SQUARE_BRACKET, json_parse("[TRUE,1", j));
	compare_error(PARSE_NO_SQUARE_BRACKET, json_parse("[[]", j));

	compare_error(PARSE_NO_CURLY_BRACKET, json_parse("{\"TRUE\":1", j));
	compare_error(PARSE_NO_CURLY_BRACKET, json_parse("{\"a\":{}", j));

	compare_error(PARSE_MISSING_KEY, json_parse("{:1}", j));
	compare_error(PARSE_MISSING_KEY, json_parse("{1:1}", j));
	compare_error(PARSE_MISSING_KEY, json_parse("{{\"a\":1}", j));

	compare_error(PARSE_MISSING_COMMA, json_parse("[TRUE 1]", j));
	compare_error(PARSE_MISSING_COMMA, json_parse("[TRUE, 1  2]", j));

	compare_error(PARSE_MISSING_COLON, json_parse("{\"TRUE\" 1", j));
	compare_error(PARSE_MISSING_COLON, json_parse("{\"TRUE\", 1", j));
	
}


int main()
{
	test_null();
	test_boolean();
	test_number();
	test_string();
	test_array();
	test_object();
	test_error();
	cout <<"测试结果："<<pass<<"/"<<amount<<"(" << pass / amount * 100 << "%)" << endl;

	return 0;
}
