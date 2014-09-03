#ifndef FTWRAPPER_H
#define FTWRAPPER_H

#include <windows.h>
#include "ftd2xx.h"

int connect_FT232H(FT_HANDLE *p_ftHandle) ;
int setup_FT232H(FT_HANDLE ftHandle) ;
int disconnect_FT232H(FT_HANDLE ftHandle) ;

#endif //FTWRAPPER_H
