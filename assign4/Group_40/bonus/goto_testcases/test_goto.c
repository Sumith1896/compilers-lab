// Relational operators are not associative.
void main();

main()
{
	int x, y, z;

	x = y;
	goto label1;
	y = z;
label1:
	z = y;
}

