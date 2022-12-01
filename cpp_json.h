// cpp_json.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream> 
#include<vector>
#include<unordered_map>
#include <sstream>
#include<string>
#include <algorithm>

using namespace std;

enum json_type {JSON_NULL, JSON_BOOLEAN, JSON_STRING, JSON_NUMBER, JSON_ARRAY, JSON_OBJECT};
enum parse_state {PARSE_COMPLETE, PARSE_WRONG_WORD, PARSE_EXTRA_WORD};

ostream& operator<<(ostream&, const parse_state &);
ostream& operator<<(ostream&, const json_type &);

union json_value {
	double number;
	bool boolean;
	string  str;
	vector<json_value> array;
	vector<unordered_map<string, json_value>> object;
	json_value(){}
	~json_value() {}
};

struct json {
	json_value value;
	json_type type;
	json(){}
};

struct parse_text
{
	string s;
	size_t i;
};

parse_state json_parse(string, json&);
parse_state value_parse(parse_text&, json&);
// TODO: 在此处引用程序需要的其他标头。
