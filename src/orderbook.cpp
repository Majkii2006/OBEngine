#include <chrono>
#include <limits>
#include <memory>

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
        if (actual_ask == std::numeric_limits<double>::max() ){
        	actual_ask = 0;
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
    auto start = std::chrono::high_resolution_clock::now();
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
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Filled " << initial_quantity - quantity << " / " << initial_quantity
              << " units @ Avg. price: "
              << average_order_price(price_sum, all_realized_quantity) << "\n";
    std::cout << "Time taken: " << duration.count() << " microseconds" << std::endl;
}

void OrderBook::market_order_sell(int quantity) {
    auto start = std::chrono::high_resolution_clock::now();
    if (quantity <= 0) {
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
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Filled " << initial_quantity - quantity << " / " << initial_quantity
              << " units @ Avg. price: "
              << average_order_price(price_sum, all_realized_quantity) << "\n";
    std::cout << "Time taken: " << duration.count() << " microseconds" << std::endl;
}

void OrderBook::limit_order_buy(int quantity, double price) {
	auto start = std::chrono::high_resolution_clock::now();
	if (quantity <= 0) {
		std::cout << "\nNothing to buy" << std::endl;
		return;
	}

	if (price < get_ask_price()) {
		std::cout << "\nAdding to the passive buyers..." 
			<< std::endl;
		std::unique_ptr<Order> order = std::make_unique<Order>
			(ID::get_unique_id(), Order::Side::Buy, price, quantity, Order::Type::Limit);
		std::cout << "Created new order with ID: " << order->get_id() << std::endl;
		add_order(order);
		return;
	}

	int initial_quantity { quantity };
	int all_realized_quantity = 0;
    	double price_sum = 0;

	for (auto it { orders.rbegin() }; it != orders.rend(); ++it){
		if ((*it)->get_side() == Order::Side::Sell && price >= (*it)->get_price()) {
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
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    	std::cout << "\nFilled " << initial_quantity - quantity << " / " << initial_quantity
              << " units @ Avg. price: "
              << average_order_price(price_sum, all_realized_quantity) << "\n";
	std::cout << "Time taken: " << duration.count() << " microseconds" << std::endl;
	if ( all_realized_quantity != initial_quantity ) {
		//If partially filled add to orderbook as passive buyer:
		int left_quantity { initial_quantity - all_realized_quantity };
		std::unique_ptr<Order> order = std::make_unique<Order>
			(ID::get_unique_id(), Order::Side::Buy, price, left_quantity, Order::Type::Limit);
		add_order(order);
		
	}
	
}

void OrderBook::limit_order_sell(int quantity, double price) {
	auto start = std::chrono::high_resolution_clock::now();
	if (quantity <= 0) {
		std::cout << "\nNothing to buy" << std::endl;
		return;
	}
	if (price > get_bid_price()) {
		std::cout << "\nAdding to the passive buyers..." 
			<< std::endl;
		std::unique_ptr<Order> order = std::make_unique<Order>
			(ID::get_unique_id(), Order::Side::Sell, price, quantity, Order::Type::Limit);
		std::cout << "Created new order with ID: " << order->get_id() << std::endl;
		add_order(order);
		return;
	}
	int initial_quantity { quantity };
	int all_realized_quantity = 0;
    	double price_sum = 0;
	
	sort_orders_ascending();

	for (auto it { orders.rbegin() }; it != orders.rend(); ++it){
		if ((*it)->get_side() == Order::Side::Buy && price <= (*it)->get_price()) {
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
	auto stop = std::chrono::high_resolution_clock::now(); 
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		std::cout << "\nFilled " << initial_quantity - quantity << " / " << initial_quantity
              << " units @ Avg. price: "
              << average_order_price(price_sum, all_realized_quantity) << "\n";
	std::cout << "Time taken: " << duration.count() << " microseconds" << std::endl;

	if ( all_realized_quantity != initial_quantity ) {
		//If partially filled add to orderbook as passive buyer:
		int left_quantity { initial_quantity - all_realized_quantity };
		std::unique_ptr<Order> order = std::make_unique<Order>
			(ID::get_unique_id(), Order::Side::Sell, price, left_quantity, Order::Type::Limit);
		add_order(order);
		
	}
}

void OrderBook::add_order(std::unique_ptr<Order>& order) {
    // Gdy ordery maja taki sam side i cene dodajemy quantity
    if (order->get_side() == Order::Side::Buy) {
    	    for (auto it { orders.begin() }; it != orders.end(); ++it) {
		if ((*it)->get_side() == Order::Side::Buy && (*it)->get_price() == order->get_price()) {
			(*it)->update_order_quantity(order->get_quantity(), 1);
			delete_order(order->get_id());
			return;
		} 
    	    }
    }
    else if (order->get_side() == Order::Side::Sell) {
    	    for (auto it { orders.begin() }; it != orders.end(); ++it){
		if ((*it)->get_side() == Order::Side::Sell && (*it)->get_price() == order->get_price()) {
			(*it)->update_order_quantity(order->get_quantity(), 1);
			delete_order(order->get_id());
			return;
		}
    	    }
    }

    orders.push_back(std::move(order));
    sort_orders_descending();
    update_bid_ask();
    calc_spread();
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

const std::string OrderBook::visualize_quantity(int quantity) const {
	std::string quant { " |" };
	const int CONST_BASE { 50 };
	int res_of_division { static_cast<int>(quantity/CONST_BASE) };
	if (res_of_division < 1) {
		return quant.append("■");
	}
	for (int i { 0 }; i < res_of_division; ++i) {
		quant.append("■");		
	}
	return quant;
}

void OrderBook::print_all_orders_compact() const {
	std::cout << "Price  |  Quant" << std::endl;
	std::cout << "==============" << std::endl;	
	for (auto& order : orders) {
		int temp_quantity { order->get_quantity() };
		if (order->get_side() == Order::Side::Sell) {
			std::cout << RED << BOLD << order->get_price() << RESET << "  " 
				<< order->get_quantity() << visualize_quantity(temp_quantity) << std::endl;

		}
	}

    	    if (get_ask_price() != 0 && get_bid_price() != 0) {
	    	    std::cout << YELLOW << "------------" << std::endl;
    	    }


	for (auto& order : orders) {
		int temp_quantity { order->get_quantity() };
		if (order->get_side() == Order::Side::Buy) {
			std::cout << GREEN << BOLD << order->get_price() << RESET << "  " 
				<< order->get_quantity() << visualize_quantity(temp_quantity) << std::endl;
		}
	}
	std::cout << "==============" << std::endl;	
}



void OrderBook::print_all_orders_full() const {
    std::cout << "\n======= ORDERS FOR '" << get_name() << "' =======\n";


    for (auto& order : orders) {
        if (order->get_side() == Order::Side::Sell) {
            std::cout << RED << "\nID: " << order->get_id() << " (Sell)" << RESET << "\n";
            std::cout << "Price: " << order->get_price() << "\n";
            std::cout << "Quantity: " << order->get_quantity() << "\n";
        }
    }

    if (get_ask_price() != 0 && get_bid_price() != 0) {
	    std::cout << YELLOW << "------------ Spread: " << calc_spread() << " pips" << RESET << std::endl;
    }

    for (auto& order : orders) {
        if (order->get_side() == Order::Side::Buy) {
            std::cout << GREEN << "\nID: " << order->get_id() << " (Buy)" << RESET << "\n";
            std::cout << "Price: " << order->get_price() << "\n";
            std::cout << "Quantity: " << order->get_quantity() << "\n";
        }
    }
}

