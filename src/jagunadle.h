#ifndef JAGUNADLE_H_
#define JAGUNADLE_H_

#include <string>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <memory>

namespace jagunadle {


class TypeError : public std::runtime_error {
public:
    explicit TypeError(const std::string& what) : std::runtime_error(what) {}
};


typedef int Int;
typedef char Int8;
typedef short int Int16;
typedef long int Int32;
typedef long long int Int64;

typedef unsigned int Uint;
typedef unsigned char Uint8;
typedef unsigned short int Uint16;
typedef unsigned long int Uint32;
typedef unsigned long long int Uint64;

class JsonValue;
typedef std::vector<JsonValue> JsonArray;
typedef std::unordered_map<std::string, JsonValue> JsonObject;

class JsonValue {
    enum class Type {
        Null,
        True,
        False,
        Number,
        String,
        Array,
        Object,
    };
        
    struct ValueHolder {
        std::string raw;
        JsonArray array;
        std::shared_ptr<JsonObject> object;    
    };
    
public:
    JsonValue();
    JsonValue(bool value);
    JsonValue(Int value);
    JsonValue(Int8 value);
    JsonValue(Int16 value);
    JsonValue(Int32 value);
    JsonValue(Int64 value);
    JsonValue(Uint value);
    JsonValue(Uint8 value);
    JsonValue(Uint16 value);
    JsonValue(Uint32 value);
    JsonValue(Uint64 value);
    JsonValue(const char* value);
    JsonValue(const std::string& value);
    JsonValue(const JsonArray& value);
    JsonValue(const JsonObject& value);
        
    virtual ~JsonValue();
    JsonValue(const JsonValue& from);
    JsonValue(JsonValue&& from);
    
    bool operator==(const JsonValue& other);
    bool operator!=(const JsonValue& other);

    JsonValue& operator=(bool value);
   
    JsonValue& operator=(double value);
    JsonValue& operator=(Int value);
    JsonValue& operator=(Int8 value);
    JsonValue& operator=(Int16 value);
    JsonValue& operator=(Int32 value);
    JsonValue& operator=(Int64 value);

    JsonValue& operator=(Uint value);
    JsonValue& operator=(Uint8 value);
    JsonValue& operator=(Uint16 value);
    JsonValue& operator=(Uint32 value);
    JsonValue& operator=(Uint64 value);
    
    JsonValue& operator=(const char* value);
    JsonValue& operator=(const std::string& value);
    
    JsonValue& operator=(const JsonArray& value);
    JsonValue& operator=(const JsonObject& value);
    
    JsonValue& operator=(const JsonValue& other);
    
    bool b() const ;
    bool t() const ;
    bool f() const ;

    double d() const ;
        
    Int i() const;
    Int8 i8() const; 
    Int16 i16() const;
    Int32 i32() const;
    Int64 i64() const;
    
    Uint ui() const;
    Uint8 ui8() const;
    Uint16 ui16() const; 
    Uint32 ui32() const;
    Uint64 ui64() const;
   
    std::string s() const;
    const JsonArray& a() const;
    const JsonObject& o() const;
    bool is_null() const;
    bool is_boolean() const;
    bool is_number() const;
    bool is_string() const;
    bool is_array() const;
    bool is_object() const;
    
private:    
    bool check_type(Type t) const;
    void boolean_or_exception() const;
    void match_type_or_exception(Type t) const;
    void throw_type_error() const;
    
    void set_type(Type t);
    void set_value();
    void set_value(const std::string& v);
    void set_value(const JsonArray& v);
    void set_value(const JsonObject& v);

    std::string dump() const;
    
    Type type_;
    ValueHolder value_;  
    
    static const std::string nullstring;
	static const JsonArray nullarray;
    static const std::shared_ptr<JsonObject> nullobject;
    
    friend std::string dumps(const JsonObject& json);
};

std::string dumps(const JsonObject& json) ;

} // namespace jaguandle

#endif /* JAGUNADLE_H_ */