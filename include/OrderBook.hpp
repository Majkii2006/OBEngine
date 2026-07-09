#pragma once
#include <cmath>
#include <cstdint>

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef OB_CLASS_H
#define OB_CLASS_H

#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

namespace ID {
    size_t get_unique_id();   // deklaracja
}

class Order {
public:
    enum class Side { Sell, Buy };
    enum class Type { Market, Limit, Stop, FOK, IOC };

private:
    size_t m_id;
    Side m_side;
    double m_price;
    int m_quantity;
    Type m_type;

public:
    Order(size_t id, Side side, double price, int quantity, Type type);
    ~Order();

    size_t get_id() const;
    Side get_side() const;
    double get_price() const;
    int get_quantity() const;
    Type get_type() const;

    int update_order_quantity(int quantity, int operation_type);
};

class OrderBook {
private:
    std::string m_name;
    std::vector<std::unique_ptr<Order>> orders;

    double m_bid_price;
    double m_ask_price;

    OrderBook(std::string name);

    void sort_orders_descending();
    void sort_orders_ascending();
    void update_bid_ask();
    double average_order_price(double all_price, int quantity);

public:
    ~OrderBook();

    static OrderBook& getInstance(std::string& name);

    OrderBook(const OrderBook& other) = delete;
    void operator=(const OrderBook& other) = delete;

    const std::string& get_name() const;
    const std::vector<std::unique_ptr<Order>>& get_orders() const;
    int get_orders_size() const;

    double get_ask_price() const;
    double get_bid_price() const;
    double calc_spread() const;

    void market_order_buy(int quantity);
    void market_order_sell(int quantity);

    void limit_order_buy(int quantity, double price);
    void limit_order_sell(int quantity, double price);

    void add_order(std::unique_ptr<Order>& order);
    void refresh_state();
    void delete_order(size_t id);
    void print_all_orders() const;

};

#endif

