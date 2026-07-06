
#include <memory>
#include <string>

#include "OrderBook.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */



namespace ID {
	size_t get_unique_id() {
		static size_t id {};
		id++;
		return id+1000;
	}
}

bool input_validation(int &user_choice) {
	if (!(std::cin >> user_choice)) {
		std::cin.clear();            
		std::cin.ignore(1000, '\n'); 
		return true;
        }
        return false;
}

int main() {
	std::string name { "Main OrderBook" };
	auto &ob = OrderBook::getInstance(name);

	// These pointers have ownership only for easy making an object, its only for a while	
	std::unique_ptr<Order> order1 = std::make_unique<Order>(Order(ID::get_unique_id(), Order::Side::Buy, 4180.50, 22, Order::Type::Market)); 
	std::unique_ptr<Order> order2 = std::make_unique<Order>(Order(ID::get_unique_id(), Order::Side::Buy, 4180.75, 15, Order::Type::Market));
	std::unique_ptr<Order> order3 = std::make_unique<Order>(Order(ID::get_unique_id(), Order::Side::Sell, 4181.00, 18, Order::Type::Market)); //
	std::unique_ptr<Order> order4 = std::make_unique<Order>(Order(ID::get_unique_id(), Order::Side::Sell, 4181.25, 5, Order::Type::Market)); 
	std::unique_ptr<Order> order5 = std::make_unique<Order>(Order(ID::get_unique_id(), Order::Side::Buy, 4180.25, 20, Order::Type::Market));
	
	// With these methods we are moving ownership to the vector of uniquepointers pointing to the each of the orders
	ob.add_order(order1);
	ob.add_order(order2);
	ob.add_order(order3);
	ob.add_order(order4);
	ob.add_order(order5);


	int user_choice { };

   	do {

		ob.print_all_orders();
		std::cout << "\nOption:"; 
		std::cout << "\n1. Market"; 
		std::cout << "\n2. Limit";
		std::cout << "\n0. Quit";
		std::cout << "\nChoose: "; 

        	if (!(std::cin >> user_choice)) {
			std::cin.clear();            
			std::cin.ignore(1000, '\n'); 
            	    	continue;
        	}

        	switch (user_choice) {
        		case 1: {
        			int user_choice { };
				std::cout << "\nOption:";
				std::cout << "\n1. Buy";
				std::cout << "\n2. Sell";
				std::cout << "\nChoose:";

				if(input_validation(user_choice)) {
					continue;
				}


				switch (user_choice) {
					case 1: {
							std::cout << "You choosed buy";
							break;
						}
					case 2: {
							std::cout << "You choosed sell";
							break;
						}
				}
			break;
			}
			case 2: {
        			int user_choice { };
				std::cout << "\nOption:";
				std::cout << "\n1. Buy";
				std::cout << "\n2. Sell";
				std::cout << "\nChoose:";
	
				if(input_validation(user_choice)) {
					continue;
				}

				switch (user_choice) {
					case 1: {
							std::cout << "You choosed buy";
							break;
						}
					case 2: {
							std::cout << "You choosed sell";
							break;
						}
				}
			break;
			}
		}

	} while (user_choice != 0);

	return 0;
}
