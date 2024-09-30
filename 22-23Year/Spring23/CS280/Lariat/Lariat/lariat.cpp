#include <iostream>
#include <iomanip>
#include "lariat.h"

#define DebugMode 0
#define DebugThrows 0

#if 1
template <typename T, int Size>
std::ostream& operator<<(std::ostream& os, Lariat<T, Size> const& list)
{
    typename Lariat<T, Size>::LNode* current = list.head_;
    int index = 0;
    while (current)
    {
        os << "Node starting (count " << current->count << ")\n";
        for (int local_index = 0; local_index < current->count; ++local_index)
        {
            os << index << " -> " << current->values[local_index] << std::endl;
            ++index;
        }
        os << "-----------\n";
        current = current->next;
    }
    return os;
}
#else // fancier
#endif

/////////////////////////////Lariat Functions//////////////////////////////////////////////

template <typename T, int Size>
Lariat<T, Size>::Lariat()
    : head_(nullptr), tail_(nullptr), size_(0), nodecount_(0), asize_(Size)
{
    try
    {
        head_ = new LNode;
    }
    catch (...)
    {
        throw LariatException(LariatException::E_NO_MEMORY, "Constructor: failed to allocate new node");
    }

    tail_ = head_;
    head_->prev = nullptr;
    head_->next = nullptr;

    nodecount_++;
}

template <typename T, int Size>
Lariat<T, Size>::Lariat(Lariat const& rhs)
    : head_(nullptr), tail_(nullptr), size_(0), nodecount_(0), asize_(rhs.asize_)
{
    try
    {
        head_ = new LNode;
    }
    catch (...)
    {
        throw LariatException(LariatException::E_NO_MEMORY, "Copy Constructor: failed to allocate new node");
    }

    tail_ = head_;
    head_->next = nullptr;
    head_->prev = nullptr;
    nodecount_++;
    for (int i = 0; i < rhs.size_; i++)
    {
        push_back(rhs[i]);
    }
}

template <typename T, int Size>
template <typename U, int Size2>
Lariat<T, Size>::Lariat(Lariat<U, Size2> const& rhs)
    : head_(nullptr), tail_(nullptr), size_(0), nodecount_(0), asize_(Size)
{
    try
    {
        head_ = new LNode;
    }
    catch (...)
    {
        throw LariatException(LariatException::E_NO_MEMORY, "Copy Constructor: failed to allocate new node");
    }

    tail_ = head_;
    head_->next = nullptr;
    head_->prev = nullptr;
    nodecount_++;
    for (int i = 0; i < rhs.size_; i++)
    {
        push_back(static_cast<T>(rhs[i]));
    }
}

template <typename T, int Size>
Lariat<T, Size>::~Lariat()
{
    clear();
}

template <typename T, int Size>
Lariat<T, Size>& Lariat<T, Size>::operator=(Lariat const& rhs)
{
    clear();
    size_ = 0;
    try
    {
        head_ = new LNode;
    }
    catch (...)
    {
        throw LariatException(LariatException::E_NO_MEMORY, "Operator=: failed to allocate new node");
    }
    tail_ = head_;
    head_->next = nullptr;
    head_->prev = nullptr;
    nodecount_ = 1;
    asize_ = rhs.asize_;

    for (int i = 0; i < rhs.size_; i++)
    {
        push_back(rhs[i]);
    }

    return *this;
}

template <typename T, int Size>
template <typename U, int Size2>
Lariat<T, Size>& Lariat<T, Size>::operator=(Lariat<U, Size2> const& rhs)
{
    clear();
    size_ = 0;
    try
    {
        head_ = new LNode;
    }
    catch (...)
    {
        throw LariatException(LariatException::E_NO_MEMORY, "Templated Operator=: failed to allocate new node");
    }
    tail_ = head_;
    head_->next = nullptr;
    head_->prev = nullptr;
    nodecount_ = 1;
    asize_ = Size;

    for (int i = 0; i < rhs.size_; i++)
    {
        push_back(static_cast<T>(rhs[i]));
    }

    return *this;
}

