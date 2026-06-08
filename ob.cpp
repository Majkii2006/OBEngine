#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Order {
	public:
		enum class Side { Sell, Buy };	

	private:
		size_t id_;
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
		~Order() {
			std::cout << "\nDestructor Called" << std::endl;
		};
	
		//Getters
		size_t get_id() const { return id_; }
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



class OrderBook {	
	private:
		std::string name_ { };
		std::vector<Order*> orders;
		std::vector<Order*> buy_orders;
		std::vector<Order*> sell_orders;
		double bid_price_ {};
		double ask_price_ {};


		void match_order(Order* order) {
			for (auto it { orders.begin() }; it != orders.end() - 1; ++it) { //push_back -> na koniec wiec -1 to zapobiega sprawdzaniu siebie samego
				auto order_site = order->get_side();	
				auto it_order_site = (*it)->get_side();
				if ((*it)->get_price() == order->get_price() && (order_site == Order::Side::Buy && it_order_site == Order::Side::Sell
							|| order_site == Order::Side::Sell && it_order_site == Order::Side::Buy)) 
				{
					std::cout << "There IS matching order!" << std::endl;
					return;
				}
			}	
			std::cout << "There is no matching order" << std::endl;	



		}

		void execute_order() {
			//aktualizacja cen rynkowych kupna i sprzedazy
			return;
		}

		void update_bid_and_ask_price() {
		//minimalna cena sprzedazy i maks cena kupna potrzebuje
			if (buy_orders.size() > 0){
				auto bid_it = std::max_element(buy_orders.begin(), buy_orders.end(), [](const Order* a, const Order* b) {
							return a->get_price() < b->get_price();
						});
				this->bid_price_ = (*bid_it)->get_price();
			} 
			if (sell_orders.size() > 0){
				auto ask_it = std::min_element(sell_orders.begin(), sell_orders.end(), [](const Order* a, const Order* b){
						return a->get_price() < b->get_price();
						});
				this->ask_price_ = (*ask_it)->get_price();
			}	
			std::cout << "CURRENT BID PRICE: " << bid_price_ << std::endl;
			std::cout << "CURRENT ASK PRICE: " << ask_price_ << std::endl;
		}


	public: 
		OrderBook(std::string name) {
			name_ = name;
		};
		
		void add_order(Order& order) {
			// orders.push_back(order); //tutaj jest blad, push_back -> kopia, jednak nie 
			//orders.emplace_back(order.get_id(), order.get_side(),
			//		order.get_price(), order.get_quantity())
			orders.push_back(&order);

			if (order.get_side() == Order::Side::Buy){
				buy_orders.push_back(&order);
			}
			else {
				sell_orders.push_back(&order);
			}

			update_bid_and_ask_price();
			match_order(&order);
			execute_order();
		} 
		
		const std::string& get_name() const {
			return name_;
		}

		const std::vector<Order*>& get_orders() const { 
				return orders;
		}

		int get_number_of_orders() const {
			return orders.size(); 
		}

		double get_ask_price() const {
			return ask_price_;
		}

		double get_bid_price() const {
			return bid_price_;
		}

		void delete_order(int id) {
			for (auto it = orders.begin(); it != orders.end(); ++it) {
				if (*it == nullptr ) {
					continue;
				}	
				if ( (*it)->get_id() == id) {
					orders.erase(it);
					return;
				}
			}
			std::cout << "There is no order with that ID, returning...";
			return;
			
		}

		void print_all_orders() const {
			int counter { 1 };
			std::cout << "\nOrders for " << "'"<< get_name() << "'"<< ":" << std::endl;
			for (auto ptr : orders) {
				std::cout << "\nOrder number: " << counter << std::endl;
				std::cout << "Order ID: " << ptr->get_id() << std::endl;
				std::cout << ptr->get_price() << " ";
				std::cout << ptr->get_quantity() << " ";
				int operation = static_cast<int>(ptr->get_side());
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
	size_t get_unique_id() {
		static size_t id {};
		id++;
		return id+1000;
	}
}

int main() {

	OrderBook ob("Main OrderBook");
	
	Order order1(ID::get_unique_id(), Order::Side::Buy, 1002.25, 22); 
	Order order2(ID::get_unique_id(), Order::Side::Buy, 1000.50, 15); 
	Order order3(ID::get_unique_id(), Order::Side::Sell, 1000.75, 18); 
	Order order4(ID::get_unique_id(), Order::Side::Sell, 1001.00, 5); 
	Order order5(ID::get_unique_id(), Order::Side::Buy, 1001.00, 20);
	ob.add_order(order1);
	//std::cout << "debugging"; // potem destruktor, wydaje mi sie ze duplikuje obiekty
	ob.add_order(order2);
	//std::cout << "debugging";
	ob.add_order(order3);
	//std::cout << "debugging";
	ob.add_order(order4);
	//std::cout << "debugging";
	ob.add_order(order5);
//------------------------------
	ob.print_all_orders();

	ob.delete_order(1002);

	ob.print_all_orders();

	return 0;
}
