#pragma once
#include <cstdint>

#include <functional>
#include <iterator>
#include <stdexcept>

#ifndef OB_CLASS_H
#define OB_CLASS_H


#include <cmath>

#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>


#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */


namespace ID {
	size_t get_unique_id() {
		static size_t id {};
		id++;
		return id+1000;
	}
}



class Order {

	public:
		enum class Side { Sell, Buy };
		enum class Type { Market, Limit, Stop, FOK, IOC}; //Fill or Kill, Immediate or Cancel

	private:
		size_t m_id { };
		Side m_side { };
		double m_price { };
		int m_quantity { };
		Type m_type { };

	public:
		Order(size_t id, Side side, double price, int quantity, Type type) : 
			m_id(id), m_side(side), m_price(price), m_quantity(quantity), m_type(type) {};
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

		Type get_type() const {
			return m_type;
		}

		int update_order_quantity(int quantity, int operation_type) {
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
};

class OrderBook {
	private:
		std::string m_name { };
		std::vector<std::unique_ptr<Order>> orders;

		double m_bid_price { };
		double m_ask_price { };

		OrderBook(std::string name) : m_name(name) {}
	
		void sort_orders() {
			if (orders.size() > 1){
				std::stable_sort(orders.begin(), orders.end(), 
						[](const std::unique_ptr<Order>& a, const std::unique_ptr<Order>& b) {
								return a->get_price() > b->get_price();}
						);
			}
		}




		void update_bid_ask() {
			double actual_bid { 0.0 };
			double actual_ask { std::numeric_limits<double>::max() }; 
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
		~OrderBook() = default;

		static OrderBook& getInstance(std::string& name) {
			static OrderBook instance(name);
			return instance;
		}

		OrderBook(const OrderBook& other) = delete;
		void operator=(const OrderBook& other) = delete;

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

		double calc_spread() const {
			return m_ask_price - m_bid_price;	
		}
		

		void market_order_buy(int quantity) {
			if (quantity == 0) {
				std::cout << "\nNothing to buy" << std::endl;
				return;
			}
			int initial_quantity { quantity };
			for (auto it { orders.rbegin() }; it != orders.rend(); ++it) {
				if ( (*it)->get_side() == Order::Side::Sell ) {
					if (quantity >= (*it)->get_quantity()) {
						quantity = quantity - (*it)->get_quantity();
						delete_order((*it)->get_id());
						refresh_state();
					} 
					else if (quantity < (*it)->get_quantity()) {
						(*it)->update_order_quantity(quantity, 0);
						refresh_state();
						return;

					}
					
				}
			} 
			std::cout << "Filled " << initial_quantity - quantity << " / "<< initial_quantity << 
				" units " << "@" << std::endl;
		}

		void market_order_sell(int quantity) {

		}
		
		void add_order(std::unique_ptr<Order>& order) {
				// Jesli mamy taka sama cene oraz taki sam typ to trzeba zmienic wolumen juz istniejacego w vectorze orderu a nie nowy
				// ale to zajme sie tym na dalszym etapie

				// Wrzucenie na koniec vectora wskaznik do obiektu
				orders.push_back(std::move(order));
				// Checkowanie jesli ordery mają to samo price, tą samą strone i ten sam typ -> wtedy łączymy
				// some function	
				// Sortowanie orderow
				sort_orders();
				// Aktualizuje ceny bid i ask				
				update_bid_ask();
				
			
		}

		void refresh_state() {
			sort_orders();
			update_bid_ask();
			calc_spread();
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
			sort_orders();
		}

		void print_all_orders() const {
			std::cout << "\n======= ORDERS FOR " << "'"<< get_name() << "'"<< "=======" << std::endl;
			//std::cout << "\n 	Current ASK price: " << get_ask_price() << std::endl;
			//std::cout << "\n	Current BID price: " << get_bid_price() << std::endl;
			bool any_sell { false };
			for (auto &order : orders) {
				if ( order->get_side() == Order::Side::Sell){
					std::cout << RED <<"\nID: " << order->get_id() << " (Sell)"<< RESET << std::endl;
					std::cout << "Price: " << order->get_price() << std::endl;
					std::cout << "Quantity: " << order->get_quantity() << std::endl;
					any_sell = true;

				}
			}
			if (any_sell) {
				std::cout << YELLOW <<"\n---------------" << " Spread: " << calc_spread() <<" pts."<< RESET << std::endl;
			}	

			for (auto &order : orders) {
				if (order->get_side() == Order::Side::Buy) {
					std::cout << GREEN << "\nID: " << order->get_id() << " (Buy)"<< RESET << std::endl;
					std::cout << "Price: " << order->get_price() << std::endl;
					std::cout << "Quantity: " << order->get_quantity() << std::endl;
				}
			}
			
		}
};

#endif



