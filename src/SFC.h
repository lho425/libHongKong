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

#ifndef SFC_H
#define	SFC_H

#include "libhongkong.h"


namespace SFC {
	const int LoROM = 1;
	const int HiROM = 0;
}

class SFC_mem_ops : public memory_operetions {
    bool LoROM = false;
public:

    SFC_mem_ops(channel * &&ch) :
    memory_operetions(std::move(ch) )
    {

    }
    void set_address_ROM(unsigned int addr) override;

    void set_address_SRAM(unsigned int addr) override;

    void set_read_enable() override;

    void set_ROM_type(int type) {
      LoROM = ( (type & SFC::LoROM) !=0 );
    }
};

using SFC_HongKong = HongKong<SFC_mem_ops>;
using SFC_HongKongSim = HongKongSim<SFC_mem_ops>;

#endif	/* SFC_H */

