"""
libadic: High-performance p-adic arithmetic library

A Python interface to the libadic C++ library for p-adic number theory
and the Reid-Li criterion for the Riemann Hypothesis.

Core Classes:
    - BigInt: Arbitrary precision integers
    - Zp: p-adic integers
    - Qp: p-adic numbers
    - DirichletCharacter: Dirichlet characters
    - CyclotomicField: Cyclotomic field extensions

Mathematical Functions:
    - log_p: p-adic logarithm
    - gamma_p: Morita's p-adic Gamma function
    - kubota_leopoldt: p-adic L-functions
    - bernoulli: Bernoulli numbers

Example:
    >>> from libadic import Zp, gamma_p
    >>> x = Zp(7, 20, 15)  # 15 in Z_7 with precision O(7^20)
    >>> g = gamma_p(5, 7, 20)  # Γ_7(5)
"""

__version__ = "1.0.0"
__author__ = "Reid-Li Team"

# Import the compiled extension module
try:
    from libadic import *  # This imports from the compiled .so/.pyd file
except ImportError as e:
    import warnings
    warnings.warn(
        f"Could not import compiled libadic extension: {e}\n"
        "Please ensure the library is properly built with: python setup.py build_ext --inplace",
        ImportWarning
    )

# High-level Python utilities
from typing import Union, Tuple, List, Optional
import numpy as np


class PadicContext:
    """
    Context manager for p-adic computations with specified precision.
    
    Example:
        >>> with PadicContext(prime=7, precision=50) as ctx:
        ...     x = ctx.Zp(15)
        ...     y = ctx.Qp_from_rational(2, 3)
        ...     result = x * y
    """
    
    def __init__(self, prime: int, precision: int):
        """
        Initialize p-adic context.
        
        Args:
            prime: The prime p
            precision: Default precision for computations
        """
        self.prime = prime
        self.precision = precision
        
    def __enter__(self):
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        pass
    
    def Zp(self, value: Union[int, str, 'BigInt'] = 0) -> 'Zp':
        """Create p-adic integer with context parameters."""
        if isinstance(value, str):
            return Zp(self.prime, self.precision, BigInt(value))
        elif isinstance(value, int):
            return Zp(self.prime, self.precision, value)
        else:
            return Zp(self.prime, self.precision, value)
    
    def Qp(self, value: Union[int, str, 'BigInt'] = 0) -> 'Qp':
        """Create p-adic number with context parameters."""
        if isinstance(value, str):
            return Qp(self.prime, self.precision, BigInt(value))
        elif isinstance(value, int):
            return Qp(self.prime, self.precision, value)
        else:
            return Qp(self.prime, self.precision, value)
    
    def Qp_from_rational(self, num: int, den: int) -> 'Qp':
        """Create p-adic number from rational."""
        return Qp.from_rational(num, den, self.prime, self.precision)


def validate_reid_li_all(prime: int, precision: int = 60) -> Tuple[bool, List[dict]]:
    """
    Validate Reid-Li criterion for all primitive characters modulo prime.
    
    Args:
        prime: Prime to test
        precision: p-adic precision for computations
        
    Returns:
        Tuple of (all_valid, results_list) where results_list contains
        details for each character tested
        
    Example:
        >>> valid, results = validate_reid_li_all(7, 60)
        >>> print(f"Reid-Li holds for p=7: {valid}")
    """
    results = []
    all_valid = True
    
    # Get all primitive characters
    characters = enumerate_primitive_characters(prime)
    
    for i, chi in enumerate(characters):
        # Verify Reid-Li for this character
        valid, phi, psi = verify_reid_li(chi, prime, precision)
        
        results.append({
            'character_index': i,
            'is_odd': chi.is_odd(),
            'valid': valid,
            'phi': phi,
            'psi': psi,
            'difference': phi - psi if not valid else None
        })
        
        if not valid:
            all_valid = False
    
    return all_valid, results


def p_adic_precision_bits(prime: int, precision: int) -> int:
    """
    Calculate number of bits needed for given p-adic precision.
    
    Args:
        prime: The prime p
        precision: Precision as O(p^precision)
        
    Returns:
        Number of bits required
        
    Example:
        >>> bits = p_adic_precision_bits(7, 60)
        >>> print(f"Need {bits} bits for O(7^60)")
    """
    import math
    return int(math.ceil(precision * math.log2(prime)))


