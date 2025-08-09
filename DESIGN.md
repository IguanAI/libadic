Excellent choice. A detailed design document is the essential first step to ensure a robust, high-performance, and maintainable library. Let's create one.

---

## **Design Document: `libadic` - A High-Performance C++ Library for p-adic Arithmetic**

**Version:** 1.0
**Date:** October 26, 2023
**Author(s):** [Your Name/Team Name], AI Assistant

### **1. Introduction**

#### **1.1. Purpose and Scope**

This document outlines the design for `libadic`, a C++ library for performing high-precision arithmetic in p-adic number fields. The primary goal of this library is to provide the foundational computational tools necessary to implement and verify a new mathematical criterion for the Riemann Hypothesis, known as the Reid-Li Criterion.

The library will provide data structures and algorithms for computations involving p-adic integers, p-adic numbers, and their extensions. It will include implementations of fundamental p-adic special functions, such as the p-adic Gamma, Logarithm, and L-functions. The scope is limited to the `GL(1)/Q` case, meaning computations related to Dirichlet characters over the rational numbers.

#### **1.2. Target Audience**

The target audience for this document includes software developers, computational mathematicians, and number theorists involved in the project. It assumes a strong background in C++ but does not assume prior expertise in p-adic analysis.

#### **1.3. Goals and Objectives**

*   **Accuracy:** To be bit-for-bit accurate and mathematically rigorous. All precision must be tracked explicitly.
*   **Performance:** To be highly optimized for speed, suitable for large-scale computations.
*   **Modularity:** To be designed with a clean separation of components (base arithmetic, number fields, special functions) to facilitate testing and extension.
*   **Robustness:** To be bulletproof, with comprehensive unit tests and error handling.
*   **Documentation:** To be thoroughly documented at both the API and implementation level.

---

### **2. System Architecture**

The library will be structured in a layered architecture to ensure modularity and separation of concerns.

```
+------------------------------------------+
|          Application Layer               |
|  (Reid-Li Calculator, Test Programs)     |
+------------------------------------------+
|          High-Level API Layer            |
| (p-adic Gamma, L-functions, DFT)         |
+------------------------------------------+
|          Number Field Layer              |
|   (Q_p, Z_p, Q_p(zeta_p), Characters)    |
+------------------------------------------+
|        Base Arithmetic Layer             |
|  (GMP/MPFR Wrapper, Modular Arithmetic)  |
+------------------------------------------+
```

#### **2.1. External Dependencies**

*   **GMP (GNU Multiple Precision Arithmetic Library):** For all arbitrary-precision integer (`mpz_t`) arithmetic. This is the foundation for performance.
*   **MPFR (Multiple-Precision Floating-Point Reliably):** Can be used for complex-valued functions if needed, but the primary focus is p-adic.
*   **A modern C++ compiler (C++17 or newer):** For features like `std::optional`, `std::variant`, and templates.
*   **CMake:** For cross-platform build system management.
*   **Google Test (or similar):** For the unit testing framework.

---

### **3. Detailed Component Design**

#### **3.1. Layer 1: Base Arithmetic (`src/base/`)**

*   **`gmp_wrapper.h`:** A lightweight C++ wrapper around GMP's `mpz_t` to provide RAII (Resource Acquisition Is Initialization), operator overloading (`+`, `-`, `*`, `/`, `%`), and type safety. This is critical for avoiding manual memory management.
*   **`modular_arith.h`:** Functions for fast modular arithmetic, including modular inverse (`modInverse`) and modular exponentiation (`power`), built on top of the GMP wrapper.

#### **3.2. Layer 2: Number Field Layer (`src/fields/`)**

This layer defines the core mathematical objects.

##### **`class Zp` (p-adic Integers)**

*   **Description:** Represents an element of the ring of p-adic integers, `Z_p`.
*   **Internal Representation:**
    ```cpp
    class Zp {
    private:
        long prime;
        long precision; // The power of p, N, in O(p^N)
        BigInt coeffs; // A GMP-wrapped integer representing the sum: c0 + c1*p + ...
    };
    ```
*   **Key Methods:**
    *   Constructors: `Zp(long p, long N)`, `Zp(long p, long N, const BigInt& value)`.
    *   Arithmetic Operators: `+`, `-`, `*`.
    *   Division: `operator/(const Zp& other)`: Throws an exception if `other` is divisible by `p`. Computes the modular inverse and multiplies.
    *   `valuation()`: Returns 0.
    *   `unit_part()`: Returns `*this`.
    *   `teichmuller()`: Returns the Teichmüller representative `ω(this)`.

##### **`class Qp` (p-adic Numbers)**

