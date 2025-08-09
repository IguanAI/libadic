#ifndef LIBADIC_CHARACTERS_H
#define LIBADIC_CHARACTERS_H

#include "libadic/zp.h"
#include "libadic/cyclotomic.h"
#include <vector>
#include <map>
#include <numeric>
#include <functional>

namespace libadic {

/**
 * Dirichlet character modulo n
 * A completely multiplicative function χ: (Z/nZ)* → C*
 * For p-adic computations, we use Teichmüller lifts
 */
class DirichletCharacter {
private:
    long conductor;
    long modulus;
    long prime;
    std::vector<long> generators;  // Generators of (Z/nZ)*
    std::vector<long> generator_orders;  // Orders of generators
    std::vector<long> character_values;  // Values on generators
    mutable std::map<long, Cyclotomic> value_cache;  // Cache computed values
    
    /**
     * Find generators of (Z/nZ)*
     */
    void compute_generators() {
        generators.clear();
        generator_orders.clear();
        
        if (modulus == 1) {
            return;
        }
        
        // Factor modulus
        std::vector<std::pair<long, long>> factorization;
        long n = modulus;
        
        // Check for powers of 2
        long pow2 = 0;
        while (n % 2 == 0) {
            pow2++;
            n /= 2;
        }
        if (pow2 > 0) {
            factorization.push_back({2, pow2});
        }
        
        // Check odd primes
        for (long p = 3; p * p <= n; p += 2) {
            long pow = 0;
            while (n % p == 0) {
                pow++;
                n /= p;
            }
            if (pow > 0) {
                factorization.push_back({p, pow});
            }
        }
        if (n > 1) {
            factorization.push_back({n, 1});
        }
        
        // For each prime power, find a generator
        for (const auto& [p, k] : factorization) {
            long pk = 1;
            for (long i = 0; i < k; ++i) pk *= p;
            
            if (p == 2 && k >= 3) {
                // (Z/2^kZ)* ≅ Z/2 × Z/2^(k-2)
                generators.push_back(-1);  // Generator of Z/2
                generator_orders.push_back(2);
                
                generators.push_back(3);   // Generator of Z/2^(k-2)
                generator_orders.push_back(pk / 4);
            } else if (p == 2 && k == 2) {
                // (Z/4Z)* ≅ Z/2
                generators.push_back(-1);
                generator_orders.push_back(2);
            } else if (p == 2 && k == 1) {
                // (Z/2Z)* is trivial
                continue;
            } else {
                // (Z/p^kZ)* is cyclic of order p^(k-1)(p-1)
                long order = pk - pk / p;
                
                // Find a primitive root mod p
                long g = 2;
                while (true) {
                    bool is_primitive = true;
                    
                    // Check if g generates (Z/pZ)*
                    for (long d = 2; d * d <= p - 1; ++d) {
                        if ((p - 1) % d == 0) {
                            if (pow_mod(g, d, p) == 1 || pow_mod(g, (p - 1) / d, p) == 1) {
                                is_primitive = false;
                                break;
                            }
                        }
                    }
                    
                    if (is_primitive) {
                        // Lift to generator mod p^k
                        if (k > 1 && pow_mod(g, p - 1, p * p) == 1) {
                            g += p;  // Adjust if needed
                        }
                        break;
                    }
                    g++;
                }
                
                generators.push_back(g);
                generator_orders.push_back(order);
            }
        }
    }
    
    static long pow_mod(long base, long exp, long mod) {
        long result = 1;
        base %= mod;
        while (exp > 0) {
            if (exp % 2 == 1) {
                result = (result * base) % mod;
            }
            base = (base * base) % mod;
            exp /= 2;
        }
        return result;
    }
    
