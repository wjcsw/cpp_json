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
	if (expected != actual.value.boolean) cout<< __FILE__ << ":" << __LINE__ << "  值错误! expected: " << expected << ", actual:" << actual.type << endl;
	else pass++;
}

inline
void compare_error(parse_state expected, parse_state actual) {
	amount++;
	if (expected != actual) cout<< __FILE__ << ":" << __LINE__ << "  状态错误! expected: " << expected << ", actual: " << actual << endl;
	else pass++;
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
	compare_type(JSON_NULL, j);
	compare_boolean(false, j);
}

void test_error() {
	json j;
	j.type = JSON_NULL;
	compare_error(PARSE_WRONG_WORD, json_parse("tue", j));
	compare_error(PARSE_WRONG_WORD, json_parse("n x", j));
	compare_error(PARSE_EXTRA_WORD, json_parse("FALSEx", j));
	compare_error(PARSE_EXTRA_WORD, json_parse("TRUE x", j));
	compare_error(PARSE_WRONG_WORD, json_parse("TRUE x", j));
}


int main()
{
	test_null();
	test_boolean();
	test_error();
	cout <<"测试结果："<<pass<<"/"<<amount<<"(" << pass / amount * 100 << "%)" << endl;

	return 0;
}
