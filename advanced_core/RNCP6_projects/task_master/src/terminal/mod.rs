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

use crate::{terminal, AllData, DisplayState};

use crossterm::event::{poll, read, Event};

fn stop_task_master(data: AllData) -> AllData {
    let _ = disable_raw_mode();
    exit(0);
    data
}

fn select_process(_selection: usize, mut data: AllData) -> AllData {
    if let Some(selected_process) = data
        .monitored_processes
        .get(0)
        .unwrap()
        .vec_of_single_processes
        .get(0)
    {
        data.display = DisplayState::DetailedSingular(selected_process);
    }
    data
}

fn key_press(key_event: KeyEvent, mut data: AllData) -> AllData {
    let KeyEvent { code, .. } = key_event;
    match code {
        KeyCode::Char(c) => match c {
            'q' => stop_task_master(data),
            '1' => select_process(0, data),
            '2' => select_process(1, data),
            '3' => select_process(2, data),
            '4' => select_process(3, data),
            '5' => select_process(4, data),
            _ => data,
        },
        _ => data,
    }
}

fn print_header(data: &mut AllData) {
    let _ = match data.display {
        DisplayState::Overview => writeln!(stdout(), "Overview"),
        DisplayState::DetailedSingular(_proccess) => writeln!(stdout(), "detailed view"),
    };
    // so i have 2 stati
}

fn print_middle(_data: &mut AllData) {
    for _i in 0..5 {
        let _ = writeln!(stdout(), "Row Line");
    }
}

fn print_helpline(_data: &AllData) {
    let _ = writeln!(stdout(), "Help Line at the bottom");
}

fn update_programs(data: AllData) -> AllData {
    data
}

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
    let mut smth;
    loop {
        if poll(Duration::from_millis(500))? {
            smth = match read()? {
                Event::Key(event) => key_press(event, data),
                _ => panic!("smth else happened"),
                // Event::FocusGained => println!("FocusGained"),
                // Event::FocusLost => println!("FocusLost"),
                // Event::Resize(width, height) => println!("New size {}x{}", width, height),
            };
            // print_terminal(data);
        } else {
            smth = update_programs(data);
        }
        data = smth;
    }
}
