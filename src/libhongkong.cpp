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

#include <iostream>
#include <fstream>


#include "libHongKong.h"

unsigned char memory_operetions::read() const {
    return ch->in();
}






//shared_ptr LoadROM(unsigned int startAddress, unsigned int endAddress);

void HongKongBase::LoadROM(unsigned char data[], unsigned int startAddress, unsigned int length) {//CAUTION! endAddress >  data_length -1 will crash.
    do_LoadROM(data, startAddress, length);
}

void HongKongBase::LoadROM(std::ostream& ofs, unsigned int startAddress, unsigned int length) {
    do_LoadROM(ofs, startAddress, length);
}

void HongKongBase::do_LoadROM(unsigned char data[], unsigned int startAddress, unsigned int length) {//CAUTION! endAddress >  data_length -1 will crash.
    mem_ops->set_read_enable();
    for (uint addr = startAddress; addr < startAddress + length; addr++) {
        mem_ops->set_address_ROM(addr);
        data[addr] = mem_ops->read(); //read mem data
    }
}

void HongKongBase::do_LoadROM(std::ostream& ofs, unsigned int startAddress, unsigned int length) {
    mem_ops->set_read_enable();
    for (uint addr = startAddress; addr < startAddress + length; addr++) {
        mem_ops->set_address_ROM(addr);
        ofs.put(mem_ops->read()); //read mem data
    }
}

