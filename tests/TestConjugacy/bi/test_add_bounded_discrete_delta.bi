/*
 * Test analytical solution for addition of bounded discrete random variates.
 */
program test_add_bounded_discrete_delta(N:Integer <- 10000) {
  m:TestAddBoundedDiscreteDelta;
  test_conjugacy(m, N, 2);
}
