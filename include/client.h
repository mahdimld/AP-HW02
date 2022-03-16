#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "server.h"
#include "crypto.h"
#include <map>

class Server;
class Client
{
public:
	Client(std::string id, const Server& server);
	std::string get_id();
	std::string get_publickey() const;
	double get_wallet();
	std::string sign(std::string txt) const;
	bool transfer_money(std::string receiver, double value);
	size_t generate_nonce();
private:
	Server const* const server;
	const std::string id;
	std::string public_key;
	std::string private_key;
};

static void  show_wallets(const  Server& server)
 {

    std::map<std::shared_ptr<Client>, double>* ptr = (std::map<std::shared_ptr<Client>, double>*)&server;
 	std::cout << std::string(20, '*') << std::endl;
 	 for(auto member = ptr->begin();member != ptr->end();member++){    
        std::cout << member->first->get_id() <<  " : "  << member->second << std::endl;
     }
 		
 	std::cout << std::string(20, '*') << std::endl;
 }

#endif //CLIENT_H