#include "header.hpp"

class Message {

    private:

        Header header; 

    public: 

        Header getHeader(){return header;};    
        void serHeader(Header hdr){header = hdr;};
        
};