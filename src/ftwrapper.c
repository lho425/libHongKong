
#include "ftwrapper.h"
#include <stdio.h>

int connect_FT232H(FT_HANDLE *p_ftHandle) {
    FT_STATUS ftStatus;
    DWORD dwNumDevs;

    ftStatus = FT_CreateDeviceInfoList(&dwNumDevs);
    if (ftStatus != FT_OK) {
        return 1 << 0;
    }
    if (dwNumDevs < 1) {
        return 1 << 1;
    }

    FT_Open(0, p_ftHandle);
    if (ftStatus != FT_OK) {
      fprintf(stderr, "faild to open FT232H!\n");
        return 1 << 2;
    }
    return 0;
}

int setup_FT232H(FT_HANDLE ftHandle) {
    FT_STATUS ftStatus;
    ftStatus = FT_ResetDevice(ftHandle);
    if (ftStatus != FT_OK) {
        return 1 << 3;
    }
    
    //see AN_135_MPSSE_Basics.pdf page16.
    ftStatus |= FT_SetBitMode(ftHandle, 0x0, 0x00);//reset.
    ftStatus |= FT_SetBitMode(ftHandle, 0x0, 0x02);//enable mpsse. See D2XX_Programmer's_Guide(FT_000071).pdf page69.
    if (ftStatus != FT_OK) {
        return 1 << 4;
    }



    ftStatus = FT_SetTimeouts(ftHandle, 1000, 1000);
    if (ftStatus != FT_OK) {
        return 1 << 5;
    }

    /*** Flush FT232H receive buffer if not empty ***/
    DWORD recv,retn;
    ftStatus |= FT_GetQueueStatus(ftHandle, &recv);
    if ((ftStatus == FT_OK) && (recv > 0)){
      char buf[recv];
      FT_Read(ftHandle, &buf, recv, &retn);
    }
    //Read out the data from FT2232H receive buffer



    //unsigned char _c[] = {0x8B};
    unsigned char _c[] = {0x8A};
    DWORD _D;
    ftStatus = FT_Write(ftHandle, _c, sizeof (_c), & _D  );

    FT_SetUSBParameters(ftHandle, 64*1024, 64*1024);

    return 0;
}

int disconnect_FT232H(FT_HANDLE ftHandle) {
    FT_STATUS ftStatus;
    ftStatus = FT_Close(ftHandle);
    if (ftStatus != FT_OK) {
      fprintf(stderr,"faild to close FT232H!\n");
        return 1;
    }
    return 0;
}

/*
lowlevel_IO_D2XX::lowlevel_IO_D2XX() {


    if (connect_FT232H() != 0) {
        throw string("lowlevel_IO_D2XX::lowlevel_IO_D2XX(): connect_FT232H() failed");
    }

    if (setup_FT232H() != 0) {
        disconnect_FT232H();
        throw string("lowlevel_IO_D2XX::lowlevel_IO_D2XX(): setup_FT232H() failed");
    }

}

lowlevel_IO_D2XX::~lowlevel_IO_D2XX() {
    disconnect_FT232H();
}
*/
