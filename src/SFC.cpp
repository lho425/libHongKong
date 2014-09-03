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

#include "SFC.h"
#include "ch.h"
#include "libHongKong.h"



void SFC_mem_ops::set_address_ROM(unsigned int addr) {
  if(LoROM){
    addr = (addr &0x1F8000)<<1//0x1F8000=111111000000000000000 A15~A20
      |(addr & 0x7FFF);//   0x7FFF=000000111111111111111 A0~A14
  }
  ch->out(1, addr & 0xFF);
  ch->out(2, (addr >> 8) & 0xFF);
  ch->out(3, (addr >> 16) & 0xFF);
}

void SFC_mem_ops::set_address_SRAM(unsigned int addr) {
  ch->out(1, addr & 0xFF);
  ch->out(2, (addr >> 8) & 0xFF);
  ch->out(3, ( (addr >> 16) | 0x60) & 0xFF ); //0x60 = 01100000
  //0x60 : A21,A22 bit . See SFC casette electrical diagram and 74139 datasheet.
  //0x60 : A21,A22 bit . SFCカセットの回路図と74139のデータシートを参照.
}

void SFC_mem_ops::set_read_enable() {
  ch->out(4, 0); //CS,OE active (active low)
}




