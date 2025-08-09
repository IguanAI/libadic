#ifndef LIBADIC_PADIC_LOG_H
#define LIBADIC_PADIC_LOG_H

#include "libadic/qp.h"
#include <stdexcept>

namespace libadic {

class PadicLog {
private:
    static bool check_convergence_condition(const Qp& x) {
        if (x.is_zero()) {
            return false;
        }
        
        long p = x.get_prime();
        Qp one(p, x.get_precision(), 1);
        Qp diff = x - one;
        
        if (p == 2) {
            return diff.valuation() >= 2;
        } else {
            return diff.valuation() >= 1;
        }
    }
    
    static long compute_required_terms(long /*p*/, long precision, long u_valuation) {
        if (u_valuation <= 0) {
            return precision * 2;
        }
        
        long terms = precision / u_valuation + 10;
        return std::min(terms, precision * 3L);
    }
    
public:
    /**
     * Compute the p-adic logarithm using the standard Taylor series
     * log(1+u) = u - u²/2 + u³/3 - u⁴/4 + ...
     * 
     * PRECISION NOTE: When terms n are divisible by p, precision is lost
     * mathematically. This is a fundamental property of p-adic arithmetic,
     * not a bug. We use higher working precision internally to compensate,
     * but some precision loss is unavoidable and mathematically correct.
     * 
     * @param x A p-adic number with valuation 0 and x ≡ 1 (mod p)
     * @return The p-adic logarithm of x
     */
    static Qp log(const Qp& x) {
        if (x.is_zero()) {
            throw std::domain_error("Logarithm of zero is undefined");
        }
        
        if (x.valuation() != 0) {
            throw std::domain_error("p-adic logarithm requires valuation 0");
        }
        
        long p = x.get_prime();
        long N = x.get_precision();
        
        // Use higher working precision to compensate for precision loss
        // Add extra precision based on how many times we'll divide by p
        long p_divides_count = 0;
        for (long n = p; n <= N * 2; n *= p) {
            p_divides_count++;
        }
        long working_precision = N + p_divides_count + 5; // Add buffer for precision loss
        
        Qp one(p, working_precision, 1);
        Qp u = x.with_precision(working_precision) - one;
        
        if (!check_convergence_condition(x)) {
            throw std::domain_error("p-adic logarithm does not converge: x must be ≡ 1 (mod p)");
        }
        
        // For log(1+p), u = p which has valuation 1
        long u_val = u.valuation();
        long terms = compute_required_terms(p, working_precision, u_val);
        
        // Start with first term directly instead of zero
        if (terms < 1) {
            return Qp(p, N, 0);
        }
        
        // Use working precision for computation
        Qp result = u;  // First term: u^1/1 = u
        Qp u_power = u * u;  // Start with u^2
        
        for (long n = 2; n <= terms; ++n) {
            Qp divisor(p, working_precision, n);
            Qp term = u_power / divisor;
            
            if (term.valuation() >= working_precision) {
                break;
            }
            
            if (n % 2 == 1) {
                result = result + term;
            } else {
                result = result - term;
            }
            
            u_power = u_power * u;
            
            if (u_power.valuation() >= working_precision) {
                break;
            }
        }
        
        // Return result with requested precision
        return result.with_precision(N);
    }
    
    static Qp log_unit(const Zp& x) {
        if (!x.is_unit()) {
            throw std::domain_error("log_unit requires a unit in Zp");
        }
        
        long p = x.get_prime();
        long N = x.get_precision();
        
        Zp one(p, N, 1);
        Zp congruent_check = x.with_precision(1);
        
        if (congruent_check != one.with_precision(1)) {
            throw std::domain_error("Unit must be ≡ 1 (mod p) for log to converge");
        }
        
        return log(Qp(x));
    }
    
    static Qp log_via_exp_inverse(const Qp& x, long iterations = 10) {
        if (x.is_zero() || x.valuation() != 0) {
            throw std::domain_error("Invalid input for log via exp inverse");
        }
        
        long p = x.get_prime();
        long N = x.get_precision();
        
        Qp one(p, N, 1);
        Qp y = x - one;
        
        if (!check_convergence_condition(x)) {
            throw std::domain_error("Does not satisfy convergence condition");
        }
        
        Qp result = y;
        
        for (long k = 0; k < iterations; ++k) {
            Qp exp_result = exp_truncated(result, N);
            Qp correction = (x - exp_result) / exp_result;
            
            if (correction.valuation() >= N) {
                break;
            }
            
            result += correction;
        }
        
        return result;
    }
    
    static Qp log_product(const Qp& x, const Qp& y) {
        if (x.valuation() != 0 || y.valuation() != 0) {
            throw std::domain_error("Both inputs must have valuation 0");
        }
        
        return log(x) + log(y);
    }
    
    static bool verify_log_properties(const Qp& x, long tolerance_val) {
        if (x.is_zero() || x.valuation() != 0) {
            return false;
        }
        
        try {
            Qp log_x = log(x);
            Qp exp_log_x = exp_truncated(log_x, x.get_precision());
            
            Qp diff = exp_log_x - x;
            return diff.valuation() >= tolerance_val;
            
        } catch (const std::exception&) {
            return false;
        }
    }
    
private:
    static Qp exp_truncated(const Qp& x, long precision) {
        long p = x.get_prime();
        
        if (x.valuation() < 1) {
            throw std::domain_error("exp requires positive valuation for convergence");
        }
        
        Qp result(p, precision, 1);
        Qp x_power = x;
        BigInt factorial(1);
        
        for (long n = 1; n <= precision * 2; ++n) {
            factorial *= BigInt(n);
            Qp term = x_power / Qp(p, precision, factorial);
            
            if (term.valuation() >= precision) {
                break;
            }
            
            result += term;
            x_power *= x;
        }
        
        return result;
    }
};

inline Qp log_p(const Qp& x) {
    return PadicLog::log(x);
}

inline Qp log_p(const Zp& x) {
    return PadicLog::log_unit(x);
}

} // namespace libadic

#endif // LIBADIC_PADIC_LOG_H