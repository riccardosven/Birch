/*
 * Test linear inverse-gamma-Gaussian conjugacy.
 */
program test_negative_linear_normal_inverse_gamma_gaussian(N:Integer <- 10000) {
  m:TestNegativeLinearNormalInverseGammaGaussian;
  test_conjugacy(m, N, 3);
}
