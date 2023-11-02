use crossterm::{
    cursor::MoveTo,
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
        structs::{ProcessConfig, SingleProcessStruct},
    },
    terminal, AllData,
};

use crossterm::event::{poll, read, Event};

fn stop_task_master(data: &mut AllData) {
    let _ = disable_raw_mode();
    let _ = write!(data.debug_file, "Shutdown");
    exit(0);
}

fn decrease_process_idx(data: &mut AllData) {
    let _ = write!(
        data.debug_file,
        "Decreasing display index from: {}",
        data.display.idx
    );
    if let DisplayVariant::Overview = data.display.state {
        if data.display.idx > 0 {
            data.display.idx = data.display.idx - 1;
        }
    }
    let _ = writeln!(data.debug_file, " to: {}", data.display.idx);
}
fn increase_process_idx(data: &mut AllData) {
    let _ = write!(
        data.debug_file,
        "Decreasing display index from: {}",
        data.display.idx
    );
    if let DisplayVariant::Overview = data.display.state {
        if data.display.idx + 1 < data.process_array.len() {
            data.display.idx = data.display.idx + 1;
        }
    }
    let _ = writeln!(data.debug_file, " to: {}", data.display.idx);
}

fn select_process(data: &mut AllData) {
    let _ = write!(
        data.debug_file,
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
        data.debug_file,
        " to {:?}, Sisplay index: {}",
        data.display.state, data.display.idx
    );
}
fn print_full_config_single_process(data: &AllData) {
    if let DisplayVariant::DetailedSingular = data.display.state {
        if let Some(conf) = get_process_conf(data, data.display.idx as usize) {}
    }
}
fn pause_resume_process(data: &mut AllData) {
    let _ = writeln!(data.debug_file, "pause/resume");
}

fn reload_taskmaster(data: &mut AllData) {}
fn reload_process(data: &mut AllData) {}
fn kill_process(data: &mut AllData) {
    let _ = writeln!(data.debug_file, "Kill Process");
}

fn reload_config(data: &mut AllData) {
    let _ = writeln!(data.debug_file, "reload");
    match data.display.state {
        DisplayVariant::Overview => reload_taskmaster(data),
        DisplayVariant::DetailedSingular => reload_process(data),
    }
}

fn key_press(key_event: KeyEvent, data: &mut AllData) {
    let KeyEvent { code, .. } = key_event;
    let _ = writeln!(data.debug_file, "");
    let _ = writeln!(data.debug_file, "Key Pressed: {:?}", key_event.code);
    match code {
        KeyCode::Char(c) => match c {
            'q' => stop_task_master(data),
            'd' => select_process(data),
            'j' => increase_process_idx(data),
            'k' => decrease_process_idx(data),
            's' => pause_resume_process(data),
            'r' => reload_config(data),
            'p' => print_full_config_single_process(data),
            't' => kill_process(data),
            _ => (),
        },
        _ => (),
    }
}

fn print_header(data: &AllData) {
    let _ = match data.display.state {
        DisplayVariant::Overview => writeln!(stdout(), "Overview!"),
        DisplayVariant::DetailedSingular => {
            writeln!(stdout(), "Detailed view!")
        }
    };
    let _ = writeln!(stdout(), "detailed view of process: {}", data.display.idx);
}

fn construct_name(process_conf: &ProcessConfig) -> String {
    if let Some(name) = &process_conf.name {
        name.to_string()
    } else {
        process_conf.launch_command.to_string()
    }
}

fn print_process_list(data: &mut AllData) {
    for i in data.display.idx..data.display.idx + 5 {
        if let Some(process_conf) = get_process_conf(data, i) {
            let _ = write!(stdout(), "{}: {}", i, construct_name(process_conf),);
            if let Some(process) = data.process_array.get_mut(i) {
                let _ = writeln!(stdout(), " {}", process.get_status());
            } else {
                let _ = writeln!(stdout(), " no entry in porcess_array for index: {}", i);
            }
        }
    }
}

fn print_process_details(process: (&SingleProcessStruct, &ProcessConfig)) {}

fn get_process_conf(data: &AllData, idx: usize) -> Option<&ProcessConfig> {
    let mut iter = data.monitored_group_structs.iter();
    while let Some(process_group) = iter.next() {
        if idx < process_group.end_idx as usize {
            return Some(&process_group.conf);
        }
    }
    None
}

fn get_process_data(data: &AllData, idx: usize) -> Option<(&SingleProcessStruct, &ProcessConfig)> {
    if let Some(process) = data.process_array.get(data.display.idx as usize) {
        if let Some(process_conf) = get_process_conf(data, idx) {
            return Some((process, process_conf));
        }
    }
    None
}

fn print_middle(data: &mut AllData) {
    match data.display.state {
        DisplayVariant::Overview => print_process_list(data),
        DisplayVariant::DetailedSingular => {
            if let Some(process_details_tuple) = get_process_data(data, data.display.idx as usize) {
                print_process_details(process_details_tuple);
            }
        }
    }
}

fn print_helpline(data: &AllData) {
    let _ = match data.display.state {
        DisplayVariant::Overview => writeln!(
            stdout(),
            "j: Scroll Down k: Scroll Up d: Select detailed Processview r: Reload Config q: Stop Taskmaster"
        ),
        DisplayVariant::DetailedSingular => writeln!(
            stdout(),
            "s: Stop/Resume Process t: Kill Process r: Reload Process d: Overview q: Close Taskmaster"
        ),
    };
}

// fn update_programs(data: &mut AllData) {}

fn print_terminal(_data: &mut AllData) {
    let _ = disable_raw_mode();
    let _ = execute!(
        stdout(),
        MoveTo(0, 0),
        terminal::Clear(terminal::ClearType::All)
    );
    print_header(_data);
    print_middle(_data);
    print_helpline(_data);
    let _ = enable_raw_mode();
}

pub fn check_for_events(mut data: AllData) -> io::Result<()> {
    let _ = enable_raw_mode();
    let _ = writeln!(data.debug_file, "Start of Keyloop");
    loop {
        if poll(Duration::from_millis(500))? {
            match read()? {
                Event::Key(event) => key_press(event, &mut data),
                Event::FocusGained => println!("FocusGained"),
                Event::FocusLost => println!("FocusLost"),
                Event::Resize(width, height) => println!("New size {}x{}", width, height),
                _ => panic!("smth else happened"),
            };
        } else {
            // update_programs(&mut data);
        }
        print_terminal(&mut data);
    }
}
