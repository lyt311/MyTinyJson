#include <iostream>
#include "parser.h"


//int main() {
//    auto source = R"(
//    {
//    "person":{
//		"name":"lyt",		
//        "age":24,
//	    "height":176,
//        "student":true,
//        "hobby":["football","LEGO","music"],
//        "null":null
//    }
//    }
//  )";
//    Scanner scanner(source);
//    Parser parser(scanner);
//    JsonElement* element = parser.Parse();
//    std::cout << element->Dumps();
//    delete element;
//}