int mc91(int n);
void main();

main()
{
	int a;
	int n;
	a = mc91(n);
	print(a);
	return;
}

mc91(int n)
{
	if(n > 100) {
		return n - 10;
	} 
	return mc91(mc91(n+11));
}
