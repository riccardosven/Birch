class TestScaledGammaExponential < Model {
  λ:Random<Real>;
  x:Random<Real>;

  a:Real;
  k:Real;
  θ:Real;

  function initialize() {
    a <- simulate_uniform(0.0, 10.0);
    k <- simulate_uniform(1.0, 10.0);
    θ <- simulate_uniform(0.0, 10.0);
  }

  fiber simulate() -> Event {
    λ ~ Gamma(k, θ);
    x ~ Exponential(a*λ);
  }

  function forward() -> Real[_] {
    y:Real[2];
    y[1] <- λ.value();
    assert !x.hasValue();
    y[2] <- x.value();
    return y;
  }

  function backward() -> Real[_] {
    y:Real[2];
    y[2] <- x.value();
    assert !λ.hasValue();
    y[1] <- λ.value();
    return y;
  }

  function marginal() -> Distribution<Real> {
    return x.distribution()!.graft();
  }
}