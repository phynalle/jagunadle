# jagunadle

jagunadle is a lightweight (oriented) json library for C++ 11. 



it's easy to use like this:

    jagunadle::JsonObject my; 
    my["key"] = "value";
    my["trueKey"] = true;
    my["number"] = 123;
    my["pi"] = 3.141592;
    std::string serialized_json = jagunadle::dumps(my);

JsonObject represents std::unordered\_map&lt;JsonValue&gt;. the key, value pair can be inserted by using simple interface of map container.


----------


Here's another example:

    jagunadle::JsonObject inner {
        {"key", "value"},
        {"serialize", true},
        {"number", 1234},
    };  
    jagunadle::JsonArray arr = {"one", 2, true, inner, 5.0};
    jagunadle::JsonObject outer {
        {"array", arr},
        {"parse", false},
    };  
    std::string serialized_json = jagunadle::dumps(outer);

JsonValue can be initialized with,  or assigned with types of this : bool for boolean, char~long long and double for number, char\* and std::string for string, JsonArray for array, and JsonObject for object.   
JsonArray represents std::vector&lt;JsonValue&gt;.
It means that any types can be put in any container intuitively. How easy it is!!

 On this version, parsing json is not implemented yet. I'll do it soon.   
