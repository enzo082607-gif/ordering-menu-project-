#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <limits>
#include <fstream>

struct Item {
    int id;
    std::string name;
    double price;
};

void showMenu(const std::vector<Item>& menu) {
    std::cout << "\n---- MENU ----\n"; //shows the menu
    for (const auto &it : menu) { //shows the loop where it shows each item in the menu 
        std::cout << std::setw(2) << it.id << ". " << std::left << std::setw(20) << it.name
                  << " ₱ " << std::right << std::fixed << std::setprecision(2) << it.price << '\n'; //proper format of showing the menu
    }
}

int getInt(const std::string &prompt) {
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v) {
            return v;
        }
        std::cout << "Invalid input. Please enter a number." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void showCart(const std::map<int,int>& cart, const std::vector<Item>& menu) {
    if (cart.empty()) {
        std::cout << "Your cart is empty." << std::endl;
        return;
    }
    std::cout << "\n---- CART ----\n"; //prints the CART
    for (auto &p : cart) {
        int id = p.first; //the ID
        int qty = p.second; //quantity of the item
        const Item* itPtr = nullptr;
        for (const auto &it : menu) if (it.id == id) { itPtr = &it; break; }
        if (!itPtr) continue; //searches the menu vector to find the Item with matching ID
        double line = qty * itPtr->price; //multiplies the total price for that Item
        std::cout << std::setw(2) << qty << " x " << std::left << std::setw(18) << itPtr->name
                  << " ₱ " << std::right << std::fixed << std::setprecision(2) << line << '\n'; //to show proper alignment
    }
}

void printReceipt(const std::map<int,int>& cart, const std::vector<Item>& menu, double taxRate) { //takes the cart,menu, tax
    std::cout << "\n---- RECEIPT ----\n"; //prints the receipt
    double subtotal = 0.0; //accumalate the prices of all items in the cart
    for (auto &p : cart) {
        int id = p.first;
        int qty = p.second; //retrieves the ID & qty from the cart
        const Item* itPtr = nullptr;
        for (const auto &it : menu) if (it.id == id) { itPtr = &it; break; }
        if (!itPtr) continue;
        double line = qty * itPtr->price;
        subtotal += line;
        std::cout << std::setw(2) << qty << " x " << std::left << std::setw(18) << itPtr->name
                  << " ₱ " << std::right << std::fixed << std::setprecision(2) << line << '\n';
    }
    if (subtotal <= 0.0) {
        std::cout << "No items ordered." << std::endl; //if subtotal is 0.0 or less it means no iterms were ordered
        return;
    }
    double tax = subtotal * taxRate; //calculates the tax amount by subtotal * tax rate
    double total = subtotal + tax; //calculates the total amount by subtotal+tax
    std::cout << std::string(40, '-') << std::endl;
    std::cout << std::left << std::setw(26) << "Subtotal:" << " ₱ " << std::right << std::fixed << std::setprecision(2) << subtotal << std::endl;
    std::cout << std::left << std::setw(26) << ("Tax (" + std::to_string(int(taxRate*100)) + "%):") << " ₱ " << std::right << std::fixed << std::setprecision(2) << tax << std::endl;
    std::cout << std::left << std::setw(26) << "Total:" << " ₱ " << std::right << std::fixed << std::setprecision(2) << total << std::endl;
} //prints the subtotal, tax, total with proper formatting 

