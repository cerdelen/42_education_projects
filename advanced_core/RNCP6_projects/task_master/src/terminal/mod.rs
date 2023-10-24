use crossterm::{
    event::{KeyCode, KeyEvent},
    terminal::{disable_raw_mode, enable_raw_mode},
};
use std::{io, process::exit, time::Duration};
use tui::{backend::CrosstermBackend, Terminal};
use tui::{
    layout::{Constraint, Direction, Layout},
    style::{Color, Modifier, Style},
    text::{Span, Spans},
    widgets::{Block, Borders, Paragraph, Wrap},
};

use crate::{terminal, AllData};

use crossterm::event::{poll, read, Event};

pub fn build_terminal() -> Terminal<CrosstermBackend<io::Stdout>> {
    let stdout = io::stdout();
    let backend = CrosstermBackend::new(stdout);
    let mut terminal = Terminal::new(backend).expect("Terminal is not opening!");
    let _ = tui::Terminal::clear(&mut terminal);
    terminal
}

pub struct GUIState {
    pub cursor: (u16, u16),
}

fn key_press(key_event: KeyEvent, data: &mut AllData) {
    // println!("{:?}", key_event);
    if key_event == KeyEvent::from(terminal::KeyCode::Char('q')) {
        let _ = disable_raw_mode();
        exit(0);
    }
    if key_event == KeyEvent::from(terminal::KeyCode::Char('h')) && data.display.cursor.0 > u16::MIN
    {
        data.display.cursor.0 -= 1;
    }
    if key_event == KeyEvent::from(terminal::KeyCode::Char('j')) && data.display.cursor.1 < u16::MAX
    {
        data.display.cursor.1 += 1;
    }
    if key_event == KeyEvent::from(terminal::KeyCode::Char('k')) && data.display.cursor.1 > u16::MIN
    {
        data.display.cursor.1 -= 1;
    }
    if key_event == KeyEvent::from(terminal::KeyCode::Char('l')) && data.display.cursor.0 < u16::MAX
    {
        data.display.cursor.0 += 1;
    }
    if key_event == KeyEvent::from(terminal::KeyCode::Char('1')) {
        data.display.cursor.0 = 0;
        data.display.cursor.1 = 0;
    }
}

pub fn check_for_events(data: &mut AllData) -> io::Result<()> {
    let _ = enable_raw_mode();
    loop {
        if poll(Duration::from_millis(500))? {
            match read()? {
                Event::Key(event) => key_press(event, data),
                _ => panic!("smth else happened"),
                // Event::FocusGained => println!("FocusGained"),
                // Event::FocusLost => println!("FocusLost"),
                //             Event::Resize(width, height) => println!("New size {}x{}", width, height),
            }
            draw_term(data);
        } else {
            // Timeout expired and no `Event` is available
        }
    }
    // Ok(())
}

pub fn draw_term(data: &mut AllData) {
    enable_raw_mode().expect("raw mode cannot be enabled!");
    data.term
        .draw(|f| {
            let chunks = Layout::default()
                .direction(Direction::Vertical)
                .margin(1)
                .constraints([Constraint::Percentage(80), Constraint::Percentage(20)].as_ref())
                .split(f.size());

            let block = Block::default().title("Status").borders(Borders::ALL);
            let block2 = Block::default().title("Help").borders(Borders::ALL);
            let text = vec![
                Spans::from(vec![
                    Span::raw("First"),
                    Span::styled("line", Style::default().add_modifier(Modifier::ITALIC)),
                    Span::raw("."),
                ]),
                Spans::from(Span::styled("Second line", Style::default().fg(Color::Red))),
            ];
            let par = Paragraph::new(text)
                .block(Block::default().title("Paragraph").borders(Borders::ALL))
                .style(Style::default().fg(Color::White).bg(Color::Black))
                // .alignment(Alignment::Center)
                .wrap(Wrap { trim: true });
            f.render_widget(block, chunks[0]);
            f.render_widget(block2, chunks[1]);
            f.render_widget(par, chunks[0]);
            f.set_cursor(data.display.cursor.0, data.display.cursor.1);
        })
        .expect("error of draw term");
    // disable_raw_mode().expect("raw mode not enabled!");
}
