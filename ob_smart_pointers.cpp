#include <algorithm>
#include <iostream>
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
			
		}

		void delete_order(size_t id) {
			for (auto it { orders.begin() }; it != orders.end(); ++it){
				if ((*it)->get_id() == id){
					(*it).reset();
					orders.erase(it);		
					return;
				}
			}							
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

int main() {

	OrderBook ob("OrderBook");
	// These pointers have ownership only for easy making an object, its only for a while	
	std::unique_ptr<Order> order1 = std::make_unique<Order>(Order(ID::get_unique_id(), Order::Side::Buy, 4180.50, 22)); 
	std::unique_ptr<Order> order2 = std::make_unique<Order>(Order(ID::get_unique_id(), Order::Side::Buy, 4180.75, 15));
	std::unique_ptr<Order> order3 = std::make_unique<Order>(Order(ID::get_unique_id(), Order::Side::Sell, 4181.00, 18)); //
	std::unique_ptr<Order> order4 = std::make_unique<Order>(Order(ID::get_unique_id(), Order::Side::Sell, 4181.25, 5)); 
	std::unique_ptr<Order> order5 = std::make_unique<Order>(Order(ID::get_unique_id(), Order::Side::Buy, 4180.25, 20));
	std::unique_ptr<Order> order6 = std::make_unique<Order>(Order(ID::get_unique_id(), Order::Side::Buy, 4180.60, 2));
	std::unique_ptr<Order> order7 = std::make_unique<Order>(Order(ID::get_unique_id(), Order::Side::Buy, 4181.25, 20)); //
	
	// With these methods we are moving ownership to the vector of uniquepointers pointing to the each of the orders
	ob.add_order(order1);
	ob.add_order(order2);
	ob.add_order(order3);	
	ob.add_order(order4);
	ob.add_order(order5);
	ob.add_order(order6);
	ob.add_order(order7);

	ob.print_all_orders();

	ob.delete_order(1001);

	ob.print_all_orders();

	return 0;
}
