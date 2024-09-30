////////////////////////////////////////////////////////////////////////////////
#ifndef LARIAT_H
#define LARIAT_H
////////////////////////////////////////////////////////////////////////////////

#include <string>  // error strings
#include <utility> // error strings
#include <cstring> // memcpy

class LariatException : public std::exception
{

private:
    int m_ErrCode;
    std::string m_Description;

public:
    LariatException(int ErrCode, const std::string& Description) : m_ErrCode(ErrCode), m_Description(Description) {}

    virtual int code(void) const
    {
        return m_ErrCode;
    }

    virtual const char* what(void) const throw()
    {
        return m_Description.c_str();
    }

    virtual ~LariatException() throw()
    {
    }

    enum LARIAT_EXCEPTION
    {
        E_NO_MEMORY,
        E_BAD_INDEX,
        E_DATA_ERROR
    };
};

// forward declaration for 1-1 operator<<
template <typename T, int Size>
class Lariat;

template <typename T, int Size>
std::ostream& operator<<(std::ostream& os, Lariat<T, Size> const& rhs);

template <typename T, int Size>
class Lariat
{
public:
#define nodePair std::pair<LNode *, int>

    template <typename U, int Size2>
    friend class Lariat;

    Lariat();                  // default constructor     DONE
    Lariat(Lariat const& rhs); // copy constructor        DONE
    template <typename U, int Size2>
    Lariat(Lariat<U, Size2> const& rhs);
    ~Lariat(); // destructor              DONE
    // more ctor(s) and assignment(s)

    Lariat& operator=(Lariat const& rhs); // DONE

    template <typename U, int Size2>
    Lariat<T, Size>& operator=(Lariat<U, Size2> const& rhs);

    // inserts
    void insert(int index, const T& value); // DONE
    void push_back(const T& value);         // DONE
    void push_front(const T& value);        // DONE

    // deletes
    void erase(int index); // DONE
    void pop_back();       // DONE
    void pop_front();      // DONE

    // access
    T& operator[](int index);             // for l-values DONE
    const T& operator[](int index) const; // for r-values DONE
    T& first();                           // DONE
    T const& first() const;               // DONE
    T& last();                            // DONE
    T const& last() const;                // DONE

    unsigned find(const T& value) const; // returns index, size (one past last) if not found DONE

    friend std::ostream& operator<< <T, Size>(std::ostream& os, Lariat<T, Size> const& list);

    // and some more
    size_t size(void) const; // total number of items (not nodes)     DONE
    void clear(void);        // make it empty     DONE

    void compact(); // push data in front reusing empty positions and delete remaining nodes //DONE
private:
    struct LNode
    { // DO NOT modify provided code
        LNode* next = nullptr;
        LNode* prev = nullptr;
        int count = 0; // number of items currently in the node
        T values[Size];
    };
    // DO NOT modify provided code
    LNode* head_;           // points to the first node
    LNode* tail_;           // points to the last node
    int size_;              // the number of items (not nodes) in the list
    mutable int nodecount_; // the number of nodes in the list
    int asize_;             // the size of the array within the nodes

    ////////////////////////Helper Functions/////////////////////////////////

    std::pair<LNode*, int> FindElement(int index) const;
    T& ShiftLeft(LNode* node, int index);
    T& ShiftRight(LNode* node, int index);
    void Split(LNode* node, T* overflow);
};

#include "lariat.cpp"

#endif // LARIAT_H
