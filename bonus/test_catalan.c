int catalan(int n);
int main();

catalan(int n)
{
    // base case
    int res;
    int i;
    int a, b;
    i = 0;
    res = 0;
    if (n <= 1) return 1;

    while (i < n){
        a = catalan(i);
        b = catalan(n-i-1);
        res = res + a*b;
        i = i+1;
    }
 
    return res;
}
 
// driver program to test above function
main()
{
    int p;
    p = 1;
    print("Running catalan:\n");
    while (p <= 10){
        print("catalan(");
        print(p);
        print("):\t");
        print(catalan(p));
        print("\n");
        p = p + 1;
    }
    return 0;
}
