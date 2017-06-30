/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_IO_H__
#define __ARCOS_IO_H__

#include <arcOS/types.h>
#include <asm/io.h> /* Arch specific can overwrite the following functions */

#ifndef readb
#define readb readb
static inline u8 readb(const volatile void *addr)
{
	return *(const volatile u8 *)addr;
}
#endif

#ifndef readw
#define readw readw
static inline u16 readw(const volatile void *addr)
{
	return *(const volatile u16 *)addr;
}
#endif

#ifndef readl
#define readl readl
static inline u32 readl(const volatile void *addr)
{
	return *(const volatile u32 *)addr;
}
#endif

#ifndef writeb
#define writeb writeb
static inline void writeb(u8 value, volatile void *addr)
{
	*(volatile u8 *)addr = value;
}
#endif

#ifndef writew
#define writew writew
static inline void writew(u16 value, volatile void *addr)
{
	*(volatile u16 *)addr = value;
}
#endif

#ifndef writel
#define writel writel
static inline void writel(u32 value, volatile void *addr)
{
	*(volatile u32 *)addr = value;
}
#endif

#ifndef ioremap
#define ioremap ioremap
static inline void *ioremap(phys_addr_t offset, size_t size)
{
	return (void *)(unsigned long)offset;
}
#endif

#ifndef phys_to_virt
#define phys_to_virt phys_to_virt
static inline void *phys_to_virt(phys_addr_t addr)
{
	return (void *)(unsigned long)addr;
}
#endif

#ifndef virt_to_phys
#define virt_to_phys virt_to_phys
static inline phys_addr_t virt_to_phys(volatile void *addr)
{
	return (phys_addr_t)(unsigned long)addr;
}
#endif

#ifndef IOBASE
#define IOBASE	((void *)0)
#endif

#ifndef inb
#define inb inb
static inline u8 inb(unsigned long addr)
{
	return readb(IOBASE + addr);
}
#endif

#ifndef inw
#define inw inw
static inline u16 inw(unsigned long addr)
{
	return readw(IOBASE + addr);
}
#endif

#ifndef inl
#define inl inl
static inline u32 inl(unsigned long addr)
{
	return readl(IOBASE + addr);
}
#endif

#ifndef outb
#define outb outb
static inline void outb(u8 value, unsigned long addr)
{
	writeb(value, IOBASE + addr);
}
#endif

#ifndef outw
#define outw outw
static inline void outw(u16 value, unsigned long addr)
{
	writew(value, IOBASE + addr);
}
#endif

#ifndef outl
#define outl outl
static inline void outl(u32 value, unsigned long addr)
{
	writel(value, IOBASE + addr);
}
#endif

#endif /* __ARCOS_IO_H__ */
