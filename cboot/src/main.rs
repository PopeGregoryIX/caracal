#![no_std]
#![no_main]
#![allow(dead_code)]
#![allow(non_snake_case)]
#![allow(non_camel_case_types)]

mod panic;
#[path = "boot/bootboot.rs"]
mod bootboot;
#[path = "arch/cpu/x86_64/cpu.rs"]
mod cpu;
#[path = "arch/cpu/x86_64/debug.rs"]
mod debug;
mod utility;

extern crate rlibc;

#[no_mangle]
pub extern "C" fn bmain() -> ! {
   
   use bootboot::*;

   let bootboot_r = unsafe { & (*(BOOTBOOT_INFO as *const BOOTBOOT)) };
   let processorId = cpu::GetProcessorId().unwrap();

   if cpu::IsBoot(bootboot_r.bspid)
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

        debugOutput.Puts("Initialising BSP.\n");
        debugOutput.Puti(65535, 10);
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
