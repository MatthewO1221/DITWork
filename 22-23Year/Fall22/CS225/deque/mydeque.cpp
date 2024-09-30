#include "mydeque.h"
#include <iostream>
#include <iomanip>

namespace DigiPen
{

    // implemented
    Deque::Deque() : b(0), e(0), size(0), capacity(0), array(nullptr)
    {
    }

    // implemented
    Deque::Deque(int *array_, unsigned int size_) : b(0), e(size_), size(e), capacity(size), array(new int[size])
    {
        for (int i = 0; i < e; ++i)
            array[i] = array_[i];
    }

    // implemented
    // trim capacity to size when coping
    Deque::Deque(const Deque &rhs) : b(0), e(rhs.size),
                                     size(e), capacity(size),
                                     array(size ? new int[size] : nullptr) // size == 0 => array = nullptr in ctor
    {
        for (int i = 0; i < e; ++i)
            array[i] = rhs[i];
    }

    // implemented
    int Deque::Size() const { return size; }

    // implemented
    bool Deque::Empty() const { return (size == 0); }

    // implemented
    int Deque::Capacity() const { return capacity; }

    void Deque::Push_back(int val)
    {
        if (size == capacity)
        { // double the array size, or change size=0 to size=1
            reallocate(capacity ? capacity * 2 : 1);
        }

        this->array[e % capacity] = val;
        size++;
        e = (e + 1) % capacity;
    }

    int Deque::Pop_back()
    {
        if (size == capacity / 4)
        { // half the array

            reallocate(capacity / 2);
        }
        // Print();
        size--;
        e = (e + capacity - 1) % capacity;

        return array[(e) % capacity];
    }

    void Deque::Push_front(int val)
    {
        if (size == capacity)
        { // double the array size, or change size=0 to size=1
            reallocate(capacity ? capacity * 2 : 1);
        }

        this->array[(b + capacity - 1) % capacity] = val;
        size++;
        b = (b + capacity - 1) % capacity;
    }

    int Deque::Pop_front()
    {
        if (size == capacity / 4)
        { // half the array
            reallocate(capacity / 2);
        }

        size--;
        b = (b + capacity + 1) % capacity;

        return array[(b + capacity - 1) % capacity];
    }

    // if current capacity is not enough for the combined:
    // reallocate and trim, else do not reallocate at all
    Deque &Deque::operator+=(const Deque &op2)
    {
        if (size + op2.size > capacity)
        {
            reallocate(size + op2.size);
        }

        for (int i = 0; i < op2.size; i++)
        {
            this->array[(e + capacity - 1 + i) % capacity] = op2.array[(b + capacity - 1 + i) % capacity];
        }

        return *this;
    }

    // implemented
    void Deque::Print() const
    {
        std::cout << "size = " << size << " capacity = " << capacity << std::endl;

        if (array == nullptr)
        {
            std::cout << "array=nullptr" << std::endl;
            return;
        }

        // deque may still be empty, but array is allocated, so print some data
        int width = 3;
        // line of indices
        for (int i = 0; i < capacity; ++i)
        {
            std::cout << std::setw(width) << i;
        }
        std::cout << " indices" << std::endl;
        // line of data
        for (int i = 0; i < capacity; ++i)
        {
            if ((b <= i && i < e)               //  ....b.....e.....
                || (e < b && (b <= i || i < e)) //  ....e.....b.....
                || (b == e && size > 0))        //  ....be.......... either empty or full - look at size
                std::cout << std::setw(width) << array[i];
            else
                std::cout << std::setw(width) << ".";
        }
        std::cout << " data" << std::endl;

        char ch1 = 'b', ch2 = 'e';
        int dist1 = b + 1, dist2 = e - b;
        if (e < b)
        {
            ch1 = 'e';
            ch2 = 'b';
            dist1 = e + 1;
            dist2 = b - e;
        }
        std::cout << std::setw(width * dist1) << ch1;
        std::cout << std::setw(width * dist2) << ch2;
        std::cout << std::endl;
    }

    // implemented
    void Deque::reallocate(int new_capacity)
    {

        // clean - dtor uses this
        if (new_capacity == 0)
        {
            delete[] array;
            array = nullptr;
            b = 0;
            e = 0;
            size = 0;
            capacity = 0;
        }
        else
        {

            // do not check whether new_capacity makes sense etc - it is caller's responsibility
            int *new_array = new int[new_capacity];
            for (int i = 0; i < size; ++i)
            {
                new_array[i] = array[(b + i) % capacity]; // new b is 0
            }

            // for (int i=size;i<capacity;++i) new_array[i]=0;

            delete[] array;

            array = new_array;

            b = 0;
            e = size;
            // same size
            capacity = new_capacity;
        }
    }

    // My code

    Deque &Deque::operator=(Deque rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        std::copy(rhs.array, rhs.array + rhs.size, array);
        this->b = rhs.b % rhs.capacity;
        this->e = rhs.e % rhs.capacity;
        this->size = rhs.size;
        this->capacity = rhs.capacity;

        return *this;
    }

    Deque::~Deque()
    {
        delete[] array;
    }

    void Deque::Clear()
    {
        while (!this->Empty())
        {
            this->Pop_back();
        }
    }

    void Deque::swap(Deque &other)
    {

        int tempb = other.b;
        int tempe = other.e;
        int tempsize = other.size;
        int tempcapacity = other.capacity;
        int *temparray = other.array;

        other.b = this->b;
        other.e = this->e;
        other.size = this->size;
        other.capacity = this->capacity;
        other.array = this->array;

        this->b = tempb;
        this->e = tempe;
        this->size = tempsize;
        this->capacity = tempcapacity;
        this->array = temparray;
    }

    Deque Deque::operator+(const Deque &rhs) const
    {
        Deque newDeque(this->array, this->size);

        while (newDeque.capacity < this->size + rhs.size)
        {
            newDeque.reallocate(newDeque.capacity ? newDeque.capacity * 2 : 1);
        }

        for (int i = 0; i < rhs.size; i++)
        {
            newDeque.Push_back(rhs.array[(b + i) % rhs.capacity]);
        }

        // newDeque.Print();
        return newDeque;
    }

    Deque &Deque::reverse()
    {
        Deque tempDeque(*this);

        for (int i = 0; i < this->size; i++)
        {
            this->array[(b + i) % capacity] = tempDeque.array[((e - 1) - i) % capacity];
        }

        return *this;
    }

    Deque Deque::operator~() const
    {
        Deque newDeque(*this);

        newDeque.reverse();

        return newDeque;
    }

    int &Deque::operator[](unsigned int pos)
    {

        return this->array[(b + pos) % capacity];
    }

    int Deque::operator[](unsigned int pos) const
    {
        // printf("%d",(b+capacity - 1 + pos)%capacity);
        return this->array[(b + pos) % capacity];
    }

    // implemented
    std::ostream &operator<<(std::ostream &os, const Deque &d)
    {
        for (int i = 0; i < d.Size(); ++i)
            os << d[i] << " "; // this is public Deque::operator[], operator<< does not need to be friend
        return os;
    }
}
