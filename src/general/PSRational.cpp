//
//  PSRational.cpp
//  qparse
//
//  Created by Florent Jacquemard on 02/11/16.
//

#include "PSRational.hpp"

namespace pse {

// default constructor
PSRatio::PSRatio(long n, long d)
{
    if (d == 0L)
    {
        //std::cerr << "Division by Zero" << std::endl;
        ERROR("Division by Zero");
        exit(1);
    }
    if (d < 0L) { n = -n; d = -d; }
    if (n == 0L)
    {
        num = 0L;   den = 1L;
    }
    else
    {
        long g = gcd(n, d);
        num = n/g; den = d/g;
    }
}


PSRatio& PSRatio::operator=(const PSRatio& rhs)
{
    num = rhs.num;
    den = rhs.den;
    return *this;
}


PSRatio& PSRatio::operator=(long rhs)
{
    num = rhs;
    den = 1;
    return *this;
}


// binary shortcut operators
const PSRatio& PSRatio::operator+=(const PSRatio& rhs)
{
    long g1 = gcd(den, rhs.den);
    if (g1 == 1L)
    {  // 61% probability!
        num = num*rhs.den + den*rhs.num;
        den = den*rhs.den;
    }
    else
    {
        long t = num * (rhs.den/g1) + (den/g1)*rhs.num;
        long g2 = gcd(t, g1);
        num = t/g2;
        den = (den/g1) * (rhs.den/g2);
    }
    return *this;
}


const PSRatio& PSRatio::operator+=(long rhs)
{
    num = num + den*rhs;
    return *this;
}


const PSRatio& PSRatio::operator-=(const PSRatio& rhs)
{
    long g1 = gcd(den, rhs.den);
    if (g1 == 1L)
    {  // 61% probability!
        num = num*rhs.den - den*rhs.num;
        den = den*rhs.den;
    }
    else
    {
        long t = num * (rhs.den/g1) - (den/g1)*rhs.num;
        long g2 = gcd(t, g1);
        num = t/g2;
        den = (den/g1) * (rhs.den/g2);
    }
    return *this;
}


const PSRatio& PSRatio::operator-=(long rhs)
{
    num = num - den*rhs;
    return *this;
}


const PSRatio& PSRatio::operator*=(const PSRatio& rhs)
{
    long g1 = gcd(num, rhs.den);
    long g2 = gcd(den, rhs.num);
    num = (num/g1) * (rhs.num/g2);
    den = (den/g2) * (rhs.den/g1);
    return *this;
}


const PSRatio& PSRatio::operator*=(long rhs)
{
    long g = gcd(den, rhs);
    num *= rhs/g;
    den /= g;
    return *this;
}


const PSRatio& PSRatio::operator/=(const PSRatio& rhs)
{
    if (rhs == 0)
    {
        //std::cerr << "Division by Zero" << std::endl;
        ERROR("Division by Zero");
        exit(1);
    }
    long g1 = gcd(num, rhs.num);
    long g2 = gcd(den, rhs.den);
    num = (num/g1) * (rhs.den/g2);
    den = (den/g2) * (rhs.num/g1);
    if (den < 0L) { num = -num; den = -den; }
    return *this;
}


const PSRatio& PSRatio::operator/=(long rhs)
{
    if (rhs == 0L)
    {
        //std::cerr << "Division by Zero" << std::endl;
        ERROR("Division by Zero");
        exit(1);
    }
    long g = gcd(num, rhs);
    num /= g;
    den *= rhs/g;
    if (den < 0L) { num = -num; den = -den; }
    return *this;
}


// increment/decrement iterators
const PSRatio& PSRatio::operator++()
{
    return (*this += 1);
}


const PSRatio PSRatio::operator++(int)
{
    PSRatio oldVal = *this;
    ++(*this);
    return oldVal;
}


const PSRatio& PSRatio::operator--()
{
    return (*this -= 1);
}


const PSRatio PSRatio::operator--(int)
{
    PSRatio oldVal = *this;
    --(*this);
    return oldVal;
}


// greatest common divisor: // euclid's algorithm
// static
long PSRatio::gcd(long u, long v)
{
    long a = labs(u); // absolute value
    long b = labs(v);
    long tmp;
    
    if (b > a)
    {
        tmp = a; a = b; b = tmp;
    }
    
    for(;;)
    {
        if (b == 0L)
            return a;
        else if (b == 1L)
            return b;
        else
        {
            tmp = b; b = a % b; a = tmp;
        }
    }
}


//long gcd(long a, long b)
//{
//    if( b == 0 )
//    return a;
//    return gcd(b, a%b);
//}


// static
long PSRatio::lcm(long u, long v)
{
    long a = labs(u);
    long b = labs(v);

    return a*b / gcd(a,b);
}


// binary operators
const PSRatio operator+(const PSRatio& lhs, const PSRatio& rhs)
{
    return PSRatio(lhs) += rhs;
}


const PSRatio operator-(const PSRatio& lhs, const PSRatio& rhs)
{
    return PSRatio(lhs) -= rhs;
}


const PSRatio operator*(const PSRatio& lhs, const PSRatio& rhs)
{
    return PSRatio(lhs) *= rhs;
}


const PSRatio operator/(const PSRatio& lhs, const PSRatio& rhs)
{
    return PSRatio(lhs) /= rhs;
}


PSRatio rabs(const PSRatio& r)
{
    if (r.numerator() < 0)
        return -r;
    else
        return r;
}


// boolean operators
bool operator==(const PSRatio& lhs, const PSRatio& rhs)
{
    return (lhs.numerator() == rhs.numerator() &&
            lhs.denominator() == rhs.denominator());
}


bool operator!=(const PSRatio& lhs, const PSRatio& rhs)
{
    return (lhs.numerator() != rhs.numerator() ||
            lhs.denominator() != rhs.denominator());
}


bool operator<(const PSRatio& lhs, const PSRatio& rhs)
{
    return (toDouble(lhs) < toDouble(rhs));
}


bool operator>(const PSRatio& lhs, const PSRatio& rhs)
{
    return (toDouble(lhs) > toDouble(rhs));
}


bool operator<=(const PSRatio& lhs, const PSRatio& rhs)
{
    return ((lhs < rhs) || (lhs == rhs));
}


bool operator>=(const PSRatio& lhs, const PSRatio& rhs)
{
    return ((lhs > rhs) || (lhs == rhs));
}


std::ostream& operator<<(std::ostream& ostr, const PSRatio& r)
{
    if (r.denominator() == 1L)
        ostr << r.numerator();
    else
    {
        std::ostringstream buf;
        buf.flags(ostr.flags());         // copy stream flags
        buf.fill(ostr.fill());           // copy fill character
        
        buf << r.numerator() << "/" << r.denominator();
        
        ostr << buf.str();
    }

    return ostr;
}


std::istream& operator>>(std::istream& istr, PSRatio& r)
{
    // accepted format # and #/#
    // where # is integer number
    long n = 0, d = 1;
    
    istr >> n;
    if ( istr.peek() == '/' )
    {
        istr.ignore(1);
        istr >> d;
    }
    if ( istr ) r = PSRatio(n,d);

    return istr;
}


// double -> Rational conversion
static PSRatio toRational(double x, double limit, int iterations)
{
    double intpart;
    double fractpart = modf(x, &intpart);
    double d = 1.0 / fractpart;
    long left = long(intpart);

    if ( d > limit || iterations == 0 )
    {
        return PSRatio(left);
    }
    else
    {
        return PSRatio(left) + toRational(d, limit * 0.1, --iterations).invert();
    }
}


PSRatio toRational(double x, int iterations)
{
    if ( x == 0.0 ||
         x < numeric_limits<long>::min() ||
         x > numeric_limits<long>::max() )
    {
        return PSRatio(0,1);
    }
    else
    {
        int sign = x < 0.0 ? -1 : 1;
        return sign * toRational(sign * x, 1.0e12, iterations);
    }
}

double toDouble(const PSRatio& r)
{
    return double(r.numerator())/r.denominator();
}

long trunc(const PSRatio& r)
{
    return r.numerator() / r.denominator();
}

long floor(const PSRatio& r)
{
    long q = r.numerator() / r.denominator();
    return (r.numerator() < 0 && r.denominator() != 1) ? --q : q;
}

long ceil(const PSRatio& r)
{
    long q = r.numerator() / r.denominator();
    return (r.numerator() >= 0 && r.denominator() != 1) ? ++q : q;
}

PSRatio pow2Rational(long n)
{
    if (n >= 0)
        return PSRatio(((long) 1) << n, 1);
    else
        return PSRatio(1, ((long) 1) << (- n));    
}


long log2floor(const PSRatio& r)
{
    if (r <= PSRatio(0, 1))
    {
        ERROR("log2floor({}) undef", r);
        return LONG_MIN;
    }
    else if (r == PSRatio(1, 1))
    {
        return 0;
    }
    else if (PSRatio(1, 1) < r)
    {
        long c = 2;
        long ret = 0;
        while (PSRatio(c, 1) <= r)
        {
            c <<= 1;
            ++ret;
        }
        return ret;
    }
    else
    {
        assert (r < PSRatio(1, 1));
        long c = 1;
        long ret = 0;
        while (r < PSRatio(1, c))
        {
            c <<= 1;
            --ret;
        }
        return ret;
    }
}


long log2ceil(const PSRatio& r)
{
    if (r <= PSRatio(0, 1))
    {
        ERROR("log2ceil({}) undef", r);
        return LONG_MIN;
    }
    else if (r == PSRatio(1, 1))
    {
        return 0;
    }
    else if (r > PSRatio(1, 1))
    {
        long c = 1;
        long ret = 0;
        while (r > PSRatio(c, 1))
        {
            c <<= 1;
            ++ret;
        }
        return ret;
    }
    else
    {
        assert (r < PSRatio(1, 1));
        long c = 2;
        long ret = 0;
        while (r <= PSRatio(1, c))
        {
            c <<= 1;
            --ret;
        }
        return ret;
    }
}


std::ostream& PSRatio::printint(std::ostream& o) const
{
    long i = floor(*this);
    PSRatio rest = *this - PSRatio(i);

    if (i == 0)
    {
        o << *this;
    }
    else
    {
        o << i;
        if (rest != 0)
        {
            if (i > 0)
                o << "+";
            o << rest;
        }
    }
    
    return o;
}


std::string PSRatio::to_string() const
{
    std::stringstream s = std::stringstream();
    printint(s);
    return s.str();
}


} // end namespace pse
