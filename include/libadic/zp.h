#ifndef LIBADIC_ZP_H
#define LIBADIC_ZP_H

#include "libadic/gmp_wrapper.h"
#include "libadic/modular_arith.h"
#include <stdexcept>
#include <vector>
#include <algorithm>

namespace libadic {

class Zp {
private:
    long prime;
    long precision;
    BigInt value;
    
    void validate_prime() const {
        if (prime < 2) {
            throw std::invalid_argument("Prime must be >= 2");
        }
    }
    
    void validate_precision() const {
        if (precision < 1) {
            throw std::invalid_argument("Precision must be >= 1");
        }
    }
    
    void normalize() {
        BigInt p_power = BigInt(prime).pow(precision);
        value = value % p_power;
        if (value.is_negative()) {
            value += p_power;
        }
    }
    
public:
    Zp() : prime(2), precision(1), value(0) {}
    
    Zp(long p, long N) : prime(p), precision(N), value(0) {
        validate_prime();
        validate_precision();
    }
    
    Zp(long p, long N, const BigInt& val) : prime(p), precision(N), value(val) {
        validate_prime();
        validate_precision();
        normalize();
    }
    
    Zp(long p, long N, long val) : prime(p), precision(N), value(val) {
        validate_prime();
        validate_precision();
        normalize();
    }
    
    Zp(const Zp& other) = default;
    Zp(Zp&& other) noexcept = default;
    Zp& operator=(const Zp& other) = default;
    Zp& operator=(Zp&& other) noexcept = default;
    
    long get_prime() const { return prime; }
    long get_precision() const { return precision; }
    const BigInt& get_value() const { return value; }
    
    Zp with_precision(long new_precision) const {
        if (new_precision < precision) {
            BigInt new_value = value % BigInt(prime).pow(new_precision);
            return Zp(prime, new_precision, new_value);
        } else {
            return Zp(prime, new_precision, value);
        }
    }
    
    Zp lift_precision(long new_precision) const {
        if (new_precision <= precision) {
            return *this;
        }
        return Zp(prime, new_precision, value);
    }
    
    Zp operator+(const Zp& other) const {
        if (prime != other.prime) {
            throw std::invalid_argument("Cannot add p-adic numbers with different primes");
        }
        long min_prec = std::min(precision, other.precision);
        BigInt p_power = BigInt(prime).pow(min_prec);
        BigInt sum = (value + other.value) % p_power;
        return Zp(prime, min_prec, sum);
    }
    
    Zp operator-(const Zp& other) const {
        if (prime != other.prime) {
            throw std::invalid_argument("Cannot subtract p-adic numbers with different primes");
        }
        long min_prec = std::min(precision, other.precision);
        BigInt p_power = BigInt(prime).pow(min_prec);
        BigInt diff = (value - other.value) % p_power;
        if (diff.is_negative()) {
            diff += p_power;
        }
        return Zp(prime, min_prec, diff);
    }
    
    Zp operator*(const Zp& other) const {
        if (prime != other.prime) {
            throw std::invalid_argument("Cannot multiply p-adic numbers with different primes");
        }
        long min_prec = std::min(precision, other.precision);
        BigInt p_power = BigInt(prime).pow(min_prec);
        BigInt prod = (value * other.value) % p_power;
        return Zp(prime, min_prec, prod);
    }
    
    Zp operator/(const Zp& other) const {
        if (prime != other.prime) {
            throw std::invalid_argument("Cannot divide p-adic numbers with different primes");
        }
        if (other.is_zero()) {
            throw std::domain_error("Division by zero");
        }
        if (other.value.is_divisible_by(BigInt(prime))) {
            throw std::domain_error("Cannot divide by non-unit in Zp");
        }
        long min_prec = std::min(precision, other.precision);
        BigInt p_power = BigInt(prime).pow(min_prec);
        BigInt inv = other.value.mod_inverse(p_power);
        BigInt result = (value * inv) % p_power;
        return Zp(prime, min_prec, result);
    }
    
    Zp operator-() const {
        BigInt p_power = BigInt(prime).pow(precision);
        BigInt neg = (p_power - value) % p_power;
        return Zp(prime, precision, neg);
    }
    
    Zp& operator+=(const Zp& other) {
        *this = *this + other;
        return *this;
    }
    
    Zp& operator-=(const Zp& other) {
        *this = *this - other;
        return *this;
    }
    
    Zp& operator*=(const Zp& other) {
        *this = *this * other;
        return *this;
    }
    
    Zp& operator/=(const Zp& other) {
        *this = *this / other;
        return *this;
    }
    
    bool operator==(const Zp& other) const {
        if (prime != other.prime) {
            return false;
        }
        long min_prec = std::min(precision, other.precision);
        BigInt p_power = BigInt(prime).pow(min_prec);
        return (value % p_power) == (other.value % p_power);
    }
    
