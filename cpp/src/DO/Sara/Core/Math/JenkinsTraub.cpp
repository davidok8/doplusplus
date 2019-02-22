#include <DO/Sara/Core/EigenExtension.hpp>
#include <DO/Sara/Core/Math/NewtonRaphson.hpp>
#include <DO/Sara/Core/Math/UnivariatePolynomial.hpp>

#include <complex>
#include <ctime>
#include <memory>


namespace DO { namespace Sara {

  // Cauchy's method to calculate the polynomial lower bound.
  auto compute_moduli_lower_bound(const UnivariatePolynomial<double>& P)
      -> double
  {
    auto Q = P;
    Q[0] = -std::abs(Q[0] / Q[Q.degree()]);
    for (int i = 1; i <= Q.degree(); ++i)
      Q[i] = std::abs(Q[i] / Q[Q.degree()]);

    auto x = 1.;
    auto newton_raphson = NewtonRaphson<double>{Q};
    x = newton_raphson(x, 50);

    return x;
  }

  // Sigma is a real polynomial. So (s1, s2) is a pair of identical real numbers
  // or a conjugate complex pair.
  auto sigma_generic_formula(const std::complex<double>& s1)
      -> UnivariatePolynomial<double>
  {
    const auto a = std::real(s1);
    const auto b = std::imag(s1);
    return Z.pow<double>(2) - 2 * a * Z + (a * a + b * b);
  }

  // See formula (2.2) at page 547.
  // Don't use because overflow and underflow problems would occur (page 563).
  auto K1_generic_recurrence_formula(const UnivariatePolynomial<double>& K0,
                                     const UnivariatePolynomial<double>& P,
                                     const UnivariatePolynomial<double>& sigma,
                                     const std::complex<double>& s1)
      -> UnivariatePolynomial<double>
  {
    const auto s2 = std::conj(s1);

    Matrix2cd a, b, c;
    a <<  P(s1),  P(s2),  //
         K0(s1), K0(s2);

    b <<     K0(s1),     K0(s2),  //
         s1 * P(s1), s2 * P(s2);

    c << s1 * P(s1), s2 * P(s2),  //
              P(s1),      P(s2);

    const auto m = std::real(a.determinant() / c.determinant());
    const auto n = std::real(b.determinant() / c.determinant());

    return ((K0 + (m * Z + n)*P) / sigma).first;
  }

  // See formula (2.7) at page 548.
  auto
  sigma_formula_from_shift_polynomials(const UnivariatePolynomial<double>& K0,
                                       const UnivariatePolynomial<double>& K1,
                                       const UnivariatePolynomial<double>& K2,
                                       const std::complex<double>& s1)
      -> UnivariatePolynomial<double>
  {
    const auto s2 = std::conj(s1);
    const auto a2 = std::real(K1(s1) * K2(s2) - K1(s2) * K2(s1));
    const auto a1 = std::real(K0(s2) * K2(s1) - K0(s1) * K2(s2));
    const auto a0 = std::real(K0(s1) * K1(s2) - K0(s2) * K1(s1));

    // return (a2 * Z.pow(2) + a1 * Z + a0) / a2;
    return Z.pow<double>(2) + (a1 / a2) * Z + (a0 / a2);
  }

  // See formula at "Stage 1: no-shift process" at page 556.
  auto K1_no_shift_polynomial(const UnivariatePolynomial<double>& K0,
                              const UnivariatePolynomial<double>& P)
      -> UnivariatePolynomial<double>
  {
    auto K1 = (K0 - (K0(0) / P(0)) * P) / Z;
    return K1.first;
  }

  // This is the scaled recurrence formula (page 563).
  auto K0_polynomial(const UnivariatePolynomial<double>& P)
      -> UnivariatePolynomial<double>
  {
    return derivative(P) / P.degree();
  }

  // Fixed-shift process.
  struct Stage2
  {
    enum ConvergenceType : std::uint8_t
    {
      NoConvergence = 0,
      LinearFactor = 1,
      QuadraticFactor = 2
    };

    //! @{
    //! @brief parameters.
    int M{5};
    int L{20};

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dist{0, 94.0};
    double beta;
    //! @}

