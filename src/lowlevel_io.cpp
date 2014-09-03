/*

    Copyright 2014 LHO425.

This file is distributed under the MIT License.
See LICENSE/MIT.TXT for details.

*/





#include <iostream>
#include <fstream>
#include <memory>


#include "lowlevel_io.h"



#include <windows.h>
#include "ftd2xx.h"

int lowlevel_IO_D2XX::connect_FT232H() {
    FT_STATUS ftStatus;
    DWORD dwNumDevs;

    ftStatus = FT_CreateDeviceInfoList(&dwNumDevs);
    if (ftStatus != FT_OK) {
        return 1 << 0;
    }
    if (dwNumDevs < 1) {
        return 1 << 1;
    }

    FT_Open(0, &ftHandle);
    if (ftStatus != FT_OK) {
        std::cerr << "faild to open FT232H!" << std::endl;
        return 1 << 2;
    }
    return 0;
}

int lowlevel_IO_D2XX::setup_FT232H() {
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



    return 0;
}

int lowlevel_IO_D2XX::disconnect_FT232H() {
    FT_STATUS ftStatus;
    ftStatus = FT_Close(ftHandle);
    if (ftStatus != FT_OK) {
        std::cerr << "faild to close FT232H!" << std::endl;
        return 1;
    }
    return 0;
}

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

/*******IO operetions****/
void lowlevel_IO_D2XX::write_sub_4bit(unsigned char data) {
    
    return write_sub_8bit(data & 0x0F);
    //7 6 5 4 3 2 1 0  <-ADBUS = data
    //3 2 1 0          <-GPIOL

}

void lowlevel_IO_D2XX::write_sub_8bit(unsigned char data) {
    unsigned char buf[] = {
        0x80,
        data,
        0xFF //ADBUS0..7  output 
    };
 
    DWORD BytesWritten;
    FT_Write(ftHandle, buf, sizeof (buf), &BytesWritten);

}

void lowlevel_IO_D2XX::write(unsigned char data) {
    unsigned char buf[] = {
        0x82,
        data,
        0xFF
    };
    DWORD BytesWritten;
    FT_Write(ftHandle, buf, sizeof (buf), &BytesWritten);

}

unsigned char lowlevel_IO_D2XX::read() {
    DWORD BytesWritten;
    unsigned char buf[] = {
        0x82,
        0x0,
        0x0,
	0x83
    };
    FT_Write(ftHandle, buf, sizeof (buf), &BytesWritten); //send input command

    DWORD BytesReceived;
    unsigned char r_buf;
    FT_Read(ftHandle, &r_buf, sizeof (char), &BytesReceived);
    return r_buf;
}


/*******IO operetions****/

int lowlevel_IO_simulator::sub4bit_to_ch(unsigned char sub4bit) {
    //GOIOL1..3 are the ch select bit.
    //See HongKong_FT232R electrical diagram and 74HC138 ligic table.

    //HongKong_FT232R の回路図と74HC138の論理表を参照

    sub4bit = sub4bit & 0x0F;

    if (sub4bit != 0) {
        return (8 -
                (sub4bit >> 1)
                );


    }
    return 0;
}

void lowlevel_IO_simulator::write_sub_4bit(unsigned char data) {
	//data is current sub4bit.
    int ch = sub4bit_to_ch(data);
    if (sub4bit_to_ch(_prev_sub4bit) == ch && (_prev_sub4bit & 1) == 1 && (data & 1) ==0)//チャンネルが前と同じで、1bit目が1から0に変わってたら永続化する
    	_ch_data[ch] = _ch_data_not_persisted;
    
    _prev_sub4bit = data;
}

void lowlevel_IO_simulator::write(unsigned char data) {
    //SRAM書き込みはまだ実装してない
    _ch_data_not_persisted = data;

}



unsigned char lowlevel_IO_simulator::read() {
    unsigned int addr = 0;
    unsigned char buf;
    // if OE? CE? LoROM?

    //A0...A20
    addr += _ch_data[1] << 0 * 8;
    addr += _ch_data[2] << 1 * 8;
    addr += (_ch_data[3] & 0x1F) << 2 * 8; //0x1F:0001,1111, use A16..A20 bit.

    ROM_img.seekg(addr);
    ROM_img.read((char*) &buf, sizeof (buf));

    if (ROM_img.eof()) {
        //buf = 0;
        throw string("lowlevel_IO_simulator::read(): end of file !");

    }

    return buf;
}



