#include "host.hpp"

class Header {
    
    private: 

        int message_type;
        Host sender;
        Host receiver;

    public: 

        Header(int msg_t, Host snd, Host rcv){
            message_type = msg_t;
            sender = snd;
            receiver = rcv;
        };
        Host getSender(){return sender;}
        Host getReceiver(){return receiver;}
        void setReceiver(Host rcv){ receiver = rcv;}

};