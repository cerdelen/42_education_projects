mod my_config;
mod my_utils;
mod terminal;
use std::fs::File;
use std::io::{stdout, Write};
use std::process::exit;

use crate::terminal::check_for_events;
use config::ConfigError;
use my_utils::enums::*;
use my_utils::structs::*;

fn print_config_of_process(writer: &mut File, process: &ProcessGroupStruct) {
    let _ = writeln!(
        writer,
        "  name: {:?}, launch_processes: {:?}, amt: {}",
        process.conf.name, process.conf.launch_command, process.conf.amt
    );
}

fn build_process_groups(config: &TaskConfig, debug_file: &mut File) -> Vec<ProcessGroupStruct> {
    let mut process_idx: usize = 0;
    let mut process_groups: Vec<ProcessGroupStruct> = vec![];
    process_groups.reserve(config.applications.len());
    let _ = writeln!(
        debug_file,
        "Reserved space for Config array: {}",
        config.applications.len()
    );
    for app in &config.applications {
        process_idx = process_idx + app.amt;
        process_groups.push(app.build_command(process_idx));
    }
    let _ = writeln!(debug_file, "Processes groups[{}]:", process_groups.len());
    let mut iter = process_groups.iter();
    while let Some(process) = iter.next() {
        print_config_of_process(debug_file, process)
    }
    process_groups
}

fn launch_processes(
    groups: &mut Vec<ProcessGroupStruct>,
    debug_file: &mut File,
) -> Vec<SingleProcessStruct> {
    let mut processes: Vec<SingleProcessStruct> = vec![];
    println!("starting with launch processes");
    for group in groups {
        let mut i: usize = 0;
        while i < group.conf.amt {
            group.launch_one_process(&mut processes);
            i = i + 1;
        }
    }
    let _ = writeln!(debug_file, "Processes array len: {}", processes.len());
    processes
}

fn setup_debug() -> Option<File> {
    match File::create("debug_dump") {
        Ok(file) => Some(file),
        Err(err) => {
            let _ = writeln!(stdout(), "Couldnt open Debug File: {}", err);
            None
        }
    }
}

fn main() -> Result<(), ConfigError> {
    let mut debug_file = match setup_debug() {
        Some(file) => file,
        None => exit(1),
    };

    let config: TaskConfig = TaskConfig::build()?;

    let mut process_groups: Vec<ProcessGroupStruct> =
        build_process_groups(&config, &mut debug_file);

    let state = AllData {
        display: DisplayState {
            state: DisplayVariant::Overview,
            idx: 0,
        },
        process_array: launch_processes(&mut process_groups, &mut debug_file),
        monitored_group_structs: process_groups,
        debug_file,
    };

    let _ = check_for_events(state);
    Ok(())
}
