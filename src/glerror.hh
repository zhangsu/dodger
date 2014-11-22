#ifndef DODGER_GLERROR_H_
#define DODGER_GLERROR_H_

// Checks OpenGL errors since last OpenGL call.
#define checkGlError() _checkGlError(__FILE__, __LINE__)

// Internal implementation used by checkGlError().
void _checkGlError(const char* filename, int lineno);

#endif // DODGER_GLERROR_H_
