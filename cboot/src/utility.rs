extern crate rlibc;

const LOOKUP_TABLE_UPPER: [u8; 16] = [ b'0', b'1', b'2', b'3', b'4', b'5', b'6', b'7', b'8', b'9', b'A', b'B', b'C', b'D', b'E', b'F' ];
const LOOKUP_TABLE_LOWER: [u8; 16] = [ b'0', b'1', b'2', b'3', b'4', b'5', b'6', b'7', b'8', b'9', b'a', b'b', b'v', b'd', b'e', b'f' ];

pub fn ItoA(number: u64, base: u8, result: &mut[u8; 65])
{
    let mut index = 0;

    if base == 2
    {
        let mut num = number;
        while num != 0
        {
            result[index] = LOOKUP_TABLE_UPPER[(num & 0x01) as usize];
            num >>= 1;
            index+= 1;
        }
    }
    else if base == 10
    {
        let mut num = number;
        let mut digit: u8;
        while num != 0
        {
            digit = (number % 10) as u8;
            result[index] = LOOKUP_TABLE_UPPER[digit as usize];
            index = index + 1;
            num = num / 10;
        }
    }
    else if base == 16
    {
        let mut bitShift = 64;
		while bitShift != 0
		{
			bitShift -= 4;
			let digit = ((number >> bitShift) & 0x0F) as u8;
			result[index as usize] = LOOKUP_TABLE_UPPER[digit as usize];
			if index != 0 || digit != 0 { index+= 1;}
		}
    }

    if index == 0 { result[index as usize] = b'\0'; index += 1; }
	result[index as usize] = b'\0';
}
