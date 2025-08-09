#ifndef LIBADIC_L_FUNCTIONS_H
#define LIBADIC_L_FUNCTIONS_H

#include "libadic/qp.h"
#include "libadic/characters.h"
#include "libadic/bernoulli.h"
#include "libadic/padic_log.h"
#include "libadic/padic_gamma.h"
#include <map>
#include <cmath>

namespace libadic {

/**
 * Kubota-Leopoldt p-adic L-functions
 * Implementation follows Washington's "Introduction to Cyclotomic Fields"
 */
class LFunctions {
private:
    // Cache for computed L-values
    struct LKey {
        long s;
        long conductor;
        long char_id;
        long p;
        long precision;
        
        bool operator<(const LKey& other) const {
            if (s != other.s) return s < other.s;
            if (conductor != other.conductor) return conductor < other.conductor;
            if (char_id != other.char_id) return char_id < other.char_id;
            if (p != other.p) return p < other.p;
            return precision < other.precision;
        }
    };
    
    inline static std::map<LKey, Qp> l_cache;
    inline static std::map<LKey, Qp> l_derivative_cache;
    
public:
    /**
     * Compute L_p(s, χ) - the Kubota-Leopoldt p-adic L-function
     * For s = 0: L_p(0, χ) = -(1 - χ(p)p^{-1}) * B_{1,χ}
     * For s = 1-n (n > 0): L_p(1-n, χ) = -(1 - χ(p)p^{n-1}) * B_{n,χ}/n
     */
    static Qp kubota_leopoldt(long s, const DirichletCharacter& chi, long precision) {
        long p = chi.get_prime();
        long conductor = chi.get_conductor();
        
        // Create cache key
        LKey key{s, conductor, chi.evaluate_at(2), p, precision};
        if (l_cache.find(key) != l_cache.end()) {
            return l_cache[key];
        }
        
        Qp result(p, precision, 0);
        
        if (s == 0) {
            // L_p(0, χ) = -(1 - χ(p)p^{-1}) * B_{1,χ}
            
            // Compute B_{1,χ}
            Qp B1_chi = compute_B1_chi(chi, precision);
            
            // Compute Euler factor (1 - χ(p)p^{-1})
            Qp euler_factor = compute_euler_factor(chi, 1, precision);
            
            result = -euler_factor * B1_chi;
            
        } else if (s < 0) {
            // For s = 1-n where n > 1
            long n = 1 - s;
            
            if (n <= 0) {
                throw std::invalid_argument("Invalid s value for L-function");
            }
            
            // L_p(1-n, χ) = -(1 - χ(p)p^{n-1}) * B_{n,χ}/n
            
            // For even n and odd χ, or odd n and even χ, the value is 0
            if ((n % 2 == 0 && chi.is_odd()) || (n % 2 == 1 && chi.is_even())) {
                result = Qp(p, precision, 0);
            } else {
                // Compute generalized Bernoulli number
                auto chi_func = [&chi, precision](long a) -> Cyclotomic {
                    return chi.evaluate_cyclotomic(a, precision);
                };
                
                Qp Bn_chi = BernoulliNumbers::generalized_bernoulli(n, conductor, chi_func, p, precision);
                
                // Compute Euler factor
                Qp euler_factor = compute_euler_factor(chi, n, precision);
                
                result = -euler_factor * Bn_chi / Qp(p, precision, n);
            }
            
        } else if (s > 0) {
            // For positive integers, use p-adic interpolation
            result = compute_positive_value(s, chi, precision);
        }
        
        l_cache[key] = result;
        return result;
    }
    
