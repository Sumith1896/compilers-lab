
void main();

main(){
  int a, b, x, y, t, gcd, lcm;
  x = 14;
  y = 36; 
 
  a = x;
  b = y;
 
  for(a = 1; b != 0; a = a + 1) {
    t = b;
    b = (a / b) * b;
    a = t;
  }
 
  gcd = a;
  lcm = (x*y)/gcd;
}