def format_padic(x: Union['Zp', 'Qp'], max_terms: int = 10) -> str:
    """
    Format p-adic number for display with limited terms.
    
    Args:
        x: p-adic number to format
        max_terms: Maximum number of terms to show
        
    Returns:
        Formatted string representation
        
    Example:
        >>> x = Zp(7, 20, 123)
        >>> print(format_padic(x, max_terms=5))
    """
    if hasattr(x, 'digits'):
        digits = x.digits()[:max_terms]
        p = x.prime
        
        terms = []
        for i, d in enumerate(digits):
            if d != 0:
                if i == 0:
                    terms.append(str(d))
                elif i == 1:
                    terms.append(f"{d}*{p}")
                else:
                    terms.append(f"{d}*{p}^{i}")
        
        if len(digits) < x.precision:
            terms.append(f"O({p}^{x.precision})")
        
        return " + ".join(terms) if terms else "0"
    else:
        return str(x)


# Convenience functions for common operations
def sqrt_mod_p(a: int, p: int) -> Optional[int]:
    """
    Find square root of a modulo prime p if it exists.
    
    Args:
        a: Value to find square root of
        p: Prime modulus
        
    Returns:
        Square root if it exists, None otherwise
        
    Example:
        >>> r = sqrt_mod_p(2, 7)
        >>> if r: print(f"√2 ≡ {r} (mod 7)")
    """
    if not is_quadratic_residue(BigInt(a), BigInt(p)):
        return None
    
    # Use Tonelli-Shanks or similar algorithm
    # For now, use brute force for small primes
    for r in range(p):
        if (r * r) % p == a % p:
            return r
    
    return None


def factorial_mod_p(n: int, p: int, precision: int) -> 'Zp':
    """
    Compute n! in Z_p removing factors of p.
    
    Args:
        n: Value to compute factorial of
        p: Prime
        precision: p-adic precision
        
    Returns:
        n! with factors of p removed as element of Z_p
        
    Example:
        >>> f = factorial_mod_p(10, 7, 20)
        >>> print(f)
    """
    result = Zp(p, precision, 1)
    for i in range(2, n + 1):
        if i % p != 0:
            result = result * Zp(p, precision, i)
    return result


# Mathematical constants
def padic_pi(p: int, precision: int) -> 'Qp':
    """
    Compute p-adic analogue of π (if it exists).
    
    Note: The p-adic numbers don't have a direct analogue of π,
    but certain related constants exist in specific contexts.
    
    Args:
        p: Prime
        precision: Precision
        
    Returns:
        p-adic constant related to π
    """
    # This would need proper mathematical definition
    raise NotImplementedError("p-adic π requires specific mathematical context")


# Validation utilities
def check_convergence_log(x: Union['Zp', 'Qp']) -> bool:
    """
    Check if x satisfies convergence condition for p-adic logarithm.
    
    Args:
        x: p-adic number to check
        
    Returns:
        True if log_p(x) converges
        
    Example:
        >>> x = Zp(7, 20, 1 + 7)
        >>> print(check_convergence_log(x))  # True
    """
    p = x.prime
    
    if p == 2:
        # For p=2, need x ≡ 1 (mod 4)
        return (x.to_bigint() % 4) == 1
    else:
        # For p≠2, need x ≡ 1 (mod p)
        return (x.to_bigint() % p) == 1


# Export main classes and functions
__all__ = [
    # Core classes
    'BigInt',
    'Zp',
    'Qp',
    'DirichletCharacter',
    'CyclotomicField',
    'CyclotomicElement',
    
    # Mathematical functions
    'log_p',
    'log_unit',
    'gamma_p',
    'log_gamma_p',
    'kubota_leopoldt',
    'kubota_leopoldt_derivative',
    'bernoulli',
    'generalized_bernoulli',
    
    # Modular arithmetic
    'mod_inverse',
    'mod_pow',
    'chinese_remainder',
    'is_quadratic_residue',
    'legendre_symbol',
    'primitive_root',
    
    # Character functions
    'enumerate_characters',
    'enumerate_primitive_characters',
    'principal_character',
    'legendre_character',
    
    # Reid-Li validation
    'verify_reid_li',
    'compute_phi_odd',
    'compute_phi_even',
    'validate_reid_li_all',
    
    # Utilities
    'PadicContext',
    'format_padic',
    'p_adic_precision_bits',
    'sqrt_mod_p',
    'factorial_mod_p',
    'check_convergence_log',
]