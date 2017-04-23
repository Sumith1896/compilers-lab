int mul(int a, int b);
int mul(int a, int b, int c);
void main();

mul(int a, int b) {
 	int res;
 	res = a*b;
 	return res;
}

mul(int a, int b, int c) {
 	int res;
 	res = a*b;
 	res = res*c;
 	return res;
}

main()
{
	int i,a1,a2;
	i=10;
	a1=mul(i, i + 1);a1=a1/2;
	a2=mul(i, i + 1, 2*i + 1);a2=a2/6;
	print("Sum of first ");
	print(i);
	print(" natural numbers: \t\t");
	print(a1);
	print("\n");
	print("Sum of squares of first ");
	print(i);
	print(" natural numbers: \t");
	print(a2);
	print("\n");
	return;
}
