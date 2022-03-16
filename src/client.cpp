#include "client.h"

Client::Client(std::string id, const Server& server):
id{id},
server{&server}
{
    crypto::generate_key(public_key, private_key);

}


std::string Client::get_id(){return id;}
std::string Client::get_publickey() const{return public_key;}
double Client::get_wallet(){

    Server svr{*server};

    return svr.get_wallet(id);
}

std::string Client::sign(std::string txt) const{

    return crypto::signMessage(private_key, txt);
}
bool Client::transfer_money(std::string receiver, double value){

Server svr{*server};
std::string trx{};
trx = id + "-" + receiver + "-" + std::to_string(value);
if(svr.add_pending_trx(trx,sign(trx))){

    pending_trxs.push_back(trx);
    return true;

}

return false;

}

size_t Client::generate_nonce(){

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<> dist(0, 1000);

return dist(mt);

}