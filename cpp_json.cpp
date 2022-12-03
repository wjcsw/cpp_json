// cpp_json.cpp: 定义应用程序的入口点。
//

#include "cpp_json.h"
int amount = 0;
double pass = 0;

inline
void compare_type(json_type expected, json& actual) {
	amount++;
	if (expected != actual.type) cout<< __FILE__ << ":" << __LINE__ << "  类型错误! expected: " << expected << ", actual: " << actual.type << endl;
	else pass++;
}

inline
void compare_boolean(bool expected, json& actual) {
	amount++;
	if (expected != actual.value.boolean) cout<< __FILE__ << ":" << __LINE__ << "  值错误! expected: " << expected << ", actual:" << actual.value.boolean << endl;
	else pass++;
}

inline
void compare_error(parse_state expected, parse_state actual) {
	amount++;
	if (expected != actual) cout<< __FILE__ << ":" << __LINE__ << "  状态错误! expected: " << expected << ", actual: " << actual << endl;
	else pass++;
}

inline
void compare_number(double expected, json& actual) {
	amount++;
	if (expected != actual.value.number) cout << __FILE__ << ":" << __LINE__ << "  值错误! expected: " << expected << ", actual: " << actual.value.number << endl;
	else pass++;
}

inline
void compare_string(string expected, json& actual) {
	amount++;
	if (expected != actual.value.str) cout << __FILE__ << ":" << __LINE__ << "  值错误! expected: " << expected << ", actual: " << actual.value.str << endl;
	else pass++;
}

inline
void compare_array_size(int expected, json& actual) {
	amount++;
	if (expected != actual.value.array.size()) cout << __FILE__ << ":" << __LINE__ << "   大小错误! expected: " << expected << ", actual: " << actual.value.array.size() << endl;
	else pass++;
}

inline
void compare_object_size(int expected, json& actual) {
	amount++;
	if (expected != actual.value.object.size()) cout << __FILE__ << ":" << __LINE__ << "  大小错误! expected: " << expected << ", actual: " << actual.value.object.size() << endl;
	else pass++;
}

void test_string() {
	json j;
	j.type = JSON_NULL;

	json_parse("\"sda\"", j);
	compare_type(JSON_STRING, j);
	compare_string("sda", j);
	json_free(j);

	json_parse("\"sdaas\\n\\rsdas\"", j);
	compare_type(JSON_STRING, j);
	compare_string("sdaas\n\rsdas", j);
	json_free(j);

	json_parse("\"\u0001\"", j);
	compare_type(JSON_STRING, j);
	compare_string("\u0001", j);
	json_free(j);
}

void test_null() {
	json j;
	j.type = JSON_BOOLEAN;
	json_parse("null", j);
	compare_type(JSON_NULL, j);
}

void test_boolean() {
	json j;
	j.type = JSON_NULL;
	json_parse("true", j);
	compare_type(JSON_BOOLEAN, j);
	compare_boolean(true, j);
	json_parse("FALSE", j);
	compare_type(JSON_BOOLEAN, j);
	compare_boolean(false, j);
}

void test_number() {
	json j;
	j.type = JSON_NULL;
	json_parse("0", j);
	compare_type(JSON_NUMBER, j);
	compare_number(0, j);
	json_parse("1.03", j);
	compare_type(JSON_NUMBER, j);
	compare_number(1.03, j);
	json_parse("1e10", j);
	compare_type(JSON_NUMBER, j);
	compare_number(1e10, j);
	json_parse("-10e-1", j);
	compare_type(JSON_NUMBER, j);
	compare_number(-10e-1, j);
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
	string expected = "sda";
	if(expected!= j.value.object[0].first) cout << __FILE__ << ":" << __LINE__ << "  key错误! expected: " << expected << ", actual: " << j.value.object[0].first << endl;
	compare_number(2.3, j.value.object[0].second);
	expected = "ta";
	if (expected != j.value.object[1].first) cout << __FILE__ << ":" << __LINE__ << "  key错误! expected: " << expected << ", actual: " << j.value.object[1].first << endl;
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
}


int main()
{
	test_null();
	test_boolean();
	test_error();
	test_number();
	test_string();
	test_array();
	test_object();
	cout <<"测试结果："<<pass<<"/"<<amount<<"(" << pass / amount * 100 << "%)" << endl;

	return 0;
}
