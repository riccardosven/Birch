/*
 * Test scaled gamma-exponential conjugacy.
 */
program test_negative_scaled_gamma_exponential(N:Integer <- 10000) {
  m:TestNegativeScaledGammaExponential;
  test_conjugacy(m, N, 2);
}
