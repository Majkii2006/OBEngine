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
		void set_quantity(int number) {
			quantity_ = quantity_ - number;
			std::cout << "Zostało " << quantity_ << std::endl;
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



		void delete_order(size_t id) {
    	    	    auto lambda = [id](const Order* ptr) { return ptr->get_id() == id; };
    	    	    
    	    	    buy_orders.erase(std::remove_if(buy_orders.begin(), buy_orders.end(), lambda), buy_orders.end());
    	    	    sell_orders.erase(std::remove_if(sell_orders.begin(), sell_orders.end(), lambda), sell_orders.end());

    	    	    for (auto it = orders.begin(); it != orders.end(); ++it) {
        		if ((*it)->get_id() == id) {
            	    	    Order* ptr_to_delete = *it; 
            	    	    orders.erase(it);           
            	    	    delete ptr_to_delete;       
            	    	    update_bid_and_ask_price();
            	    	    return;
        		}
    	    	    }
    	    	    std::cout << "There is no order with that ID, returning...";
		}

		void match_order(Order* order) {
			// Dla kupującego:
			if (order->get_side() == Order::Side::Buy){
				//szukamy zlecenia sprzedazy
				if (order->get_price() >= get_ask_price()){
					for (auto it { sell_orders.begin() }; it != sell_orders.end(); ++it){
						if (order->get_quantity() < (*it)->get_quantity()) {
							std::cout << "Found the matching order" << std::endl;
							(*it)->set_quantity(order->get_quantity());
							delete_order(order->get_id());
							break;
						}
						else if (order->get_quantity() == (*it)->get_quantity()){
							std::cout << "Found the matching order, fully bought" << std::endl;
							delete_order((*it)->get_id());
							delete_order(order->get_id());
							break;
						}
						else {
							
						}
					}
						
				}
				else {
					std::cout << "\nYour order was added to passive buyers" << std::endl;
				}
			}
			if (order->get_side() == Order::Side::Sell){
				//szukamy zlecenia kupna
			}
		}	
		

		void execute_order() {
			return;
		}

		void update_bid_and_ask_price() {
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
			//std::cout << "CURRENT BID PRICE: " << bid_price_ << std::endl;
			//std::cout << "CURRENT ASK PRICE: " << ask_price_ << std::endl;
		}

		void sort_vectors() {
			std::stable_sort(buy_orders.begin(), buy_orders.end(), [](const Order* a, const Order* b) {
					return a->get_price() >	b->get_price(); // posortowane malejaco
				});
			std::stable_sort(sell_orders.begin(), sell_orders.end(), [](const Order* a, const Order* b) {
					return a->get_price() > b->get_price(); // posortowane malejaco
					});
		}


	public: 
		OrderBook(std::string name) {
			name_ = name;
		};
		
		void add_order(Order& order) {
			// orders.push_back(order);  
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
			sort_vectors();
			match_order(&order);
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
			std::cout << "\n 	Current ASK price: " << get_ask_price() << std::endl;
			std::cout << "\n	Current BID price: " << get_bid_price() << std::endl;
			for (auto ptr : sell_orders) {
				std::cout << "\nOrder number: " << counter << std::endl;
				std::cout << "Order ID: " << ptr->get_id() << std::endl;
				std::cout << ptr->get_price() << " ";
				std::cout << ptr->get_quantity() << " ";
				std::cout << "Sell" << std::endl;	
				counter++;
			}
			for (auto ptr : buy_orders) {
				std::cout << "\nOrder number: " << counter << std::endl;
				std::cout << "Order ID: " << ptr->get_id() << std::endl;
				std::cout << ptr->get_price() << " ";
				std::cout << ptr->get_quantity() << " ";
				std::cout << "Buy" << std::endl;	
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
	
	Order *order1 = new Order(ID::get_unique_id(), Order::Side::Buy, 4180.50, 22); 
	Order *order2 = new Order(ID::get_unique_id(), Order::Side::Buy, 4180.75, 15);
	Order *order3 = new Order(ID::get_unique_id(), Order::Side::Sell, 4181.00, 18); // 
	Order *order4 = new Order(ID::get_unique_id(), Order::Side::Sell, 4181.25, 5); 
	Order *order5 = new Order(ID::get_unique_id(), Order::Side::Buy, 4180.25, 20);
	Order *order6 = new Order(ID::get_unique_id(), Order::Side::Buy, 4180.60, 2);
	Order *order7 = new Order(ID::get_unique_id(), Order::Side::Buy, 4181.00, 18); //
	ob.add_order(*order1);
	ob.add_order(*order2);
	ob.add_order(*order3);
	ob.add_order(*order4);
	ob.add_order(*order5);
	ob.add_order(*order6);
	ob.add_order(*order7);
//------------------------------
	ob.print_all_orders();



	return 0;
}