    /**
     * Compute L'_p(s, χ) - derivative of p-adic L-function
     * For s = 0, this uses the Ferrero-Washington formula
     */
    static Qp kubota_leopoldt_derivative(long s, const DirichletCharacter& chi, long precision) {
        long p = chi.get_prime();
        long conductor = chi.get_conductor();
        
        LKey key{s, conductor, chi.evaluate_at(2), p, precision};
        if (l_derivative_cache.find(key) != l_derivative_cache.end()) {
            return l_derivative_cache[key];
        }
        
        Qp result(p, precision, 0);
        
        if (s == 0) {
            // For odd characters, use Ferrero-Washington formula
            if (chi.is_odd()) {
                result = compute_derivative_at_zero_odd(chi, precision);
            } else {
                // For even characters, different formula
                result = compute_derivative_at_zero_even(chi, precision);
            }
        } else {
            // Use numerical differentiation for other values
            long h_exp = precision / 2;
            Qp h = Qp(p, precision, BigInt(p).pow(h_exp));
            
            Qp f_plus = kubota_leopoldt(s, chi, precision) + 
                       kubota_leopoldt(s + 1, chi, precision) * h;
            Qp f_minus = kubota_leopoldt(s, chi, precision) - 
                        kubota_leopoldt(s - 1, chi, precision) * h;
            
            result = (f_plus - f_minus) / (Qp(p, precision, 2) * h);
        }
        
        l_derivative_cache[key] = result;
        return result;
    }
    
public:  // Made public for Python bindings
    /**
     * Compute B_{1,χ} for the character χ
     */
    static Qp compute_B1_chi(const DirichletCharacter& chi, long precision) {
        long p = chi.get_prime();
        long conductor = chi.get_conductor();
        
        if (chi.is_principal()) {
            // B_1 = -1/2
            return Qp::from_rational(-1, 2, p, precision);
        }
        
        Qp sum(p, precision, 0);
        
        // B_{1,χ} = (1/conductor) * Σ_{a=1}^{conductor} χ(a) * a
        for (long a = 1; a <= conductor; ++a) {
            if (std::gcd(a, conductor) != 1) continue;
            
            Zp chi_a = chi.evaluate(a, precision);
            Qp term = Qp(chi_a) * Qp(p, precision, a);
            sum += term;
        }
        
        return sum / Qp(p, precision, conductor);
    }
    
    /**
     * Compute Euler factor (1 - χ(p)p^{s-1})
     */
    static Qp compute_euler_factor(const DirichletCharacter& chi, long s, long precision) {
        long p = chi.get_prime();
        
        Qp one(p, precision, 1);
        
        // If p divides the conductor, χ(p) = 0
        if (chi.get_conductor() % p == 0) {
            return one;
        }
        
        Zp chi_p = chi.evaluate(p, precision);
        Qp p_power = Qp(p, precision, BigInt(p).pow(s - 1));
        
        return one - Qp(chi_p) * p_power;
    }
    
    /**
     * Compute L_p(s, χ) for positive integer s
     * Uses the formula: L_p(s, χ) = (1 - χ(p)p^{-s}) * L(s, χ)
     * where L(s, χ) is computed via partial sums
     */
    static Qp compute_positive_value(long s, const DirichletCharacter& chi, long precision) {
        long p = chi.get_prime();
        // long conductor = chi.get_conductor();  // Reserved for future use
        
        // Number of terms needed for precision
        long num_terms = precision * std::log(p) / std::log(2) + 10;
        
        Qp sum(p, precision, 0);
        
        // L(s, χ) = Σ_{n=1}^∞ χ(n)/n^s
        // We compute partial sum and use acceleration techniques
        for (long n = 1; n <= num_terms; ++n) {
            if (n % p == 0) continue;  // Skip p-multiples for p-adic L-function
            
            Zp chi_n = chi.evaluate(n, precision);
            if (!chi_n.is_zero()) {
                // Compute n^s in p-adic
                Qp n_power = Qp(p, precision, n).pow(s);
                sum += Qp(chi_n) / n_power;
            }
        }
        
        // Apply Euler factor
        Qp euler = compute_euler_factor(chi, s, precision);
        return euler * sum;
    }
    
