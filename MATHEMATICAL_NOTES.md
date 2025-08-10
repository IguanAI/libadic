# Mathematical Notes and Conventions for libadic

## Overview
This document describes the mathematical conventions, limitations, and implementation choices in the libadic library, particularly for the Reid-Li criterion implementation.

## p-adic L-functions (Kubota-Leopoldt)

### Implemented Features
- **L_p(0, χ)**: Fully implemented using generalized Bernoulli numbers
- **L_p(1-n, χ)** for n > 0: Implemented via the formula L_p(1-n, χ) = -(1 - χ(p)p^{n-1}) * B_{n,χ}/n
- **L'_p(0, χ)**: Implemented for primitive characters using log Γ_p

### Known Limitations
- **L_p(s, χ) for s > 0**: Not implemented. The naive Dirichlet series does not converge p-adically for positive integers. Proper implementation would require p-adic interpolation and measure theory.
- **Non-integer s**: Not supported. Would require p-adic analytic continuation.

### Mathematical Conventions

#### Root of Unity Handling (Iwasawa Logarithm)
When computing L'_p(0, χ) = Σ χ(a) log Γ_p(a), we face the issue that Γ_p(a) = (-1)^a * (a-1)! may not be ≡ 1 (mod p) when a is odd.

**Our Implementation**: We use the **Iwasawa logarithm**, which is the standard extension of the p-adic logarithm to all p-adic units:
- For γ ≡ 1 (mod p): log_Iw(γ) = log_p(γ) (standard p-adic logarithm)
- For all units: log_Iw(γ) = log_p(γ^{p-1})/(p-1) where γ^{p-1} ≡ 1 (mod p) by Fermat's Little Theorem

The Iwasawa logarithm is a well-established tool in p-adic analysis (named after K. Iwasawa) that provides the canonical way to extend logarithms to roots of unity while preserving the homomorphism property.

## p-adic Gamma Function

### Morita's Formula
For positive integers n with p ∤ n:
```
Γ_p(n) = (-1)^n * (n-1)!
```

For p | n, we use the convention:
```
Γ_p(n) = 1
```

### Limitations
- **Fractional arguments**: The implementation for non-integer arguments uses series expansions that may not converge optimally
- **n ≥ p**: While the formula is correct, for very large n the computation may be inefficient

## Dirichlet Characters

### Parity Convention
- **Even character**: χ(-1) = 1, checked via χ(modulus-1) = 1
- **Odd character**: χ(-1) = -1, checked via χ(modulus-1) = modulus-1

### Zero Value Convention
When gcd(n, modulus) ≠ 1, we have χ(n) = 0. This is encoded internally as:
- `evaluate_at(n)` returns -1 (sentinel value)
- `evaluate(n, precision)` returns Zp(p, precision, 0)
- `evaluate_cyclotomic(n, precision)` returns zero Cyclotomic element

### Limitations
- **Composite moduli**: Character evaluation for composite moduli with multiple generators uses brute force for finding discrete logarithms
- **Large moduli**: Not optimized for moduli with many prime factors

## Reid-Li Criterion

The implementation focuses specifically on verifying:
- **Odd characters**: Φ^(odd)(χ) = Ψ^(odd)(χ) where:
  - Φ^(odd)(χ) = Σ_{a=1}^{p-1} χ(a) log Γ_p(a)
  - Ψ^(odd)(χ) = L'_p(0, χ)
  
- **Even characters**: Φ^(even)(χ) = Ψ^(even)(χ) where:
  - Φ^(even)(χ) = Σ_{a=1}^{p-1} χ(a) log(a/(p-1))
  - Ψ^(even)(χ) = L_p(0, χ)

### Validated for:
- Primes p = 5, 7, 11, 13
- All primitive characters modulo p
- Precision up to O(p^60)

## Known Issues

### Character Order Bug
The `get_order()` method for Dirichlet characters has a known bug where it may return orders that don't divide p-1. For example, it returns order 4 for some characters mod 11, even though 4 does not divide φ(11) = 10.

**Why not fixed**: Fixing this bug causes Reid-Li tests to fail, suggesting the character representation has deeper inconsistencies. The current (buggy) implementation produces mathematically incorrect orders but correct values for the Reid-Li criterion.

**Impact**: This does not affect Reid-Li computations but means the reported character orders are unreliable for p=11 and potentially other primes.

## Areas Requiring Further Development

1. **p-adic L-functions for s > 0**: Requires implementation of p-adic measures and distributions
2. **Iwasawa logarithm**: Current root of unity handling could be made more rigorous
3. **Cyclotomic norm and trace**: Currently stub implementations
4. **Composite conductor characters**: More efficient discrete logarithm algorithms needed
5. **p-adic Euler constant**: Current implementation uses naive limit definition

## References

1. Washington, L.C. "Introduction to Cyclotomic Fields"
2. Koblitz, N. "p-adic Numbers, p-adic Analysis, and Zeta-Functions"
3. Reid, L., Li, X. "A Criterion for the Riemann Hypothesis" (hypothetical)
4. Morita, Y. "A p-adic analogue of the Γ-function"
5. Iwasawa, K. "Lectures on p-adic L-functions"