template <typename T, int Size>
void Lariat<T, Size>::insert(int index, const T& value)
{
    if (index > size_ || index < 0)
    {
#if DebugThrows
        std::cout << "Index is: " << index << " Size is: " << size_ << std::endl;
#endif
        throw LariatException(LariatException::E_BAD_INDEX, "Subscript is out of range");
    }

    if (!head_)
    {
        try
        {
            head_ = new LNode;
        }
        catch (...)
        {
            throw LariatException(LariatException::E_NO_MEMORY, "Insert: failed to allocate new node");
        }
        tail_ = head_;
        head_->next = nullptr;
        head_->prev = nullptr;
        nodecount_ = 1;
    }

    if (index == 0)
    {
#if DebugMode
        std::cout << "Front" << std::endl;
#endif
        push_front(value);
    }
    else if (index == size_)
    {
#if DebugMode
        std::cout << "Back" << std::endl;
#endif
        push_back(value);
    }
    else
    {
#if DebugMode
        std::cout << "Shift" << std::endl;
#endif
        nodePair local = FindElement(index);
        T overflow = ShiftRight(local.first, local.second);
        local.first->values[local.second] = value;
        if (local.first->count == asize_)
        {
#if DebugMode
            std::cout << "Split" << std::endl;
#endif
            Split(local.first, &overflow);
            local.first->count--;
        }

        local.first->count++;
        size_++;
    }
}

template <typename T, int Size>
void Lariat<T, Size>::clear()
{
    LNode* cur = head_;

    while (cur)
    {
        cur = head_->next;
        delete head_;
        head_ = cur;
    }

    size_ = 0;
    nodecount_ = 0;
    head_ = nullptr;
    tail_ = nullptr;
}

template <typename T, int Size>
size_t Lariat<T, Size>::size(void) const
{
    return size_;
}

template <typename T, int Size>
unsigned Lariat<T, Size>::find(const T& value) const
{
    LNode* cur = head_;
    unsigned index = 0;
    while (cur)
    {
        for (int i = 0; i < cur->count; i++)
        {
            if (cur->values[i] == value)
            {
                return index;
            }
            index++;
        }

        cur = cur->next;
    }

    return size_;
}

template <typename T, int Size>
T& Lariat<T, Size>::first()
{
    return head_->values[0];
}

template <typename T, int Size>
T const& Lariat<T, Size>::first() const
{
    return head_->values[0];
}

template <typename T, int Size>
T& Lariat<T, Size>::last()
{
    return tail_->values[tail_->count - 1];
}

template <typename T, int Size>
T const& Lariat<T, Size>::last() const
{
    return tail_->values[tail_->count - 1];
}

template <typename T, int Size>
T& Lariat<T, Size>::operator[](int index)
{
    nodePair pair = FindElement(index);
    LNode* node = pair.first;
    return node->values[pair.second];
}

template <typename T, int Size>
const T& Lariat<T, Size>::operator[](int index) const
{
    nodePair pair = FindElement(index);
    LNode* node = pair.first;
    return node->values[pair.second];
}

template <typename T, int Size>
void Lariat<T, Size>::pop_back()
{
    if (tail_)
    {
        tail_->count--;
        size_--;

        if (tail_->count == 0)
        {
            if (tail_ == head_)
            {
                delete tail_;
                tail_ = nullptr;
                head_ = nullptr;

            }
            else
            {
                tail_ = tail_->prev;
                delete tail_->next;
            }
            
        }
    }
    
}

template <typename T, int Size>
void Lariat<T, Size>::pop_front()
{
    ShiftLeft(head_, 0);
    if (head_->count == 0)
    {
        LNode* cur = head_;
        head_ = head_->next;
        head_->prev = nullptr;
        delete cur;
    }
    head_->count--;
    size_--;
}

template <typename T, int Size>
void Lariat<T, Size>::erase(int index)
{
    if (index > size_ || index < 0)
    {
        throw LariatException(LariatException::E_BAD_INDEX, "Erase: subscript is out of range");
    }
    else if (index == 0)
    {
        pop_front();
    }
    else if (index == size_ - 1)
    {
        pop_back();
    }
    else
    {
        nodePair pair = FindElement(index);
        LNode* node = pair.first;

        ShiftLeft(node, pair.second);
        node->count--;
        size_--;
    }
}

template <typename T, int Size>
void Lariat<T, Size>::push_back(const T& value)
{
    if (!tail_)
    {
        try
        {
            head_ = new LNode;
        }
        catch (...)
        {
            throw LariatException(LariatException::E_NO_MEMORY, "PushBack: failed to allocate new node");
        }
        tail_ = head_;
        head_->next = nullptr;
        head_->prev = nullptr;
        nodecount_ = 1;
    }

    if (tail_->count == asize_)
    {
        Split(tail_, nullptr);
    }

    tail_->values[tail_->count] = value;
    tail_->count++;
    size_++;
}

