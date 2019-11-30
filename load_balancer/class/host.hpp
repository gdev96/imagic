#include <string>

using namespace std;

class Host{  

    private:

        string address;
        int port;

    public:

        Host(string adr,int prt){            
            address = adr;
            port = prt;
        }
        string getAddress(){return address;};
        int getPort(){return port;};
        void setAddress(string adr){address = adr;};
        void serPort(int prt){port = prt;};
    
};  