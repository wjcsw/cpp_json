#include "cpp_json.h""

void swap(json & left, json & right) {
	swap(left.type, right.type);
	swap(left.value, right.value);
}

bool operator!=(const json& left, const json& right) {
	return !(left == right);
}

bool operator==(const json& left, const json& right) {
	if (left.type != right.type) return false;
	switch (left.type)
	{
	case JSON_NULL: return true;
	case JSON_BOOLEAN: return left.value.boolean == right.value.boolean;
	case JSON_NUMBER: return left.value.number == right.value.number;
	case JSON_STRING: return left.value.str == right.value.str;
	case JSON_ARRAY: 
		for (size_t i = 0; i < left.value.array.size(); i++) {
			if (left.value.array[i] != right.value.array[i]) return false;
		}
		return true;
	case JSON_OBJECT:
		for (size_t i = 0; i < left.value.object.size(); i++) {
			if ((left.value.object[i].first != right.value.object[i].first)|| (left.value.object[i].second != right.value.object[i].second)) return false;
		}
		return true;
	default:
		break;
	}
}
ostream& operator<<(ostream& out, const json& j) {
	switch (j.type)
	{
	case JSON_NULL: break;
	case JSON_BOOLEAN: out<<j.value.boolean; break;
	case JSON_NUMBER: out<<j.value.number; break;
	case JSON_STRING: out<<"\""<<j.value.str<< "\""; break;
	case JSON_ARRAY:
		out << "[";
		for (size_t i = 0; i < j.value.array.size(); i++) {
			out << j.value.array[i];
			if (i != j.value.array.size() - 1) out << ", ";
		}
		out << "]";
		break;
	case JSON_OBJECT:
		out << "{";
		for (size_t i = 0; i < j.value.object.size(); i++) {
			out << "\"" << j.value.object[i].first << "\"" << " : " << j.value.object[i].second;
			if (i != j.value.array.size() - 1) out << ", ";
		}
		out << "}";
		break;
	default:
		out << "非法JSON类型" << endl;
		break;
	}
	return out;
}

ostream& operator<<(ostream& out, const parse_state& state) {
	switch (state)
	{
	case PARSE_COMPLETE:
		out << "PARSE_COMPLETE";
		break;
	case PARSE_WRONG_WORD:
		out << "PARSE_WRONG_WORD";
		break;
	case PARSE_EXTRA_WORD:
		out << "PARSE_EXTRA_WORD";
		break;
	case PARSE_NO_WORD:
		out << "PARSE_NO_WORD";
		break;
	case PARSE_NO_DOUBLE_QUOTSTION:
		out << "PARSE_NO_DOUBLE_QUOTSTION";
		break;
	case PARSE_INVALID_UNICODE:
		out << "PARSE_INVALID_UNICODE";
		break;
	case PARSE_NO_SQUARE_BRACKET:
		out << "PARSE_NO_SQUARE_BRACKET";
		break;
	case PARSE_NO_CURLY_BRACKET:
		out << "PARSE_NO_CURLY_BRACKET";
		break;
	case PARSE_MISSING_COMMA:
		out << "PARSE_MISSING_COMMA";
		break;
	case PARSE_MISSING_COLON:
		out << "PARSE_MISSING_COLON";
		break;
	case PARSE_INVALID_ESCAPE:
		out << "PARSE_INVALID_ESCAPE";
		break;
	case PARSE_MISSING_KEY:
		out << "PARSE_MISSING_KEY";
		break;
	default:
		out << "invaild error"; break;
		break;
	}
	return out;
}

ostream& operator<<(ostream& out, const json_type& type) {
	switch (type)
	{
	case JSON_NULL:out << "JSON_NULL"; break;
	case JSON_BOOLEAN:out << "JSON_BOOLEAN"; break;
	case JSON_NUMBER:out << "JSON_NUMBER"; break;
	case JSON_STRING:out << "JSON_STRING"; break;
	case JSON_ARRAY:out << "JSON_ARRAY"; break;
	case JSON_OBJECT:out << "JSON_OBJECT"; break;
	default:
		out << "invaild type"; break;
		break;
	}
	return out;
}

void jump_space(parse_text& p) {
	while (p.i < p.s.size() && (p.s[p.i] == ' '|| p.s[p.i] == '\t'|| p.s[p.i] == '\n' || p.s[p.i] == '\r')) p.i++;
}

