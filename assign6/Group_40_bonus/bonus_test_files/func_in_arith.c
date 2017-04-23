int fn(int n);
void main();

main()
{
	int a;
	int n;
	n = 20;
	a = 1 + fn(n);
	print(a);
	return;
}

fn(int n)
{	
	print(n);
	print("\n");
	if(n > 100)
		return 100;
	return fn(n + 1);
}
