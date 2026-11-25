#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTS 100

typedef struct {
    int id;
    char name[50];
    float weight;
    float price;
    char date[11];
    int batchCode;
    char engineCode[10];
    int bin;
} Product;

void displayProduct(Product p) {
    printf("Product ID: %d | Name: %s | Weight: %.2f | Price: %.2f | Date: %s | Batch Code: %d | Engine Code: %s | Bin: %d\n",
           p.id, p.name, p.weight, p.price, p.date, p.batchCode, p.engineCode, p.bin);
}

void loadProducts(Product products[], int *total) {
    FILE *file = fopen("products.txt", "r");
    if (!file) {
        printf("Error opening file. Loading demo data...\n");
        // Fallback to hardcoded values
        strcpy(products[0].name, "Headlight Bulb LED");
        products[0].id = 0;
        products[0].weight = 0.1;
        products[0].price = 14.99;
        strcpy(products[0].date, "20/04/2025");
        products[0].batchCode = 1238;
        strcpy(products[0].engineCode, "ENG05");
        products[0].bin = 3;
        *total = 1;
        return;
    }

    int i = 0;
    while (i < MAX_PRODUCTS && fscanf(file, "%d,%49[^,],%f,%f,%10[^,],%d,%9[^,],%d",
           &products[i].id,
           products[i].name,
           &products[i].weight,
           &products[i].price,
           products[i].date,
           &products[i].batchCode,
           products[i].engineCode,
           &products[i].bin) == 8) {
        i++;
    }

    *total = i;
    fclose(file);
}

void sortProductsByWeight(Product products[], int total) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = i + 1; j < total; j++) {
            if (products[i].weight > products[j].weight) {
                Product temp = products[i];
                products[i] = products[j];
                products[j] = temp;
            }
        }
    }
}

int main() {
    Product products[MAX_PRODUCTS];
    int totalProducts = 0;

    loadProducts(products, &totalProducts);

    // Now display only the second half of the products
    printf("\nDisplaying second half of products:\n");
    for (int i = totalProducts / 2; i < totalProducts; i++) {
        displayProduct(products[i]);
    }

    return 0;
}
