/*
 * Test linear Gaussian-Gaussian conjugacy.
 */
program test_linear_gaussian_gaussian(N:Integer <- 10000) {
  m:TestLinearGaussianGaussian;
  test_conjugacy(m, N, 2);
}
