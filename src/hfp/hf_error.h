// gl.h should be included earlier
#ifndef _HF_ERROR_
#define _HF_ERROR_

#ifdef WIN32

	void ReportError(const char* msg, int errorCode, const char* fName, int lineNum);

#else

#define	ReportError(msg, error, fName, lineNum)	printf("%s (0x%x) at %s : %d\n", msg, error, fName, lineNum);

#endif

// used to automatically trace some GL errors
#define	GL_CALL(func, error_action)											\
	{																						\
		func;																				\
		GLenum error = glGetError();												\
		if (error != GL_NO_ERROR) {												\
			ReportError(#func" - failed", error, __FILE__, __LINE__);	\
			error_action;																\
		}																					\
	}

#endif
