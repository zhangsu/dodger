#ifndef DODGER_GLERROR_H_
#define DODGER_GLERROR_H_

#define checkGlError() _checkGlError(__FILE__, __LINE__)

void _checkGlError(const char* filename, int lineno);

#endif // DODGER_GLERROR_H_