int main() {
    std::vector<Item> menu = {
        {1, "Chicken", 150.00},
        {2, "Pizza", 450.00},
        {3, "Ice Cream", 80.00},
        {4, "Burger", 140.00}
    }; //using vector as the menu that holds the items, this menu vector shows the items

    std::map<int,int> cart; // id -> quantity // this cart/map stores the items thats been added
    const double TAX_RATE = 0.12; // 12% //constant double var/TAX RATE initialize to 0.12% 

    std::cout << "Welcome to the Ordering Menu" << std::endl; //shows Welcome to the Ordering Menu

    while (true) { //infinite loop 
        std::cout << "\nOptions:\n";
        std::cout << " 1) Show Menu and Add Item\n";
        std::cout << " 2) View Cart\n";
        std::cout << " 3) Remove Item from Cart\n";
        std::cout << " 4) Checkout\n";
        std::cout << " 0) Exit without buying\n"; //interactives things inside the loop

        int opt = getInt("Choose an option: "); 
        if (opt == 0) {
            std::cout << "Goodbye!" << std::endl; //if 0 is chosen farewell message is printed
            break;
        }

        if (opt == 1) {
            showMenu(menu); //display the list of the items
            int choice = getInt("Enter item number to add (0 to cancel): "); //prompts the user to enter the item number they want to add using getInt the user can enter 0
            if (choice == 0) continue; //if 0 is chosen it skips the rest of the code and return to the inside loop
            const Item* selected = nullptr;
            for (const auto &it : menu) if (it.id == choice) { selected = &it; break; }
            if (!selected) { std::cout << "Invalid item number." << std::endl; continue; } //if selected is still nullptr it shows invalid item number  
            int qty = getInt("Enter quantity: "); //prompts the user to enter the quantity of the selected items using getInt
            if (qty <= 0) { std::cout << "Quantity must be positive." << std::endl; continue; } //if it enters a zero or negative, a error message is printed 
            cart[selected->id] += qty;
            std::cout << qty << " x " << selected->name << " added to cart." << std::endl; //updates the cart
        }
        else if (opt == 2) {
            showCart(cart, menu); //cart menu
        }
        else if (opt == 3) { //if 3 is chosen which is Remove Item from Cart
            if (cart.empty()) { std::cout << "Cart is empty." << std::endl; continue; } //if the cart has no contents informs the user to restart the loop
            showCart(cart, menu); //shows the current cart contents 
            int rem = getInt("Enter item id to remove (0 to cancel): "); //prompts the user to enter the item ID they want to remove
            if (rem == 0) continue; //if 0 is chosen cancel removal and restart the loop
            auto it = cart.find(rem);
            if (it == cart.end()) { std::cout << "Item not in cart." << std::endl; continue; } //search the item by its ID, if not found restart the loop
            int qty = getInt("Enter quantity to remove (enter 0 to remove all): "); //ask the user how many items they want to remove
            if (qty <= 0) { cart.erase(it); std::cout << "Item removed from cart." << std::endl; } 
            else {
                if (qty >= it->second) { cart.erase(it); std::cout << "Item removed from cart." << std::endl; }  //updates the cart 
                else { it->second -= qty; std::cout << "Removed " << qty << " units." << std::endl; }
            }
        }
        else if (opt == 4) {
            if (cart.empty()) { std::cout << "Cart is empty. Add items before checkout." << std::endl; continue; } //if cart is empty return to the loop
            printReceipt(cart, menu, TAX_RATE); //calls the printReceipt to display the receipt of the user
            std::cout << "\nProceed to confirm purchase? (1 = yes, 0 = no): ";
            int confirm = getInt("");
            if (confirm == 1) {
                std::cout << "Purchase confirmed. Thank you!" << std::endl; //1= purchased confirmed thank you!
                // Optionally save receipt to file
                std::ofstream out("receipt.txt"); //save the receipt details to a file receipt.txt
                if (out) {
                    out << "RECEIPT\n";
                    double subtotal = 0.0;
                    for (auto &p : cart) {
                        int id = p.first; int q = p.second;
                        const Item* itPtr = nullptr;
                        for (const auto &it : menu) if (it.id == id) { itPtr = &it; break; }
                        if (!itPtr) continue;
                        double line = q * itPtr->price; subtotal += line;
                        out << q << " x " << itPtr->name << " PHP " << std::fixed << std::setprecision(2) << line << '\n';
                    }
                    double tax = subtotal * TAX_RATE; double total = subtotal + tax;
                    out << "------------------------------\n";
                    out << "Subtotal: PHP " << std::fixed << std::setprecision(2) << subtotal << '\n';
                    out << "Tax (" << int(TAX_RATE*100) << "%): PHP " << tax << '\n';
                    out << "Total: PHP " << total << '\n';
                    out.close(); //closes the file using out.close
                    std::cout << "Receipt saved to receipt.txt" << std::endl;
                } //
                break;
            } else {
                std::cout << "Checkout canceled." << std::endl;
            } //if the receipt was succesfully saved exit the main loop, if the user does not confirm checkout canceled
        }
        else {
            std::cout << "Unknown option. Choose 0-4." << std::endl;
        } //if the user enters invalid option 0,1,2,3,4 print an error message
    }

    return 0; //means successful completion
}
