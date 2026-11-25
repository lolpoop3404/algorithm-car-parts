#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define SIZE 40
#define MAX_PRODUCTS 40
#define MAX_LINELEN 256
#define NUM_BINS 9 //for number of vans

typedef struct{
    int line_code;
    int batch_code;
    char batch_date[20]; //date in formt yyyy/mm/dd
    int prod_id;
    char product_name[40];
    char target_enginecode[20];
    int bin_num;
    float weight; //this is the weight for the product 
    float price; // this is the price for the product 
} product; //this is the struct template for the product (alias for the struct used within the code)

//function for random number generator
int rand_int(int min, int max) 
{
    return rand() % (max - min + 1) + min; //this generates a random integer between a min and max value range for the data in the product struct
}

//function for string generator
void rand_str(char *str, int length)
{
    const char *charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"; //this is the charset to randomise string
    for (int i = 0; i < length - 1; i++)
    {
        str[i] = charset[rand() % strlen(charset)]; //this generates a random string of characters from the charset
    }
    str[length - 1] = '\0'; //this adds a null terminator to the end of the string
}

//function for random product generator
void generate_randProducts(product products[], int num_products)
{
    for(int i = 0; i < num_products; i++)
    {
        //randomise line_code and batch_code
        products[i].line_code = rand_int(1, 4); //this generates a random line code for the product struct from 1-4
        products[i].batch_code = rand_int(1000, 9999); //this generates a random line code and batch code for the product struct

        //randomise date of the products
       snprintf(products[i].batch_date, SIZE, "%02d/%02d/%02d", rand_int(1,28), rand_int(1,12),
                rand_int(2024,2025)); 

        products[i].prod_id = rand_int(1000, 9999); //generates a random product id for product struct (random 4 digit products id)
        rand_str(products[i].product_name, 10); //random product name (random 10 char products name)
        rand_str(products[i].target_enginecode, 5); //generates a random string of characters for target engine code (random 5 char engine code)
        products[i].bin_num = rand_int(1, NUM_BINS); //randomise bin num between 1 - 9
        products[i].weight = (float)rand_int(1, 50) / 10.0; //generates a random weight for the product struct (0.1kg to 5.0kg)
        products[i].price = (float)rand_int(50, 1000) / 10.0; //generates a random price for the product struct (5.0 to 100.0)
    }
}

void merge(product arr[], int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1; //size of the left half of the array
    int n2 = right - mid; //size of the right half of the array

    product* Left = malloc(n1 * sizeof(product)); //allocates memory for the left half of the array
    product* Right = malloc(n2 * sizeof(product)); //allocates memory for the right half of the array


    if (Left == NULL || Right == NULL) //checks if memory allocation was successful
    {
        printf("Memory allocation failed\n");
        exit(1); //exit the program if memory allocation fails
    }

    //copy data to temp arrays Left[] and Right[]
    for(int i = 0; i < n1; i++)
    {
        Left[i] = arr[left + i]; //copies left half of array into L
    }

    for(int j = 0; j < n2; j++)
    {
        Right[j] = arr[mid + 1 + j]; //copies right half of array into R
    }

    //merge the temporary arrays back into arr[left.. right]
    i = 0;
    j = 0;
    k = left;

    //while loop to merge the two halves of the array
    while( i < n1 && j < n2)
    {
        if(Left[i].weight <= Right[j].weight) //compares the weight of the left and right half of the array
        {
            arr[k++] = Left[i++]; //if the left half <= right half then copy left half into arr
        }
        else 
        {
            arr[k++] = Right[j++]; //if the right half <= left half then copy right half into arr
        }
    }

    //copy remaining elements of Left[] if any
    while (i < n1)
    {
        arr[k++] = Left[i++]; //copy the remaining elements of the left half into arr
    }

    //copy remaining elements of Right[] if any
    while (j < n2)
    {
        arr[k++] = Right[j++]; //copy remaining elements of the right half into arr
    }

    //free memory allocated for temp arrays
    free(Left);
    free(Right);
}

void mergesort(product arr[], int left, int right)
{
    if (left < right) {
        int mid = left + (right - left) / 2; //finds the mid point and split into the left half
        mergesort(arr, left, mid); //sorts left half of the array
        mergesort(arr, mid + 1, right); //sorts right half of the array
        merge(arr, left, mid , right); //merges the two halves of the array
    }
}

