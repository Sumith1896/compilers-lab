void main();

main(){
  int a, b, x, y, t, gcd, lcm;
  x = 14;
  y = 36; 
 
  a = x;
  b = y;
 
  while(b!=0){
    t = a - (a / b) * b;
    a = b;
    b = t;
  }
 
  gcd = a;
  lcm = (x*y)/gcd;

  print("x: ");   print(x);   print("\n");
  print("y: ");   print(y);   print("\n");
  print("lcm: "); print(lcm); print("\n");
  print("gcd: "); print(gcd); print("\n");

  return;
}
