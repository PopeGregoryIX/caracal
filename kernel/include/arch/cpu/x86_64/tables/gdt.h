/**
 * @file gdt.h
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief Gdt class and associated definitions.
 * @version 0.1
 * @date 2021-09-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef GDT_H_
#define GDT_H_

#include <stdint.h>

///	@brief Set if GDT entry is present
#define GDT_PRESENT		(1 << 7)
///	@brief Set if this is a user segment (ring 3)
#define	GDT_USER		(3 << 5)
///	@brief Set if this is a system (CS/DS) segment
#define GDT_SYSTEM		(1 << 4)
/// @brief Set if this is an executable (code) segment
#define GDT_EXECUTABLE	(1 << 3)
/// @brief Set if the CS can be called from lower privilege code
#define GDT_CONFORMING 	(1 << 2)
/// @brief Set if the segment expands down (base > limit)
#define GDT_DIRECTION 	(1 << 2)
/// @brief Set if the DS is writable, or if the CS is readable (required)
#define GDT_RW			(1 << 1)
/// @brief Set by the system if the segment has been accessed
#define GDT_ACCESSED	(1 << 0)

/// @brief Set for 4k granularity
#define GDT_GRAN_4K		(1 << 3)
/// @brief Set for a 32 bit (as opposed to 16 bit) segment
#define GDT_SIZE32		(1 << 2)
/// @brief Set for a 64 bit segment (long mode only).
#define GDT_SIZE64		(1 << 1)

/**
 * @brief Reloads GDTR with its current values.
 * @param gdtr A pointer to the GDTR structure.
 * @param cs Code segment to jump to following loading GDTR.
 * @param ds Data segment to load in to ds, es, fs, gs and ss following loading GDTR.
 */
extern "C" void __loadGdt(void* gdtr, uint8_t cs, uint8_t ds);

namespace arch
{
	/**
	 * @brief A manager for the Global Descriptor table.
	 */
	class Gdt
	{
	public:
		static inline Gdt& GetInstance( void )	{	return instance_;	}

		Gdt( void );

		void Load( void );

		/// @brief An entry in the Global Descriptor Table
		struct GdtEntry
		{
			uint16_t	limitLow;		///< @brief Low 16 bits of the segment limit.
			uint16_t	baseLow;		///< @brief Low 16 bits of the segment base.
			uint8_t		baseMid;		///< @brief Middle 8 bits of the segment base.
			uint8_t		access;			///< @brief Access Flags
			uint8_t		limitHigh:4;	///< @brief High 4 bits of the segment limit.
			uint8_t		flags:4;		///< @brief GDT flags.
			uint8_t		baseHigh;		///< @brief High 8 bits of the segment limit.

			GdtEntry()
			: limitLow(0), baseLow(0), baseMid(0), access(0),
			  limitHigh(0), flags(0), baseHigh(0){}

			void setBase(uint32_t base)
			{
				baseLow = (uint16_t)(base & 0xFFFF);
				baseMid = (uint8_t)((base >> 16)& 0xFF);
				baseHigh = (uint8_t)((base >> 24)& 0xFF);
			}

			void setLimit(uint32_t limit)
			{
				limitLow = (uint16_t)(limit & 0xFFFF);
				limitHigh = (uint8_t)((limit >> 16) & 0x0F);
			}
		} __attribute__((packed));

		struct GdtSystemEntry : public GdtEntry
		{
			uint16_t	limitLow;		///< @brief Low 16 bits of the segment limit.
			uint16_t	baseLow;		///< @brief Low 16 bits of the segment base.
			uint8_t		baseMid;		///< @brief Middle 8 bits of the segment base.
			uint8_t		access;			///< @brief Access Flags
			uint8_t		limitHigh:4;	///< @brief High 4 bits of the segment limit.
			uint8_t		flags:4;		///< @brief GDT flags.
			uint8_t		baseHigh;		///< @brief High 8 bits of the segment limit.
			uint32_t	baseUpper;
			uint32_t	reserved;

			GdtSystemEntry()
			: limitLow(0), baseLow(0), baseMid(0), access(0),
			  limitHigh(0), flags(0), baseHigh(0), baseUpper(0), reserved(0){}

			void setBase(uint64_t base)
			{
				baseLow = (uint16_t)(base & 0xFFFF);
				baseMid = (uint8_t)((base >> 16)& 0xFF);
				baseHigh = (uint8_t)((base >> 24)& 0xFF);
				baseUpper = (uint32_t)((base >> 32) & 0xFFFFFFFF);
			}

			void setLimit(uint32_t limit)
			{
				limitLow = (uint16_t)(limit & 0xFFFF);
				limitHigh = (uint8_t)((limit >> 16) & 0x0F);
			}
		} __attribute__((packed));

		struct GdtDescriptor
		{
			uint16_t size;
			uintptr_t offset;
		} __attribute__((packed));

		void SetEntry(int index, GdtSystemEntry entry);
	private:
		GdtEntry gdt_[0x2000] __attribute__((aligned(64)));
		GdtDescriptor gdtr_ __attribute__((aligned(64)));
		uint16_t entries_;

		static Gdt instance_;
	};
}


#endif /* GDT_H_ */
