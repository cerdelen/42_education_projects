use serde::{Serialize, Deserialize};
use std::sync::{mpsc, Arc, Mutex};
use std::thread;

mod server;
mod client;
mod shared_logic;
use shared_logic::logging::Logger;
// use shared_logic::
use shared_logic::logging::setup_loggers;
fn main() ->Result<(),()>{
    let loggers = setup_loggers()?;
    let (client_to_server_sender, client_to_server_receiver) = mpsc::channel();
    let (server_to_client_sender, server_to_client_receiver) = mpsc::channel();

    let client_to_server_receiver = Arc::new(Mutex::new(client_to_server_receiver));
    let server_to_client_receiver = Arc::new(Mutex::new(server_to_client_receiver));

    let server_thread = thread::spawn(move || {
        server::server(client_to_server_receiver, server_to_client_sender, loggers.1)
    });

    client::client(server_to_client_receiver, client_to_server_sender, loggers.0);
    server_thread.join().unwrap();
    Ok(())
}