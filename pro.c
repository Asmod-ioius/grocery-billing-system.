#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITEMS 20
#define GST_RATE 0.05
#define DISCOUNT_THRESHOLD 1000.0
#define DISCOUNT_RATE 0.10
#define MAX_ATTEMPTS 3

struct Item {
    int id;
    char name[50];
    float price;
    int quantity;
    float total;
};

char customerName[50];

// Function declarations
int login();
void displayMenu();
void displayItems(struct Item items[], int itemCount);
void addItemToCart(struct Item items[], int itemCount);
void generateBill(struct Item items[], int itemCount, float charity, int paymentMethod);
void searchItem(struct Item items[], int itemCount);
void saveBillToFile(struct Item items[], int itemCount, float charity, int paymentMethod);
int selectPaymentMethod();

int main() {
    if (!login()) {
        printf("\nToo many failed attempts! Exiting...\n");
        return 1;
    }

    printf("Enter Customer Name: ");
    scanf(" %[^\n]", customerName);

    struct Item items[MAX_ITEMS] = {
        {1, "Rice (1kg)", 50.00, 0, 0}, {2, "Sugar (1kg)", 40.00, 0, 0},
        {3, "Milk (1L)", 60.00, 0, 0}, {4, "Eggs (12pcs)", 75.00, 0, 0},
        {5, "Flour (1kg)", 45.00, 0, 0}, {6, "Cooking Oil (1L)", 150.00, 0, 0},
        {7, "Salt (1kg)", 20.00, 0, 0}, {8, "Tea (250g)", 120.00, 0, 0},
        {9, "Coffee (200g)", 180.00, 0, 0}, {10, "Soap (1pc)", 30.00, 0, 0},
        {11, "Shampoo (200ml)", 90.00, 0, 0}, {12, "Toothpaste (100g)", 50.00, 0, 0},
        {13, "Detergent (1kg)", 70.00, 0, 0}, {14, "Biscuits (1 pack)", 35.00, 0, 0},
        {15, "Soft Drink (1.5L)", 90.00, 0, 0}, {16, "Juice (1L)", 110.00, 0, 0},
        {17, "Butter (500g)", 200.00, 0, 0}, {18, "Cheese (250g)", 160.00, 0, 0},
        {19, "Pasta (500g)", 85.00, 0, 0}, {20, "Tomato Ketchup (500g)", 75.00, 0, 0}
    };

    float charity = 0;
    int choice, paymentMethod = -1;

    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            displayItems(items, MAX_ITEMS);
        } else if (choice == 2) {
            addItemToCart(items, MAX_ITEMS);
        } else if (choice == 3) {
            if (paymentMethod == -1) {
                paymentMethod = selectPaymentMethod();
            }
            generateBill(items, MAX_ITEMS, charity, paymentMethod);
        } else if (choice == 4) {
            searchItem(items, MAX_ITEMS);
        } else if (choice == 5) {
            saveBillToFile(items, MAX_ITEMS, charity, paymentMethod);
        } else if (choice == 6) {
            printf("Enter charity amount (₹): ");
            scanf("%f", &charity);
        } else if (choice == 7) {
            printf("\nThank you for shopping with us!\n");
            exit(0);
        } else {
            printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

// Login Function
int login() {
    char correctUsername[] = "Asmodious";
    char correctPassword[] = "6969";
    char username[30], password[30];
    int attempts = 0;

    while (attempts < MAX_ATTEMPTS) {
        printf("\nLOGIN SYSTEM\n");
        printf("Username: ");
        scanf("%s", username);
        printf("Password: ");
        scanf("%s", password);

        if (strcmp(username, correctUsername) == 0 && strcmp(password, correctPassword) == 0) {
            printf("\nLogin Successful! Welcome, %s.\n", username);
            return 1;
        } else {
            printf("Incorrect Username or Password. (%d/%d attempts)\n", ++attempts, MAX_ATTEMPTS);
        }
    }
    return 0;
}

// Menu Function
void displayMenu() {
    printf("\n------ GROCERY BILLING SYSTEM ------\n");
    printf("1. View Available Items\n");
    printf("2. Add Items to Cart\n");
    printf("3. Generate Bill\n");
    printf("4. Search Item\n");
    printf("5. Save Bill to File\n");
    printf("6. Add Charity\n");
    printf("7. Exit\n");
}

// Display Items
void displayItems(struct Item items[], int itemCount) {
    printf("\nAvailable Items:\n");
    printf("-------------------------------------------------\n");
    printf("%-5s %-25s %-8s\n", "ID", "Item", "Price (₹)");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < itemCount; i++) {
        printf("%-5d %-25s %-8.2f\n", items[i].id, items[i].name, items[i].price);
    }
    printf("-------------------------------------------------\n");
}

// Add Items to Cart
void addItemToCart(struct Item items[], int itemCount) {
    int itemId, qty;
    printf("\nEnter item ID to add to cart (0 to stop): ");
    while (1) {
        scanf("%d", &itemId);
        if (itemId == 0) break;
        if (itemId > 0 && itemId <= itemCount) {
            printf("Enter quantity for %s: ", items[itemId - 1].name);
            scanf("%d", &qty);
            items[itemId - 1].quantity += qty;
        } else {
            printf("Invalid item ID! Try again.\n");
        }
    }
}

