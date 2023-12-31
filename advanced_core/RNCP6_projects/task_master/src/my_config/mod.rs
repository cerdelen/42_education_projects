use crate::my_utils::structs::{
    ProcessConfig, ProcessGroupStruct, SingleProcessStruct, TaskConfig,
};
use config::{Config, ConfigError, File, FileFormat};
use std::process::{Command, Stdio};

impl ProcessGroupStruct {
    pub fn launch_one_process(&mut self) {
        let mut child = self.cmd.spawn();
        let mut retries = self.conf.restart_tries.unwrap_or_default();
        while child.is_err() {
            if retries == 0 {
                break;
            }
            retries -= 1;
            child = self.cmd.spawn();
        }
        self.vec_of_single_processes.push(SingleProcessStruct {
            restart_tries: retries,
            child,
        });
    }
}

impl ProcessConfig {
    pub fn build_command(&self) -> ProcessGroupStruct {
        let mut cmd = Command::new(&self.launch_command);
        cmd.args(self.args.as_deref().unwrap_or_default());
        cmd.envs(
            self.envs
                .as_deref()
                .unwrap_or_default()
                .iter()
                .map(|(key, value)| (key, value)),
        );
        if let Some(stdout) = &self.stdout {
            if let Ok(file) = std::fs::OpenOptions::new()
                .read(true)
                .write(true)
                .create(true)
                .append(true)
                .open(stdout)
            {
                cmd.stdout(Stdio::from(file));
            }
        };
        if let Some(stderr) = &self.stderr {
            if let Ok(file) = std::fs::OpenOptions::new()
                .read(true)
                .write(true)
                .create(true)
                .append(true)
                .open(stderr)
            {
                cmd.stderr(Stdio::from(file));
            }
        };
        cmd.current_dir(self.work_dir.as_ref().unwrap_or(&".".to_string()));

        ProcessGroupStruct {
            start_time: std::time::Instant::now()
                + std::time::Duration::from_secs(self.time_of_launch.unwrap_or(0)),
            conf: self.clone(),
            vec_of_single_processes: vec![],
            cmd,
        }
    }
}

impl TaskConfig {
    pub fn build() -> Result<TaskConfig, ConfigError> {
        let config_file = match std::env::args().nth(1) {
            Some(arg) => arg,
            None => panic!("Provide Config File plz!"),
        };
        let conf = Config::builder()
            .add_source(File::new(&config_file, FileFormat::Yaml))
            .build()?;
        let task_config = conf.try_deserialize::<TaskConfig>()?;
        Ok(task_config)
    }
}
