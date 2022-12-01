#include "cpp_json.h""

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
	default:
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
		break;
	}
	return out;
}

void jump_space(parse_text& p) {
	while (p.i< p.s.size()&& p.s[p.i] == ' ') p.i++;
}


parse_state value_parse(parse_text& p, json & result) {
	jump_space(p);
	string t;
	switch (p.s[p.i])
	{
	case 'n':  t = p.s.substr(p.i, 4); if (t == "null") { result.type = JSON_NULL; p.i += 4; return PARSE_COMPLETE; }
			else { return PARSE_WRONG_WORD; }
	case 't':  t = p.s.substr(p.i, 4); if (t == "true") { result.type = JSON_BOOLEAN; result.value.boolean = true; p.i += 4; return PARSE_COMPLETE; }
			else { return PARSE_WRONG_WORD; }
	case 'f':  t = p.s.substr(p.i, 5); if (t == "false") { result.type = JSON_BOOLEAN; result.value.boolean = false; p.i += 5; return PARSE_COMPLETE; }
			else { return PARSE_WRONG_WORD; }
	default:
		break;
	}
}

parse_state json_parse(string json_text, json& result) {
	transform(json_text.begin(), json_text.end(), json_text.begin(), ::tolower);
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