#pragma once
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
};

class OrderBook {
	private:
		std::string m_name { };
		std::vector<std::unique_ptr<Order>> orders;

		double m_bid_price { };
		double m_ask_price { };
	
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
			size_t counter { 1 };
			std::cout << "\n======= ORDERS FOR " << "'"<< get_name() << "'"<< "=======" << std::endl;
			std::cout << "\n 	Current ASK price: " << get_ask_price() << std::endl;
			std::cout << "\n	Current BID price: " << get_bid_price() << std::endl;
			for (auto &order : orders) {
				if ( order->get_side() == Order::Side::Sell){
					std::cout << "\nOrder number: " << counter << std::endl;
					std::cout << "Order ID: " << order->get_id() << std::endl;
					std::cout << order->get_price() << " ";
					std::cout << order->get_quantity() << " ";
					std::cout << "Sell" << std::endl;	
					counter++;
				}
			}
			for (auto &order : orders) {
				if (order->get_side() == Order::Side::Buy) {
					std::cout << "\nOrder number: " << counter << std::endl;
					std::cout << "Order ID: " << order->get_id() << std::endl;
					std::cout << order->get_price() << " ";
					std::cout << order->get_quantity() << " ";
					std::cout << "Buy" << std::endl;	
					counter++;
				}
			}
			
		}
};

#endif



