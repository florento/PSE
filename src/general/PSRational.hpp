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

namespace pse {

/// class of rational numbers
class PSRatio
{
public:
    PSRatio() : num(0), den(1) {}

    /// default constructor
    PSRatio(long n, long d = 1);
    
    /// copy constructor
    PSRatio(const PSRatio& rhs) : num(rhs.num), den(rhs.den) {}
    
    ~PSRatio(void) {}
    
    long numerator(void) const { return num; }
    long denominator(void) const { return den; }
    
    bool null(void) const { return (num == 0); }
    bool integral(void) const { return (den == 1L); }
    
    /// assignment operator
    PSRatio& operator=(const PSRatio& rhs);

    /// assignment operator
    PSRatio& operator=(long rhs);
    
    // unary operators
    PSRatio operator+(void) const { return *this; }
    PSRatio operator-(void) const { return PSRatio(-num, den); }
    PSRatio invert(void) const { return PSRatio(den, num); }
    
    // binary shortcut operators
    const PSRatio& operator+=(const PSRatio& rhs);
    const PSRatio& operator-=(const PSRatio& rhs);
    const PSRatio& operator*=(const PSRatio& rhs);
    const PSRatio& operator/=(const PSRatio& rhs);
    const PSRatio& operator+=(long rhs);
    const PSRatio& operator-=(long rhs);
    const PSRatio& operator*=(long rhs);
    const PSRatio& operator/=(long rhs);
    
    // increment/decrement iterators
    const PSRatio& operator++();
    const PSRatio operator++(int);
    const PSRatio& operator--();
    const PSRatio operator--(int);
    
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
PSRatio toRational(double x, int iterations = 5);

/// Rational -> double conversion
double toDouble(const PSRatio& r);

/// Rational -> long conversion
long trunc(const PSRatio& r);

/// Rational -> long conversion
long floor(const PSRatio& r);

/// Rational -> long conversion
long ceil(const PSRatio& r);

/// logarithm base 2 rounded to long
/// @param r must be strictly positive.
long log2floor(const PSRatio& r);

/// logarithm base 2 rounded to long
/// @param r must be strictly positive.
long log2ceil(const PSRatio& r);

/// power of 2
PSRatio pow2Rational(long n);

/// binary operators
const PSRatio operator+(const PSRatio& lhs, const PSRatio& rhs);
const PSRatio operator-(const PSRatio& lhs, const PSRatio& rhs);
const PSRatio operator*(const PSRatio& lhs, const PSRatio& rhs);
const PSRatio operator/(const PSRatio& lhs, const PSRatio& rhs);
PSRatio rabs(const PSRatio& rhs);

/// boolean operators
bool operator==(const PSRatio& lhs, const PSRatio& rhs);
bool operator!=(const PSRatio& lhs, const PSRatio& rhs);
bool operator<=(const PSRatio& lhs, const PSRatio& rhs);
bool operator>=(const PSRatio& lhs, const PSRatio& rhs);
bool operator<(const PSRatio& lhs, const PSRatio& rhs);
bool operator>(const PSRatio& lhs, const PSRatio& rhs);

// output operator
std::ostream& operator<< (std::ostream& ostr, const PSRatio& r);
std::istream& operator>> (std::istream& istr, PSRatio& r);

} // end namespace pse

// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
// See: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::PSRatio> : fmt::ostream_formatter {};

namespace std
{
    template<>
    class hash<pse::PSRatio>
    {
    public:
        std::size_t operator()(const pse::PSRatio& x) const
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

