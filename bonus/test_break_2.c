void main();

main()
{
	int i,j,x,count;
	
	x=500;
	count=0;

	for(i=1 ; i<1000 ; i=i+1) {
		
		if(i>x)
			break;

		for(j=1 ; j<1000 ; j=j+1) {
			
			if(j>i)
				break;
			
			count = count+1;
		}
	}

	print("(x*x+x)/2:  "); print((x*x+x)/2); print("\n");
	print("count:      "); print(count);     print("\n");

	return;
}
