// mod shared_logic;
use std::sync::{mpsc, Arc,Mutex};
use crate::shared_logic::{ClientMessage, ServerMessage, Signal, logging::Logger};

fn kill_taskmaster(){}
fn reload_taskmaster(){}

pub fn server(
    client_to_server_receiver: Arc<Mutex<mpsc::Receiver<ClientMessage>>>,
    server_to_client_sender: mpsc::Sender<ServerMessage>,
	mut logger: Logger,
){
	logger.log("Started Server!");
    loop {
        match client_to_server_receiver.lock() {
            Err(err) => {
                println!("{}", err);
                continue;
            },
            Ok(lll) => {
                match lll.recv() {
                    Err(err) => {
                        println!("{}", err);
                            continue;
                    },
                    Ok(msg) => {
                        match msg {
                            ClientMessage::Sig(Signal::KillTaskMaster) => kill_taskmaster(),
                            ClientMessage::Sig(Signal::ReloadTaskMaster) => reload_taskmaster(),
                        } 
                    }
                }
            }
        }
    }
}