/*
 * MATLAB Compiler: 4.1 (R14SP1)
 * Date: Sat Oct 24 15:12:47 2009
 * Arguments: "-B" "macro_default" "-m" "-W" "main" "-T" "link:exe" "OBR" 
 */

#include <stdio.h>
#include "mclmcr.h"
#ifdef __cplusplus
extern "C" {
#endif
extern const unsigned char __MCC_OBR_public_data[];
extern const char *__MCC_OBR_name_data;
extern const char *__MCC_OBR_root_data;
extern const unsigned char __MCC_OBR_session_data[];
extern const char *__MCC_OBR_matlabpath_data[];
extern const int __MCC_OBR_matlabpath_data_count;
extern const char *__MCC_OBR_classpath_data[];
extern const int __MCC_OBR_classpath_data_count;
extern const char *__MCC_OBR_mcr_runtime_options[];
extern const int __MCC_OBR_mcr_runtime_option_count;
extern const char *__MCC_OBR_mcr_application_options[];
extern const int __MCC_OBR_mcr_application_option_count;
#ifdef __cplusplus
}
#endif

static HMCRINSTANCE _mcr_inst = NULL;


static int mclDefaultPrintHandler(const char *s)
{
    return fwrite(s, sizeof(char), strlen(s), stdout);
}

static int mclDefaultErrorHandler(const char *s)
{
    int written = 0, len = 0;
    len = strlen(s);
    written = fwrite(s, sizeof(char), len, stderr);
    if (len > 0 && s[ len-1 ] != '\n')
        written += fwrite("\n", sizeof(char), 1, stderr);
    return written;
}

bool OBRInitializeWithHandlers(
    mclOutputHandlerFcn error_handler,
    mclOutputHandlerFcn print_handler
)
{
    if (_mcr_inst != NULL)
        return true;
    if (!mclmcrInitialize())
        return false;
    if (!mclInitializeComponentInstance(&_mcr_inst, __MCC_OBR_public_data,
                                        __MCC_OBR_name_data,
                                        __MCC_OBR_root_data,
                                        __MCC_OBR_session_data,
                                        __MCC_OBR_matlabpath_data,
                                        __MCC_OBR_matlabpath_data_count,
                                        __MCC_OBR_classpath_data,
                                        __MCC_OBR_classpath_data_count,
                                        __MCC_OBR_mcr_runtime_options,
                                        __MCC_OBR_mcr_runtime_option_count,
                                        true, NoObjectType, ExeTarget, NULL,
                                        error_handler, print_handler))
        return false;
    return true;
}

bool OBRInitialize(void)
{
    return OBRInitializeWithHandlers(mclDefaultErrorHandler,
                                     mclDefaultPrintHandler);
}

void OBRTerminate(void)
{
    if (_mcr_inst != NULL)
        mclTerminateInstance(&_mcr_inst);
}

int main(int argc, const char **argv)
{
    int _retval;
    if (!mclInitializeApplication(__MCC_OBR_mcr_application_options,
                                  __MCC_OBR_mcr_application_option_count))
        return 0;
    
    if (!OBRInitialize())
        return -1;
    _retval = mclMain(_mcr_inst, argc, argv, "obr", 1);
    if (_retval == 0 /* no error */) mclWaitForFiguresToDie(NULL);
    OBRTerminate();
    mclTerminateApplication();
    return _retval;
}
