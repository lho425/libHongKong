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

#ifndef CH_H
#define	CH_H
#include <memory>
#include "lowlevel_io.h"
class channel {
    std::unique_ptr<lowlevel_IO> lowIO;
    
    static unsigned char ch_to_sub_4bit(int ch);

public:
    channel(lowlevel_IO * lio) : lowIO(lio)
    {
        
    }


    void out(int ch, unsigned char data) const;
    unsigned char in() const ;
};


#endif	/* CH_H */

