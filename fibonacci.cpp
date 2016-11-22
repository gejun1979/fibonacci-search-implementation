#include <iostream>
#include <algorithm>
#include <vector>
#include <time.h>
#include <sstream>

#define COMPARE_NUMBER_CALCULATE

#ifdef COMPARE_NUMBER_CALCULATE
int64_t compare = 0;
#endif

template<class ForwardIt, class T>
ForwardIt __upper_bound(ForwardIt first, ForwardIt last, const T& value)
{
    ForwardIt it;
    typename std::iterator_traits<ForwardIt>::difference_type count, step;
    count = std::distance(first, last);
    
    while (count > 0) {
        it = first;
        step = count / 2;
        std::advance(it, step);
#ifdef COMPARE_NUMBER_CALCULATE
        compare++;
#endif
        if (!(value < *it)) {
            first = ++it;
            count -= step + 1;
        }
        else count = step;
    }
    return first;
}

template<class ForwardIt, class T>
ForwardIt fibonacci_upper_bound(ForwardIt first, ForwardIt last, const T& value)
{
    typedef typename std::iterator_traits<ForwardIt>::difference_type diff_type;
    static diff_type fib[] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169, 63245986, 102334155, 165580141, 267914296, 433494437, 701408733, 1134903170, 1836311903 };
    
    diff_type len = std::distance(first, last);
    diff_type* index = std::upper_bound(fib, fib + sizeof(fib) / sizeof(diff_type), len);
    
    while (index >= fib) {
        ForwardIt mid = first + *(--index);
#ifdef COMPARE_NUMBER_CALCULATE
        compare++;
#endif
        if (!((mid >= last) || (value<*mid))) {
            first = ++mid;
            --index;
        }
    }
    
    return first;
}

template<class ForwardIt, class T>
ForwardIt lut_upper_bound(ForwardIt first, ForwardIt last, const T& value)
{
    typedef typename std::iterator_traits<ForwardIt>::difference_type diff_type;
    static diff_type lut[] = { 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824 };
    
    diff_type len = std::distance(first, last);
    diff_type* index = std::upper_bound(lut, lut + sizeof(lut) / sizeof(diff_type), len);
    
    while (index >= lut) {
        ForwardIt mid = first + *(--index);
#ifdef COMPARE_NUMBER_CALCULATE
        compare++;
#endif
        if (!((mid >= last) || (value<*mid))) {
            first = ++mid;
        }
    }
    
    return first;
}

class TestFramework
{
    std::vector<int> m_testVec;
    int m_testNumber;
    
public:
    template<class ForwardIt, class T>
    using testCallBack = ForwardIt (*)(ForwardIt first, ForwardIt last, const T& value);

    TestFramework(int num)
    :m_testNumber(num)
    {
    };
    
    void initTest()
    {
        for (int i = 0; i<m_testNumber; i+=2) {
            m_testVec.push_back(i);
        }
    };
    
    template<class ForwardIt, class T>
    void runPerfTest( testCallBack<ForwardIt,T> testFunction, std::string description )
    {
        time_t lTime1;
        time_t lTime2;

#ifdef COMPARE_NUMBER_CALCULATE
        compare = 0;
#endif
        time(&lTime1);
        for (int i = 0; i<m_testNumber; ++i) {
            testFunction(m_testVec.begin(), m_testVec.end(), i);
        }
        time(&lTime2);
        
        std::cout << "============ " << description.c_str() << " ============" << std::endl;
        std::cout << "Time cost is " << (lTime2 - lTime1) << "." << std::endl;
#ifdef COMPARE_NUMBER_CALCULATE
        std::cout << "Compare is " << compare << "." << std::endl;
#endif
    };
    
    template<class ForwardIt, class T>
    void runCorrectTest( testCallBack<ForwardIt,T> testFunction, std::string description )
    {
        std::cout << "============ " << description.c_str() << " ============" << std::endl;
        for (int i = -10; i<m_testNumber+10; ++i) {
            if (testFunction(m_testVec.begin(), m_testVec.end(), i) != std::upper_bound(m_testVec.begin(), m_testVec.end(), i)) {
                std::cout << "Failed correct check." << std::endl;
                return;
            }
        }

        std::cout << "Pass the correctness check." << std::endl;
    };
};

int main(void)
{
    TestFramework tester(30000000);
    tester.initTest();

    tester.runPerfTest(__upper_bound<std::vector<int>::iterator, int>, "__upper_bound");
    tester.runPerfTest(fibonacci_upper_bound<std::vector<int>::iterator, int>, "fibonacci_upper_bound");
    tester.runPerfTest(lut_upper_bound<std::vector<int>::iterator, int>, "lut_upper_bound");
    tester.runPerfTest(std::upper_bound<std::vector<int>::iterator, int>, "std::upper_bound");
    
    tester.runCorrectTest(__upper_bound<std::vector<int>::iterator, int>, "__upper_bound");
    tester.runCorrectTest(lut_upper_bound<std::vector<int>::iterator, int>, "lut_upper_bound");
    tester.runCorrectTest(fibonacci_upper_bound<std::vector<int>::iterator, int>, "fibonacci_upper_bound");

    return 0;
}

