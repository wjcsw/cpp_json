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

void test_string() {
	json j;
	j.type = JSON_NULL;

	json_parse("\"sda\"", j);
	compare_type(JSON_STRING, j);
	compare_string("sda", j);
	json_free(j);

	j.type = JSON_NULL;
	json_parse("\"sdaas\\n\\rsdas\"", j);
	compare_type(JSON_STRING, j);
	compare_string("sdaas\n\rsdas", j);
	json_free(j);

	j.type = JSON_NULL;
	json_parse("\"\\u0001\"", j);
	compare_type(JSON_STRING, j);
	compare_string("\u0001", j);
	json_free(j);
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
	j.type = JSON_NULL;
	json_parse("0", j);
	compare_type(JSON_NUMBER, j);
	compare_number(0, j);
	json_free(j);

	j.type = JSON_NULL;
	json_parse("1.03", j);
	compare_type(JSON_NUMBER, j);
	compare_number(1.03, j);
	json_free(j);

	j.type = JSON_NULL;
	json_parse("1e10", j);
	compare_type(JSON_NUMBER, j);
	compare_number(1e10, j);
	json_free(j);

	j.type = JSON_NULL;
	json_parse("-10e-1", j);
	compare_type(JSON_NUMBER, j);
	compare_number(-10e-1, j);
	json_free(j);
}

void test_array() {
	json j;
	j.type = JSON_NULL;

	json_parse("[\"sda\",2.3, null]", j);
	compare_type(JSON_ARRAY, j);
	compare_array_size(3,j);
	compare_string("sda", j.value.array[0]);
	compare_number(2.3, j.value.array[1]);
	compare_type(JSON_NULL, j.value.array[2]);
	json_free(j);
}

void test_object() {
	json j;
	j.type = JSON_NULL;

	json_parse("{\"sda\" : 2.3, \"ta\":null}", j);
	compare_type(JSON_OBJECT, j);
	compare_object_size(2, j);

	compare_key("sda", j, 0);
	compare_number(2.3, j.value.object[0].second);

	compare_key("ta", j, 1);
	compare_type(JSON_NULL, j.value.object[1].second);

	json_free(j);
}

void test_error() {
	json j;
	j.type = JSON_NULL;
	compare_error(PARSE_NO_WORD, json_parse("", j));
	compare_error(PARSE_WRONG_WORD, json_parse("tue", j));
	compare_error(PARSE_WRONG_WORD, json_parse("n x", j));
	compare_error(PARSE_EXTRA_WORD, json_parse("01", j));
	compare_error(PARSE_EXTRA_WORD, json_parse("1e01", j));
	compare_error(PARSE_EXTRA_WORD, json_parse("1u01", j));
	compare_error(PARSE_EXTRA_WORD, json_parse("FALSEx", j));
	compare_error(PARSE_EXTRA_WORD, json_parse("TRUE x", j));
	compare_error(PARSE_NO_DOUBLE_QUOTSTION, json_parse("\"TRUE x", j));
	compare_error(PARSE_INVALID_UNICODE, json_parse("\"\\uDBFF\\uEFFF\"", j));
	compare_error(PARSE_NO_SQUARE_BRACKET, json_parse("[TRUE,1", j));
	compare_error(PARSE_NO_CURLY_BRACKET, json_parse("{\"TRUE\":1", j));
	compare_error(PARSE_MISSING_COMMA, json_parse("[TRUE 1", j));
	compare_error(PARSE_MISSING_COLON, json_parse("{\"TRUE\" 1", j));
	
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
