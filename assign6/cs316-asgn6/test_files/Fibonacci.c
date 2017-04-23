void main();
int fibonacci (int prev_number, int number);
int i, size;
int prev_number;
int number;

main()
{
    prev_number=0;
    number=1;
    size = 20;
    i = 1;
    fibonacci (prev_number,number);
    return ;
}

fibonacci (int prev_number, int number)
{
    int next_num;

    if (i==size){}
    else
    {
      next_num=prev_number+number;
      prev_number=number;
      number=next_num;
      i = i+1; 
      fibonacci (prev_number,number); 
    }
    return 0;
}
