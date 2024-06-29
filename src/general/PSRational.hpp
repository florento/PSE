//
//  PSRational.hpp
//
//

#ifndef PSRational_hpp
#define PSRational_hpp

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <math.h>

#include "trace.hpp"
// #include "hash.hpp"

#if defined(__GNUC__) && (__GNUC__ < 3) && !defined(__SPSL__)
#include <limits.h>

template <class T> class numeric_limits
{
public:
    static T min() {return T();}
    static T max() {return T();}
};

template<> class numeric_limits <char>
{
public:
    static long min() { return CHAR_MIN; }
    static long max() { return CHAR_MAX; }
};

template<> class numeric_limits <short>
{
public:
    static long min() { return SHRT_MIN; }
    static long max() { return SHRT_MAX; }
};

template<> class numeric_limits <int>
{
public:
    static long min() { return INT_MIN; }
    static long max() { return INT_MAX; }
};

template<> class numeric_limits <long>
{
public:
    static long min() { return LONG_MIN; }
    static long max() { return LONG_MAX; }
};

template<> class numeric_limits <long long>
{
public:
    static long long min() {
        return -1-(long long)((~(unsigned long long)0)>>1); }
    static long long max() {
        return (long long)((~(unsigned long long)0)>>1); }
};

#else
#include <limits>
using std::numeric_limits;
#endif

/// class of rational numbers
class Rational
{
public:
    Rational() : num(0), den(1) {}

    /// default constructor
    Rational(long n, long d = 1);
    
    /// copy constructor
    Rational(const Rational& rhs) : num(rhs.num), den(rhs.den) {}
    
    ~Rational(void) {}
    
    long numerator(void) const { return num; }
    long denominator(void) const { return den; }
    
    bool null(void) const { return (num == 0); }
    bool integral(void) const { return (den == 1L); }
    
    /// assignment operator
    Rational& operator=(const Rational& rhs);

    /// assignment operator
    Rational& operator=(long rhs);
    
    // unary operators
    Rational operator+(void) const { return *this; }
    Rational operator-(void) const { return Rational(-num, den); }
    Rational invert(void) const { return Rational(den, num); }
    
    // binary shortcut operators
    const Rational& operator+=(const Rational& rhs);
    const Rational& operator-=(const Rational& rhs);
    const Rational& operator*=(const Rational& rhs);
    const Rational& operator/=(const Rational& rhs);
    const Rational& operator+=(long rhs);
    const Rational& operator-=(long rhs);
    const Rational& operator*=(long rhs);
    const Rational& operator/=(long rhs);
    
    // increment/decrement iterators
    const Rational& operator++();
    const Rational operator++(int);
    const Rational& operator--();
    const Rational operator--(int);
    
    // -- better implemented as explicit conversion
    // -- function toDouble (see below)
    // operator double(void) const { return double(num)/den; }

    /// auxillary helper function to normalize the rationals
    static long gcd(long u, long v);
    static long lcm(long u, long v);
    
    /// print in format int+rat
    std::ostream& printint(std::ostream& o) const;

    std::string to_string() const;
    
private:
    // Data

    /// numerator
    long num;

    /// denominator (keep > 0!)
    long den;
    
};

/// double -> Rational conversion
Rational toRational(double x, int iterations = 5);

/// Rational -> double conversion
double toDouble(const Rational& r);

/// Rational -> long conversion
long trunc(const Rational& r);

/// Rational -> long conversion
long floor(const Rational& r);

/// Rational -> long conversion
long ceil(const Rational& r);

/// logarithm base 2 rounded to long
/// @param r must be strictly positive.
long log2floor(const Rational& r);

/// logarithm base 2 rounded to long
/// @param r must be strictly positive.
long log2ceil(const Rational& r);

/// power of 2
Rational pow2Rational(long n);

/// binary operators
const Rational operator+(const Rational& lhs, const Rational& rhs);
const Rational operator-(const Rational& lhs, const Rational& rhs);
const Rational operator*(const Rational& lhs, const Rational& rhs);
const Rational operator/(const Rational& lhs, const Rational& rhs);
Rational rabs(const Rational& rhs);

/// boolean operators
bool operator==(const Rational& lhs, const Rational& rhs);
bool operator!=(const Rational& lhs, const Rational& rhs);
bool operator<=(const Rational& lhs, const Rational& rhs);
bool operator>=(const Rational& lhs, const Rational& rhs);
bool operator<(const Rational& lhs, const Rational& rhs);
bool operator>(const Rational& lhs, const Rational& rhs);

// output operator
std::ostream& operator<< (std::ostream& ostr, const Rational& r);
std::istream& operator>> (std::istream& istr, Rational& r);

// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
// See: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<Rational> : fmt::ostream_formatter {};

namespace std
{
    template<>
    class hash<Rational>
    {
    public:
        std::size_t operator()(const Rational& x) const
        {
            using std::hash;
         
            // return std::hash<long>()(x.numerator()) ^ std::hash<long>()(x.denominator());

            // see also hash.hpp
            // 32 bit prime, see http://www.isthe.com/chongo/tech/comp/fnv/
            std::size_t h = 2166136261;
            h = (h * 16777619) ^ hash<long>()(x.numerator());
            h = (h * 16777619) ^ hash<long>()(x.denominator());
            return h;
        }
    };
}


#endif /* PSRational_hpp */

