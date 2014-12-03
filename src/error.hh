#ifndef DODGER_GLERROR_HH_
#define DODGER_GLERROR_HH_

#include <seal.h>

// Checks OpenGL errors since last OpenGL call.
#define checkGlError() _checkGlError(__FILE__, __LINE__)

// Checks SEAL errors returned by a SEAL call.
#define checkSealError(err) _checkSealError((err), __FILE__, __LINE__)

// Internal implementation used by checkGlError().
void _checkGlError(const char* filename, int lineno);

// Internal implementation used by checkSealError().
void _checkSealError(seal_err_t err, const char* filename, int lineno);

#endif // DODGER_GLERROR_HH_
