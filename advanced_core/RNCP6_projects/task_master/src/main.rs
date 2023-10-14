use std::io::Stdout;
// use serde::Deserialize;
use config::ConfigError;
use my_config::ProcessGroupStruct;
use terminal::check_for_events;
use tui::{Terminal, backend::CrosstermBackend};

mod my_config;
use crate::my_config::TaskConfig;

mod terminal;

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

pub struct AllData {
	curr_conf:		TaskConfig,
	term:			Terminal<CrosstermBackend<Stdout>>,	
	display:		terminal::GUIState,
}

fn main() -> Result<(), ConfigError>{
	let mut term: tui::Terminal<tui::backend::CrosstermBackend<std::io::Stdout>> = terminal::build_terminal();

	let config: TaskConfig = TaskConfig::build()?;

	let mut process_groups = build_process_groups(&config);

	launch_groups(&mut process_groups);

	let mut state = AllData {
		curr_conf: config,
		term,
		display: terminal::GUIState{cursor: (0,0)},
	};

	terminal::draw_term(&mut state);

	let _ = check_for_events(&mut state);
	// println!("{:?}!", process_groups[0].vec_of_single_processes[0].child);
	// println!("end of main");
	Ok(())
}
