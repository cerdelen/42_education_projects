use config::ConfigError;
use crossterm::{
    cursor::{MoveTo, Hide, Show},
    event::{KeyCode, KeyEvent},
    execute,
    terminal::{disable_raw_mode, enable_raw_mode, Clear, ClearType},
};
use std::{
    io::{self, stdout, Write},
    process::exit,
    time::Duration,
};

use crate::{
    my_utils::{
        enums::DisplayVariant,
        structs::{Logger, ProcessConfig, SingleProcessStruct, TaskConfig},
    },
    terminal, AllData,
};

use crossterm::event::{poll, read, Event};

fn stop_task_master(data: &mut AllData, logging: &mut Logger) {
    let _ = execute!(stdout(), Show);
    let _ = disable_raw_mode();
    let _ = write!(logging.d, "Shutdown");
    exit(0);
}

fn decrease_process_idx(data: &mut AllData, logging: &mut Logger) {
    let _ = write!(
        logging.d,
        "Decreasing display index from: {}",
        data.display.idx
    );
    if let DisplayVariant::Overview = data.display.state {
        if data.display.idx > 0 {
            data.display.idx = data.display.idx - 1;
        }
    }
    let _ = writeln!(logging.d, " to: {}", data.display.idx);
}
fn increase_process_idx(data: &mut AllData, logging: &mut Logger) {
    let _ = write!(
        logging.d,
        "Decreasing display index from: {}",
        data.display.idx
    );
    if let DisplayVariant::Overview = data.display.state {
        if data.display.idx + 1 < data.process_array.len() {
            data.display.idx = data.display.idx + 1;
        }
    }
    let _ = writeln!(logging.d, " to: {}", data.display.idx);
}

fn select_process(data: &mut AllData, logging: &mut Logger) {
    let _ = write!(
        logging.d,
        "Changing DisplayVariant from {:?}",
        data.display.state
    );
    match data.display.state {
        DisplayVariant::DetailedSingular => data.display.state = DisplayVariant::Overview,
        DisplayVariant::Overview => {
            if let Some(_) = data.process_array.get(data.display.idx as usize) {
                data.display.state = DisplayVariant::DetailedSingular;
            }
        }
    };
    let _ = writeln!(
        logging.d,
        " to {:?}, Display index: {}",
        data.display.state, data.display.idx
    );
}

fn print_full_config_single_process(data: &mut AllData, logging: &mut Logger) {
    if let DisplayVariant::DetailedSingular = data.display.state {
        if let Some(conf) = get_process_conf(data, data.display.idx as usize, logging) {}
    }
}

fn reload_taskmaster(data: &mut AllData, logging: &mut Logger) -> Result<(), ConfigError> {
    let config: TaskConfig = TaskConfig::build()?;
	let it = config.applications.iter();

	for new_conf in config.applications {
		new_conf.
	}


	// compare this taskconfig with implemented taskconfig
	// kill all commands not there any more 
	// start all commands that are new
	// dont touch commands that are unchanged
	// that means copy already existing processes over
	Ok(())
}

fn reload_process(data: &mut AllData, logging: &mut Logger) {}

///Tries to kill Process
fn kill_process(data: &mut AllData, logging: &mut Logger) {
    // if let DisplayVariant::DetailedSingular = data.display.state {
        let _ = write!(logging.d, "Kill Process: ");
        if let Some(process) = data.process_array.get_mut(data.display.idx) {
            match &mut process.child {
                Ok(child) => match child.kill() {
                    Ok(()) => {
                        let _ = writeln!(logging.d, "Successful!");
                    }
                    Err(err) => {
                        let _ = writeln!(logging.d, "{}", err);
                    }
                },
                Err(err) => {
                    let _ = writeln!(logging.d, "{}", err);
                }
            }
        }
    // }
}

///Reload
///
///In Overview: Reloads whole config File
///In Detailed View: Reloads single Process
fn reload_command(data: &mut AllData, logging: &mut Logger) {
    let _ = writeln!(logging.d, "reload");
    match data.display.state {
		DisplayVariant::DetailedSingular => reload_process(data, logging),
		DisplayVariant::Overview => {
			if let Err(err) = reload_taskmaster(data, logging) {
					let _ = write!(logging.d, "Error reloading Config file: {}", err);
				}
		}
	}
}

///Handler for KeyEvents
///
///Calls necessary functions to execute Key Commands
fn key_press(key_event: KeyEvent, data: &mut AllData, logging: &mut Logger) {
    let KeyEvent { code, .. } = key_event;
    let _ = writeln!(logging.d, "");
    let _ = writeln!(logging.d, "Key Pressed: {:?}", key_event.code);
    match code {
        KeyCode::Char(c) => match c {
            'q' => stop_task_master(data, logging),
            'd' => select_process(data, logging),
            'j' => increase_process_idx(data, logging),
            'k' => decrease_process_idx(data, logging),
            'r' => reload_command(data, logging),
            'p' => print_full_config_single_process(data, logging),
            't' => kill_process(data, logging),
            _ => (),
        },
        _ => (),
    }
}

