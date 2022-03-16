#include "server.h"


Server::Server():
clients{}
{}

std::shared_ptr<Client> Server::add_client(std::string id){


std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<> dist(1000, 9999);

std::string temp{id};
for(auto member = clients.begin();member != clients.end();member++){    
    if(member->first->get_id() == id){
        temp += std::to_string(dist(mt));
    }                
    
  }
  Client cli(temp, *this);
  std::shared_ptr<Client> cl_shp = std::make_shared<Client>(cli);

  clients.insert({cl_shp,5});
  return cl_shp;

}

std::shared_ptr<Client> Server::get_client(std::string id) const{

std::shared_ptr<Client> cl_shp{nullptr};    

    for(auto member = clients.begin();member != clients.end();member++){    
        if(member->first->get_id() == id){
            cl_shp = member->first;
        }       
     }
  return cl_shp;
}
double Server::get_wallet(std::string id){

double amount{};    

    for(auto member = clients.begin();member != clients.end();member++){    
        if(member->first->get_id() == id){
            amount = member->second;
        }       
     }
  return amount;


}

bool Server::parse_trx(std::string trx, std::string &sender, std::string &receiver, double &value){

    size_t index1 = trx.find("-");
    size_t index2 = trx.find("-",index1+1);

    
    if(index1 == std::string::npos || index2 == std::string::npos){
        throw std::runtime_error("Error!!!!");
    }

    sender = trx.substr(0,index1);
    receiver = trx.substr(index1+1,(index2-index1)-1);
    value = std::stod(trx.substr(index2+1));
    

    return true;

}

bool Server::add_pending_trx(std::string trx, std::string signature){


std::string sender{},receiver{};
double value{};
parse_trx(trx, sender, receiver, value);
std::shared_ptr<Client> senderClient{get_client(sender)},receiverClient{get_client(receiver)};
bool authentic = crypto::verifySignature(senderClient->get_publickey(), trx, signature);

if(senderClient == nullptr || receiverClient == nullptr){
    return false;
}


return ((senderClient->get_wallet() >= value) && authentic);


}

size_t Server::mine(){

size_t nonce{};
std::string sender{},receiver{};
std::shared_ptr<Client> clie{},senderClient{},receiverClient{};
double value{};
std::string mempool{};
    for(const auto& trx : pending_trxs)
        mempool += trx;


for(size_t i{};i<pending_trxs.size();){
    
    parse_trx(pending_trxs[i],sender,receiver,value);
    clie = get_client(sender);
    nonce = clie->generate_nonce();
    std::string hash = crypto::sha256(mempool + std::to_string(nonce));
    if(hash.substr(0, 10).find("000") != std::string::npos){
        clients[clie] += 6.25;
        std::cout<<"winner id: "<<clie->get_id()<<std::endl;
        break;
    }
    i++;
    if(i == pending_trxs.size()){
        i = 0;
    }
        
}

for(size_t i{}; i<pending_trxs.size(); i++){
parse_trx(pending_trxs[i], sender, receiver, value);
senderClient = get_client(sender);
receiverClient = get_client(receiver);
clients[senderClient] -= value;
clients[receiverClient] += value;

}

return nonce;

}
