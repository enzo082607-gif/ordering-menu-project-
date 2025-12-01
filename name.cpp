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
    std::cout << "\n---- MENU ----\n";
    for (const auto &it : menu) {
        std::cout << std::setw(2) << it.id << ". " << std::left << std::setw(20) << it.name
                  << " ₱ " << std::right << std::fixed << std::setprecision(2) << it.price << '\n';
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
    std::cout << "\n---- CART ----\n";
    for (auto &p : cart) {
        int id = p.first;
        int qty = p.second;
        const Item* itPtr = nullptr;
        for (const auto &it : menu) if (it.id == id) { itPtr = &it; break; }
        if (!itPtr) continue;
        double line = qty * itPtr->price;
        std::cout << std::setw(2) << qty << " x " << std::left << std::setw(18) << itPtr->name
                  << " ₱ " << std::right << std::fixed << std::setprecision(2) << line << '\n';
    }
}

void printReceipt(const std::map<int,int>& cart, const std::vector<Item>& menu, double taxRate) {
    std::cout << "\n---- RECEIPT ----\n";
    double subtotal = 0.0;
    for (auto &p : cart) {
        int id = p.first;
        int qty = p.second;
        const Item* itPtr = nullptr;
        for (const auto &it : menu) if (it.id == id) { itPtr = &it; break; }
        if (!itPtr) continue;
        double line = qty * itPtr->price;
        subtotal += line;
        std::cout << std::setw(2) << qty << " x " << std::left << std::setw(18) << itPtr->name
                  << " ₱ " << std::right << std::fixed << std::setprecision(2) << line << '\n';
    }
    if (subtotal <= 0.0) {
        std::cout << "No items ordered." << std::endl;
        return;
    }
    double tax = subtotal * taxRate;
    double total = subtotal + tax;
    std::cout << std::string(40, '-') << std::endl;
    std::cout << std::left << std::setw(26) << "Subtotal:" << " ₱ " << std::right << std::fixed << std::setprecision(2) << subtotal << std::endl;
    std::cout << std::left << std::setw(26) << ("Tax (" + std::to_string(int(taxRate*100)) + "%):") << " ₱ " << std::right << std::fixed << std::setprecision(2) << tax << std::endl;
    std::cout << std::left << std::setw(26) << "Total:" << " ₱ " << std::right << std::fixed << std::setprecision(2) << total << std::endl;
}

int main() {
    std::vector<Item> menu = {
        {1, "Chicken", 150.00},
        {2, "Pizza", 450.00},
        {3, "Ice Cream", 80.00},
        {4, "Burger", 140.00}
    };

    std::map<int,int> cart; // id -> quantity
    const double TAX_RATE = 0.12; // 12%

    std::cout << "Welcome to the Ordering Menu" << std::endl;

    while (true) {
        std::cout << "\nOptions:\n";
        std::cout << " 1) Show Menu and Add Item\n";
        std::cout << " 2) View Cart\n";
        std::cout << " 3) Remove Item from Cart\n";
        std::cout << " 4) Checkout\n";
        std::cout << " 0) Exit without buying\n";

        int opt = getInt("Choose an option: ");
        if (opt == 0) {
            std::cout << "Goodbye!" << std::endl;
            break;
        }

        if (opt == 1) {
            showMenu(menu);
            int choice = getInt("Enter item number to add (0 to cancel): ");
            if (choice == 0) continue;
            const Item* selected = nullptr;
            for (const auto &it : menu) if (it.id == choice) { selected = &it; break; }
            if (!selected) { std::cout << "Invalid item number." << std::endl; continue; }
            int qty = getInt("Enter quantity: ");
            if (qty <= 0) { std::cout << "Quantity must be positive." << std::endl; continue; }
            cart[selected->id] += qty;
            std::cout << qty << " x " << selected->name << " added to cart." << std::endl;
        }
        else if (opt == 2) {
            showCart(cart, menu);
        }
        else if (opt == 3) {
            if (cart.empty()) { std::cout << "Cart is empty." << std::endl; continue; }
            showCart(cart, menu);
            int rem = getInt("Enter item id to remove (0 to cancel): ");
            if (rem == 0) continue;
            auto it = cart.find(rem);
            if (it == cart.end()) { std::cout << "Item not in cart." << std::endl; continue; }
            int qty = getInt("Enter quantity to remove (enter 0 to remove all): ");
            if (qty <= 0) { cart.erase(it); std::cout << "Item removed from cart." << std::endl; }
            else {
                if (qty >= it->second) { cart.erase(it); std::cout << "Item removed from cart." << std::endl; }
                else { it->second -= qty; std::cout << "Removed " << qty << " units." << std::endl; }
            }
        }
        else if (opt == 4) {
            if (cart.empty()) { std::cout << "Cart is empty. Add items before checkout." << std::endl; continue; }
            printReceipt(cart, menu, TAX_RATE);
            std::cout << "\nProceed to confirm purchase? (1 = yes, 0 = no): ";
            int confirm = getInt("");
            if (confirm == 1) {
                std::cout << "Purchase confirmed. Thank you!" << std::endl;
                // Optionally save receipt to file
                std::ofstream out("receipt.txt");
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
                    out.close();
                    std::cout << "Receipt saved to receipt.txt" << std::endl;
                }
                break;
            } else {
                std::cout << "Checkout canceled." << std::endl;
            }
        }
        else {
            std::cout << "Unknown option. Choose 0-4." << std::endl;
        }
    }

    return 0;
}
