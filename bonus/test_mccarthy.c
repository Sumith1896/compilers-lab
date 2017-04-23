int mc91(int n);
void main();

main()
{
	int a;
	int n;
	for(a = 1; a <= 100; a = a + 1) {
		print("mc91(");
		print(a);
		print("):\t");
		print(mc91(a));
		print("\n");
	}
	return;
}

mc91(int n)
{
	if(n > 100) {
		return n - 10;
	} 
	return mc91(mc91(n+11));
}
