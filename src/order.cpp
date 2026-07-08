#include "OrderBook.hpp"

Order::Order(size_t id, Side side, double price, int quantity, Type type)
    : m_id(id), m_side(side), m_price(price), m_quantity(quantity), m_type(type) {}

Order::~Order() {}

size_t Order::get_id() const {
    return m_id;
}

Order::Side Order::get_side() const {
    return m_side;
}

double Order::get_price() const {
    return m_price;
}

int Order::get_quantity() const {
    return m_quantity;
}

Order::Type Order::get_type() const {
    return m_type;
}

int Order::update_order_quantity(int quantity, int operation_type) {
    if (operation_type == 1) {
        return m_quantity = m_quantity + quantity;
    } 
    else if (operation_type == 0) {
        return m_quantity = m_quantity - quantity;
    }
    else {
        return -1000;
    }
}

