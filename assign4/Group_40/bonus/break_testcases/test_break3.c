// Relational operators are not associative.
void main();

main()
{
	int x, y, z;

	while(x != y) {
		while(y != z) {
			break;
		}
		y = z + 2;
		break;
		z = x;
	}
	y = z;
}

