#ifndef INTERRUPTS_H
#define INTERRUPTS_H 

#ifdef __cplusplus
extern "C" {
#endif

uint32_t __SP_INIT;

extern void __thumb_startup();

typedef void (* const InterruptFunc)(void);

typedef struct {
	void * 			__ptr;
	InterruptFunc 	__func[0x61];
} VectorTable;

extern const VectorTable __vect_table;
  
#ifdef __cplusplus
}
#endif
 
#endif
