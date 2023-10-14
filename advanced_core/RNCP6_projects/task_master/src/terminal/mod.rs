
use crossterm::terminal::{enable_raw_mode, disable_raw_mode};
use tui::{
    layout::{Constraint, Direction, Layout},
    widgets::{Block, Borders}, 
};
use std::{io, thread, time::Duration};
use tui::{backend::CrosstermBackend, Terminal};

use crate::AllData;

use crossterm::event::{poll, read, Event};

pub fn build_terminal() -> Terminal<CrosstermBackend<io::Stdout>> {
	let stdout = io::stdout();
    let backend = CrosstermBackend::new(stdout);
	let mut terminal = Terminal::new(backend).expect("Terminal is not opening!");
	let _ = tui::Terminal::clear(&mut terminal);
	return terminal;
}

pub enum DisplayState {
	Display1,
	ProcessOverview,
}

pub fn check_for_events() -> io::Result<()> {
	loop {
        if poll(Duration::from_millis(500))? {
            match read()? {
                Event::FocusGained => println!("FocusGained"),
                Event::FocusLost => println!("FocusLost"),
                Event::Key(event) => println!("{:?}", event),
                Event::Mouse(event) => println!("{:?}", event),
                #[cfg(feature = "bracketed-paste")]
                Event::Paste(data) => println!("Pasted {:?}", data),
                Event::Resize(width, height) => println!("New size {}x{}", width, height),
				Event::Paste(paste) => println!("Paste {:?}", paste),
            }
        } else {
            // Timeout expired and no `Event` is available
        }
    }
    Ok(())
}

pub fn draw_term(data: &mut AllData) {
	enable_raw_mode().expect("raw mode not enabled!");
	data.term.draw(|f| {
		let chunks = Layout::default()
			.direction(Direction::Vertical)
			.margin(1)
			.constraints(
				[
					Constraint::Percentage(50),
					Constraint::Percentage(50),
				].as_ref()
			)
			.split(f.size());

        let block = Block::default()
            .title("Status")
            .borders(Borders::ALL);
		let block2 = Block::default()
			.title("Block 2")
            .borders(Borders::ALL);
        f.render_widget(block, chunks[0]);
        f.render_widget(block2, chunks[1]);
		// f.set_cursor(0, 0);
    }).expect("error in use term");
	thread::sleep(Duration::from_millis(5000));
	disable_raw_mode().expect("raw mode not enabled!");
}
