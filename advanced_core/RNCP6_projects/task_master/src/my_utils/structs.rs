use crate::my_utils::enums::*;
use serde::Deserialize;
use std::process::Command;

#[derive(Deserialize, Debug, Clone)]
pub struct ProcessConfig {
    pub launch_command: String,
    pub args: Option<Vec<String>>,
    pub amt: usize,
    pub time_of_launch: Option<u64>,
    pub restart: ProcessRestart,
    pub unexpected_signals: Option<Vec<ProcessSignals>>, // this will have to change somehow
    pub restart_tries: Option<u32>,
    pub gracefull_signal: ProcessSignals,
    pub gracefull_period: u32,
    pub stdout: Option<String>, // maybe empty then to stdout/sdterr (look up if there is smth like fds i can just put in)
    pub stderr: Option<String>, // maybe empty then to stdout/sdterr (look up if there is smth like fds i can just put in)
    pub envs: Option<Vec<(String, String)>>, //(maybe a map)
    pub work_dir: Option<String>,
    pub name: Option<String>,
    // An umask to set before launching the programm??
}

#[derive(Deserialize, Debug, Clone)]
pub struct TaskConfig {
    pub applications: Vec<ProcessConfig>,
}

pub struct SingleProcessStruct {
    pub restart_tries: u32, // i will init this to config number and then decreas with each retry
    pub child: Result<std::process::Child, std::io::Error>,
}

pub struct ProcessGroupStruct<'a> {
    pub conf: ProcessConfig,
    pub cmd: Command,
    pub processes: &'a mut [SingleProcessStruct], // of size amt
    pub start_time: std::time::Instant,
}

pub struct AllData {
    pub monitored_processes: Vec<ProcessGroupStruct>,
    pub display: DisplayState,
}

// struct x {
// 	monitored_processes: Vec
// }
