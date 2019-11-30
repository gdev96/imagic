#include "host.hpp"
#include "message.hpp"
#include "balancer_executor.hpp"
#include <queue>
#include <map>

class LoadBalancer {

    private:

        BalancerExecutor balancer_executor;
        queue <Message> message_queue;
        map <Host,int> map_load_server; //map (address_server,load_server)
        
};