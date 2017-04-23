void main();

main()
{
	int x, y, z;
	x = 0;y=0;z=0;

	while(x <= y) {
		if(y==z+100)
			break;
		y = y + 2;
		x = x-1;
	}

	print("x: "); print(x); print("\n");
	print("y: "); print(y); print("\n");
	print("z: "); print(z); print("\n");

	return;
}