// Generate Bill and Display
void generateBill(struct Item items[], int itemCount, float charity, int paymentMethod) {
    float subtotal = 0, gstAmount, discount = 0, finalTotal;
    time_t now;
    time(&now);

    printf("\n------ GROCERY BILL ------\n");
    printf("Customer Name: %s\n", customerName);
    printf("Date & Time  : %s", ctime(&now));
    printf("-------------------------------------------------------------\n");
    printf("%-5s %-20s %-8s %-8s %-10s\n", "ID", "Item", "Price", "Qty", "Total");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < itemCount; i++) {
        if (items[i].quantity > 0) {
            items[i].total = items[i].price * items[i].quantity;
            subtotal += items[i].total;
            printf("%-5d %-20s %-8.2f %-8d %-10.2f\n", items[i].id, items[i].name, items[i].price, items[i].quantity, items[i].total);
        }
    }

    if (subtotal >= DISCOUNT_THRESHOLD) {
        discount = subtotal * DISCOUNT_RATE;
    }
    gstAmount = subtotal * GST_RATE;
    finalTotal = subtotal + gstAmount - discount + charity;

    printf("-------------------------------------------------------------\n");
    printf("Subtotal   : ₹%.2f\n", subtotal);
    printf("GST (5%%)   : ₹%.2f\n", gstAmount);
    printf("Discount   : ₹%.2f\n", discount);
    printf("Charity    : ₹%.2f\n", charity);
    printf("Payment by : ");
    switch (paymentMethod) {
        case 1: printf("Cash\n"); break;
        case 2: printf("Card\n"); break;
        case 3: printf("UPI\n"); break;
        default: printf("Unknown\n"); break;
    }
    printf("Final Total: ₹%.2f\n", finalTotal);
    printf("Thanks for shopping at Shop Lifters");
}

// Save Bill to File
void saveBillToFile(struct Item items[], int itemCount, float charity, int paymentMethod) {
    FILE *file = fopen("bill.txt", "w");
    if (file == NULL) {
        printf("Error saving bill to file!\n");
        return;
    }

    float subtotal = 0, gstAmount, discount = 0, finalTotal;
    time_t now;
    time(&now);

    fprintf(file, "\n------ GROCERY BILL ------\n");
    fprintf(file, "Customer Name: %s\n", customerName);
    fprintf(file, "Date & Time  : %s", ctime(&now));
    fprintf(file, "-------------------------------------------------------------\n");
    fprintf(file, "%-5s %-20s %-8s %-8s %-10s\n", "ID", "Item", "Price", "Qty", "Total");
    fprintf(file, "-------------------------------------------------------------\n");

    for (int i = 0; i < itemCount; i++) {
        if (items[i].quantity > 0) {
            items[i].total = items[i].price * items[i].quantity;
            subtotal += items[i].total;
            fprintf(file, "%-5d %-20s %-8.2f %-8d %-10.2f\n", items[i].id, items[i].name, items[i].price, items[i].quantity, items[i].total);
        }
    }

    if (subtotal >= DISCOUNT_THRESHOLD) {
        discount = subtotal * DISCOUNT_RATE;
    }
    gstAmount = subtotal * GST_RATE;
    finalTotal = subtotal + gstAmount - discount + charity;

    fprintf(file, "-------------------------------------------------------------\n");
    fprintf(file, "Subtotal   : ₹%.2f\n", subtotal);
    fprintf(file, "GST (5%%)   : ₹%.2f\n", gstAmount);
    fprintf(file, "Discount   : ₹%.2f\n", discount);
    fprintf(file, "Charity    : ₹%.2f\n", charity);
    fprintf(file, "Payment by : ");
    switch (paymentMethod) {
        case 1: fprintf(file, "Cash\n"); break;
        case 2: fprintf(file, "Card\n"); break;
        case 3: fprintf(file, "UPI\n"); break;
        default: fprintf(file, "Unknown\n"); break;
    }
    fprintf(file, "Final Total: ₹%.2f\n", finalTotal);
    fprintf(file,"Thanks for shopping at Shop Lifters");

    fclose(file);
    printf("\nBill saved successfully to 'bill.txt'!\n");
}

// Search Item
void searchItem(struct Item items[], int itemCount) {
    char searchName[50];
    int found = 0;

    printf("\nEnter item name to search: ");
    scanf(" %[^\n]", searchName);

    for (int i = 0; i < itemCount; i++) {
        if (strstr(items[i].name, searchName) != NULL) {
            printf("ID: %d | Name: %s | Price: ₹%.2f\n", items[i].id, items[i].name, items[i].price);
            found = 1;
        }
    }

    if (!found) {
        printf("No item found with the name \"%s\".\n", searchName);
    }
}

// Select Payment Method
int selectPaymentMethod() {
    int method;
    printf("\nSelect Payment Method:\n");
    printf("1. Cash\n2. Card\n3. UPI\n");
    printf("Enter choice: ");
    scanf("%d", &method);
    return method;
}

//Thank You//