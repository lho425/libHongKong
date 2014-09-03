/*

    Copyright 2014 LHO425.

This file is part of libHongKong.

libHongKong is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libHongKong is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libHongKong.  If not, see <http://www.gnu.org/licenses/>.

Additional permission under GNU GPL version 3 section 7

If you modify this Program, or any covered work, by linking or
combining it with FTDI D2XX Driver (or a modified version of that library),
containing parts covered by the terms of the license written
at http://www.ftdichip.com/Drivers/D2XX.htm, the licensors of
this Program grant you additional permission to convey the resulting work.

*/

#ifndef LIBHONGKONG_H
#define	LIBHONGKONG_H
#include <memory>




#include "ch.h"

class memory_operetions {
protected:
    std::unique_ptr<channel> ch;
public:

    memory_operetions( channel * &&ch) : ch(ch) {

    }

    virtual
    ~memory_operetions() {

    }
    virtual void set_address_ROM(unsigned int addr) = 0;

    virtual void set_address_SRAM(unsigned int addr) = 0;

    virtual void set_read_enable() = 0;

    virtual unsigned char read();

    virtual void set_ROM_type(int type) = 0;



};

class HongKongBase {
    std::unique_ptr<memory_operetions> mem_ops;
protected:


    //generic algorism of copy casette memory data
    //カセット吸い出しの一般的なアルゴリズム
    virtual void do_LoadROM(unsigned char data[], unsigned int startAddress, unsigned int length); //unsafe
    //CAUTION! length >  data_length  will crash.

    virtual void
    do_LoadROM(std::ostream& ofs, unsigned int startAddress, unsigned int length);

public:


    void LoadROM(unsigned char data[], unsigned int startAddress, unsigned int length); //unsafe
    //CAUTION! endAddress >  data_length -1 will crash.

    void LoadROM(std::ostream& ofs, unsigned int startAddress, unsigned int length);

    void
    set_ROM_type(int type) {
        mem_ops->set_ROM_type(type);
    }

    HongKongBase( memory_operetions *&&m_pos) //throws any exception.
        : mem_ops(std::move(m_pos) ) {
    }

    HongKongBase(HongKongBase &&) = default;
    HongKongBase& operator=(HongKongBase &&) = default;

    HongKongBase(HongKongBase &) = delete;
    HongKongBase& operator=(HongKongBase &) = delete;

    virtual
    ~HongKongBase() = default;

};

#include "fastio.h"

template<class mem_ops_T>
class HongKong  {

    static FT_HANDLE get_inited_FT_HANDLE() {
        FT_HANDLE ftHandle;
        FT_STATUS ftStatus;
        DWORD dwNumDevs;
        ftStatus = FT_CreateDeviceInfoList(&dwNumDevs);
        if (ftStatus != FT_OK) {
            throw 1 << 0;
        }
        if (dwNumDevs < 1) {
            throw 1 << 1;
        }

        FT_Open(0, &ftHandle);
        if (ftStatus != FT_OK) {
            std::cerr << "faild to open FT232H!" << std::endl;
            throw 1 << 2;
        }


        ftStatus = FT_ResetDevice(ftHandle);
        if (ftStatus != FT_OK) {
            throw 1 << 3;
        }

        //see AN_135_MPSSE_Basics.pdf page16.
        //ftStatus |= FT_SetUSBParameters(ftHandle, 65536, 65536);
        //ftStatus |= FT_SetUSBParameters(ftHandle, 0, 0);
        ftStatus |= FT_SetBitMode(ftHandle, 0x0, 0x00); //reset.
        ftStatus |= FT_SetBitMode(ftHandle, 0x0, 0x02); //enable mpsse. See D2XX_Programmer's_Guide(FT_000071).pdf page69.
        if (ftStatus != FT_OK) {
            throw 1 << 4;
        }

        ftStatus = FT_SetTimeouts(ftHandle, 1000, 1000);
        if (ftStatus != FT_OK) {
            throw 1 << 5;
        }

        //Purge USB receive buffer first by reading out all old data from FT2232H receive buffer
        DWORD recv, retn;
        ftStatus |= FT_GetQueueStatus(ftHandle, &recv);
        // Get the number of bytes in the FT2232H
        //  receive buffer
        if ((ftStatus == FT_OK) && (recv > 0)) {
            char buf[recv];
            FT_Read(ftHandle, &buf, recv, &retn);
        }
        //Read out the data from FT2232H receive buffer



        //unsigned char _c[] = {0x8B};
        unsigned char _c[] = {0x8A};
        DWORD _D;
        ftStatus = FT_Write(ftHandle, _c, sizeof (_c), & _D);



        return ftHandle;

    }
    std::shared_ptr<FTbuf> _ftbuf_sptr;
    HongKongBase _hongkongbase;
public:
    shared_ptr<FTbuf_Reader> ftbufReader_sptr;

    HongKong() : _ftbuf_sptr(std::make_shared<FTbuf>(get_inited_FT_HANDLE(), 5 * 1024 * 1024, 64 * 1024)),
        _hongkongbase(new mem_ops_T(
                                    new channel(
                                           new lowlevel_IO_FastIO(std::make_shared<FTbuf_Writer>(_ftbuf_sptr))
                                           )
                                    )
                     ),
        ftbufReader_sptr(std::make_shared<FTbuf_Reader>(_ftbuf_sptr)) {
        //        int r = get_inited_FT_HANDLE(&ftHandle);
        //        if (r != 0)
        //            throw string("init FT err :"); //+std::to_string(r); //fixme!!!


    }

    void LoadROM(unsigned char data[], unsigned int startAddress, unsigned int length){
        return _hongkongbase.LoadROM(data, startAddress, length);
    }

    void LoadROM(std::ostream& ofs, unsigned int startAddress, unsigned int length){
        return _hongkongbase.LoadROM(ofs, startAddress, length);
    }

    void
    set_ROM_type(int type) {
        return _hongkongbase.set_ROM_type(type);
    }

};

template<class mem_ops_T>
class HongKongSim : public HongKongBase {
public:

    HongKongSim() :
        HongKongBase(new mem_ops_T(
                                    new channel(new lowlevel_IO_simulator("testdata.txt"))
                                    )
                     ) {
    }

};


#endif	/* LIBHONGKONG_H */

