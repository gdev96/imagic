#include <iostream>
#include <string>
#include <queue>


using namespace std;

class Host{  

    private:

        string address;
        int port;

    public:
        
        Host(string adr, int prt){            
            address = adr;
            port = prt;
        }

        string getAddress(){return address;}

        int getPort(){return port;}
        
        void setAddress(string adr){address = adr;}

        void setPort(int prt){port = prt;}
        
        void toString(){
            cout << "Address: " << address << " Port: " << port;
        }
};  

class Header {
    
    private: 

        int message_type;
        Host *sender;
        Host *receiver;

    public:   
        
        Header(int msg_t, string snd_adr, string rcv_adr, int snd_prt, int rcv_prt){

            message_type = msg_t;
                    
            sender = new Host(snd_adr, snd_prt);

            receiver = new Host(rcv_adr, rcv_prt);            

        }

        Host * getSender(){return sender;}

        Host * getReceiver(){return receiver;}

        int getMessageType(){return message_type;}

        void setReceiver(Host * rcv){ receiver = rcv;} 
              
        void toString(){
            cout << "Message Type: " << message_type << "\n"
                << "Sender: \nAddress: " << sender -> getAddress() << " Port: " << sender -> getPort() << "\n"
                << "Receiver: \nAddress:" << receiver -> getAddress() << " Port: " << receiver -> getPort() << "\n";          
        }

};

class Message {

    private:

        Header *header; 
        int payload_lenght;
        
    public: 

        Message(int msg_t, string snd_adr, string rcv_adr, int snd_prt, int rcv_prt){
            header = new Header (msg_t, snd_adr, rcv_adr, snd_prt, rcv_prt);
        }

        Header* getHeader(){return this->header;}  

        void setHeader(Header* hdr){header = hdr;}

        void toString(){
        cout << "HEADER: \nMessage Type: " << header -> getMessageType() << "\n"
             << "Sender: \nAddress: " << header -> getSender() -> getAddress() << " Port: " << header -> getSender() -> getPort() << "\n"
             << "Receiver: \nAddress:" << header -> getReceiver() -> getAddress() << " Port: " << header -> getReceiver() ->getPort() << "\n";  
        }
};

class ConnectorClient{

    private:

        queue <Message> messageQueue;

    public:

        ConnectorClient(){};
       
};

class ConnectorServer{

    private:

        queue <Message> messageQueue;

    public:

        void send(Message msg);

};


 




