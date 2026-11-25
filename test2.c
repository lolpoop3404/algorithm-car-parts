#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 40
#define MAX_PRODUCTS 40
#define MAX_LINELEN 256
#define MAX_FILE 4

typedef struct {
    int line_code;
    int batch_code;
    char batch_date[20], month_date[10], year_date[10];
    int prod_id;
    char product_name[40];
    char target_enginecode[20];
    int bin_num;
    float weight;
    float price;
} product;

int rand_int(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void rand_str(char *str, int length) {
    const char *charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < length - 1; i++) {
        str[i] = charset[rand() % strlen(charset)];
    }
    str[length - 1] = '\0';
}

void generate_randProducts(product products[], int num_products) {
    for(int i = 0; i < num_products; i++) {
        products[i].line_code = rand_int(100, 999);
        products[i].batch_code = rand_int(1000, 9999);

        int day = rand_int(1, 28);
        int month = rand_int(1, 12);
        int year = rand_int(2024, 2025);
        snprintf(products[i].batch_date, SIZE, "%02d/%02d/%04d", day, month, year);
        snprintf(products[i].month_date, SIZE, "%02d", month);
        snprintf(products[i].year_date, SIZE, "%04d", year);

        products[i].prod_id = rand_int(1000, 9999);
        rand_str(products[i].product_name, 10);
        rand_str(products[i].target_enginecode, 5);
        products[i].bin_num = rand_int(1, 9);
        products[i].weight = (float)rand_int(1, 50) / 10.0;
        products[i].price = (float)rand_int(50, 1000) / 10.0;
    }
}

int readProducts(const char* filename, product products[], int startIndex) {
    char line[MAX_LINELEN];
    FILE* fp = fopen(filename, "r");

    if(!fp) {
        printf("Error opening the file %s\n", filename);
        return 0;
    }

    int count = 0;
    while(fgets(line, sizeof(line), fp) && count + startIndex < MAX_PRODUCTS) {
        product* p = &products[startIndex + count];
        char* token = strtok(line, ",");
        if(!token) continue;
       
        p->line_code = atoi(token);
        if(!(token = strtok(NULL, ","))) continue; strcpy(p->product_name, token);
        if(!(token= strtok(NULL, ","))) continue; p->weight = atof(token);
        if(!(token = strtok(NULL,","))) continue; p->price = atof(token);
        if(!(token = strtok(NULL,","))) continue; strcpy(p->batch_date, token);
        if(!(token = strtok(NULL,","))) continue; p->batch_code = atoi(token);
        if(!(token = strtok(NULL,","))) continue; strcpy(p->target_enginecode, token);
        if(!(token = strtok(NULL,","))) continue; p->bin_num = atoi(token);

        count++;
    }

    fclose(fp);
    return count;
}

void merge(product arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    product* Left = malloc(n1 * sizeof(product));
    product* Right = malloc(n2 * sizeof(product));

    if (Left == NULL || Right == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    for(int i = 0; i < n1; i++) {
        Left[i] = arr[left + i];
    }

    for(int j = 0; j < n2; j++) {
        Right[j] = arr[mid + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;

    while(i < n1 && j < n2) {
        if(Left[i].weight <= Right[j].weight) {
            arr[k++] = Left[i++];
        }
        else {
            arr[k++] = Right[j++];
        }
    }

    while (i < n1) {
        arr[k++] = Left[i++];
    }

    while (j < n2) {
        arr[k++] = Right[j++];
    }

    free(Left);
    free(Right);
}

void mergesort(product arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergesort(arr, left, mid);
        mergesort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void print_products(product products[], int num_products) {
    for(int i = 0; i < num_products; i++) {
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

int binarySearch(product products[], int size, float target_weight) {
    int left = 0, right = size - 1;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if(products[mid].weight == target_weight) {
            result = mid;
            right = mid - 1;
        }
        else if(products[mid].weight < target_weight) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return result;
}

int removeProduct(product products[], int total, int target_id) {
    int foundIndex = -1;

    for (int i = 0; i < total; i++) {
        if (products[i].prod_id == target_id) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Product with ID %d not found.\n", target_id);
        return total;
    }

    for (int i = foundIndex; i < total - 1; i++) {
        products[i] = products[i + 1];
    }

    printf("Product with ID %d removed successfully.\n", target_id);
    return total - 1;
}

int main() {
    printf("Program Started\n");
    srand(time(NULL));

    product products[MAX_PRODUCTS];
    int total = 0;

    total += readProducts("products1.csv", products, total);
    total += readProducts("products2.csv", products, total);
    total += readProducts("products3.csv", products, total);
    total += readProducts("products4.csv", products, total);

    printf("Total Products Read: %d\n", total);

    mergesort(products, 0, total - 1);
    print_products(products, total);

    generate_randProducts(products, MAX_PRODUCTS);
    mergesort(products, 0, MAX_PRODUCTS - 1);
    printf("Sorted Product Data (by weight): \n");
    print_products(products, MAX_PRODUCTS);

    float target_weight;
    printf("\nEnter the target weight to look for:\n");
    scanf("%f", &target_weight);
    int index = binarySearch(products, MAX_PRODUCTS, target_weight);
    if (index != -1) {
        printf("Product with weight %.2f found at index %d\n", target_weight, index);
        printf("Product ID: %d | Name: %s | Weight: %.2f | Price: %.2f | Date: %s | Batch Code: %d | Engine Code: %s | Bin: %d\n",
                products[index].prod_id,
                products[index].product_name,
                products[index].weight,
                products[index].price,
                products[index].batch_date,
                products[index].batch_code,
                products[index].target_enginecode,
                products[index].bin_num);
    } else {
        printf("Product with weight %.2f isn't found\n", target_weight);
    }

    int target_id;
    printf("\nEnter the Product ID to remove: ");
    scanf("%d", &target_id);
    total = removeProduct(products, total, target_id);

    printf("\nUpdated Product List:\n");
    print_products(products, total);

    printf("\nDelivery Report - # of products/van: \n");
    int van_count[10] = {0};
    for (int i = 0; i < total; i++) {
        if (products[i].bin_num >= 1 && products[i].bin_num <= 9) {
            van_count[products[i].bin_num]++;
        }
    }
    for (int i = 1; i <= 9; i++) {
        printf("Van %d: %d products on van\n", i, van_count[i]);
    }

    return 0;
}