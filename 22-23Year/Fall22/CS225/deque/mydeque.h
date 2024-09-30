#ifndef DEQUE_H
#define DEQUE_H
#include <fstream>

namespace DigiPen {

    class Deque {
        public:
            // big 4
            Deque(); //Done
            Deque( Deque const& rhs ); //Done
            Deque& operator=(Deque rhs); //Done
            ~Deque(); //Done

            // from array
            Deque( int * array_, unsigned int size_ ); //Done
            
            // simple stuff
            int   Size() const; //Done
            bool  Empty() const; //Done
            void  Clear(); //Done
            int   Capacity() const; //Done

            // real stuff
            void  Push_front(int val); //Done
            int   Pop_front(); //Done
            void  Push_back(int val); //Done
            int   Pop_back(); //Done
            int&  operator[](unsigned int pos) ; //Done
            int   operator[](unsigned int pos) const ; //Done

            // useful stuff
            void swap( Deque& other ) ; //Done

            // non-standard stuff
            Deque&    operator+=(const Deque& rhs);       // concatenate 2 deques //Done
            Deque     operator+(const Deque& rhs) const;  // concatenate 2 deques //Done
            Deque&    reverse() ;                // reverse in-place //Done
            Deque     operator~() const;                 // return new reverse //Done

            // implemented
            void Print() const;
        private:
            // implemented
            // NOP if new_capacity <= current size
            void reallocate( int new_capacity );
        private:
            int b;    // index where deque's data starts
            int e;    // index where deque's data ends
            int size;
            int capacity; // size of the array
            int * array;  // data array
    };

    // non friend - but has to be declared in the header
    std::ostream & operator<<(std::ostream & os, const Deque &d);

}

#endif
