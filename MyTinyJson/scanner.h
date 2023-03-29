#pragma once
//用于将传入的json字符串，解析成token形式
#include <iostream>
#include <string>

class Scanner {
public:
    Scanner(std::string source) : source_(source), current_(0) {}

    enum class JsonTokenType {

        BEGIN_OBJECT,   // {
        END_OBJECT,     // }   

        COLON_SEPARATOR, // :       
        COMMA_SEPARATOR, // ,

        VALUE_NUMBER,  //number
        VALUE_STRING,  //string

        LITERAL_TRUE,
        LITERAL_FALSE,
        LITERAL_NULL,

        BEGIN_ARRAY,    //  [
        END_ARRAY,      //  ]

        END_OF_SOURCE  //标志解析完成
    };

    //运算符<< 重载
    friend std::ostream& operator<<(std::ostream& os, const JsonTokenType& type) {
        switch (type) {
        case JsonTokenType::BEGIN_ARRAY:
            os << "[";
            break;
        case JsonTokenType::END_ARRAY:
            os << "]";
            break;
        case JsonTokenType::BEGIN_OBJECT:
            os << "{";
            break;
        case JsonTokenType::END_OBJECT:
            os << "}";
            break;
        case JsonTokenType::COLON_SEPARATOR:
            os << ":";
            break;
        case JsonTokenType::COMMA_SEPARATOR:
            os << ",";
            break;
        case JsonTokenType::VALUE_NUMBER:
            os << "number";
            break;
        case JsonTokenType::VALUE_STRING:
            os << "string";
            break;
        case JsonTokenType::LITERAL_TRUE:
            os << "true";
            break;
        case JsonTokenType::LITERAL_FALSE:
            os << "false";
            break;
        case JsonTokenType::LITERAL_NULL:
            os << "null";
            break;
        case JsonTokenType::END_OF_SOURCE:
            os << "EOF";
            break;
        default:
            break;
        }
        return os;
    }


    //从当前扫描位置出发，扫描接下来的，并返回下来的token类型
    JsonTokenType Scan();

    std::string GetValueString() { return value_string_; };
    float GetValueNumber() { return value_number_; };
    void Rollback();
private:
    bool IsAtEnd();
    char Advance();

    void ScanTrue();
    void ScanFalse();
    void ScanNull();
    void ScanString();
    void ScanNumber();

    bool IsDigit(char c);
    char Peek();
    char PeekNext();

private:
    std::string source_; //json source
    size_t current_; //当前处理的索引
    size_t prev_pos_;//之前处理的索引

    float value_number_;        ///< number value
    std::string value_string_;  ///< string value
};


