#include "scanner.h"

#include "error.h"


void Scanner::Rollback() {
    current_ = prev_pos_;
}
//判断是否扫描完
bool Scanner::IsAtEnd() {
    return current_ >= source_.size();
}
//获取下一个字符
char Scanner::Advance() {
    return source_[current_++];
}
//判断是不是整数
bool Scanner::IsDigit(char c) {
    return c >= '0' && c <= '9';
}
//读取字符
char Scanner::Peek() {
    if (IsAtEnd()) return '\0';
    return source_[current_];
}
//读取下一个字符
char Scanner::PeekNext() {
    if (current_ + 1 >= source_.size()) return '\0';
    return source_[current_ + 1];
}
//验证是否是true这种形式
void Scanner::ScanTrue() {
    //往后数三位
    if (source_.compare(current_, 3, "rue") == 0) {
        current_ += 3;
    }
    else {
        Error("Scan `true` error");
    }
}
void Scanner::ScanFalse() {
    if (source_.compare(current_, 4, "alse") == 0) {
        current_ += 4;
    }
    else {
        Error("Scan `false` error");
    }
}
void Scanner::ScanNull() {
    if (source_.compare(current_, 3, "ull") == 0) {
        current_ += 3;
    }
    else {
        Error("Scan `null` error");
    }
}
void Scanner::ScanNumber() {
    size_t pos = current_ - 1;
    //例如 -99 负数
    while (IsDigit(Peek())) {
        Advance();
    }

    // 例如9.99 有小数
    if (Peek() == '.' && IsDigit(PeekNext())) {
        Advance();
        while (IsDigit(Peek())) {
            Advance();
        }
    }
    value_number_ = std::atof(source_.substr(pos, current_ - pos).c_str());
}
void Scanner::ScanString() {
    size_t pos = current_;
    //遍历一直到"
    while (Peek() != '\"' && !IsAtEnd()) {
        Advance();
        if (Peek() == '\\') {
            Advance();
            switch (Peek()) {
            case 'b':
            case 't':
            case 'n':
            case 'f':
            case 'r':
            case '\"':
            case '\\':
                Advance();
                break;
            case 'u':
                // unicode's length is 4
                for (int i = 0; i < 4; i++) {
                    Advance();
                }
                break;
            default:
                break;
            }
        }
    }
    //这里出现了一个很隐蔽的bug，导致调试了很久，
    //if不是在while之内的
    if (IsAtEnd()) {
        Error("invalid string: missing closing quote");
    }
    Advance();
    value_string_ = source_.substr(pos, current_ - pos - 1);
}

//从当前扫描位置出发，扫描接下来的，并返回下来的token类型
// return JsonTokenType();
Scanner::JsonTokenType Scanner::Scan() {
    //首先判断是否扫描完毕
    if (IsAtEnd()) {
        return JsonTokenType::END_OF_SOURCE;
    }
    prev_pos_ = current_;

    //获取下一个字符
    char c = Advance();

    switch (c) {
    case '{':
        return JsonTokenType::BEGIN_OBJECT;
    case '}':
        return JsonTokenType::END_OBJECT;
    case '[':
        return JsonTokenType::BEGIN_ARRAY;
    case ']':
        return JsonTokenType::END_ARRAY;
    case ':':
        return JsonTokenType::COLON_SEPARATOR;
    case ',':
        return JsonTokenType::COMMA_SEPARATOR;
    case 't':
        ScanTrue(); //判断必须是true 这种，不能是trueff，tru这种
        return JsonTokenType::LITERAL_TRUE;
    case 'f':
        ScanFalse();
        return JsonTokenType::LITERAL_FALSE;
    case 'n':
        ScanNull();
        return JsonTokenType::LITERAL_NULL;
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        ScanNumber();
        return JsonTokenType::VALUE_NUMBER;
    case '\"':
        ScanString();
        return JsonTokenType::VALUE_STRING;
    case ' ':
    case '\r':
    case '\n':
    case '\t':
        return Scan();
    default:
        // error
        std::string message = "Unsupported Token: ";
        message += c;
        Error(std::string(message));
    }
}



