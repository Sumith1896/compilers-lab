void main();

main()
{
	int x, y;

	y = 500;
	x = y;

label1:
	x=x+1;
	y=y-1;
	if(y>100) {
		goto label1;
	}

	print("x: ");   print(x);   print("\n");
	print("y: ");   print(y);   print("\n");
	return;
}
