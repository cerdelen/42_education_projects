use serde::Deserialize;

#[derive(Debug)]
pub enum DisplayVariant {
    Overview,
    DetailedSingular,
}

#[derive(Deserialize, Debug, Clone)]
pub enum ProcessSignals {
    Signal1,
    Signal2,
}

#[derive(Deserialize, Debug, Clone)]
pub enum ProcessRestart {
    Always,
    Never,
    OnUnexpectedExits,
}
