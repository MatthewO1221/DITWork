#ifndef REPLACE_H
#define REPLACE_H
#include <tuple>
#include <iostream>
// replace find_substr with repl_substr in str
// if str_size (the size of the string is enough to complete the operation: 
//      perform replacement and return <true, str size after replacement>
// else return <false,0> (state of the string does not matter)
// strings are not null terminated!!!
std::tuple<bool,int> replace(   char const * find_substr, int find_size,
                                char const * repl_substr, int repl_size,
                                char * str              , int str_size,
                                int buffer_size );
#endif
