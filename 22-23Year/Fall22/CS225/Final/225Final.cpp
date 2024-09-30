#include <iostream>
#include <vector>
#include <random>


//Problem 1
class Polynomial 
{
    public: 
        Polynomial()
        :degree(0), coefficients(new int[1])
        {

        }
        Polynomial(int setDegree)
        :degree(setDegree), coefficients(new int[setDegree + 1])
        {

        }

        int& operator[](const int index)
        {
            return coefficients[index];
        }

        const int& operator[](const int index) const
        {
            return coefficients[index];
        }

        const int operator()(const int &x)
        {
            int sum = 0;

            for (int i = 0; i < degree + 1; i++)
            {
                sum += (*this)[i] * Pow(x, i);
            }
            
            return sum;
        }

        Polynomial(const Polynomial &rhs)
        :degree(rhs.degree), coefficients(new int[degree + 1])
        {
            for (int i = 0; i < degree + 1; i++)
            {
                coefficients[i] = rhs[i];
            }
            
        }

        Polynomial& operator=(const Polynomial &rhs)
        {
            degree = rhs.degree;

            for (int i = 0; i < degree + 1; i++)
            {
                coefficients[i] = rhs[i];
            }
            
            return *this;
        }

    private:
    int degree;
    int * coefficients;

    int Pow(int x, int exp)
    {
        int value = x;

        if (exp == 0)
        {
            return 1;
        }

        for (int i = 0; i < exp - 1; i++)
        {
            x *= value;
        }
        
        return x;
    }
};

///Problem 2

class BoundedPolynomial : public Polynomial
{
    public:
    BoundedPolynomial(int setDegree, int setBound)
    :p(setDegree), bound(setBound)
    {

    }

    BoundedPolynomial(const BoundedPolynomial &rhs)
    :p(rhs.p), bound(rhs.bound)
    {

    }

    BoundedPolynomial &operator=(const BoundedPolynomial &rhs)
    {
        p = rhs.p;
        bound = rhs.bound;

        return *this;
    }

    int& operator[](const int index)
    {
        return p[index];
    }

    const int& operator[](const int index) const
    {
        return p[index];
    }

    void Print() const
    {
        p.Print();
    }

    int operator()(const int x)
    {
        return p(x) > bound ? bound : p(x);
    }

    private:

    Polynomial p;
    int bound;
};

//Problem 3:


//Problem 4:

class SSA
{
    int * data;
    int size;
    public:
    SSA(int *b, int *e)
    :data(new int[e-b]), size(e-b)
    {
        for (int i = 0; i < size; i++)
        {
            data[i] = *b; 
            ++b;
        }
        std::sort(data, data+size);
        
    }

    int& operator[] (int pos)
    {
        std::sort(data, data+size);
        return data[pos];
    }
};

//Problem 5:

class Power5
{
private:
    int power;
public:
    Power5(int setPower)
    :power(setPower)
    {

    }

    int operator*(Power5 power)
    {

    }
};




//Problem 6:

template <typename T>
T power(T base, int n)
{
    T sum = base;
    if (n == 0)
    {
        return 1;
    }
    
    for (int i = 0; i < n - 1; i++)
    {
        sum *= base;
    }

    return sum;
    
}

//Problem 7:

template <typename U, int V>
class Power
{
private:
    
public:
    
    Power()
    {   
    }
    ~Power();

    U Get(const U base)
    {
        U sum = base;

        if (V == 0)
        {
            return 1;
        }

        for (int i = 0; i < V - 1; i++)
        {
            sum *= base;
        }
        
        return sum;
        
    }
};

//Problem 8:

int main()
{
    std::vector<int*> pointers(10);

    for (int i = 0; i < 10; i++)
    {
        pointers[i] = new int[20];
    }

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            pointers[i][j] = std::rand() % 100;
        }
        
    }

    struct PrintData
    {
        void operator()(int array[20]) const
        {
            for (int i = 0; i < 20; i++)
            {
                std::cout << array[i] << ", ";
            }
            std::cout << std::endl;
        }
    };

    std::for_each(pointers.begin(), pointers.end(), PrintData());

    struct SortData
    {
        bool operator()(int array[20], int array2[20])
        {
            int sum1 = 0, sum2 = 0;

            for (int i = 0; i < 20; i++)
            {
                sum1 += array[i];
                sum2 += array2[i];
            }

            return sum1 < sum2 ? true : false;
            
        }
    };
    
    std::sort(pointers.begin(), pointers.end(), SortData());

    for (int i = 0; i < 10; i++)
    {
        delete[] pointers[i];
    }
    

    return 0;
    
};


//Problem 9:

template< class InputIt, class InputIt2, class InputIt3, class OutputIt, class TrinaryOperation >
OutputIt transform(InputIt begin1, InputIt end1, InputIt2 begin2, InputIt3 begin3, OutputIt out, TrinaryOperation)
{
    while (begin1 != end1)
    {
        *out++ = TrinaryOperation(*begin1++, *begin2++, *begin3++);


        return out;
    }
    
}


/*
C needs to implement operator= and operator*=
*/
//Help stuff
/*
const T Polynomial<T, N>::operator()(const V x)
{
    T returnValue = 0;

    for (int i = 0; i < N + 1; i++)
    {

        returnValue += (*this)[i] * Pow(x, i);
    }

    return returnValue;
}

template <typename T, int N>
T Polynomial<T, N>::Pow(T value, int exp)
{
    T factor = value;

    if (exp == 0)
    {
        return 1;
    }

    for (int i = 0; i < exp - 1; i++)
    {
        value *= factor;
    }

    return value;
}
*/