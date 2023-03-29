#pragma once

#include "json_element.h"
#include "scanner.h"

class Parser {
    using JsonTokenType = Scanner::JsonTokenType;
public:
    Parser(Scanner scanner): scanner_(scanner) {}
    JsonElement* Parse();
private:
    JsonObject* ParseObject();
    JsonArray* ParseArray();
private:
    Scanner scanner_;
};


