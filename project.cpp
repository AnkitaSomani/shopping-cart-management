#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
using namespace std;

struct Item {
    int srNo;
    int quantity;
    double price;
    string name;
};

struct Coupon {
    int cartValue;
    double discount;
};

class ShoppingCart {
private:
    unordered_map<int, Item> inventory;
    priority_queue<double> discountHeap;
    double cartTotal;

public:
    ShoppingCart() : cartTotal(0.0) {}

    void loadInventory(const string& filename) {
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            Item item;

            getline(ss, token, ',');
            item.srNo = stoi(token);
            getline(ss, token, ',');
            item.name = token;
            getline(ss, token, ',');
            item.quantity = stoi(token);
            getline(ss, token, ',');
            item.price = stod(token);

            inventory[item.srNo] = item;
        }
    }

    void loadDiscountCoupons(const string& filename) {
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            Coupon coupon;

            getline(ss, token, ',');
            coupon.cartValue = stoi(token);
            getline(ss, token, ',');
            coupon.discount = stod(token);

            discountHeap.push(coupon.discount);
        }
    }

    void displayStoreItems() {
        cout << "\nAvailable Items in Store:\n";
        cout << "------------------------------------------------------------------------------------------\n";
        cout << setw(10) << "SrNo"
             << setw(30) << "Name"
             << setw(20) << "Qty"
             << setw(20) << "Price\n";
        cout << "------------------------------------------------------------------------------------------\n";
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            const Item& item = it->second;
            cout << setw(10) << item.srNo
                 << setw(30) << item.name
                 << setw(20) << item.quantity
                 << setw(20) << item.price << "\n";
        }
        cout << "------------------------------------------------------------------------------------------\n\n";
    }

    void addItemToCart(int srNo, int quantity) {
        if (inventory.find(srNo) != inventory.end()) {
            string itemName = inventory[srNo].name;

            if (quantity <= 0) {
                cout << "Please enter a valid quantity" << endl;
                return;
            }

            if (inventory[srNo].quantity >= quantity) {
                cartTotal += inventory[srNo].price * quantity;
                inventory[srNo].quantity -= quantity;

                ofstream cartFile("cart.csv", ios::app);
                if (cartFile.is_open()) {
                    cartFile << srNo << ","
                             << itemName << ","
                             << quantity << ","
                             << inventory[srNo].price << "\n";
                    cartFile.close();
                }

                cout << "Added " << itemName << " (" << quantity << ") to the cart" << endl;
            } else {
                cout << "Insufficient quantity of " << itemName << endl;
                cout << "Only " << inventory[srNo].quantity << " available" << endl;
            }
        } else {
            cout << "Item does not exist" << endl;
        }
    }

    map<int, int> loadCartFromFile() {
        map<int, int> cart;
        ifstream cartFile("cart.csv");
        string line;

        while (getline(cartFile, line)) {
            stringstream ss(line);
            string token;
            int srNo, quantity;

            getline(ss, token, ',');
            srNo = stoi(token);
            getline(ss, token, ','); // skip name
            getline(ss, token, ',');
            quantity = stoi(token);

            cart[srNo] += quantity;
        }

        cartFile.close();
        return cart;
    }

    void removeItem(int srNo, int quantity) {
        auto cart = loadCartFromFile();

        if (quantity <= 0) {
            cout << "Please enter a valid quantity" << endl;
            return;
        }

        if (cart.find(srNo) != cart.end()) {
            string itemName = inventory[srNo].name;

            if (cart[srNo] >= quantity) {
                cart[srNo] -= quantity;
                cartTotal -= inventory[srNo].price * quantity;
                inventory[srNo].quantity += quantity;

                ofstream cartFile("cart.csv");
                for (map<int, int>::iterator it = cart.begin(); it != cart.end(); ++it) {
                    int serial = it->first;
                    int qty = it->second;

                    if (qty > 0) {
                        cartFile << serial << ","
                                 << inventory[serial].name << ","
                                 << qty << ","
                                 << inventory[serial].price << "\n";
                    }
                }

                cartFile.close();
                cout << "Removed " << itemName << " (" << quantity << ") from the cart" << endl;
            } else {
                cout << "Insufficient quantity of item in the cart!" << endl;
            }
        } else {
            cout << "Oops! Item not in cart!" << endl;
        }
    }

    void displayCart() {
        ifstream cartFile("cart.csv");
        string line;
        double total = 0.0;

        cout << "In your cart!:\n";
        cout << "------------------------------------------------------------------------------------------\n";
        cout << setw(10) << "SrNo"
             << setw(30) << "Name"
             << setw(20) << "Qty"
             << setw(20) << "Price\n";
        cout << "------------------------------------------------------------------------------------------\n";

        while (getline(cartFile, line)) {
            stringstream ss(line);
            string token;
            int srNo, quantity;
            double price;
            string name;

            getline(ss, token, ',');
            srNo = stoi(token);
            getline(ss, token, ',');
            name = token;
            getline(ss, token, ',');
            quantity = stoi(token);
            getline(ss, token, ',');
            price = stod(token);

            cout << setw(10) << srNo
                 << setw(30) << name
                 << setw(20) << quantity
                 << setw(20) << price << "\n";
            total += price * quantity;
        }

        cartFile.close();
        cout << "------------------------------------------------------------------------------------------\n";
        cout << "Cart Total: Rs." << fixed << setprecision(2) << total << "\n";
    }

    void applyDiscount() {
        while (!discountHeap.empty()) {
            double discount = discountHeap.top();

            if (cartTotal >= discount) {
                cartTotal -= discount;
                cout << "Applied discount of Rs." << discount << endl;
                cout << "New Total: Rs." << fixed << setprecision(2) << cartTotal << "\n";
                return;
            } else {
                discountHeap.pop();
            }
        }

        cout << "No applicable discount" << endl;
    }

    void checkout() {
        displayCart();
        applyDiscount();
        cout << "Checking out..." << endl;
        cout << "Please pay Rs." << fixed << setprecision(2) << cartTotal << endl;

        ofstream cartFile("cart.csv", ios::trunc);
        cartFile.close();
    }

    void userInteraction(int &cnt) {
        int choice;
        cnt = 0;

        do {
            displayStoreItems();
            cout << "\nSelect an option:\n";
            cout << "1. Add Item to Cart\n";
            cout << "2. Remove Item from Cart\n";
            cout << "3. View Cart\n";
            cout << "4. Checkout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                cnt++;
                continue;
            }

            switch (choice) {
                case 1: {
                    int srNo, quantity;
                    cout << "Enter the serial number of the item to add: ";
                    cin >> srNo;
                    cout << "Enter the quantity to add: ";
                    cin >> quantity;
                    addItemToCart(srNo, quantity);
                    break;
                }
                case 2: {
                    int srNo, quantity;
                    cout << "Enter the serial number of the item to remove: ";
                    cin >> srNo;
                    cout << "Enter the quantity to remove: ";
                    cin >> quantity;
                    removeItem(srNo, quantity);
                    break;
                }
                case 3:
                    displayCart();
                    break;
                case 4:
                    checkout();
                    cout << "Thank you for shopping!" << endl;
                    break;
                default:
                    cnt++;
                    cout << "Please enter a valid choice" << endl;
                    break;
            }

            if (choice >= 1 && choice <= 4) cnt = 0;

            if (cnt >= 4) {
                cout << "Too many invalid attempts. Session terminated.\n";
                break;
            }
        } while (choice != 4);
    }
};

int main() {
    ShoppingCart cart;
    int invalidInputCount = 0;

    cart.loadInventory("inventory.csv");
    cart.loadDiscountCoupons("coupons.csv");

    cart.userInteraction(invalidInputCount);

    if (invalidInputCount >= 4) {
        cout << "Too many invalid attempts. Exiting..." << endl;
    }

    return 0;
}
