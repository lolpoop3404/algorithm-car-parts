#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define SIZE 40
#define MAX_PRODUCTS 40
#define MAX_LINELEN 256
#define MAX_FILE 4 

typedef struct{
    int line_code;
    int batch_code;
    char batch_date[20], month_date[10], year_date[10]; //date in formt yyyy/mm/dd
    int prod_id;
    char product_name[40];
    char target_enginecode[20];
    int bin_num;
    float weight; //this is the weight for the product 
    float price; // this is the price for the product 
} product; //this is the struct template for the product (alias for the struct used within the code)

int rand_int(int min, int max) 
{
    return rand() % (max - min + 1) + min; //this generates a random integer between a min and max value range for the data in the product struct
}

void rand_str(char *str, int length)
{
    const char *charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"; //this is the charset to randomise string
    for (int i = 0; i < length - 1; i++)
    {
        str[i] = charset[rand() % strlen(charset)]; //this generates a random string of characters from the charset
    }
    str[length - 1] = '\0'; //this adds a null terminator to the end of the string
}

void generate_randProducts(product products[], int num_products)
{
    for(int i = 0; i < num_products; i++)
    {
        products[i].line_code = rand_int(100, 999); //this generates a random line code for the product struct from 1-4
        products[i].batch_code = rand_int(1000, 9999); //this generates a random line code and batch code for the product struct
    

    //randomise dates
    int day = rand_int(1, 28); //day of the month
    int month = rand_int(1, 12); //month of the year
    int year = rand_int(2024, 2025); //year
    snprintf(products[i].month_date, SIZE, "%02d", month); //format month as mm
    snprintf(products[i].year_date, SIZE, "%04d", year); //format year as yyyy
    snprintf(products[i].batch_date, SIZE, "%02d/%02d/%04d", day, month, year); //format date as dd/mm/yyyy

    products[i].prod_id = rand_int(1000, 9999); //generates a random product id for product struct (random 4 digit products id)
    rand_str(products[i].product_name, 10); //random product name (random 10 char products name)
    rand_str(products[i].target_enginecode, 5); //generates a random string of characters for target engine code (random 5 char engine code)
    products[i].bin_num = rand_int(1, 9); //randomise bin num between 1 - 9
    products[i].weight = (float)rand_int(1, 50) / 10.0; //generates a random weight for the product struct (0.1kg to 5.0kg)
    products[i].price = (float)rand_int(50, 1000) / 10.0; //generates a random price for the product struct (5.0 to 100.0)
    }
}

int readProducts(const char* filename, product products[], int startIndex)
{
    char line[MAX_LINELEN]; //buffer for reading lines from file
    FILE *fp = fopen(filename, "r"); //open file for reading

    if (!fp) //check if file opened successfully
    {
        printf("Error opening file: %s\n", filename); //print error message
        return 0; //return 0 if file not opened successfully
    }

    int count = 0; //counter for number of products read
    while(fgets(line, sizeof(line), fp) && count + startIndex < MAX_PRODUCTS) 
    {
        product* p = &products[startIndex + count]; //get pointer to current product
        char* token = strtok(line, ","); //tokenize line by comma
        if (!token) continue; // if token not null

        p->line_code = atoi(token); //convert token to int and assign to line_code
        if(!(token = strtok(NULL, ","))) continue; strcpy(p->product_name, token); //get product name
        if(!(token = strtok(NULL, ","))) continue; p->weight = atoi(token); //get weight
        if(!(token = strtok(NULL, ","))) continue; p->price = atof(token); //get price
        if(!(token = strtok(NULL, ","))) continue; strcpy(p->batch_date, token); //get batch date
        if(!(token = strtok(NULL, ","))) continue; p->batch_code = atoi(token); //get batch code
        if(!(token = strtok(NULL, ","))) continue; strcpy(p->target_enginecode, token); //get target engine code
        if(!(token = strtok(NULL, ","))) continue; p->bin_num = atoi(token); //get bin number

        count++; //increment count of products read
    }

        fclose(fp); //close file
        return count; //return number of products read
}

int binarySearch(product products[], int size, float target_weight)
{
    int left = 0, right =  size - 1;
    int result = -1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2; //find mid point of array

        if(products[mid].weight == target_weight)
        {
            result = mid; //if found then assign as mid
            right = mid - 1; //look for first occurence
        }
        else if(products[mid].weight < target_weight)
        {
            left = mid + 1; //if product weight < target weight then move left to mid + 1
        }
        else 
        {
            right = mid - 1; //if product weight > target weight then move right to mid - 1
        }
    }

    return result; //return index of product found

    int index = binarySearch(products, MAX_PRODUCTS, target_weight); //search for product with weight 2.5kg
    if (index != -1) //if product found
    {
        printf("Product with weight %.2f found at index %d\n", target_weight, index); //print product found message
        printf("Product ID: %d | Name: %s | Weight: %.2f | Price: %.2f | Date: %s | Batch Code: %d | Engine Code: %s | Bin: %d\n",
                products[index].prod_id,
                products[index].product_name,
                products[index].weight,
                products[index].price,
                products[index].batch_date,  
                products[index].batch_code,
                products[index].target_enginecode,
                products[index].bin_num); //prints the product details in the array
    }
    else
    {
        printf("Product with weight %.2f not found\n", target_weight);
    }
}
    
int main()
{
        printf("Program Started\n"); //prints the program started message
        srand(time(NULL)); //seed the random number generator

        product products[MAX_PRODUCTS]; //create an array of products
        int total = 0; 

        int i = 0; //this is the index for the product array

        //reads the 4 files printed in the program named products1.csv, products2.csv, products3.csv, products4.csv
        total += readProducts("C:\\Users\\lolpo\\Documents\\textfiles\\products1.csv", products, total);
        total += readProducts("C:\\Users\\lolpo\\Documents\\textfiles\\products2.csv", products, total);
        total += readProducts("C:\\Users\\lolpo\\Documents\\textfiles\\products3.csv", products, total);
        total += readProducts("C:\\Users\\lolpo\\Documents\\textfiles\\products4.csv", products, total); //read products from file

        printf("Total Products Read: %d\n", total); //prints the total number of products read from the file

        mergesort(products, 0, total - 1); //sort products by weight

        print_products(products, total); //prints the products in the array

        generate_randProducts(products, MAX_PRODUCTS); //generates random product data
        mergesort(products, 0, MAX_PRODUCTS - 1); //sort products by weight using mergesort
        printf("Sorted Product Data (by weight): \n"); //print the sorted product data
        print_products(products, MAX_PRODUCTS); //print the sorted product data
        
        printf("Product ID: %d | Name: %s | Weight: %.2f | Price: %.2f | Date: %s | Batch Code: %d | Engine Code: %s | Bin: %d\n",
                products[i].prod_id,
                products[i].product_name,
                products[i].weight,
                products[i].price,
                products[i].batch_date,  
                products[i].batch_code,
                products[i].target_enginecode,
                products[i].bin_num); //prints the product details in the array

        printf("\nDelivery Report - # of products/van: \n");

        int van_count[10] = {0}; //this is the array count for the product in van

        for (int i = 0; i < total; i++) //loop through the products in the array
        {
            van_count[products[i].bin_num]++; //increment the count of products in the bin
        }

        //print summary report#
        for(int i = 1; i < 10; i++) //loop through the bins
        {
            printf("Bin %d: %d products\n", i, van_count[i]); //print the number of products in the bin
        }

        return 0;
}   