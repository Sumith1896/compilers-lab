void main();

main()
{
	int array[3];
	int a;
	array[1] = 1;
	a = 1 + array[1];
	array[a] = a;
	a = 1 + array[a-1];
	print(a);
	return;
}