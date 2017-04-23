
void main();
int fn(int a);		// fn prototype has parameter name 'a'

main()
{
	int a;
	fn(2);
	return;
}

fn(int b)		// fn definition has parameter name 'b',
			// different from prototype parameter name 'a'
{
	return 1;
}