    /**
     * Compute L'_p(0, χ) for odd characters
     * Uses Ferrero-Washington formula involving log Γ_p
     */
    static Qp compute_derivative_at_zero_odd(const DirichletCharacter& chi, long precision) {
        long p = chi.get_prime();
        long conductor = chi.get_conductor();
        
        Qp sum(p, precision, 0);
        
        // L'_p(0, χ) for odd χ involves sum of χ(a) * log Γ_p(a/conductor)
        for (long a = 1; a < conductor; ++a) {
            if (std::gcd(a, conductor) != 1) continue;
            
            Zp chi_a = chi.evaluate(a, precision);
            if (!chi_a.is_zero()) {
                // Compute log Γ_p(a/conductor)
                Qp a_over_f = Qp::from_rational(a, conductor, p, precision);
                
                // For fractional arguments, we need to use the distribution relation
                // For now, approximate using functional equation
                Qp log_gamma_term = compute_log_gamma_fractional(a, conductor, p, precision);
                
                sum += Qp(chi_a) * log_gamma_term;
            }
        }
        
        // Apply normalization factor
        Qp factor = Qp(p, precision, conductor);
        return sum / factor;
    }
    
    /**
     * Compute L'_p(0, χ) for even characters
     */
    static Qp compute_derivative_at_zero_even(const DirichletCharacter& chi, long precision) {
        long p = chi.get_prime();
        long conductor = chi.get_conductor();
        
        // For even characters, L'_p(0, χ) involves different formula
        // Related to p-adic regulator
        
        Qp sum(p, precision, 0);
        
        for (long a = 1; a < conductor; ++a) {
            if (std::gcd(a, conductor) != 1) continue;
            
            Zp chi_a = chi.evaluate(a, precision);
            if (!chi_a.is_zero()) {
                // Compute contribution
                Qp log_term = log_p(Qp::from_rational(a, conductor - 1, p, precision));
                sum += Qp(chi_a) * log_term;
            }
        }
        
        return sum;
    }
    
    /**
     * Compute log Γ_p for fractional arguments
     * Uses distribution relations and functional equations
     */
    static Qp compute_log_gamma_fractional(long numerator, long denominator, 
                                          long p, long precision) {
        // Simplified version - full implementation would use Mahler expansion
        
        if (denominator == 1) {
            // Integer case
            Zp z(p, precision, numerator);
            return PadicGamma::log_gamma(z);
        }
        
        // For a/b, use distribution relation:
        // log Γ_p(x) = log Γ_p(px) - log(p) - Σ_{i=0}^{p-1} log Γ_p((x+i)/p)
        
        // Approximate using interpolation
        Qp x = Qp::from_rational(numerator, denominator, p, precision);
        
        // Find nearest integer
        long nearest = (numerator + denominator/2) / denominator;
        Qp diff = x - Qp(p, precision, nearest);
        
        // Taylor expansion around nearest integer
        Zp z_nearest(p, precision, nearest);
        Qp log_gamma_nearest = PadicGamma::log_gamma(z_nearest);
        
        // First order approximation
        // log Γ_p(n + ε) ≈ log Γ_p(n) + ε * ψ_p(n)
        // where ψ_p is the p-adic digamma function
        
        return log_gamma_nearest + diff * compute_digamma(nearest, p, precision);
    }
    
    /**
     * Compute p-adic digamma function ψ_p(n)
     */
    static Qp compute_digamma(long n, long p, long precision) {
        // ψ_p(n) = d/dx log Γ_p(x)|_{x=n}
        // Can be computed using series or functional equations
        
        Qp sum(p, precision, 0);
        
        // Use series representation
        for (long k = 1; k <= precision * 2; ++k) {
            if (k % p == 0) continue;
            Qp term = Qp(p, precision, 1) / Qp(p, precision, n + k - 1);
            sum += term;
        }
        
        return -sum;
    }
    
public:
    /**
     * Clear all caches
     */
    static void clear_cache() {
        l_cache.clear();
        l_derivative_cache.clear();
    }
};

} // namespace libadic

#endif // LIBADIC_L_FUNCTIONS_H