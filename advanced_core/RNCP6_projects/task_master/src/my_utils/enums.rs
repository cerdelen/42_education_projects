use std::usize;

use serde::Deserialize;
// use config::{Config, ConfigError, File, FileFormat};
// use std::process::{Command, Stdio};
// use crate::my_utils::structs::SingleProcessStruct;

#[allow(dead_code)]
pub enum DisplayState {
    Overview,
    DetailedSingular(usize),
}

#[derive(Deserialize, Debug, Clone)]
pub enum ProcessSignals {
    Signal1,
    Signal2,
}

#[derive(Deserialize, Debug, Clone)]
pub enum ProcessRestart {
    Always,
    Never,
    OnUnexpectedExits,
}