    //! The polynomial of which we want to find the roots.
    UnivariatePolynomial<double>& P;
    //! P(s1) and P(s2).
    std::complex<double>& P_s1;
    std::complex<double>& P_s2;

    //! Quadratic real polynomial divisor.
    UnivariatePolynomial<double>& sigma;
    //! The roots of sigma.
    std::complex<double>& s1;
    std::complex<double>& s2;

    //! Stage 2: fixed-shift polynomial.
    //! Stage 3: variable-shift polynomial.
    UnivariatePolynomial<double> K0;
    UnivariatePolynomial<double> K1;
    //! K0(s1) and K0(s2)
    std::complex<double>& K0_s1;
    std::complex<double>& K0_s2;
    //! Auxiliary variables.
    double a, b, c, d;
    double u, v;
    UnivariatePolynomial<double> Q_P, P_r;
    UnivariatePolynomial<double> Q_K0, K0_r;

    //! Fixed-shift coefficients to update sigma.
    MatrixXcd K1_{3, 2};

    // 1. Determine moduli lower bound $\beta$.
    auto determine_lower_bound() -> void
    {
      beta = compute_moduli_lower_bound(P);
    }

    // 2. Form polynomial sigma(z).
    auto form_quadratic_divisor_sigma() -> void
    {
      constexpr auto i = std::complex<double>{0, 1};

      const auto phase = dist(rd);
      s1 = beta * std::exp(i * phase);
      s2 = std::conj(s1);

      sigma = Z.pow<double>(2) - 2 * std::real(s1) * Z + std::real(s1 * s2);
    }

    // 3.1 Evaluate the polynomial at divisor roots.
    auto evaluate_polynomial_at_divisor_roots() -> void
    {
      P_s1 = P(s1);
      P_s2 = P(s1);
    }

    // 3.2 Evaluate the fixed-shift polynomial at divisor roots.
    auto evaluate_shift_polynomial_at_divisor_roots() -> void
    {
      K0_s1 = K0(s1);
      K0_s2 = K0(s1);
    }

    // 3.3 Calculate coefficient of linear remainders (cf. formula 9.7).
    auto calculate_coefficients_of_linear_remainders() -> void
    {
      // See stage 2 formula (9.7) (page 563).
      Matrix4cd M;
      Vector4cd y;

      M <<
        1, -s2, 0,   0,
        0,   0, 1, -s2,
        1, -s1, 0,   0,
        0,   0, 1, -s1;

      y << P_s1, P_s2, K0_s1, K0_s2;
      Vector4cd x = M.colPivHouseholderQr().solve(y);

      a = std::real(x[0]);
      b = std::real(x[1]);
      c = std::real(x[2]);
      d = std::real(x[3]);

      u = -std::real(s1 + s2);
      v = std::real(s1 * s2);
    }

    // 3.4 Calculate the next fixed/variable-shift polynomial (cf. formula 9.8).
    auto calculate_next_fixed_shit_polynomial() -> void
    {
      P_r = b * (Z + u) + a;
      K0_r = c * (Z + u) + d;

      Q_P = ((P - P_r) / sigma).first;
      Q_K0 = ((K0 - K0_r) / sigma).first;

      const auto c0 = b * c - a * d;
      const auto c1 = (a * a + u * a * b + v * b * b) / c0;
      const auto c2 = (a * c + u * a * d + v * b * d) / c0;

      K1 = c1 * Q_K0 + (Z - c2) * Q_P + b;
    }

    // 3.5 Calculate the new quadratic polynomial sigma (cf. formula 6.7).
    auto calculate_next_quadratic_divisor() -> void
    {
      K1_(0, 0) = K1(s1);
      K1_(0, 1) = K1(s2);

      K1_(1, 0) = (K1_(0, 0) - K1(0) / P(0) * P_s1) / s1;
      K1_(1, 1) = (K1_(0, 1) - K1(0) / P(0) * P_s2) / s2;

      K1_(2, 0) = (K1_(1, 0) - K1(0) / P(0) * P_s1) / s1;
      K1_(2, 1) = (K1_(1, 1) - K1(0) / P(0) * P_s2) / s2;
    }

    auto check_convergence_linear_factor() -> void;
    auto check_convergence_quadratic_factor() -> void;
  };


} /* namespace Sara */
} /* namespace DO */
