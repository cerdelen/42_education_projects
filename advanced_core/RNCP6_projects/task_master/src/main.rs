mod my_config;
mod my_utils;
mod terminal;
use my_utils::enums::*;
use my_utils::structs::*;
// use crate::my_utils::enums;
use crate::terminal::check_for_events;
use config::ConfigError;

fn build_process_groups(config: &TaskConfig) -> Vec<ProcessGroupStruct> {
    let mut process_groups: Vec<ProcessGroupStruct> = vec![];
    process_groups.reserve(config.applications.len());
    for app in &config.applications {
        process_groups.push(app.build_command());
    }
    process_groups
}

fn launch_groups(groups: &mut Vec<ProcessGroupStruct>) {
    for group in groups {
        while group.vec_of_single_processes.len() < group.conf.amt {
            group.launch_one_process();
        }
    }
}

fn main() -> Result<(), ConfigError> {
    let config: TaskConfig = TaskConfig::build()?;

    let mut process_groups: Vec<ProcessGroupStruct> = build_process_groups(&config);

    launch_groups(&mut process_groups);

    let state = AllData {
        display: DisplayState::Overview,
        monitored_processes: process_groups,
    };

    let _ = check_for_events(state);
    Ok(())
}
