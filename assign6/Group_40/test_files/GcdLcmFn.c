int gcd(int x, int y);
int main();
 
main() {
  int x, y, hcf, lcm;
  x = 14;
  y = 36; 
 
  hcf = gcd(x, y);
  lcm = (x*y)/hcf;
 
  return 0;
}
 
gcd(int x, int y) {
  if (x == 0) {
    return y;
  }
 
  while (y != 0) {
    if (x > y) {
      x = x - y;
    }
    else {
      y = y - x;
    }
  }
 
  return x;
}
