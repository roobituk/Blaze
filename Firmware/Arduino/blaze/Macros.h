#ifndef MACROS_H
#define MACROS_H

#ifndef ASSERT
#define ASSERT(X)
#endif

#define BIT(x)			(1 << x)
#define SET_BIT(x, r)	(r |= BIT(x))
#define CLR_BIT(x, r) 	(r &= ~BIT(x))

#define CLI				asm("CPSID i")
#define SEI				asm("CPSIE i")
#define NOP				asm("NOP")

#define CLK_PER_US		(F_CPU / 1000000L)
#define CLK_TO_US(a)	((a) / CLK_PER_US)	
#define US_TO_CLK(a)	((a) * CLK_TO_US)		

// Math macros
#define MIN(a,b)		((a) < (b) ? (a) : (b))
#define MAX(a,b)		((a) > (b) ? (a) : (b))
//#define ABS(x)			((x) > 0 ? (x) : -(x))

// Lowercase alternatives
#define min				MIN
#define max				MAX
#define abs				ABS

#define clockCyclesPerMicrosecond()		CLK_PER_US
#define clockCyclesToMicroseconds(a)	CLK_TO_US(a)
#define microsecondsToClockCycles(a)	US_TO_CLK(a)

#endif