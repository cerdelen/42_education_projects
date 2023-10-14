use serde::Deserialize;
use std::{process::{Command, Stdio}, io::ErrorKind};
use config::{Config, File, FileFormat, ConfigError};

#[derive(Deserialize, Debug, Clone)]
pub enum ProcessSignals {
	Signal1,
	Signal2,
}

#[derive(Deserialize, Debug, Clone)]
pub enum ProcessRestart{
	Always,
	Never,
	OnUnexpectedExits,
}

#[derive(Deserialize, Debug, Clone)]
pub struct ProcessConfig {
	pub launch_command:			String,
	pub args:					Option<Vec<String>>,
	pub amt:					usize,
	pub time_of_launch:			Option<u64>,
	pub restart:				ProcessRestart,
	pub unexpected_signals:		Option<Vec<ProcessSignals>>, // this will have to change somehow
	pub restart_tries:			Option<u32>,
	pub gracefull_signal:		ProcessSignals,
	pub gracefull_period:		u32,
	pub stdout:					Option<String>, // maybe empty then to stdout/sdterr (look up if there is smth like fds i can just put in)
	pub stderr:					Option<String>, // maybe empty then to stdout/sdterr (look up if there is smth like fds i can just put in)
	pub envs:					Option<Vec<(String, String)>>,	//(maybe a map)
	pub work_dir:				Option<String>,
	pub name:					Option<String>,
	// An umask to set before launching the programm??
}

pub struct SingleProcessStruct {
	pub restart_tries:				u32,	// i will init this to config number and then decreas with each retry
	pub child:						Result<std::process::Child, std::io::Error>,
}

pub struct ProcessGroupStruct<'a> {
	pub conf:								&'a ProcessConfig,
	pub cmd:								Command,
	pub vec_of_single_processes:			Vec<SingleProcessStruct>, // of size amt
	pub start_time:							std::time::Instant,
}

impl ProcessGroupStruct<'_> {
	pub fn launch_one_process(&mut self) {
		let mut child = self.cmd.spawn();
		let mut retries = self.conf.restart_tries.unwrap_or_default();
		while let Err(_) = &child {
			if retries == 0 {
				break ;
			}
			retries -= 1;
			child = self.cmd.spawn();
		}
		self.vec_of_single_processes.push(
			SingleProcessStruct {
				restart_tries: retries,
				child,
			}
		);
	}
}

impl ProcessConfig {
	pub fn build_command(&self) -> ProcessGroupStruct {
		let mut cmd = Command::new(&self.launch_command);
		cmd.args(self.args.as_deref().unwrap_or_default());
		cmd.envs(self.envs.as_deref().unwrap_or_default().iter().map(|&(ref key, ref value)| (key, value)));
		if let Some(stdout) = &self.stdout {
			if let Ok(file) = std::fs::OpenOptions::new()
				.read(true)
				.write(true)
				.create(true)
				.append(true)
				.open(stdout) {
					cmd.stdout(Stdio::from(file));
			}
		};
		if let Some(stderr) = &self.stderr {
			if let Ok(file) = std::fs::OpenOptions::new()
				.read(true)
				.write(true)
				.create(true)
				.append(true)
				.open(stderr) {
					cmd.stderr(Stdio::from(file));
			}
		};
		cmd.current_dir(self.work_dir.as_ref().unwrap_or(&".".to_string()));

		ProcessGroupStruct {
			start_time: std::time::Instant::now() + 
			std::time::Duration::from_secs(self.time_of_launch.unwrap_or(0)),
			conf: self,
			vec_of_single_processes: vec![],
			cmd,
		}
		// println!("{:?}", process_group.cmd.get_program());
		// println!("{:?}", process_group.cmd.get_args());
		// println!("{:?}", process_group.cmd.get_envs());
		// println!("{:?}", self.unexpected_signals);
	}
}

#[derive(Deserialize, Debug, Clone)]
pub struct TaskConfig {
	pub applications: Vec<ProcessConfig>,
}

impl TaskConfig {
	pub fn build() -> Result<TaskConfig, ConfigError> {
		let config_file = match std::env::args().nth(1) {
			Some(arg) => { arg },
			None => panic!("Provide Config File plz!"),
		};
		let conf = Config::builder()
			.add_source(File::new(&config_file, FileFormat::Yaml))
			.build()?;
		let task_config = conf.try_deserialize::<TaskConfig>()?;
		Ok(task_config)
	}
}

