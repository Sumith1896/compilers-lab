int global_variable;

int f1();
int f2();
int f3();
int main();

f1(){
	return 2;
}

f2(){
	return 2000;
}

f3(){
	return 3;
}

main(){
	int a,b,c;
	float d;
	a = f1();
	b = f2();
	c = f3();
	d = 20.22;
	global_variable = a+b;
	print("a : ");
	print(a);
	print("\nb : ");
	print(b);
	print("\nc : ");
	print(c);
	print("\nd : ");
	print(d);
	print("\nglobal_variable : ");
	print(global_variable);
	print("\n");

	if(a == 1) return 0;
	else if(b == 1) return 1;
	else if(c == 1) return 2;
	else return 3;
	return 0;	
}