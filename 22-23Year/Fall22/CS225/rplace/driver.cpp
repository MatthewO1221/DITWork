#include "replace.h"
#include <cstring>
#include <iostream>
#include <tuple>

void print_result( std::tuple<bool,int> const& res, char * str )
{
    if ( std::get<0>(res) ) {
        std::cout << "FINAL size = " << std::get<1>(res) << ": -->";
        for ( int i=0; i<std::get<1>(res); ++i ) {
            std::cout << str[i];
        }
        std::cout << "<--\n";
    } else {
        std::cout << "not enough space\n";
    }
}

///////////////////////////////////////////////////////////////////////////////////
// neither string itself or search and replace substring have to be NULL-terminated
///////////////////////////////////////////////////////////////////////////////////


void test0() // no shifting
{
    int const buffer_size = 8;
    char str [buffer_size]  = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[]             = { 'c', 'd' };
    char rplc[]             = { '1', '2' };

    print_result(   replace( find, 2, rplc, 2, str, 8, buffer_size ), 
            str 
            );
}

void test1() // no shifting
{
    int const buffer_size = 12;
    char str [buffer_size]  = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[]             = { 'c', 'd' };
    char rplc[]             = { '1', '2' };

    print_result(   replace( find, 2, rplc, 2, str, 8, buffer_size ), 
            str 
            );
}

void test2() // no shifting, several occurences
{
    int const buffer_size = 20; 
    char str [buffer_size]  = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c', 'd' };
    char find[]             = { 'a', 'b', 'c' };
    char rplc[]             = { '1', '2', '3' };

    print_result(   replace( find, 3, rplc, 3, str, 9, buffer_size ), 
            str 
            );
}

void test3() // no shifting, several occurences, one in the very end
{
    int const buffer_size = 20; 
    char str [buffer_size]  = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[]             = { 'a', 'b', 'c' };
    char rplc[]             = { '1', '2', '3' };

    print_result(   replace( find, 3, rplc, 3, str, 8, buffer_size ), 
            str 
            );
}

void test4() // no shifting, several occurences, overlapping
{
    int const buffer_size = 20; 
    char str [buffer_size]  = { 'a', 'b', 'c', 'd', 'z', 'z', 'z', 'z' };
    char find[]             = { 'z', 'z', 'z' };
    char rplc[]             = { '1', '2', '3' };

    print_result(   replace( find, 3, rplc, 3, str, 8, buffer_size ), 
            str 
            );
}


////////////////////////////////////////////////////////////////////////////////
void test5() // string shifted left 1 occurence
{
    int const buffer_size = 12;
    char str [buffer_size]  = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[]             = { 'c', 'd' };
    char rplc[]             = { '1' };

    print_result(   replace( find, 2, rplc, 1, str, 8, buffer_size ), 
            str 
            );
}

void test6() // string shifted left 2 occurences
{
    int const buffer_size = 12;
    char str [buffer_size]  = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[]             = { 'a', 'b' };
    char rplc[]             = { '1' };

    print_result(   replace( find, 2, rplc, 1, str, 8, buffer_size ), 
            str 
            );
}

void test7() // string shifted left 2 occurences, one in the very end
{
    int const buffer_size = 12;
    char str [buffer_size]  = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[]             = { 'b', 'c' };
    char rplc[]             = { '1' };

    print_result(   replace( find, 2, rplc, 1, str, 8, buffer_size ), 
            str 
            );
}

void test8() // string shifted left 2 occurences, overlapping
{
    int const buffer_size = 12;
    char str [buffer_size]  = { 'a', 'b', 'c', 'z', 'z', 'z', 'b', 'c' };
    char find[]             = { 'z', 'z' };
    char rplc[]             = { '1' };

    print_result(   replace( find, 2, rplc, 1, str, 8, buffer_size ), 
            str 
            );
}

////////////////////////////////////////////////////////////////////////////////
void test9() // string shifted right, one occurences
{
    int const buffer_size = 12;
    char str [buffer_size]  = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[]             = { 'c', 'd' };
    char rplc[]             = { '1', '2', '3', '4' };

    print_result(   replace( find, 2, rplc, 4, str, 8, buffer_size ), 
            str 
            );
}

