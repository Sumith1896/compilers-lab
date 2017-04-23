// Relational operators are not associative.
void main();

main()
{
	int x, y, z;

	while(x != y) {
		if(z == 2) {
			goto label1;
		} 
		y = z + 2;
		z = x;
	}
	goto label1;
	y = z;

label1:
	z = y;
}

