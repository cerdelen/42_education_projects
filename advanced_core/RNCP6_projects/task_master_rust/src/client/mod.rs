use crate::shared_logic::{ClientMessage, ServerMessage, Signal, logging::Logger};
use std::sync::{mpsc, Arc,Mutex};


pub fn client(
    server_to_client_receiver: Arc<Mutex<mpsc::Receiver<ServerMessage>>>,
    client_to_server_sender: mpsc::Sender<ClientMessage>,
	mut logger: Logger,
){
	logger.log("Started Client!");
    match client_to_server_sender.send(ClientMessage::Sig(Signal::ReloadTaskMaster)) {
        Err(err) => {
			println!("{}", err);
			return ;
		},
		_ => (),
	}
	loop {
		
	}
}