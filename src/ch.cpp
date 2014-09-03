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

unsigned char channel::ch_to_sub_4bit(int ch){
    //GOIOL1..3 are the ch select bit.
    //See HongKong_FT232R electrical diagram and 74HC138 ligic table.
    
    //HongKong_FT232R の回路図と74HC138の論理表を参照
/*
	unsigned char c;//cbus_4bit_data;
	switch(ch){
	case 1:
		c = 7<<1;
		break;
	case 2:
		c=6<<1
		break;
	//THIS IS FOOL!
*/
	if(ch>0){
		return (8- ch)<<1;//THIS IS COOL!
	}else{//ch==0
		return 0;
	}
}




void channel::out(int ch, unsigned char data){
	unsigned char _ch = ch_to_sub_4bit(ch) & 0xFE;//1110
	lowIO->write_sub_4bit(_ch | 1);//ck=Low. select ch.
	lowIO->write(data);//set output data
	lowIO->write_sub_4bit(_ch | 0 );//ck=Hi. select ch and output data persistently.
		//cbus0 bit is connected to CK bit of 74HCT574 on HongKong_FT232R.
}

unsigned char channel::in(){
	unsigned char _ch = ch_to_sub_4bit(0) & 0xFE;//1110
	lowIO->write_sub_4bit(_ch | 0);//select ch0 and ch0 direction L (B->A). see74HCT245.
	return lowIO->read();
}
