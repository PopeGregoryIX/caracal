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
        let mut debugOutput = unsafe { InitDebugOutput(bootboot_r) };
        debugOutput.Puts("CBoot Version 0.0.1\n");
        debugOutput.Puts("BSP OK. All Processor ID's:\n");
        let mut chr = b'0';
        chr += processorId as u8;
        debugOutput.Putc(chr);
   }
   else
   {
        let mut debugOutput = unsafe { InitDebugOutput(bootboot_r) };
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

unsafe fn InitDebugOutput(bootboot_r: &bootboot::BOOTBOOT) -> debug::DebugOutput
{
    use debug::*;

    unsafe {
        let font: *mut psf2_t = &_binary___src_data_font_psf_start as *const u64 as *mut psf2_t;

        let screenHeightChars = bootboot_r.fb_height / (*font).height;
        let screenWidthChars = bootboot_r.fb_width / (*font).width;

        let debugOutput = DebugOutput { bytesPerPixel: 4, currentX: 0, currentY: 0, 
            heightChars: screenHeightChars as u16, widthChars: screenWidthChars as u16, tabStop: 4,
            bytesPerScanline: bootboot_r.fb_scanline,
            lfb: LFB, font: &_binary___src_data_font_psf_start as *const u64 as *mut psf2_t };

        return debugOutput;
    }
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
