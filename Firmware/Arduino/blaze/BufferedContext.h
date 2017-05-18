
#ifndef BUFFERED_CONTEXT_H
#define BUFFERED_CONTEXT_H

#include "Context.h"

/*!-----------------------------------------------------------------------------

*/
class BufferedContext : public Context {
	
public:

	uint8_t *buffer() const { return m_pBuffer; }

protected:

	// Buffer to hold the off-screen frame
	uint8_t *m_pBuffer = NULL;

};

#endif
