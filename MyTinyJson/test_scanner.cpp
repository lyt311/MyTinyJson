#include <iostream>

#include "parser.h"

int main(){
    auto source = R"(
{
    "person":{
		"name":"lyt",		
        "age":24,
	    "height":176,
        "student":true,
        "hobby":["football","LEGO","music"],
        "null":null
}
    }
    )";

    Scanner scanner(source);
    Scanner::JsonTokenType type;
    while ((type = scanner.Scan()) != Scanner::JsonTokenType::END_OF_SOURCE) {
        std::cout << "Type: " << type;
        if (type == Scanner::JsonTokenType::VALUE_NUMBER) {
            std::cout << " Value: " << scanner.GetValueNumber();
        }
        else if (type == Scanner::JsonTokenType::VALUE_STRING) {
            std::cout << " Value: " << scanner.GetValueString();
        }
        std::cout << '\n';
    }
}