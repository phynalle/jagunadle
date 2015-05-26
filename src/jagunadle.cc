#include "jagunadle.h"

#include <sstream>

std::string jagunadle::dumps(const JsonObject& json) {
    if (json.empty()) return "{}";
    
    bool need_comma = false;
    std::ostringstream oss;
    oss << "{";
    for (auto& kv : json) {
        auto& k = kv.first;
        auto& v = kv.second;
        
        if (v.is_null()) continue;   
        
        if (need_comma) {
            oss << ", ";
        } else {
            need_comma = true;
        }
        oss << '"' << k << "\": " << v.dump();
    }
    oss << "}";
    return oss.str();
}

const std::string jagunadle::JsonValue::nullstring;
const jagunadle::JsonArray jagunadle::JsonValue::nullarray;
const jagunadle::JsonObjectPtr jagunadle::JsonValue::nullobject;

jagunadle::JsonValue::JsonValue() { set_type(Type::Null); }
jagunadle::JsonValue::JsonValue(bool value) { *this = value; }
jagunadle::JsonValue::JsonValue(double value) { *this = value; }
jagunadle::JsonValue::JsonValue(Int value) { *this = value; }
jagunadle::JsonValue::JsonValue(Int8 value) { *this = value; }
jagunadle::JsonValue::JsonValue(Int16 value) { *this = value; }
jagunadle::JsonValue::JsonValue(Int32 value) { *this = value; }
jagunadle::JsonValue::JsonValue(Int64 value) { *this = value; }
jagunadle::JsonValue::JsonValue(Uint value) { *this = value; }
jagunadle::JsonValue::JsonValue(Uint8 value) { *this = value; }
jagunadle::JsonValue::JsonValue(Uint16 value) { *this = value; }
jagunadle::JsonValue::JsonValue(Uint32 value) { *this = value; }
jagunadle::JsonValue::JsonValue(Uint64 value) { *this = value; }
jagunadle::JsonValue::JsonValue(const char* value) { *this = value; }
jagunadle::JsonValue::JsonValue(const std::string& value) { *this = value; }
jagunadle::JsonValue::JsonValue(const JsonArray& value) { *this = value; }
jagunadle::JsonValue::JsonValue(const JsonObject& value) { *this = value; }
    
jagunadle::JsonValue::JsonValue(const JsonValue& from) {
    copy(from);  
}

jagunadle::JsonValue::JsonValue(JsonValue&& from) 
    : type_(std::move(from.type_)), value_(std::move(from.value_)) {
}

bool jagunadle::JsonValue::operator==(const JsonValue& other) {
    return (type_ == other.type_);        
}
bool jagunadle::JsonValue::operator!=(const JsonValue& other) { return !(*this == other); }

jagunadle::JsonValue& jagunadle::JsonValue::operator=(bool value) {
    set_value();
    set_type(value ? Type::True : Type::False);
    return *this;
}

jagunadle::JsonValue& jagunadle::JsonValue::operator=(double value) {
    set_value(std::to_string(value));
    set_type(Type::Number);
    return *this;
}

jagunadle::JsonValue& jagunadle::JsonValue::operator=(Int value) { return (*this = static_cast<Int64>(value)); }
jagunadle::JsonValue& jagunadle::JsonValue::operator=(Int8 value) { return (*this = static_cast<Int64>(value)); }
jagunadle::JsonValue& jagunadle::JsonValue::operator=(Int16 value) { return (*this = static_cast<Int64>(value)); }
jagunadle::JsonValue& jagunadle::JsonValue::operator=(Int32 value) { return (*this = static_cast<Int64>(value)); }
jagunadle::JsonValue& jagunadle::JsonValue::operator=(Int64 value) {
    set_value(std::to_string(value));
    set_type(Type::Number);
    return *this;
}

jagunadle::JsonValue& jagunadle::JsonValue::operator=(Uint value) { return (*this = static_cast<Uint64>(value)); }
jagunadle::JsonValue& jagunadle::JsonValue::operator=(Uint8 value) { return (*this = static_cast<Uint64>(value)); }
jagunadle::JsonValue& jagunadle::JsonValue::operator=(Uint16 value) { return (*this = static_cast<Uint64>(value)); }
jagunadle::JsonValue& jagunadle::JsonValue::operator=(Uint32 value) { return (*this = static_cast<Uint64>(value)); }
jagunadle::JsonValue& jagunadle::JsonValue::operator=(Uint64 value) {
    set_value(std::to_string(value));
    set_type(Type::Number);
    return *this;
}

jagunadle::JsonValue& jagunadle::JsonValue::operator=(const char* value) {
    return (*this = std::string(value));
}

jagunadle::JsonValue& jagunadle::JsonValue::operator=(const std::string& value) {
    set_value(value);
    set_type(Type::String);
    return *this;
}

jagunadle::JsonValue& jagunadle::JsonValue::operator=(const JsonArray& value) {
    set_value(value);
    set_type(Type::Array);
    return *this;
}

jagunadle::JsonValue& jagunadle::JsonValue::operator=(const JsonObject& value) {
    set_value(value);
    type_ = Type::Object;
    return *this;
}

