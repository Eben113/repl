#include <iostream>
#include <windows.h>
#include <functional>
#include <string_view>
#include <string>
#include <algorithm>


std::vector<std::string_view> split(std::string_view str){
        constexpr std::string_view delim(" ");
        std::vector<std::string_view>  output;

        for(auto first = str.data(),
                second = str.data(),
                last = first + str.size();
            first != last || second != last;
            first = second + 1){

                second = std::find_first_of(first, last, std::cbegin(delim), std::cend(delim));

                if(first != second){
                    output.emplace_back(first, second-first);
                }
            }
        return output;
        }

int main(){
    std::cout << split("echo")[0];
    
    return 0;
}
