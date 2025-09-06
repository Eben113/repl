#include <iostream>
#include <windows.h>
#include <functional>
#include <string_view>
#include <string>

int main(){
    std::string a = "this is a test string";
    int b = a.data();
    std::cout << b;
    std::cout << a.size();
    
    return 0;
}
