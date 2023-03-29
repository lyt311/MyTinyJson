#pragma once
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "error.h"

class JsonElement;
//利用指针来避免每次调用拷贝构造器复制
//键是string，值是下面六种类型之一
using JsonObject = std::map<std::string, JsonElement*>;
using JsonArray = std::vector<JsonElement*>;

class JsonElement {
public:
    enum class Type {
        JSON_OBJECT,
        JSON_ARRAY,
        JSON_STRING,
        JSON_NUMBER,
        JSON_BOOL,
        JSON_NULL
    };

    union Value {
        JsonObject* value_object;
        JsonArray* value_array;

        std::string* value_string;
        float value_number;

        bool value_bool;
    };
    
   JsonElement():type_(Type::JSON_NULL){}

    JsonElement(JsonObject* value_object) {
        value(value_object);
    }
    JsonElement(JsonArray* value_array) {
        value(value_array);
    }
    JsonElement(std::string* value_string) {
        value(value_string);
    }
    JsonElement(float value_number) {
        value(value_number);
    }
    JsonElement(bool value_bool) {
        value(value_bool);
    }

    ~JsonElement(){
       if (type_ == Type::JSON_OBJECT) {
            JsonObject* object = value_.value_object;
             for (auto& [key ,value] : *object) {
                delete value;
            }
            delete object;
        }
        else if (type_ == Type::JSON_ARRAY) {
            JsonArray* array = value_.value_array;
            for (auto& item : *array) {
                delete item;
            }
            delete array;
        }
        else if (type_ == Type::JSON_STRING) {
            std::string* val = value_.value_string;
            delete val;
        }
    }

    void value(JsonObject* value_object) {
        type_ = Type::JSON_OBJECT;
        value_.value_object=value_object;
    }
   
    void value(JsonArray* value_array) {
        type_ = Type::JSON_ARRAY;
        value_.value_array = value_array;
    }
    void value(std::string* value_string) {
        type_ = Type::JSON_STRING;
        value_.value_string = value_string;
    }
    void value(float value_number) {
        type_ = Type::JSON_NUMBER;
        value_.value_number = value_number;
    }
    void value(bool value_bool) {
        type_ = Type::JSON_BOOL;
        value_.value_bool = value_bool;
    }  

    

    //将Json element转换为string表示,json字符串
    std::string Dumps() {
        std::stringstream ss;
        switch (type_){
            case Type::JSON_OBJECT:
                ss << *(value_.value_object);
                break;
            case Type::JSON_ARRAY:
                ss << *(value_.value_array);
                break;
            case Type::JSON_STRING:
                ss << '\"' << *(value_.value_string) << '\"';
                break;
            case Type::JSON_NUMBER:
                ss << value_.value_number;
                break;
            case Type::JSON_BOOL:
                ss << (value_.value_bool ? "true" : "false");
                break;
            case Type::JSON_NULL:
                ss << "null";
                break;
            default:
                break;
        }
        return ss.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const JsonObject& object) {
        os << "{";
        for (auto iter = object.begin(); iter != object.end(); iter++) {
            os << '\"' << iter->first << '\"' << ": " << iter->second->Dumps();
            if (iter != --object.end()) {
                os << ", ";
            }
        }
        os << "}";
        return os;
    }

    friend std::ostream& operator<<(std::ostream& os, const JsonArray& array) {
        os << "[";
        for (size_t i = 0; i < array.size(); i++) {
            os << array[i]->Dumps();
            if (i != array.size() - 1) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }


private:
    Type type_;
    Value value_;
};


