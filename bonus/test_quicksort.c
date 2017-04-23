// Code is from GeeksForGeeks: http://quiz.geeksforgeeks.org/quick-sort/
int array[6];
int partition(int low, int high);
void quickSort(int low, int high);
void printArray(int size);
void main();

partition(int low, int high) {
    int pivot, i, j, temp;

    pivot = array[high];    // pivot
    i = (low - 1);          // index of smaller element
    j = low;
    
    for(j = low; j <= high - 1; j = j + 1) 
    {
        // if current element is smaller than or
        // equal to pivot
        if (array[j] <= pivot) {
            i = i + 1;    // increment index of smaller element
            temp = array[i];
            array[i] = array[j]; 
            array[j] = temp;
        }
    }
    temp = array[i + 1];
    array[i + 1] = array[high]; 
    array[high] = temp;

    return (i + 1);
}

quickSort(int low, int high)
{
    int pi;

    if (low < high)
    {
        pi = partition(low, high);

        quickSort(low, pi - 1);
        quickSort(pi + 1, high);
    }

    return;
}
 
printArray(int size)
{
    int i;
    
    for (i=0; i < size; i=i+1) {
        print(array[i]);
        print(" ");
    }
    print("\n");

    return;
}
 
// driver program to test above functions
main()
{
    int n;

    array[0] = 10;
    array[1] = 7;
    array[2] = 8;
    array[3] = 9;
    array[4] = 1;
    array[5] = 5;
    n = 6;

    print("Original array: \n");
    printArray(n);
    quickSort(0, n - 1);
    print("Sorted array: \n");
    printArray(n);
    
    return;
}