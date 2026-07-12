
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

	std::unique_ptr<Order> order3  = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4182, 800, Order::Type::Market);
	std::unique_ptr<Order> order4  = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4183, 305, Order::Type::Market);
	std::unique_ptr<Order> order5  = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Buy, 4179, 260, Order::Type::Market);
	std::unique_ptr<Order> order6  = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Buy, 4178, 155, Order::Type::Market);
	std::unique_ptr<Order> order8  = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Buy, 4177, 500, Order::Type::Market);
	std::unique_ptr<Order> order9  = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Buy, 4176, 320, Order::Type::Market);
	std::unique_ptr<Order> order10 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Buy, 4175, 700, Order::Type::Market);
	std::unique_ptr<Order> order14 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Buy, 4181, 750, Order::Type::Market);
	std::unique_ptr<Order> order15 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Buy, 4180, 980, Order::Type::Market);
	std::unique_ptr<Order> order17 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Buy, 4179, 510, Order::Type::Market);
	std::unique_ptr<Order> order19 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Buy, 4178, 300, Order::Type::Market);
	std::unique_ptr<Order> order20 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4185, 200, Order::Type::Market);
	std::unique_ptr<Order> order21 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4186, 104, Order::Type::Market);
	std::unique_ptr<Order> order22 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4185, 320, Order::Type::Market);
	std::unique_ptr<Order> order23 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4184, 705,  Order::Type::Market);
	std::unique_ptr<Order> order24 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4183, 500, Order::Type::Market);
	std::unique_ptr<Order> order26 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4187, 200, Order::Type::Market);
	std::unique_ptr<Order> order27 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4188, 400,  Order::Type::Market);
	std::unique_ptr<Order> order28 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4189, 700, Order::Type::Market);
	std::unique_ptr<Order> order29 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4190, 300, Order::Type::Market);
	std::unique_ptr<Order> order30 = std::make_unique<Order>(ID::get_unique_id(), Order::Side::Sell, 4191, 100, Order::Type::Market);





	// With these methods we are moving ownership to the vector of uniquepointers pointing to the each of the orders
	ob.add_order(order3);
	ob.add_order(order4);
	ob.add_order(order5);
	ob.add_order(order6);
	ob.add_order(order8);
	ob.add_order(order9);
	ob.add_order(order10);
	ob.add_order(order14);
	ob.add_order(order15);
	ob.add_order(order17);
	ob.add_order(order19);
	ob.add_order(order20);
	ob.add_order(order21);
	ob.add_order(order22);
	ob.add_order(order23);
	ob.add_order(order24);
	ob.add_order(order26);
	ob.add_order(order27);
	ob.add_order(order28);
	ob.add_order(order29);
	ob.add_order(order30);



	int user_choice { };

   	do {
		std::cout << "\nOption:";
		std::cout << "\n1. Print OrderBook COMPACT"; 
		std::cout << "\n2. Print OrderBook ALL INFO"; 
		std::cout << "\n3. Market";
		std::cout << "\n4. Limit";
		std::cout << "\n9. Check ASK and BID price";
		std::cout << "\n0. Quit";
		std::cout << "\nChoose: "; 

        	if (!(std::cin >> user_choice)) {
			std::cin.clear();            
			std::cin.ignore(1000, '\n'); 
            	    	continue;
        	}
		std::cout << "\n";
        	switch (user_choice) {
			case 1: {
					ob.print_all_orders_compact();
					break;
				}

			case 2: {
					ob.print_all_orders_full();
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
				switch (user_choice) {
					case 1: {
							std::cout << "Enter order quantity: ";
							std::cin >> user_quantity;
							std::cout << "\nSubmitting market buy order for " << user_quantity 
								<< " units..." << std::endl;
							ob.market_order_buy(user_quantity);
							ob.print_all_orders_compact();
							break;
						}
					case 2: {
							std::cout << "Enter order quantity: ";
							std::cin >> user_quantity;
							std::cout << "\nSubmitting market sell order for " << user_quantity 
								<< " units..." << std::endl;
							ob.market_order_sell(user_quantity);
							ob.print_all_orders_compact();
							break;
						}
				}
			break;
			}
			case 4: {
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
							ob.print_all_orders_compact();
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
							ob.print_all_orders_compact();
							break;
						}
				}
			break;
			}

			case 9: {
					std::cout << "==========="; 
					std::cout << CYAN << "\nASK: " << ob.get_ask_price() << RESET;
					std::cout << CYAN <<"\nBID: " << ob.get_bid_price() << RESET;
					std::cout << "\n===========" << std::endl;
					break;
				}
		}

	} while (user_choice != 0);

	return 0;
}