jagunadle::JsonValue& jagunadle::JsonValue::operator=(const JsonValue& other) {
    copy(other);
    return *this;
}

jagunadle::JsonValue& jagunadle::JsonValue::operator=(JsonValue&& other) {
    value_.swap(other.value_);
    type_ = other.type_;
    return *this;
}

bool jagunadle::JsonValue::b() const {
    boolean_or_exception();
    return type_ == Type::True;
}

bool jagunadle::JsonValue::t() const {
    return b() == true;
}

bool jagunadle::JsonValue::f() const {
    return !t();
}

double jagunadle::JsonValue::d() const {
    match_type_or_exception(Type::Number);
    return std::stod(value_.raw);
}
    
jagunadle::Int jagunadle::JsonValue::i() const { return static_cast<Int>(i64()); }
jagunadle::Int8 jagunadle::JsonValue::i8() const { return static_cast<Int8>(i64()); }     
jagunadle::Int16 jagunadle::JsonValue::i16() const { return static_cast<Int16>(i64()); }     
jagunadle::Int32 jagunadle::JsonValue::i32() const { return static_cast<Int32>(i64()); }     
jagunadle::Int64 jagunadle::JsonValue::i64() const { 
    match_type_or_exception(Type::Number);
    return std::stoll(value_.raw);
} 

jagunadle::Uint jagunadle::JsonValue::ui() const { return static_cast<Uint>(ui64()); }
jagunadle::Uint8 jagunadle::JsonValue::ui8() const { return static_cast<Uint8>(ui64()); }     
jagunadle::Uint16 jagunadle::JsonValue::ui16() const { return static_cast<Uint16>(ui64()); }     
jagunadle::Uint32 jagunadle::JsonValue::ui32() const { return static_cast<Uint32>(ui64()); }     
jagunadle::Uint64 jagunadle::JsonValue::ui64() const { 
    match_type_or_exception(Type::Number);
    return std::stoull(value_.raw);
}   

std::string jagunadle::JsonValue::s() const {
    match_type_or_exception(Type::String);
    return value_.raw;
}
jagunadle::JsonArray& jagunadle::JsonValue::a() {
    match_type_or_exception(Type::Array);
    return value_.array;
}
jagunadle::JsonObject& jagunadle::JsonValue::o() {
    match_type_or_exception(Type::Object);
    return *value_.object;
}

bool jagunadle::JsonValue::is_null() const { return check_type(Type::Null); }
bool jagunadle::JsonValue::is_boolean() const { return check_type(Type::True) || check_type(Type::False); }
bool jagunadle::JsonValue::is_number() const { return check_type(Type::Number); }
bool jagunadle::JsonValue::is_string() const { return check_type(Type::String); }
bool jagunadle::JsonValue::is_array() const { return check_type(Type::Array); }
bool jagunadle::JsonValue::is_object() const { return check_type(Type::Object); }   
 
void jagunadle::JsonValue::copy(const JsonValue& from) {
    value_ = from.value_;
    type_ = from.type_;
}
 
bool jagunadle::JsonValue::check_type(Type t) const { return t == type_; }

void jagunadle::JsonValue::boolean_or_exception() const {
    if (check_type(Type::True) || check_type(Type::False)) return;
    throw_type_error();
}
void jagunadle::JsonValue::match_type_or_exception(Type t) const {
    if (check_type(t)) return;
    throw_type_error();
}
void jagunadle::JsonValue::throw_type_error() const {
    std::string type_name = "null";
    switch (type_) {
      case Type::Null:
        type_name = "null";
      case Type::True:
      case Type::False:
        type_name = "boolean";
        break;
      case Type::Number:
        type_name = "number";
        break;
      case Type::String:
        type_name = "string";
        break;
      case Type::Array:
        type_name = "array";
        break;
      case Type::Object:
        type_name = "object";
        break;
      default:
        break;
    }       
    throw TypeError("value should be "+type_name);
}


void jagunadle::JsonValue::set_type(Type t) {
    type_ = t;
}

void jagunadle::JsonValue::set_value() {
    value_ = ValueHolder();
}

void jagunadle::JsonValue::set_value(const std::string& v) {
    value_ = ValueHolder(v);
}

void jagunadle::JsonValue::set_value(const JsonArray& v) {
    value_ = ValueHolder(v);
}

void jagunadle::JsonValue::set_value(const JsonObject& v) {
    value_ = ValueHolder(v);
}

std::string jagunadle::JsonValue::dump() const {
    switch (type_) {
      case Type::True:
        return "true";
      case Type::False:
        return "false";
      case Type::Number:
        return value_.raw;
      case Type::String:
        return '"' + value_.raw + '"';
      case Type::Array: {
            std::ostringstream oss;
            oss << "[";
            auto& arr = value_.array;
            for (auto it = arr.begin(); it != arr.end(); ++it) {
                if (it != arr.begin()) {
                    oss << ", ";
                }
                oss << (*it).dump();
            }
            oss << "]";
            return oss.str();
        }
      case Type::Object:
        return dumps(*value_.object);
      default: break;
    }
    return "\"\"";
}