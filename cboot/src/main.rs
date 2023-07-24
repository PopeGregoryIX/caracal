#![no_std]
#![no_main]
#![allow(dead_code)]
#![allow(non_snake_case)]
#![allow(non_camel_case_types)]

mod panic;
#[path = "boot/bootboot.rs"]
mod bootboot;
#[path = "arch/cpu/x86_64/debug.rs"]
mod debug;

extern crate rlibc;

#[no_mangle]
pub extern "C" fn bmain() -> ! {
   
   use bootboot::*;
   let bootboot_r = unsafe { & (*(BOOTBOOT_INFO as *const BOOTBOOT)) };
   let processorId = GetProcessorId().unwrap();

   if IsBoot(bootboot_r.bspid)
   {
        let mut debugOutput = debug::DebugOutput::new(5);
        debugOutput.Puts("CBoot Version 0.0.1\n");
        debugOutput.Puts("BSP OK. All Processor ID's:\n");
        let mut chr = b'0';
        chr += processorId as u8;
        debugOutput.Putc(chr);

        for _ in 0..(bootboot_r.numcores)
        {
            debugOutput.Putc(b'\n');
        }

        debugOutput.Puts("Initialising BSP.");
   }
   else
   {
        let mut debugOutput = debug::DebugOutput::new(5);
        for _ in 0..=(processorId + 1)
        {
            debugOutput.Putc(b'\n');
        }

        let mut chr = b'0';
        chr += processorId as u8;
        debugOutput.Putc(chr);
        loop{}
   }


   loop {}
}

fn IsBoot(bspId: u16) -> bool
{
    return match GetProcessorId()
    {
        None => false,
        Some(val) => val == bspId
    }
}

fn GetProcessorId() -> Option<u16>
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
