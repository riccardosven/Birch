/*
 * Test matrix normal-inverse-gamma-Gaussian conjugacy.
 */
program test_matrix_normal_inverse_gamma_matrix_gaussian(N:Integer <- 10000) {
  m:TestMatrixNormalInverseGammaMatrixGaussian;
  test_conjugacy(m, N, m.size());
}
