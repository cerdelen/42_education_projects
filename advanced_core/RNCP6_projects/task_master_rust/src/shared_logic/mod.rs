use std::{fs::File, process::Command};
pub mod logging;





#[derive(Debug)]
pub enum ClientMessage {
    Sig(Signal),
    // Data(MyData),
}
#[derive(Debug)]
pub enum ServerMessage {
    Sig(Signal),
    // Data(MyData),
}

#[derive(Debug)]
pub enum Signal {
    KillTaskMaster,
    ReloadTaskMaster,
}