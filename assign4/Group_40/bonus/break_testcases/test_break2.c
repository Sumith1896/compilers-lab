// Relational operators are not associative.
void main();

main()
{
	int x, y, z;

	while(x != y) {
		y = z + 2;
		if(y == z)
			break;
		z = x;
	}
	
	y = z;
}

