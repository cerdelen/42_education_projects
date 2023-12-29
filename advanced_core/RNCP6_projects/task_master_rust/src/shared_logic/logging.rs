use std::fs::File;
use std::io::{stdout, Write};


pub struct Log {
    pub d: File,
    pub render: String,
}

pub struct Logger {
	pub log: Log,
	pub err: Log,
}

impl Logger {
	pub fn log(&mut self,msg: &str) {
		match write!(self.log.d, "{}\n", msg) {
			Err(err) => println!("Error to use Logger! {}", err),
			_ => (),
		}
	}
	pub fn log_err(&mut self,msg: &str) {
		match write!(self.err.d, "{}\n", msg) {
			Err(err) => println!("Error to use Logger! {}", err),
			_ => (),
		}
	}
}

fn setup_logging(path: &str) -> Result<Log, ()> {
    match File::create(path) {
        Err(err) => {
            let _ = writeln!(stdout(), "Fatal Error: Couldnt open Debug File!\n{}", err);
            Err(())
        }
        Ok(d) => {
            Ok(Log {
                d,
                render: String::from(""),
            })
        },
    }
}

pub fn setup_loggers() -> Result<(Logger, Logger), ()>{
	if !std::fs::metadata("logs").is_ok() {
		match std::fs::create_dir("logs") {
			Err(err) => {
				let _ = writeln!(stdout(), "Fatal Error: Couldnt open Directory for Logs!\n{}", err);
				return Err(())
			},
			_ => (),
		}
	}
	let client_log = setup_logging("logs/client_log")?;
	let client_err_log = setup_logging("logs/client_err_log")?;
	let server_log = setup_logging("logs/server_log")?;
	let server_err_log = setup_logging("logs/server_err_log")?;
	Ok((Logger{log: client_log, err: client_err_log}, Logger{log: server_log, err: server_err_log}))
}