    /**
     * Express a mod modulus in terms of generators
     */
    std::vector<long> express_in_generators(long a) const {
        std::vector<long> exponents(generators.size(), 0);
        
        if (std::gcd(a, modulus) != 1) {
            return exponents;  // Not in (Z/nZ)*
        }
        
        // For each generator, find the exponent
        // This is a simplified version; full implementation would use discrete log
        for (size_t i = 0; i < generators.size(); ++i) {
            long g = generators[i];
            long order = generator_orders[i];
            
            // Find e such that g^e ≡ a (mod modulus) if possible
            for (long e = 0; e < order; ++e) {
                if (pow_mod(g, e, modulus) == (a % modulus + modulus) % modulus) {
                    exponents[i] = e;
                    break;
                }
            }
        }
        
        return exponents;
    }
    
public:
    DirichletCharacter(long mod, long p) : modulus(mod), prime(p), conductor(mod) {
        compute_generators();
        character_values.resize(generators.size(), 0);
    }
    
    /**
     * Create a character from its values on generators
     */
    DirichletCharacter(long mod, long p, const std::vector<long>& gen_values) 
        : modulus(mod), prime(p), conductor(mod), character_values(gen_values) {
        compute_generators();
        if (character_values.size() != generators.size()) {
            throw std::invalid_argument("Wrong number of generator values");
        }
        compute_conductor();
    }
    
    /**
     * Compute the conductor (smallest modulus for which χ is primitive)
     */
    void compute_conductor() {
        conductor = modulus;
        
        // Check all divisors of modulus
        for (long d = 1; d < modulus; ++d) {
            if (modulus % d == 0) {
                bool factors_through_d = true;
                
                for (long a = 1; a <= modulus; ++a) {
                    if (std::gcd(a, modulus) != 1) continue;
                    if (std::gcd(a, d) != 1) continue;
                    
                    // Check if χ(a) = χ(a mod d)
                    long a_mod_d = a % d;
                    if (evaluate_at(a) != evaluate_at(a_mod_d)) {
                        factors_through_d = false;
                        break;
                    }
                }
                
                if (factors_through_d) {
                    conductor = d;
                    break;
                }
            }
        }
    }
    
    long get_conductor() const { return conductor; }
    long get_modulus() const { return modulus; }
    long get_prime() const { return prime; }
    
    /**
     * Evaluate character at n
     * Returns value as element of Z/ord(χ)Z
     */
    long evaluate_at(long n) const {
        if (std::gcd(n, modulus) != 1) {
            return 0;
        }
        
        n = (n % modulus + modulus) % modulus;
        
        // Express n in terms of generators
        std::vector<long> exps = express_in_generators(n);
        
        // Compute χ(n) = Π χ(g_i)^{e_i}
        long result = 1;
        long order = 1;
        
        for (size_t i = 0; i < generators.size(); ++i) {
            if (character_values[i] != 0) {
                order = std::lcm(order, generator_orders[i]);
                result = (result * pow_mod(character_values[i], exps[i], order)) % order;
            }
        }
        
        return result;
    }
    
    /**
     * Evaluate character and lift to p-adic number using Teichmüller lift
     */
    Zp evaluate(long n, long precision) const {
        long chi_n = evaluate_at(n);
        
        if (chi_n == 0) {
            return Zp(prime, precision, 0);
        }
        
        // Lift to p-adic using Teichmüller character
        // ω(chi_n) is the unique (p-1)-th root of unity congruent to chi_n mod p
        return Zp(prime, precision, chi_n).teichmuller();
    }
    
    /**
     * Evaluate as cyclotomic number
     */
    Cyclotomic evaluate_cyclotomic(long n, long precision) const {
        if (value_cache.find(n) != value_cache.end()) {
            return value_cache[n];
        }
        
        long chi_n = evaluate_at(n);
        
        if (chi_n == 0) {
            return Cyclotomic(prime, precision);
        }
        
        // Express as power of primitive root of unity
        long order = get_order();
        Cyclotomic zeta = Cyclotomic::zeta(prime, precision);
        
        // χ(n) = ζ^{chi_n * (p-1)/order}
        Cyclotomic result(prime, precision, Qp(prime, precision, 1));
        Cyclotomic zeta_power = zeta;
        
        long exponent = (chi_n * (prime - 1)) / order;
        for (long i = 0; i < exponent; ++i) {
            result = result * zeta_power;
        }
        
        value_cache[n] = result;
        return result;
    }
    
