int from_tower;
int to_tower;
void TOH(int n, int x,int y,int z);
void main();

main()
{
 int n,k;
 n = 5; k = n-1;
 TOH(k,10,20,30);
 return;
}

TOH(int n,int x,int  y,int z)
{
 int k;
 if(n>0)
 {
  k = n-1;
  TOH(k,x,z,y);
  from_tower = x;
  to_tower = y;
  TOH(k,z,y,x);
 }
 return ;
}