void randomiseProduct(product arr[], int n)
{
    for(int i = n - 1; i > 0; i--)
    {
        //generate a random index between 0 and i
        int j = rand () % (i + 1); //generate random index from 0 to random i value
        product temp = arr[i]; //temp variable to store product in i
        arr[i] = arr[j];
        arr[j] = temp; //swaps product in i with j and then into temp variable
    }
}
//function to print the products in the array
void print_products(product products[], int num_products) 
{
    for(int i = 0; i < num_products; i++)
    {
        //format in | | separator for tidyness
        printf("Product ID: %d | Name: %s | Weight: %.2f | Price: %.2f | Date: %s | Batch Code: %d | Engine Code: %s | Bin: %d\n",
                products[i].prod_id,
                products[i].product_name,
                products[i].weight,
                products[i].price,
                products[i].batch_date,  
                products[i].batch_code,
                products[i].target_enginecode,
                products[i].bin_num); //prints the product details in the array
    }
}

int binarySearch(product products[], int size, float target_weight)
{
    int left = 0, right = size - 1; //this define the left and right pointers for binary search
    int result = - 1; //this is result variable to score index of the product found

    while (left <= right)
    {
        int mid = left + (right - left) / 2; //finds mid point of array
        float currentWeight = products[mid].weight; //current weight of product in array

        if(currentWeight == target_weight)
        {
            result = mid; //if product found assign as mid
            right = mid - 1; //look for first occurence
        }
        else if(currentWeight < target_weight)
        {
            left = mid + 1; //if product weight < target weight then move left to mid + 1
        }
        else
        {
            right = mid - 1; //if product weight > target weight then move right to mid - 1
        }
    }

    return result; //return index of product found
}

int main()
{
    printf("Program Started\n"); //prints the program started message
    srand(time(NULL)); //seed the random number generator

    product products[MAX_PRODUCTS]; //creates an array of products
    int total = MAX_PRODUCTS; //all products in array

    printf("Total Products Read: %d\n", total); //prints the total number of products read from the file

    generate_randProducts(products, total); //generates random product data

    printf("Original unsorted data;\n"); //display initial data
    print_products(products, total); //prints the original unsorted data

    //function call
    mergesort(products, 0, total - 1); //sorts products by weight i.e mergesort
    printf("Sorted Product Data (by weight): \n"); //print the sorted product data
    print_products(products, total); //print the sorted product data

    if(total > 0)
    {
    randomiseProduct(products, total > 40 ? 40 : total); //randomise product data in array of 40
    printf("Data after %d randomised data:\n", total > 40 ? 40 : total);
    randomiseProduct(products, total); 
    }

    float target_weight;
    printf("\nEnter the target weight to look for:\n");
    scanf("%f", &target_weight);

    int index = binarySearch(products, total, target_weight);

    if (index != -1) 
    {
        printf("Products with weight %.2f:\n", target_weight);
    
        // Print all products with the same weight (since the array is sorted)
        for (int i = index; i < total && products[i].weight == target_weight; i++) 
        {
            printf("Product ID: %d | Name: %s | Weight: %.2f | Price: %.2f | Date: %s | Batch Code: %d | Engine Code: %s | Bin: %d\n",
                products[i].prod_id,
                products[i].product_name,
                products[i].weight,
                products[i].price,
                products[i].batch_date,  
                products[i].batch_code,
                products[i].target_enginecode,
                products[i].bin_num);
        }
    } 
    else 
    {
        printf("Product with weight %.2f isn't found\n", target_weight);
    }

        printf("\nDelivery Report - # of products/van: \n");

        //this is the array count for the product in van
        int van_count[10] = {0}; 

        for(int i = 0; i < total; i++)
        {
            if(products[i].bin_num >= 1 && products[i].bin_num <= 9)
            {
                van_count[products[i].bin_num]++; //this increments the count of product/van
            }
        }

        //print summary report 
        for(int i = 1; i <= NUM_BINS; i++)
        {
            printf("Van %d: %d products on van\n", i, van_count[i]); //print summary report product/van
        }

        return 0; 
    }