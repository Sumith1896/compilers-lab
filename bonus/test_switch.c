void main();

main()
{
	int i;
    for(i=1; i<8; i=i+1) {
        print("For case ");
        print(i);
        print(": ");
        switch (i) {
            case 1: print(1);
            case 2: print(2);
            case 3: print(3);
                    break;
            case 4: print(4);
            case 5: print(5);
                    break;
            case 6: print(6);
            default:
                print("def");
        }
        print("\n");
    }
    return;
}