parse_state value_parse(parse_text& p, json& result);
parse_state array_parse(parse_text& p, json& result) {
	p.i++;
	jump_space(p);
	if (p.s[p.i] != ']') {
		while (p.i < p.s.size()) {
			json j;
			parse_state t = value_parse(p, j);
			if (t != PARSE_COMPLETE) return t;
			result.value.array.push_back(j);
			jump_space(p);
			if (p.s[p.i] == ']') break;
			if (p.s[p.i] == ',') p.i++;
			else if (p.i < p.s.size()) return PARSE_MISSING_COMMA;
			jump_space(p);
		}
	}
	if (p.s[p.i] != ']') return PARSE_NO_SQUARE_BRACKET;
	p.i++;
	result.type = JSON_ARRAY;
	return PARSE_COMPLETE;
}

parse_state parse_hex4(string& p, unsigned &n) {

	for (int i = 0; i < 4; i++) {
		n = n * 16;
		if (p[i] >= '0' && p[i] <= '9') n += p[i] - '0';
		else if (p[i] >= 'A' && p[i] <= 'F') n += p[i] - 'A' + 10;
		else if (p[i] >= 'a' && p[i] <= 'f') n += p[i] - ('a' - 10);
		else return PARSE_WRONG_WORD;
	}
	return PARSE_COMPLETE;
}

parse_state string_parse(parse_text& p, json& result) {
	if (p.s[p.i] != '\"') return PARSE_MISSING_KEY;
	p.i++;
	unsigned u = 0, u2=0;
	string t;
	while (p.i < p.s.size()) {
		if (p.s[p.i] == '\\') {
			p.i++;
			switch (p.s[p.i])
			{
			case '\"':result.value.str.push_back('\"');  break;
			case '\\':result.value.str.push_back('\\'); break;
			case 'b':result.value.str.push_back('\b'); break;
			case 'r':result.value.str.push_back('\r');  break;
			case 't':result.value.str.push_back('\t'); break;
			case 'f':result.value.str.push_back('\f');  break;
			case 'n':result.value.str.push_back('\n'); break;
			case '/':result.value.str.push_back('/'); break;
			case 'u':
				p.i++;
				t = p.s.substr(p.i, 4);
				if(parse_hex4(t,u)!= PARSE_COMPLETE) return PARSE_WRONG_WORD;
				p.i += 3;
				if (u >= 0xD800 && u <= 0xDBFF) {
					if(p.i + 1>=p.s.size()|| p.i + 2 >= p.s.size() || !(p.s[p.i + 1] == '\\' && p.s[p.i + 2] == 'u')) return PARSE_INVALID_UNICODE;
					p.i += 3;
					t = p.s.substr(p.i, 4);
					if (parse_hex4(t, u2) != PARSE_COMPLETE) return PARSE_WRONG_WORD;
					p.i += 3;
					if (u2 < 0xDC00 || u2 > 0xDFFF) return PARSE_INVALID_UNICODE;
					u = 0x10000 + (u - 0xD800) * 0x400 + (u2 - 0xDC00);
				}
				if (u <= 0x7F) {
					result.value.str.push_back(u & 0xFF);
				}
				else if (u <= 0x7FF) {
					result.value.str.push_back(0xC0 | ((u >> 6) & 0xFF));
					result.value.str.push_back(0x80 | (u & 0x3F));
				}
				else if (u <= 0xFFFF) {
					result.value.str.push_back(0xE0 | ((u >> 12) & 0xFF));
					result.value.str.push_back(0x80 | ((u >> 6) & 0x3F));
					result.value.str.push_back(0x80 | (u & 0x3F));
				}
				else {
					if (u > 0x10FFFF) return PARSE_INVALID_UNICODE;
					result.value.str.push_back(0xF0 | ((u >> 18) & 0xFF));
					result.value.str.push_back(0x80 | ((u >> 12) & 0x3F));
					result.value.str.push_back(0x80 | ((u >> 6) & 0x3F));
					result.value.str.push_back(0x80 | (u & 0x3F));
				}
				break;
			default:
				return PARSE_INVALID_ESCAPE;
			}
		}
		else if (p.s[p.i] < 0x20) {
			return PARSE_INVALID_UNICODE;
		}
		else if (p.s[p.i] == '\"') { ++p.i; result.type = JSON_STRING; return PARSE_COMPLETE; }
		else result.value.str.push_back(p.s[p.i]);
		++p.i;
	}

	return PARSE_NO_DOUBLE_QUOTSTION;
}

