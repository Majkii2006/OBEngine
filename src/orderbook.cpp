#include "OrderBook.hpp"

OrderBook::OrderBook(std::string name)
    : m_name(name), m_bid_price(0.0), m_ask_price(0.0) {}

OrderBook::~OrderBook() = default;

OrderBook& OrderBook::getInstance(std::string& name) {
    static OrderBook instance(name);
    return instance;
}

void OrderBook::sort_orders_descending() {
    if (orders.size() > 1) {
        std::stable_sort(
            orders.begin(), orders.end(),
            [](const std::unique_ptr<Order>& a, const std::unique_ptr<Order>& b) {
                return a->get_price() > b->get_price();
            }
        );
    }
}

void OrderBook::sort_orders_ascending() {
    if (orders.size() > 1) {
        std::stable_sort(
            orders.begin(), orders.end(),
            [](const std::unique_ptr<Order>& a, const std::unique_ptr<Order>& b) {
                return a->get_price() < b->get_price();
            }
        );
    }
}

void OrderBook::update_bid_ask() {
    double actual_bid { 0.0 };
    double actual_ask { std::numeric_limits<double>::max() };

    if (!orders.empty()) {
        for (auto& it : orders) {
            if (it->get_side() == Order::Side::Buy) {
                if (it->get_price() > actual_bid)
                    actual_bid = it->get_price();
            }
            else {
                if (it->get_price() < actual_ask)
                    actual_ask = it->get_price();
            }
        }
        m_bid_price = actual_bid;
        m_ask_price = actual_ask;
    }
}

double OrderBook::average_order_price(double all_price, int quantity) {
    return static_cast<double>(all_price / quantity);
}

const std::string& OrderBook::get_name() const {
    return m_name;
}

const std::vector<std::unique_ptr<Order>>& OrderBook::get_orders() const {
    return orders;
}

int OrderBook::get_orders_size() const {
    return orders.size();
}

double OrderBook::get_ask_price() const {
    return m_ask_price;
}

double OrderBook::get_bid_price() const {
    return m_bid_price;
}

double OrderBook::calc_spread() const {
    return m_ask_price - m_bid_price;
}

void OrderBook::market_order_buy(int quantity) {
    if (quantity == 0) {
        std::cout << "\nNothing to buy\n";
        return;
    }

    int initial_quantity = quantity;
    int all_realized_quantity = 0;
    double price_sum = 0;

    for (auto it = orders.rbegin(); it != orders.rend(); ++it) {
        if ((*it)->get_side() == Order::Side::Sell) {
            if (quantity >= (*it)->get_quantity()) {
                price_sum += (*it)->get_price() * (*it)->get_quantity();
                all_realized_quantity += (*it)->get_quantity();
                quantity -= (*it)->get_quantity();
                delete_order((*it)->get_id());
            }
            else {
                (*it)->update_order_quantity(quantity, 0);
                price_sum += (*it)->get_price() * quantity;
                all_realized_quantity += quantity;
                quantity = 0;
                refresh_state();
                break;
            }
        }
    }

    std::cout << "Filled " << initial_quantity - quantity << " / " << initial_quantity
              << " units @ Avg. price: "
              << average_order_price(price_sum, all_realized_quantity) << "\n";
}

void OrderBook::market_order_sell(int quantity) {
    if (quantity == 0) {
        std::cout << "\nNothing to sell\n";
        return;
    }

    sort_orders_ascending();

    int initial_quantity = quantity;
    int all_realized_quantity = 0;
    double price_sum = 0;

    for (auto it = orders.rbegin(); it != orders.rend(); ++it) {
        if ((*it)->get_side() == Order::Side::Buy) {
            if (quantity >= (*it)->get_quantity()) {
                price_sum += (*it)->get_price() * (*it)->get_quantity();
                all_realized_quantity += (*it)->get_quantity();
                quantity -= (*it)->get_quantity();
                delete_order((*it)->get_id());
            }
            else {
                (*it)->update_order_quantity(quantity, 0);
                price_sum += (*it)->get_price() * quantity;
                all_realized_quantity += quantity;
                quantity = 0;
                refresh_state();
                break;
            }
        }
    }

    std::cout << "Filled " << initial_quantity - quantity << " / " << initial_quantity
              << " units @ Avg. price: "
              << average_order_price(price_sum, all_realized_quantity) << "\n";
}

void OrderBook::limit_order_buy(int quantity, double price) {
	if (quantity == 0) {
		std::cout << "Nothing to buy" << std::endl;
	}
}

void OrderBook::limit_order_sell(int quantity, double price) {
	if (quantity == 0) {
		std::cout << "Nothing to buy" << std::endl;
	}
}

void OrderBook::add_order(std::unique_ptr<Order>& order) {
    orders.push_back(std::move(order));
    sort_orders_descending();
    update_bid_ask();
}

void OrderBook::refresh_state() {
    sort_orders_descending();
    update_bid_ask();
    calc_spread();
}

void OrderBook::delete_order(size_t id) {
    for (auto it = orders.begin(); it != orders.end(); ++it) {
        if ((*it)->get_id() == id) {
            it->reset();
            orders.erase(it);
            return;
        }
    }
    update_bid_ask();
    sort_orders_descending();
}

void OrderBook::print_all_orders() const {
    std::cout << "\n======= ORDERS FOR '" << get_name() << "' =======\n";

    bool any_sell = false;

    for (auto& order : orders) {
        if (order->get_side() == Order::Side::Sell) {
            std::cout << RED << "\nID: " << order->get_id() << " (Sell)" << RESET << "\n";
            std::cout << "Price: " << order->get_price() << "\n";
            std::cout << "Quantity: " << order->get_quantity() << "\n";
            any_sell = true;
        }
    }

    if (any_sell) {
        std::cout << YELLOW << "\n--------------- Spread: "
                  << calc_spread() << " pips " << RESET << "\n";
    }

    for (auto& order : orders) {
        if (order->get_side() == Order::Side::Buy) {
            std::cout << GREEN << "\nID: " << order->get_id() << " (Buy)" << RESET << "\n";
            std::cout << "Price: " << order->get_price() << "\n";
            std::cout << "Quantity: " << order->get_quantity() << "\n";
        }
    }
}

