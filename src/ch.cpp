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

#include "ch.h"
#include "lowlevel_io.h"




void channel::out(int ch, unsigned char data){
  unsigned char IC_select_bits=0b01000000;//dir,!oe, ck, ck, ...
  if ( ch == 0 ){
    IC_select_bits = 0b10000000;//dir=output, !oe=enable
  }else{
    IC_select_bits = 0b01000000 | IC_select_bits>>ch;
    //!oe=disable, ch's ck=Hi. select ch and keep output data on ch channel IC.
  }
	  
  lowIO->write_sub_8bit(0b01000000);//!oe=disable, ck=Low
  lowIO->write(data);//set output data
  lowIO->write_sub_8bit( IC_select_bits );//if ch>=1: keep data;if ch==0,simply output
  

}

unsigned char channel::in(){
	lowIO->write_sub_8bit(0b00000000);//select ch0 and ch0 direction L (B->A). see74HCT245.
	return lowIO->read();
}
