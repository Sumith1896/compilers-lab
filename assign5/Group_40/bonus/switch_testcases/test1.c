void main();

main()
{
	int i;
	i=2;
    switch (i) {
        case 1: i = i+1;
        case 2: i = i+2;
        case 3: i = i+3;
        case 4:
        case 5: i = i+45;
                break;
        case 6: i = i+6;
    }
}

