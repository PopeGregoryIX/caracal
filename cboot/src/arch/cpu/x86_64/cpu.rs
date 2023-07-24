#[cfg(target_arch = "x86_64")]

#[path = "../../../boot/bootboot.rs"]
mod bootboot;

extern crate rlibc;

pub fn IsBoot(bspId: u16) -> bool
{
    return match GetProcessorId()
    {
        None => false,
        Some(val) => val == bspId
    }
}

pub fn GetProcessorId() -> Option<u16>
{
    use raw_cpuid::CpuId;
    let cpuid = CpuId::new();
    let features = cpuid.get_feature_info();

    if features.is_some()
    {
        return Some(features.unwrap().initial_local_apic_id() as u16);
    }

    return None;
}