void test10() // string shifted right, several occurences
{
    int const buffer_size = 20; 
    char str [buffer_size]  = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c', 'd' };
    char find[]             = { 'a', 'b', 'c' };
    char rplc[]             = { '1', '2', '3', '4', '5' };

    print_result(   replace( find, 3, rplc, 5, str, 9, buffer_size ), 
            str 
            );
}

void test11() // string shifted right, several occurences, one at the very end
{
    int const buffer_size = 20; 
    char str [buffer_size]  = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[]             = { 'a', 'b', 'c' };
    char rplc[]             = { '1', '2', '3', '4', '5' };

    print_result(   replace( find, 3, rplc, 5, str, 8, buffer_size ), 
            str 
            );
}

void test12() // string shifted right, one at the very end, overlapping
{
    int const buffer_size = 20; 
    char str [buffer_size]  = { 'a', 'b', 'c', 'z', 'z', 'z', 'z', 'z' };
    char find[]             = { 'z', 'z', 'z' };
    char rplc[]             = { '1', '2', '3', '4', '5' };

    print_result(   replace( find, 3, rplc, 5, str, 8, buffer_size ), 
            str 
            );
}


////////////////////////////////////////////////////////////////////////////////
void test13() // string shifted right, not enough space
{
    int const buffer_size = 9; // <-- only 1 extra character possible
    char str [buffer_size]  = { 'a', 'b', 'c', 'd', 'e', 'a', 'b', 'c' };
    char find[]             = { 'c', 'd' };
    char rplc[]             = { '1', '2', '3', '4' };

    print_result(   replace( find, 2, rplc, 4, str, 8, buffer_size ), 
            str 
            );
}

#include <random>
#include <regex>
#include <string>
#include <algorithm>
////////////////////////////////////////////////////////////////////////////////
void test_stress_correctness( 
        int str_size_min, int str_size_max, 
        int find_size_min, int find_size_max, 
        int repl_size_min, int repl_size_max,
        int number_distinct_chars,   // which chars to use in randomely generated strings ( 1 - to use only 'a')
        bool print_a_lot
        )
{
    std::random_device rd;
    std::mt19937 gen( rd() );

    std::uniform_int_distribution<> dis_size( str_size_min, str_size_max ); // str size
    std::uniform_int_distribution<> dis_char('a', 'a'+number_distinct_chars -1 ); // random char a...f

    // random str
    int str1_size = dis_size( gen );
    std::string str1( str1_size, 'a' ); // many 'a's
    std::generate( str1.begin(), str1.end(), [&dis_char, &gen]() { return dis_char( gen ); } ); // random chars
    if ( print_a_lot ) {
        std::cout << "String -->" << str1 << "<--\n";
    }

    // random find: choose a random substring in str
    std::uniform_int_distribution<> dis_substring_start(0, str1_size/2); // stating here
    std::uniform_int_distribution<> dis_substring_size( find_size_min, find_size_max ); // of this length
    int find_size = dis_substring_size( gen );
    std::string find = str1.substr( dis_substring_start(gen), find_size );
    if ( print_a_lot ) {
        std::cout << "Find -->" << find << "<--\n";
    }

    // random replace
    std::uniform_int_distribution<> dis_repl_size( repl_size_min, repl_size_max ); // replace substring size
    int repl_size = dis_repl_size( gen );
    std::string repl( repl_size, '1' ); // many '1's - makes it easier to see
    //std::generate( repl.begin(), repl.end(), [&dis_char, &gen]() { return dis_char( gen );} ); // random chars
    if ( print_a_lot ) {
        std::cout << "Replace -->" << repl << "<--\n";
    }

    // prepare regular expression
    std::regex find_re( find );

    // replace using STL 
    std::string str2 = std::regex_replace( str1, find_re, repl );

    // prepare for our replace
    int buffer_size = str2.size();
    if ( str1.size()>buffer_size ) { buffer_size = str1.size(); }
    char * str = new char[buffer_size]; // enough to hold all chars after replace
    std::copy( str1.begin(), str1.end(), str ); // copy initial string, no new line

    // call 
    std::tuple<bool,int> res = replace( 
            find.c_str(), find_size, 
            repl.c_str(), repl_size, 
            str, str1.size(), buffer_size );

    if ( print_a_lot ) {
        std::cout << "Expected -->" << str2 << "<--\n";
        std::cout << "Result   -->";
        for ( int i=0; i<std::get<1>(res); ++i ) {
            std::cout << str[i];
        }
        std::cout << "<--\n";
        std::cout << "Errors   -->";
    }
    // compare results
    if ( std::get<0>( res ) ) {
        if ( print_a_lot ) {
            int m = std::min<int>( std::get<1>(res), str2.size() );
            for ( int i=0; i<m; ++i ) {
                if ( str2[i] != str[i] )    { std::cout << "^"; } 
                else                        { std::cout << " "; }
            }
            std::cout << "\n";
        } else { // brief
            if ( std::get<1>( res ) == str2.size() ) { // check size
                bool all_good = true;
                for ( int i=0; i<std::get<1>(res) && all_good; ++i ) {
                    all_good = ( str2[i] == str[i] );
                }
                    if ( all_good ) {
                        std::cout << "Looks good\n";
                    } else {
                        std::cout << "\nSize is correct, some errors\n";
                    }
                } else {
                    std::cout << "Incorrect size after replace\n";
                }
            }
    } else { // if ( std::get<0>( res ) ) 
        std::cout << "Not enough space\n";
    }

    delete [] str;
}

