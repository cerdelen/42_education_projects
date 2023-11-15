mod my_config;
mod my_utils;
mod terminal;
use std::fs::File;
use std::io::{stdout, Write};
use std::process::exit;

use crate::terminal::check_for_events;
use config::ConfigError;
use crossterm::cursor::Show;
use crossterm::execute;
use crossterm::terminal::disable_raw_mode;
use my_utils::structs::*;

fn print_config_of_process(writer: &mut File, process: &ProcessGroupStruct) {
    let _ = writeln!(
        writer,
        "  name: {:?}, launch_processes: {:?}, amt: {}",
        process.conf.name, process.conf.launch_command, process.conf.amt
    );
}

fn build_process_groups(config: &TaskConfig, logging: &mut Logger) -> Vec<ProcessGroupStruct> {
    let mut process_idx: usize = 0;
    let mut process_groups: Vec<ProcessGroupStruct> = vec![];
    process_groups.reserve(config.applications.len());
    let _ = writeln!(
        &mut logging.d,
        "Reserved space for Config array: {}",
        config.applications.len()
    );
    for app in &config.applications {
        process_groups.push(app.build_command(process_idx, logging));
        process_idx = process_idx + app.amt;
    }
    let _ = writeln!(logging.d, "Processes groups[{}]:", process_groups.len());
    let mut iter = process_groups.iter();
    while let Some(process) = iter.next() {
        print_config_of_process(&mut logging.d, process)
    }
    process_groups
}

fn launch_processes(
    groups: &mut Vec<ProcessGroupStruct>,
    logging: &mut Logger,
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
    let _ = writeln!(logging.d, "Processes array len: {}", processes.len());
    processes
}

fn setup_logging() -> Logger {
    Logger {
        d: match File::create("debug_dump") {
            Ok(file) => file,
            Err(err) => {
                let _ = writeln!(stdout(), "Fatal Error: Couldnt open Debug File!\n{}", err);
                exit(1);
            }
        },
        render: String::from(""),
    }
}

fn main() -> Result<(), ConfigError> {
    let mut logging = setup_logging();

    let config: TaskConfig = TaskConfig::build()?;

    let mut process_groups: Vec<ProcessGroupStruct> = build_process_groups(&config, &mut logging);

    let state = AllData {
        display: DisplayState::init(),
        process_array: launch_processes(&mut process_groups, &mut logging),
        monitored_group_structs: process_groups,
    };

    match check_for_events(state, &mut logging) {
        Ok(_) => (),
        Err(err) => {
            let _ = writeln!(logging.d, "{}", err);
        }
    };
    let _ = execute!(stdout(), Show);
    let _ = disable_raw_mode();
    Ok(())
}
