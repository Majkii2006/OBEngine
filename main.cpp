
#include <memory>
#include <string>

#include "OrderBook.hpp"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */


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
	std::unique_ptr<Order> order1 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Buy, 4180.50, 22, Order::Type::Market); 
	std::unique_ptr<Order> order2 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Buy, 4180.75, 15, Order::Type::Market);
	std::unique_ptr<Order> order3 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4181.00, 18, Order::Type::Market);
	std::unique_ptr<Order> order4 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4181.25, 5, Order::Type::Market); 
	std::unique_ptr<Order> order5 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Buy, 4180.25, 20, Order::Type::Market);
	std::unique_ptr<Order> order6 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Buy, 4180.00, 3, Order::Type::Market);
	std::unique_ptr<Order> order7 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4181.00, 9, Order::Type::Market);




	// With these methods we are moving ownership to the vector of uniquepointers pointing to the each of the orders
	ob.add_order(order1);
	ob.add_order(order2);
	ob.add_order(order3);
	ob.add_order(order4);
	ob.add_order(order5);
	ob.add_order(order6);
	ob.add_order(order7);



	int user_choice { };

   	do {

		std::cout << "\nOption:";
		std::cout << "\n1. Print OrderBook FULL"; 
		std::cout << "\n2. Market"; 
		std::cout << "\n3. Limit";
		std::cout << "\n9. Check ASK and BID price";
		std::cout << "\n0. Quit";
		std::cout << "\nChoose: "; 

        	if (!(std::cin >> user_choice)) {
			std::cin.clear();            
			std::cin.ignore(1000, '\n'); 
            	    	continue;
        	}

        	switch (user_choice) {
			case 1: {
					ob.print_all_orders();
					break;
				}

        		case 2: {
        			int user_choice { };
				std::cout << "\nOption:";
				std::cout << "\n1. Buy";
				std::cout << "\n2. Sell";
				std::cout << "\nChoose: ";

				if(input_validation(user_choice)) {
					continue;
				}

				int user_quantity { };
				switch (user_choice) {
					case 1: {
							std::cout << "Enter order quantity: ";
							std::cin >> user_quantity;
							std::cout << "\nSubmitting market buy order for " << user_quantity 
								<< " units..." << std::endl;
							ob.market_order_buy(user_quantity);
							ob.print_all_orders();
							break;
						}
					case 2: {
							std::cout << "Enter order quantity: ";
							std::cin >> user_quantity;
							std::cout << "\nSubmitting market sell order for " << user_quantity 
								<< " units..." << std::endl;
							ob.market_order_sell(user_quantity);
							ob.print_all_orders();
							break;
						}
				}
			break;
			}
			case 3: {
        			int user_choice { };
				std::cout << "\nOption:";
				std::cout << "\n1. Buy";
				std::cout << "\n2. Sell";
				std::cout << "\nChoose: ";
	
				if(input_validation(user_choice)) {
					continue;
				}
				int user_quantity { };
				double user_price { };
				switch (user_choice) {
					case 1: {	
							std::cout << "Enter order quantity: ";
							std::cin >> user_quantity;
							std::cout << "Enter max price: " << std::endl;
							std::cin >> user_price;
							std::cout << "\nSubmitting limit buy order for " << user_quantity 
								<< " units..." << " @ " << "Price: " << user_price;
							ob.limit_order_buy(user_quantity, user_price);
							ob.print_all_orders();
							break;
						}
					case 2: {
							std::cout << "Enter order quantity: ";
							std::cin >> user_quantity;
							std::cout << "Enter min price: " << std::endl;
							std::cin >> user_price;
							std::cout << "\nSubmitting limit sell order for " << user_quantity
								<< " units..." << " @ " << "Price: " << user_price;
							ob.limit_order_sell(user_quantity, user_price);
							ob.print_all_orders();
							break;
						}
				}
			break;
			}
			case 9: {
					std::cout << CYAN << "\nASK: " << ob.get_ask_price() << RESET;
					std::cout << CYAN <<"\nBID: " << ob.get_bid_price() << RESET << std::endl;
					break;
				}
		}

	} while (user_choice != 0);

	return 0;
}
