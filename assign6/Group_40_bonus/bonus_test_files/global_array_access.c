int array[3];
void main();

main() {
	int a;
	int c;

	array[0]=0;
	array[1]=1;
	array[2]=2;
	print(array[0]); print("\n");
	print(array[1]); print("\n");
	print(array[2]); print("\n");

	a = 0;
	print(array[a]); print("\n");
	a = 1;
	print(array[a]); print("\n");
	a = 2;
	print(array[a]); print("\n");

	a = 0;
	array[a] = 10;
	print(array[a]); print("\n");
	
	a = 1;
	array[a] = 11;
	print(array[a]); print("\n");
	a = 2;
	array[a] = 12;
	print(array[a]); print("\n");

	array[0]=array[0];
	array[1]=array[0];
	array[2]=array[1];
	print(array[0]); print("\n");
	print(array[1]); print("\n");
	print(array[2]); print("\n");
	return;
}