*   **Description:** Represents an element of the field of p-adic numbers, `Q_p`.
*   **Internal Representation:**
    ```cpp
    class Qp {
    private:
        long prime;
        long precision; // Absolute precision N
        BigInt unit_part; // A Zp integer not divisible by p
        long valuation;  // The power of p
    };
    // x = unit_part * p^valuation
    ```
*   **Key Methods:**
    *   Constructors from `Zp`, or from unit part and valuation.
    *   All arithmetic operators. Division is now always defined (except by zero).
    *   `valuation()`: Returns the stored valuation.
    *   `unit_part()`: Returns the `Zp` unit part.

##### **`class Cyclotomic` (Elements of `Q_p(ζ)` )**

*   **Description:** Represents elements of the field extension needed for computations.
*   **Internal Representation:** Polynomial basis. An element is a polynomial in `ζ` with `Qp` coefficients.
    ```cpp
    class Cyclotomic {
    private:
        long prime;
        long precision;
        std::vector<Qp> coeffs; // c0 + c1*z + c2*z^2 + ...
    };
    // Where z is a root of the p-th cyclotomic polynomial
    ```
*   **Key Methods:**
    *   All arithmetic operators. Multiplication involves polynomial multiplication followed by reduction using the cyclotomic polynomial `Φ_p(x) = 1 + x + ... + x^(p-1) = 0`.
    *   `norm()`, `trace()`.

#### **3.3. Layer 3: High-Level API (`src/functions/`)**

This layer contains the implementations of the special functions.

*   **`padic_log.h`:**
    *   `Qp log_p(const Qp& x)`: Takes a `Qp` number. Checks that its valuation is 0 and its unit part is `≡ 1 (mod p)`. Implements the standard Taylor series `log(1+u) = u - u²/2 + ...`.
*   **`padic_gamma.h`:**
    *   `Zp gamma_p(const Zp& x)`: Implements Morita's p-adic Gamma function. This can be done via its definition as a limit or using more efficient formulas (e.g., Diamond's formulas).
*   **`l_functions.h`:**
    *   `Qp Lp(long s, const Character& chi)`: Implements the Kubota-Leopoldt p-adic L-function. For `s=0`, this uses the formula involving Bernoulli numbers: `Lp(0,χ) = -(1 - χ(p)p⁻¹) B_{1,χ}`.
    *   `Qp Lp_derivative(long s, const Character& chi)`: For `s=0`, this can be computed via the Ferrero-Washington formula or by differentiating the p-adic measure.

#### **3.4. Testing (`tests/`)**

*   **Strategy:** Every public method in every class will have a dedicated unit test.
*   **`test_zp.cpp`:** Test addition, subtraction, multiplication, division, and known identities for `Zp`. E.g., `(1-p) * (1 + p + p^2 + ...) = 1`.
*   **`test_qp.cpp`:** Test `Qp` arithmetic.
*   **`test_functions.cpp`:** Test special functions against known values.
    *   `log_p(1+p)` must equal the series evaluated at `p`.
    *   `gamma_p(1) = -1`, `gamma_p(2) = 1`, `gamma_p(p) = 1`.
    *   Test the reflection formula: `gamma_p(x) * gamma_p(1-x) = +/- 1`.
    *   Test `L_p` values against known results for small `p` and simple characters.

---

### **4. Milestone Definition (Phase 1 Completion)**

The milestone is achieved when a standalone test program, `milestone1_test`, can be built and run.

**`milestone1_test.cpp`:**
1.  Takes a prime `p` (e.g., 7) and precision `N` (e.g., 60) as command-line arguments.
2.  Enumerates all odd and even primitive Dirichlet characters `χ` modulo `p`.
3.  For each odd `χ`:
    a. Computes `Φ_p^{(odd)}(χ)` by summing the `log_p Γ_p` terms using the library.
    b. Computes `Ψ_p^{(odd)}(χ)` by calling the `Lp_derivative` function.
    c. Asserts that `Φ_p^{(odd)}(χ) == Ψ_p^{(odd)}(χ)` to precision `O(p^N)`.
4.  For each even `χ`:
    a. Computes `Φ_p^{(even)}(χ)` by summing the `log_p(a/(p-1))` terms.
    b. Computes `Ψ_p^{(even)}(χ)` by calling the `Lp` function at `s=0`.
    c. Asserts that `Φ_p^{(even)}(χ) == Ψ_p^{(even)}(χ)` to precision `O(p^N)`.

When this test program compiles and passes for `p=5, 7, 11`, Phase 1 is officially complete. The project will have a bulletproof, well-documented, and high-performance engine ready for the global calculations in Phase 2.