void main();

main() {
  int a, b, x, y, t, gcd, lcm, iter;
  x = 14;
  y = 36;

  for(a = x, b = y, iter = 0; b != 0; iter = iter + 1) {
    t = a - (a / b) * b;
    a = b;
    b = t;
  }

  gcd = a;
  lcm = (x * y) / gcd;

  print("x:    "); print(x);    print("\n");
  print("y:    "); print(y);    print("\n");
  print("lcm:  "); print(lcm);  print("\n");
  print("gcd:  "); print(gcd);  print("\n");
  print("iter: "); print(iter); print("\n");
  return;
}
