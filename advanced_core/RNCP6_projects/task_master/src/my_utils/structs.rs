use crate::my_utils::enums::*;
use serde::Deserialize;
use std::fs::File;
use std::process::Command;

pub struct DisplayState {
    pub state: DisplayVariant,
    pub idx: usize,
}

#[derive(Deserialize, Debug, Clone)]
pub struct TaskConfig {
    pub applications: Vec<ProcessConfig>,
}

pub struct SingleProcessStruct {
    pub restart_tries: usize, // i will init this to config number and then decreas with each retry
    pub child: Result<std::process::Child, std::io::Error>,
}

pub struct ProcessGroupStruct {
    pub conf: ProcessConfig,
    pub cmd: Command,
    pub start_idx: usize,
    pub end_idx: usize,
    pub start_time: std::time::Instant,
}

pub struct AllData {
    pub monitored_group_structs: Vec<ProcessGroupStruct>,
    pub process_array: Vec<SingleProcessStruct>,
    pub display: DisplayState,
    pub debug_file: File,
}

#[derive(Deserialize, Debug, Clone)]
pub struct ProcessConfig {
    pub launch_command: String,
    pub args: Option<Vec<String>>,
    pub amt: usize,
    pub time_of_launch: Option<u64>,
    pub restart: ProcessRestart,
    pub unexpected_signals: Option<Vec<ProcessSignals>>, // this will have to change somehow
    pub restart_tries: Option<usize>,
    pub gracefull_signal: ProcessSignals,
    pub gracefull_period: u64,
    pub stdout: Option<String>, // maybe empty then to stdout/sdterr (look up if there is smth like fds i can just put in)
    pub stderr: Option<String>, // maybe empty then to stdout/sdterr (look up if there is smth like fds i can just put in)
    pub envs: Option<Vec<(String, String)>>, //(maybe a map)
    pub work_dir: Option<String>,
    pub name: Option<String>,
    // An umask to set before launching the programm??
}