    /**
     * Check if character is even: χ(-1) = 1
     */
    bool is_even() const {
        return evaluate_at(-1) == 1;
    }
    
    /**
     * Check if character is odd: χ(-1) = -1
     */
    bool is_odd() const {
        return evaluate_at(-1) == modulus - 1;  // -1 mod modulus
    }
    
    /**
     * Check if character is primitive
     */
    bool is_primitive() const {
        return conductor == modulus;
    }
    
    /**
     * Check if character is principal (trivial)
     */
    bool is_principal() const {
        for (long val : character_values) {
            if (val != 1 && val != 0) return false;
        }
        return true;
    }
    
    /**
     * Get the order of the character
     */
    long get_order() const {
        if (is_principal()) return 1;
        
        long order = 1;
        for (size_t i = 0; i < character_values.size(); ++i) {
            if (character_values[i] != 0) {
                // Find order of character_values[i] in Z/generator_orders[i]Z
                for (long k = 1; k <= generator_orders[i]; ++k) {
                    if (pow_mod(character_values[i], k, generator_orders[i]) == 1) {
                        order = std::lcm(order, k);
                        break;
                    }
                }
            }
        }
        
        return order;
    }
    
    /**
     * Enumerate all Dirichlet characters modulo n
     */
    static std::vector<DirichletCharacter> enumerate_characters(long modulus, long prime) {
        std::vector<DirichletCharacter> characters;
        
        DirichletCharacter base(modulus, prime);
        size_t num_generators = base.generators.size();
        
        if (num_generators == 0) {
            // Only trivial character
            characters.push_back(base);
            return characters;
        }
        
        // Generate all possible combinations of values on generators
        std::vector<long> current_values(num_generators, 0);
        
        std::function<void(size_t)> generate = [&](size_t index) {
            if (index == num_generators) {
                characters.emplace_back(modulus, prime, current_values);
                return;
            }
            
            // Try all possible values for this generator
            for (long val = 0; val < base.generator_orders[index]; ++val) {
                current_values[index] = val;
                generate(index + 1);
            }
        };
        
        generate(0);
        return characters;
    }
    
    /**
     * Enumerate primitive characters only
     */
    static std::vector<DirichletCharacter> enumerate_primitive_characters(long modulus, long prime) {
        auto all_chars = enumerate_characters(modulus, prime);
        std::vector<DirichletCharacter> primitive;
        
        for (auto& chi : all_chars) {
            if (chi.is_primitive()) {
                primitive.push_back(chi);
            }
        }
        
        return primitive;
    }
    
    /**
     * Compute Gauss sum: g(χ) = Σ_{a mod n} χ(a) e^{2πia/n}
     * In p-adic setting, we use Teichmüller characters
     */
    Cyclotomic gauss_sum(long precision) const {
        Cyclotomic sum(prime, precision);
        Cyclotomic zeta = Cyclotomic::zeta(prime, precision);
        
        for (long a = 1; a <= modulus; ++a) {
            if (std::gcd(a, modulus) != 1) continue;
            
            Cyclotomic chi_a = evaluate_cyclotomic(a, precision);
            
            // Compute ζ^{a * (p-1)/modulus}
            Cyclotomic zeta_power(prime, precision, Qp(prime, precision, 1));
            for (long j = 0; j < (a * (prime - 1)) / modulus; ++j) {
                zeta_power = zeta_power * zeta;
            }
            
            sum = sum + chi_a * zeta_power;
        }
        
        return sum;
    }
    
    /**
     * L-function value L(s, χ) at integer s
     * For Reid-Li, we need s = 0
     */
    Qp L_value(long s, long precision) const {
        // This will be implemented in l_functions.h
        // Placeholder for now
        return Qp(prime, precision, 1);
    }
};

} // namespace libadic

#endif // LIBADIC_CHARACTERS_H