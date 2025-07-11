# 🛒 Shopping Cart Management System (C++)

A simple **Shopping Cart Management System** implemented in **C++**, using file handling (`CSV` files), maps, and priority queues to simulate adding, removing, and purchasing items with discount coupons.

---

## Features

- Load inventory from a CSV file
- Load discount coupons from a CSV file
- Add items to cart
- Remove items from cart
- View current cart with calculated total
- Apply the best discount available during checkout
- Store cart items in a CSV file
- Input validation and session termination after repeated invalid inputs

---

## CSV File Formats

### `inventory.csv`
Stores the available store inventory.

Example:
1,Pen,50,10.5
2,Notebook,30,45.0
3,Eraser,100,5.0

Format:
SrNo,ItemName,QuantityAvailable,Price

---

### `coupons.csv`
Stores the available discount coupons.

Example:
100,10
200,25
500,50

Format:

---

## How to Run

1. Make sure you have a **C++17** or later compiler (e.g., GCC 7+).
2. Place your `inventory.csv` and `coupons.csv` files in the same folder as your `.cpp` file.
3. Compile:
   ```bash
   g++ -std=c++17 shopping_cart.cpp -o shopping_cart





