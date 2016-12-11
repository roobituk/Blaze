#ifndef __types_h
#define __types_h

// structure represents a rectangular bounds
typedef struct {

	int16_t left;					// left
	int16_t top;					// top
	int16_t right;					// right
	int16_t bottom;					// bottom

} bounds_t, *bounds_t_ptr;


// structure contains information about a device
typedef struct {

	char *name;						// name of the device
	void (*fn_open)(void);			// pointer to a function called to open the device
	void (*fn_close)(void);			// pointer to a function called to close the device

} device_info_t, *device_info_t_ptr;


// color type
typedef uint32_t color_t;


typedef struct {

	int16_t x;
	int16_t y;

} point_t, *point_t_ptr;



#ifdef __cplusplus


typedef struct {

	uint16_t Left;
	uint16_t Top;
	uint16_t Right;
	uint16_t Bottom;

} Bounds;


typedef struct {

	uint16_t X;
	uint16_t Y;

} Point;


typedef struct {

	uint16_t X;
	uint16_t Y;
	uint16_t Width;
	uint16_t Height;

} Rectangle;


typedef uint32_t Color;




#endif





#endif


