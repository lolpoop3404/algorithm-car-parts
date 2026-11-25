#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PRODUCTS 40
#define MAX_LINELEN 256

typedef struct {
    int line_code;
    int batch_code;
    char batch_date[20];
    int prod_id;
    char product_name[40];
    char target_enginecode[20];
    int bin_num;
    float weight;
    float price;
} product;

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
        if(!(token = strtok(NULL, ","))) continue; p->weight = atof(token);
        if(!(token = strtok(NULL, ","))) continue; p->price = atof(token);
        if(!(token = strtok(NULL, ","))) continue; strcpy(p->batch_date, token);
        if(!(token = strtok(NULL, ","))) continue; p->batch_code = atoi(token);
        if(!(token = strtok(NULL, ","))) continue; strcpy(p->target_enginecode, token);
        if(!(token = strtok(NULL, ","))) continue; p->bin_num = atoi(token);

        count++;
    }

    fclose(fp);
    return count;
}

void print_products(product products[], int num_products) {
    printf("\nCurrent Product List:\n");
    printf("------------------------------------------------------------------------------------------------\n");
    printf("ID     Name        Weight  Price   Date        Batch   Engine  Bin\n");
    printf("------------------------------------------------------------------------------------------------\n");
    
    for(int i = 0; i < num_products; i++) {
        printf("%-6d %-10s  %-6.2f  %-6.2f  %-10s  %-6d  %-6s  %-3d\n",
               products[i].prod_id,
               products[i].product_name,
               products[i].weight,
               products[i].price,
               products[i].batch_date,
               products[i].batch_code,
               products[i].target_enginecode,
               products[i].bin_num);
    }
    printf("------------------------------------------------------------------------------------------------\n");
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
    product products[MAX_PRODUCTS];
    int total = 0;

    printf("Reading product files...\n");
    total += readProducts("products1.csv", products, total);
    total += readProducts("products2.csv", products, total);
    total += readProducts("products3.csv", products, total);
    total += readProducts("products4.csv", products, total);

    printf("Total Products Read: %d\n", total);
    print_products(products, total);

    // Product removal interface
    int choice;
    do {
        printf("\nOptions:\n");
        printf("1. Remove a product by ID\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if(choice == 1) {
            int target_id;
            printf("Enter Product ID to remove: ");
            scanf("%d", &target_id);
            total = removeProduct(products, total, target_id);
            print_products(products, total);
        }
    } while(choice != 2);

    printf("\nFinal Product Count: %d\n", total);
    return 0;
}