parse_state number_parse(parse_text& p, json& result) {
	size_t i = p.i;
	if (p.s[i] == '-') i++;
	if (p.s[i] < '0' || p.s[i] > '9') return PARSE_WRONG_WORD;
	if (i < p.s.size() && p.s[i] == '0') i++;
	else {
		i++;
		while (i<p.s.size() && p.s[i] >= '0' && p.s[i] <= '9') i++;
	}
	if (i < p.s.size() && p.s[i] == '.') {
		i++;
		while (i < p.s.size() && p.s[i] >= '0' && p.s[i] <= '9') i++;
	}
	if (i < p.s.size() && (p.s[i] == 'e'|| p.s[i] == 'E')) {
		i++;
		if (i < p.s.size() && p.s[i] == '-') i++;
		if (i < p.s.size() && p.s[i] == '0') i++;
		else {
			i++;
			while (i < p.s.size() && p.s[i] >= '0' && p.s[i] <= '9') i++;
		}
	}
	result.type = JSON_NUMBER;
	string t = p.s.substr(p.i, p.s.size() - p.i);
	istringstream in(t);
	in >> result.value.number;
	p.i = i;
	return PARSE_COMPLETE;
}

parse_state object_parse(parse_text& p, json& result) {
	p.i++;
	jump_space(p);
	if (p.s[p.i] != '}') {
		while (p.i < p.s.size()) {
			json j;
			parse_state t = string_parse(p, j);
			if (t != PARSE_COMPLETE) return t;
			string str = j.value.str;
			json_free(j);
			jump_space(p);
			if (p.s[p.i] == ':') p.i++;
			else return PARSE_MISSING_COLON;
			jump_space(p);
			t = value_parse(p, j);
			if (t != PARSE_COMPLETE) return t;
			result.value.object.push_back({ str,j });
			jump_space(p);
			if (p.s[p.i] == '}') break;
			if (p.s[p.i] == ',') p.i++;
			else if (p.i < p.s.size())return PARSE_MISSING_COMMA;
			jump_space(p);

		}
	}
	if (p.s[p.i] != '}') return PARSE_NO_CURLY_BRACKET;
	p.i++;
	result.type = JSON_OBJECT;
	return PARSE_COMPLETE;
}

parse_state value_parse(parse_text& p, json & result) {
	jump_space(p);
	if (p.i == p.s.size()) return PARSE_NO_WORD;
	string t;
	switch (p.s[p.i])
	{
	case 'N':
	case 'n':  t = p.s.substr(p.i, 4); transform(t.begin(), t.end(), t.begin(), ::tolower); if (t == "null") { result.type = JSON_NULL; p.i += 4; return PARSE_COMPLETE; }
			else { return PARSE_WRONG_WORD; }
	case 'T':
	case 't':  t = p.s.substr(p.i, 4); transform(t.begin(), t.end(), t.begin(), ::tolower); if (t == "true") { result.type = JSON_BOOLEAN; result.value.boolean = true; p.i += 4; return PARSE_COMPLETE; }
			else { return PARSE_WRONG_WORD; }
	case 'F':
	case 'f':  t = p.s.substr(p.i, 5); transform(t.begin(), t.end(), t.begin(), ::tolower); if (t == "false") { result.type = JSON_BOOLEAN; result.value.boolean = false; p.i += 5; return PARSE_COMPLETE; }
			else { return PARSE_WRONG_WORD; }
	case '\"': return string_parse(p, result);
	case '[': return array_parse(p, result);
	case '{': return object_parse(p, result);
	default:
		return number_parse(p, result);
	}
}

parse_state json_parse(string json_text, json& result) {
	parse_text p{ json_text,0 };
	result.type = JSON_NULL;
	parse_state state = value_parse(p, result);
	if (state == PARSE_COMPLETE) {
		jump_space(p);
		if (p.i < p.s.size()) {
			state = PARSE_EXTRA_WORD;
			result.type = JSON_NULL;
		}
	}
	return state;
}

void json_free(json& result) {
	result.type = JSON_NULL;
	json_value v;
	result.value = v;
}