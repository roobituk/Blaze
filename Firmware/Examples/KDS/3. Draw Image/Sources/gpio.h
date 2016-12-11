#ifndef __gpio_h
#define __gpio_h

// sets the specified bit in the gpio port
#define gpio_set_bit(r,b)									(set_bit(r##_PSOR,b))

// sets the specified bit mask in the gpio port
#define gpio_set_bits(r,m)									(set_bits(r##_PSOR,m))

// clears the specified bit in the gpio port
#define gpio_clr_bit(r,b)									(set_bit(r##_PCOR,b))

// clears the specified bit mask in the gpio port
#define gpio_clr_bits(r,m)									(set_bits(r##_PCOR,m))

#endif