// random, small size - debugging
void test14()
{
    test_stress_correctness( 10,12,  2,4,  1,5, 6, true );
}


// random, medium size
void test15()
{
    test_stress_correctness( 8000,12000,  3,5,  2,6, 26, false );
}

// random, medium size, only few chars
// overlapping matches are possible:
// match "aaa" in "aaaa": correct is "AAAa"
void test16()
{
    test_stress_correctness( 8000,12000,  3,5,  2,6, 2, false );
}


void test17() // random many times
{
    
    for ( int i=0;i<500; ++i ) {
        test15();
        test16();
    }
}

////////////////////////////////////////////////////////////////////////////////
// this is NOT a random test:
// large text with many substitutions
#include <cassert>
void stress_test_runtime( int find_size, int repl_size, int num_replaces ) 
{ // abcabcabc - replace abc with 12345

    assert( find_size<27 && repl_size<27 );

    int buffer_size = num_replaces * std::max( find_size, repl_size );

    // generate find substring
    char * find = new char[find_size];
    for ( int j=0; j<find_size; ++j ) {
        find[j] = 'a'+j; // abcdef...
    }

    // generate text
    char * str = new char[buffer_size]; // enough to hold all chars after replace
    for ( int i=0; i<num_replaces; ++i ) {
        std::memcpy( str +i*find_size, find, find_size );
    }

    // generate repl substring
    char * repl = new char[repl_size];
    for ( int j=0; j<repl_size; ++j ) {
        repl[j] = 'z'-j; // zyxwv....
    }

    // call 
    std::tuple<bool,int> res = replace( 
            find, find_size, 
            repl, repl_size, 
            str, find_size*num_replaces, buffer_size );

    // check results
    if ( std::get<0>( res ) ) {
        bool OK = true;
        if ( std::get<1>( res ) == repl_size*num_replaces ) { // check final size
            for ( int i=0; i<std::get<1>(res); ++i ) {
                if ( str[i] != repl[i%repl_size] ) {
                    std::cout << "error an index " << i << std::endl;
                    OK = false;
                }
            }
            if ( OK ) {
                std::cout << "Looks good\n";
            }
        } else {
            std::cout << "Incorrect size after replace\n";
        }
    } else {
        std::cout << "Not enough space\n";
    }

    delete [] str;
    delete [] find;
    delete [] repl;
}

void test18() 
{
    stress_test_runtime( 2, 20, 1<<15 ); // 2^15 - 0.85s with BF
}

void test19() 
{
    stress_test_runtime( 20, 2, 1<<15 ); // 2^15 - 5.5s with BF
}


void (*pTests[])() = { 
    test0,test1,test2,test3,test4,test5,test6,test7,test8,test9,test10,test11,test12,test13, //debugging
    test14,test15,test16,test17,        // stress test correctness
    test18,test19                       // stress test speed 
};

int main(int argc, char ** argv) try {
    if (argc >1) {
        int test = 0;
        std::sscanf(argv[1],"%i",&test);
        pTests[test]();
    }
    return 0;
} catch( char const* msg) {
    std::cout << msg << std::endl;
}