template <typename T, int Size>
void Lariat<T, Size>::push_front(const T& value)
{
    if (!head_)
    {
        try
        {
            head_ = new LNode;
        }
        catch (...)
        {
            throw LariatException(LariatException::E_NO_MEMORY, "PushFront: failed to allocate new node");
        }
        tail_ = head_;
        head_->next = nullptr;
        head_->prev = nullptr;
        nodecount_ = 1;
    }

    if (head_->count == 0)
    {
        head_->count++;
    }
    else if (head_->count == asize_)
    {
        T overflow = ShiftRight(head_, 0);
        Split(head_, &overflow);
        if (head_ == tail_)
        {
            tail_ = head_->next;
        }
    }
    else if (head_->count < asize_)
    {
        ShiftRight(head_, 0);
        head_->count++;
    }

    head_->values[0] = value;
    size_++;
}

template <typename T, int Size>
void Lariat<T, Size>::compact()
{
    if (head_ && head_->next)
    {
        LNode* leftFoot = head_, * rightFoot = head_->next;

        while (rightFoot && leftFoot)
        {
            if (leftFoot->count == asize_ || !rightFoot->next)
            {
                leftFoot = rightFoot;
                rightFoot = rightFoot->next;
                continue;
            }

            int rCount = rightFoot->count;
            rightFoot->count = 0;

            for (int i = 0; i < rCount; i++)
            {
                leftFoot->values[leftFoot->count++] = rightFoot->values[i];
                if (leftFoot->count == asize_)
                {
                    leftFoot = leftFoot->next;
                    if (leftFoot == nullptr)
                    {
                        break;
                    }
                }
            }

            rightFoot = rightFoot->next;
        }
    }

    while (tail_ && tail_->count <= 0)
    {
        if (tail_ == head_)
        {
            delete head_;
            tail_ = nullptr;
            head_ = nullptr;
            break;
        }

        tail_ = tail_->prev;
        delete tail_->next;
        tail_->next = nullptr;
    }
}

///////////////////////////////////Helper Functions///////////////////////////////////

template <typename T, int Size>
std::pair<typename Lariat<T, Size>::LNode*, int> Lariat<T, Size>::FindElement(int index) const
{
    LNode* cur = head_, * prev = nullptr;
    int totalCount = 0;

    while (cur)
    {

        totalCount += cur->count;
        if (totalCount > index)
        {
            break;
        }
        prev = cur;
        cur = cur->next;
    }

    if (!cur)
    {
        return std::make_pair(prev, prev->count - (totalCount - index));
    }
    else
    {
        return std::make_pair(cur, cur->count - (totalCount - index));
    }
}

template <typename T, int Size>
T& Lariat<T, Size>::ShiftLeft(LNode* node, int index)
{
    if (index >= asize_ || index < 0)
    {
        throw LariatException(LariatException::E_BAD_INDEX, "ShiftLeft: subscript is out of range");
    }

    if (node)
    {
        for (int i = index; i < node->count - 1; i++)
        {
            std::swap(node->values[i], node->values[i + 1]);
#if DebugMode
            std::cout << "Shifting Left:" << std::endl;
            std::cout << *this << std::endl;
#endif
        }
    }

    return node->values[node->count - 1];
}

template <typename T, int Size>
T& Lariat<T, Size>::ShiftRight(LNode* node, int index)
{

    if (index >= asize_ || index < 0)
    {
        throw LariatException(LariatException::E_BAD_INDEX, "ShiftRight: subscript is out of range");
    }

    T* overflow = nullptr;

    if (node)
    {
        for (int i = node->count - 1; i > index; i--)
        {
            std::swap(node->values[i], node->values[i - 1]);
#if DebugMode
            std::cout << "Shifting Right:" << std::endl;
            std::cout << *this << std::endl;
#endif
        }

        overflow = &node->values[index];
    }

    if (node->count == asize_)
    {
        return *overflow;
    }
    else
    {
        node->values[node->count] = *overflow;
        return *overflow;
    }
}

template <typename T, int Size>
void Lariat<T, Size>::Split(LNode* node, T* overflow)
{
    LNode* newNode;
    try
    {
        newNode = new LNode;
    }
    catch (...)
    {
        throw LariatException(LariatException::E_NO_MEMORY, "Split: failed to allocate new node");
    }

    newNode->next = node->next;
    node->next = newNode;
    newNode->prev = node;
    if (newNode->next != nullptr)
    {
        newNode->next->prev = newNode;
    }
    else
    {
        tail_ = newNode;
    }

    int index = (asize_ / 2) + 1;
    int i = 0;

    for (; i < node->count - index; i++)
    {
        newNode->values[i] = node->values[index + i];
    }

    if (overflow)
    {
        newNode->values[i++] = *overflow;
    }

    newNode->count = i;
    node->count = index;
    nodecount_++;

#if DebugMode
    std::cout << "After Split:" << std::endl;
    std::cout << *this << std::endl;
#endif
}