    bool operator!=(const Zp& other) const {
        return !(*this == other);
    }
    
    bool is_zero() const {
        return value.is_zero();
    }
    
    bool is_one() const {
        return value.is_one();
    }
    
    bool is_unit() const {
        return !value.is_divisible_by(BigInt(prime));
    }
    
    long valuation() const {
        if (is_zero()) {
            return precision;
        }
        return p_adic_valuation(value, BigInt(prime));
    }
    
    Zp unit_part() const {
        if (is_zero()) {
            return *this;
        }
        long val = valuation();
        if (val == 0) {
            return *this;
        }
        BigInt unit = value / BigInt(prime).pow(val);
        return Zp(prime, precision - val, unit);
    }
    
    Zp pow(const BigInt& exp) const {
        BigInt p_power = BigInt(prime).pow(precision);
        BigInt result = value.pow_mod(exp, p_power);
        return Zp(prime, precision, result);
    }
    
    Zp pow(long exp) const {
        return pow(BigInt(exp));
    }
    
    Zp teichmuller() const {
        return Zp(prime, precision, teichmuller_character(value, BigInt(prime), precision));
    }
    
    Zp sqrt() const {
        if (!is_unit()) {
            throw std::domain_error("Square root only defined for units in Zp");
        }
        
        BigInt p(prime);
        BigInt p_power = p.pow(precision);
        
        if (prime == 2) {
            if ((value % BigInt(8)) != BigInt(1)) {
                throw std::domain_error("No square root exists (mod 8 condition)");
            }
        } else {
            BigInt legendre = value.pow_mod((p - BigInt(1)) / BigInt(2), p);
            if (legendre != BigInt(1)) {
                throw std::domain_error("No square root exists (not a quadratic residue)");
            }
        }
        
        BigInt root = value % p;
        if (prime == 2) {
            root = BigInt(1);
        } else {
            BigInt q = p - BigInt(1);
            BigInt s(0);
            while ((q % BigInt(2)).is_zero()) {
                q /= BigInt(2);
                s += BigInt(1);
            }
            
            BigInt z(2);
            while (z.pow_mod((p - BigInt(1)) / BigInt(2), p) != (p - BigInt(1))) {
                z += BigInt(1);
            }
            
            BigInt m = s;
            BigInt c = z.pow_mod(q, p);
            BigInt t = value.pow_mod(q, p);
            BigInt r = value.pow_mod((q + BigInt(1)) / BigInt(2), p);
            
            while (!t.is_one()) {
                BigInt i(1);
                BigInt t2 = (t * t) % p;
                while (t2 != BigInt(1)) {
                    t2 = (t2 * t2) % p;
                    i += BigInt(1);
                }
                
                BigInt b = c;
                for (BigInt j(0); j < m - i - BigInt(1); j += BigInt(1)) {
                    b = (b * b) % p;
                }
                
                m = i;
                c = (b * b) % p;
                t = (t * c) % p;
                r = (r * b) % p;
            }
            root = r;
        }
        
        for (long k = 1; k < precision; ++k) {
            BigInt pk = p.pow(k);
            BigInt pk1 = pk * p;
            BigInt f = (root * root - value) % pk1;
            if (!f.is_zero()) {
                BigInt correction = (f / pk) * (BigInt(2) * root).mod_inverse(p);
                root = (root - correction * pk) % pk1;
                if (root.is_negative()) {
                    root += pk1;
                }
            }
        }
        
        return Zp(prime, precision, root);
    }
    
    std::string to_string() const {
        return value.to_string() + " (mod " + std::to_string(prime) + 
               "^" + std::to_string(precision) + ")";
    }
    
    BigInt to_bigint() const {
        return value;
    }
    
    long to_long() const {
        return value.to_long();
    }
    
    std::vector<long> p_adic_digits() const {
        std::vector<long> digits;
        BigInt temp = value;
        BigInt p(prime);
        
        for (long i = 0; i < precision; ++i) {
            digits.push_back((temp % p).to_long());
            temp /= p;
        }
        
        return digits;
    }
    
    static Zp from_rational(long numerator, long denominator, long p, long precision) {
        if (denominator == 0) {
            throw std::domain_error("Denominator cannot be zero");
        }
        
        BigInt num(numerator);
        BigInt den(denominator);
        BigInt prime_big(p);
        
        while (den.is_divisible_by(prime_big)) {
            den /= prime_big;
        }
        
        BigInt p_power = prime_big.pow(precision);
        BigInt inv = den.mod_inverse(p_power);
        BigInt result = (num * inv) % p_power;
        
        return Zp(p, precision, result);
    }
};

inline std::ostream& operator<<(std::ostream& os, const Zp& z) {
    os << z.to_string();
    return os;
}

} // namespace libadic

#endif // LIBADIC_ZP_H