fn print_header(data: &mut AllData, logging: &mut Logger) {
    let _ = match data.display.state {
        DisplayVariant::Overview => logging.render.push_str(&format!("Overview!\n")),
        DisplayVariant::DetailedSingular => logging.render.push_str(&format!("Detailed view: of process: {}\n", data.display.idx)),
    };
}

fn construct_name(process_conf: &ProcessConfig) -> String {
    if let Some(name) = &process_conf.name {
        name.to_string()
    } else {
        process_conf.launch_command.to_string()
    }
}

fn print_process_list(data: &mut AllData, logging: &mut Logger) {
    for i in data.display.idx..data.display.idx + 5 {
        if let Some(process_conf) = get_process_conf(data, i, logging) {
		 logging.render
                .push_str(&format!("{}: {}", i, construct_name(&process_conf)));
            if let Some(process) = data.process_array.get_mut(i) {
		 logging.render
                    .push_str(&format!(" {}\n", process.get_status()));
            } else {
                logging.render.push_str(&format!("\n"));
            }
        } else {
            logging.render.push_str(&format!("\n"));
        }
    }
}

fn get_process_status(process: &mut SingleProcessStruct) -> String {
    let mut ret = String::from("");
    match &mut process.child {
        Ok(child) => match child.try_wait() {
            Ok(code_opt) => match code_opt {
                Some(code) => {
                    ret.push_str(&format!("Finished: {}\n", code.to_string()));
                }
                None => ret.push_str("Running\n"),
            },
            Err(err) => ret.push_str(&format!("{}\n", err.to_string())),
        },
        Err(err) => ret.push_str(&format!("{}\n", err.to_string())),
    }
    ret
}

fn print_process_details(process: (&mut SingleProcessStruct, ProcessConfig), logging: &mut Logger) {
    let _ = writeln!(logging.d, "print_detailed_singular: ");
    logging
        .render
        .push_str(&format!("Name: {}\n", construct_name(&process.1)));
    logging
        .render
        .push_str(&format!("Status: {}\n", get_process_status(process.0)));
}

fn get_process_conf<'a>(
    data: &'a AllData,
    idx: usize,
    logging: &mut Logger,
) -> Option<ProcessConfig> {
    let mut iter = data.monitored_group_structs.iter();
    let _ = write!(logging.d, "get_process_conf idx({}): ", idx);
    while let Some(process_group) = iter.next() {
        let _ = writeln!(
            logging.d,
            "process_group.end_idx: {}",
            process_group.end_idx
        );
        if idx < process_group.end_idx as usize {
            return Some(process_group.conf.clone());
        }
    }
    let _ = writeln!(logging.d, "None");
    None
}

fn get_process_data<'a>(
    data: &'a mut AllData,
    idx: usize,
    logging: &mut Logger,
) -> Option<(&'a mut SingleProcessStruct, ProcessConfig)> {
    if let Some(process_conf) = get_process_conf(data, idx, logging) {
        if let Some(process) = data.process_array.get_mut(data.display.idx as usize) {
            return Some((process, process_conf));
        }
    }
    None
}

fn print_middle(data: &mut AllData, logging: &mut Logger) {
    match data.display.state {
        DisplayVariant::Overview => print_process_list(data, logging),
        DisplayVariant::DetailedSingular => {
            if let Some(process_details_tuple) =
                get_process_data(data, data.display.idx as usize, logging)
            {
                print_process_details(process_details_tuple, logging);
            }
        }
    }
}

fn print_helpline(data: &AllData, logging: &mut Logger) {
    let _ = match data.display.state {
        DisplayVariant::Overview => 
			logging.render.push_str(&format!("j: Scroll Down k: Scroll Up d: Select detailed Processview r: Reload Config q: Stop Taskmaster\n")),
        DisplayVariant::DetailedSingular => 
			logging.render.push_str(&format!("s: Stop/Resume Process t: Kill Process r: Reload Process d: Overview q: Close Taskmaster\n")),
    };
}

fn print_terminal(_data: &mut AllData, logging: &mut Logger) {
	logging.render.clear();
	print_header(_data, logging);
	print_middle(_data, logging);
	print_helpline(_data, logging);
    let _ = disable_raw_mode();
    let _ = execute!(
        stdout(),
        MoveTo(0, 0),
        terminal::Clear(terminal::ClearType::All),
		Hide
    );
    let _ = write!(stdout(), "{}", logging.render);
    let _ = enable_raw_mode();
}

///main Loop
///
///only returns if theres an error with reading Terminal
pub fn check_for_events(mut data: AllData, logging: &mut Logger) -> io::Result<()> {
    let _ = enable_raw_mode();
    let _ = writeln!(logging.d, "Start of Keyloop");
    loop {
        if poll(Duration::from_secs(1))? {
            match read()? {
                Event::Key(event) => key_press(event, &mut data, logging),
                Event::FocusGained => println!("FocusGained"),
                Event::FocusLost => println!("FocusLost"),
                // Event::Resize(width, height) => println!("New size {}x{}", width, height),
                _ => panic!("smth else happened"),
            };
        }
        print_terminal(&mut data, logging);
    }
}
