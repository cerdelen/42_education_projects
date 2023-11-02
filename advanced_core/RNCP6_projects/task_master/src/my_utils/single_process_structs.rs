use crate::my_utils::structs::*;

impl SingleProcessStruct {
    pub fn get_status(&mut self) -> String {
        if let Ok(child) = self.child.as_mut() {
            match child.try_wait() {
                Err(_) => "Cant try_wait Child".to_string(),
                Ok(exit_option) => match exit_option {
                    Some(exit_status) => {
                        "Exited with status ".to_string() + &exit_status.to_string()
                    }
                    None => "Still running".to_string(),
                },
            }
        } else {
            "Error with child in struct".to_string()
        }
    }
}
