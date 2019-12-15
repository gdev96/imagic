#include <mysqlx/xdevapi.h>
#include "storage_manager.h"

storage_manager::storage_manager(const message &current_request) : current_request_(current_request) {}

void storage_manager::upload_request() { //UPLOAD REQUEST -> save paths in db and files in storage
    //current_request_.get_payload()->getContent()->(std::get<2>);

    mysqlx::Session session("mysqlx://root@127.0.0.1");
    mysqlx::Schema schema= session.getSchema("test");
    //1.Prelevo l'immagine
    //2.Salvo i percorsi e la categoria nel DB
    //3.Salvo i file nei rispettivi percorsi
}

void storage_manager::view_thumbs() { //VIEW THUMBS -> get thumbs map and send response
    //1.Prelevo i percorsi delle miniature appartenenti ad una categoria
    //2.Prelevo i rispettivi file delle miniature dalla memoria
    //3.Creo la mappa di miniature, la serializzo e la invio.
}

void storage_manager::download_image() { //DOWNLOAD IMAGE -> get the image to send response
    //1.Cerco il path dell'immagine scelta nel db
    //2.Invio come risposta il file immagine scelto prelevato dalla memoria
}
