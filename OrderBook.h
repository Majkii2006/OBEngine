#pragma once

#include <cmath>

#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

class Order {

	public:
		enum class Side { Sell, Buy };

	private:
		size_t m_id { };
		Side m_side { };
		double m_price { };
		int m_quantity { };	
	public:
		Order(size_t id, Side side, double price, int quantity) : 
			m_id(id), m_side(side), m_price(price), m_quantity(quantity) {};
		~Order() { };
		
		size_t get_id() const {
			return m_id;
		}

		Side get_side() const { 
			return m_side;
		}

		double get_price() const {
			return m_price;
		}

		int get_quantity() const {
			return m_quantity;
		}
};

class OrderBook {
	private:
		std::string m_name { };
		std::vector<std::unique_ptr<Order>> orders;
	
		
		double m_bid_price { };
		double m_ask_price { };
		
		void update_bid_ask() {
			double actual_bid { 0.0 };
			double actual_ask { std::numeric_limits<double>::max() }; //nie do konca optymalne
			if (orders.size() > 0) {
				for (auto it { orders.begin() }; it != orders.end(); ++it) {
					if ( (*it)->get_side() == Order::Side::Buy) { // bid cena rynkowa sprzedazy -> jak najwieksza wsrod ofert kupna
						if ((*it)->get_price() > actual_bid){
							actual_bid = (*it)->get_price();
						}	
					}
					else {
						if ((*it)->get_price() < actual_ask) { // ask cena rynkowa kupna -> jak najnizsza wsrod ofert sprzedazy
							actual_ask = (*it)->get_price();	
						}

					}
				}
				m_bid_price = actual_bid;
				m_ask_price = actual_ask;
			}
			
		}


	public:
		OrderBook(std::string name) : m_name(name) {}
		~OrderBook() = default;

		const std::string& get_name() const {
			return m_name;
		}
		
		const std::vector<std::unique_ptr<Order>>& get_orders() const {
			return orders;
		}

		int get_orders_size() const {
			return orders.size();
		}

		double get_ask_price() const {
			return m_ask_price;
		}
		
		double get_bid_price() const {
			return m_bid_price;
		}
		
		void add_order(std::unique_ptr<Order>& order) {
				orders.push_back(std::move(order));
				// then we need to check if we can fill it up instantly
				// is it neccessery to sort it?? maybe it's not
				update_bid_ask();		
			
		}

		void delete_order(size_t id) {
			for (auto it { orders.begin() }; it != orders.end(); ++it){
				if ((*it)->get_id() == id){
					(*it).reset();
					orders.erase(it);		
					return;
				}
			}			
			update_bid_ask();		
		}

		void print_all_orders() const {
			size_t counter { 1 };
			std::cout << "\n======= ORDERS FOR " << "'"<< get_name() << "'"<< "=======" << std::endl;
			std::cout << "\n 	Current ASK price: " << get_ask_price() << std::endl;
			std::cout << "\n	Current BID price: " << get_bid_price() << std::endl;
			for (auto &ptr : orders) {
				if ( ptr->get_side() == Order::Side::Sell){
					std::cout << "\nOrder number: " << counter << std::endl;
					std::cout << "Order ID: " << ptr->get_id() << std::endl;
					std::cout << ptr->get_price() << " ";
					std::cout << ptr->get_quantity() << " ";
					std::cout << "Sell" << std::endl;	
					counter++;
				}
			}
			for (auto &ptr : orders) {
				if (ptr->get_side() == Order::Side::Buy) {
					std::cout << "\nOrder number: " << counter << std::endl;
					std::cout << "Order ID: " << ptr->get_id() << std::endl;
					std::cout << ptr->get_price() << " ";
					std::cout << ptr->get_quantity() << " ";
					std::cout << "Buy" << std::endl;	
					counter++;
				}
			}
			
		}
			




};


namespace ID {
	size_t get_unique_id() {
		static size_t id {};
		id++;
		return id+1000;
	}
}


