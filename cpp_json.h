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
enum parse_state {PARSE_COMPLETE, PARSE_WRONG_WORD, PARSE_EXTRA_WORD, PARSE_NO_WORD, PARSE_NO_DOUBLE_QUOTSTION, 
	PARSE_INVALID_UNICODE, PARSE_NO_SQUARE_BRACKET, PARSE_NO_CURLY_BRACKET, PARSE_MISSING_COMMA, PARSE_MISSING_COLON, PARSE_INVALID_ESCAPE, PARSE_MISSING_KEY
};

struct json;

class json_value {
public:
	double number;
	bool boolean;
	string  str;
	vector<pair<string, json>> object;
	vector<json> array;
};

struct json {
	json_value value;
	json_type type;
	json(){}
	json(json& j): value(j.value),type(j.type) { }
	json(const json& j) : value(j.value), type(j.type) { }
	json operator=(const json& j) {
		return json(j);
	}
};

struct parse_text
{
	string s;
	size_t i;
};

parse_state json_parse(string, json&);
void json_free(json&);
ostream& operator<<(ostream&, const json&);
ostream& operator<<(ostream&, const parse_state&);
ostream& operator<<(ostream&, const json_type&);
bool operator==(const json&, const json&);
bool operator!=(const json&, const json&);
void swap(json&, json&);

// TODO: 在此处引用程序需要的其他标头。
