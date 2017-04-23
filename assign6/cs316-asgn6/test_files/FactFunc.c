void nonrecurfn(int n);
void recurfn(int number);
int recurfactorial(int n);
int nonrecurfactorial(int n);
void main(); 

main()
{
  int number;
  int fact;
  fact = 1;
  number = 5; 
  
  print("Printing functionality to be supported to.\n");
  print(fact);

  nonrecurfn(number); 
  recurfn(number);
  return;
}

recurfn(int number)
{
  int fact;
  fact = recurfactorial(number);
  return;
}
 
recurfactorial(int n)
{
  int result ;
  int k;
  if (n == 0){
    result = 1;
  }
  else{

    result = recurfactorial(n-1);
    result = (n * result);
  }
  return result;

}

nonrecurfactorial(int n)
{
  int c;
  int result;
  result = 1;
  c = 1;
  while ( c <= n){
    result = result * c;
    c = c+1;
 }
  return result;
}

nonrecurfn(int n)
{
  int f;
  if (n < 0){ 
   }
  else
  {
   f = nonrecurfactorial(n);
  }
  return ;
}
