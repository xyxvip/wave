//
// MATLAB Compiler: 6.1 (R2015b)
// Date: Wed Jul 06 14:09:46 2016
// Arguments: "-B" "macro_default" "-W" "cpplib:Thnr" "-T" "link:lib" "Thnr" 
//

#ifndef __Thnr_h
#define __Thnr_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_Thnr
#define PUBLIC_Thnr_C_API __global
#else
#define PUBLIC_Thnr_C_API /* No import statement needed. */
#endif

#define LIB_Thnr_C_API PUBLIC_Thnr_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_Thnr
#define PUBLIC_Thnr_C_API __declspec(dllexport)
#else
#define PUBLIC_Thnr_C_API __declspec(dllimport)
#endif

#define LIB_Thnr_C_API PUBLIC_Thnr_C_API


#else

#define LIB_Thnr_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_Thnr_C_API 
#define LIB_Thnr_C_API /* No special import/export declaration */
#endif

extern LIB_Thnr_C_API 
bool MW_CALL_CONV ThnrInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_Thnr_C_API 
bool MW_CALL_CONV ThnrInitialize(void);

extern LIB_Thnr_C_API 
void MW_CALL_CONV ThnrTerminate(void);



extern LIB_Thnr_C_API 
void MW_CALL_CONV ThnrPrintStackTrace(void);

extern LIB_Thnr_C_API 
bool MW_CALL_CONV mlxThnr(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_Thnr
#define PUBLIC_Thnr_CPP_API __declspec(dllexport)
#else
#define PUBLIC_Thnr_CPP_API __declspec(dllimport)
#endif

#define LIB_Thnr_CPP_API PUBLIC_Thnr_CPP_API

#else

#if !defined(LIB_Thnr_CPP_API)
#if defined(LIB_Thnr_C_API)
#define LIB_Thnr_CPP_API LIB_Thnr_C_API
#else
#define LIB_Thnr_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_Thnr_CPP_API void MW_CALL_CONV Thnr(int nargout, mwArray& SINAD, mwArray& SNR, mwArray& THD, mwArray& SFDR, mwArray& ENOB, const mwArray& filename);

#endif
#endif
