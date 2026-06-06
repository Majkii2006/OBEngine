#include <iostream>
#include <string>
#include <vector>

class OrderBook {

	public:
	class Order {
		public:
			enum class Side { Sell, Buy };	

		private:
			int id_;
			Side side_;	
			double price_;
			int quantity_;
		public:
			Order(int id, Side side, double price, int quantity) {
				id_ = id;
				side_ = side;
				price_ = price;
				quantity_ = quantity;
			}
			~Order() = default;
	
			//Getters
			int get_id() const { return id_; }
			Side get_side() const { return side_; }
			double get_price() const { return price_; }
			int get_quantity() const { return quantity_; }
			
			//Order Methods
			void quantity_change(int amount) {
				if (amount == 0) {
					return;
				}
				if (amount > 0) {
					quantity_=+amount;
				}
				if (amount < 0) {
					quantity_=-amount;
				}
			}
		};

	private:
		std::string name_ { };
		std::vector<Order> orders;

	public: 
		OrderBook(std::string name) {
			name_ = name;
		};
		
		void add_order(const Order& order){
			// orders.push_back(order); //tutaj jest blad, push_back -> kopia
			//orders.emplace_back(order.get_id(), order.get_side(),
			//		order.get_price(), order.get_quantity());
			orders.push_back(order);
		} 
		
		const std::string& get_name() const {
			return name_;
		}

		const std::vector<Order>& get_orders() const { 
				return orders;
		}

		int get_number_of_orders() const {
			return orders.size(); 
		}

		void delete_order(int id) {
			for (auto it = orders.begin(); it != orders.end(); ++it) {
				if (it->get_id() == id) {
					orders.erase(it);
					return;
				}
				else {
					std::cout << "\nThere is no order with that ID" << std::endl;
				}
			}
			
		}

		void print_all_orders() const {
			int counter { 1 };
			std::cout << "Orders for " << "'"<< get_name() << "'"<< ":";
			for (auto it = orders.begin(); it != orders.end(); ++it) {
				std::cout << "\nOrder number: " << counter << std::endl;
				std::cout << "Order ID: " << it->get_id() << std::endl;
				std::cout << it->get_price() << " ";
				std::cout << it->get_quantity() << " ";
				int operation = static_cast<int>(it->get_side());
				if (operation == 0) {
					std::cout << "Sell" << std::endl;
				}
				else if (operation == 1) {
					std::cout << "Buy" << std::endl;
				}
				counter++;
			}
		}
		

		

};

//Methods for ID
namespace ID {
	int get_unique_id() {
		static int id {};
		id++;
		return id;
	}
}

int main() {

	OrderBook ob("Main OrderBook");
	
	OrderBook::Order order1(ID::get_unique_id(), OrderBook::Order::Side::Buy, 1000.25, 22); 
	OrderBook::Order order2(ID::get_unique_id(), OrderBook::Order::Side::Buy, 1000.50, 15); 
	OrderBook::Order order3(ID::get_unique_id(), OrderBook::Order::Side::Sell, 1000.75, 18); 
	OrderBook::Order order4(ID::get_unique_id(), OrderBook::Order::Side::Sell, 1001.00, 5); 
	ob.add_order(order1);
	//std::cout << "debugging"; // potem destruktor, wydaje mi sie ze duplikuje obiekty
	ob.add_order(order2);
	//std::cout << "debugging";
	ob.add_order(order3);
	//std::cout << "debugging";
	ob.add_order(order4);
	//std::cout << "debugging";
//------------------------------
	ob.print_all_orders();

	ob.delete_order(2);

	ob.print_all_orders();

	return 0;
}
