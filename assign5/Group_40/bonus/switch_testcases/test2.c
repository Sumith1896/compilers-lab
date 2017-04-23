// Relational operators are not associative.
void main();

main()
{
	int x, y, z;
	int i;
	i=2;
    switch (i) {
        case 1: i = i+1;
        		while(x != y) {
					if(z == 2) {
						break;
					}
					else {
						y = z + 2;
						z = x;
					}
					y = y+1;
				}
				goto label1;
				y = z;

			label1:
				z = y;
        case 2: i = i+2;
        case 3: i = i+3;
        case 4:
        case 5: i = i+45;
                break;
        case 6: i = i+6;
        default: i = i*i;
    }
}
