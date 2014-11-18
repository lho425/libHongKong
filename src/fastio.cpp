/*

    Copyright 2014 LHO425.

This file is distributed under the MIT License.
See LICENSE/MIT.TXT for details.

*/

#include <windows.h>
#include "ftd2xx.h"

#include "fastio.h"
#include <iostream>
#include <sstream>
#include <cstdio>
#include <climits>
#include <cassert>

using namespace std;


#include <windows.h>
#include "ftd2xx.h"


/***Async***/

void FTbuf::flush(){
  FT_STATUS ftStatus;
  DWORD BytesWritten;
  auto &used_buf_size = write_buf_next_offset;
  ftStatus = FT_Write(ftHandle, write_buf, used_buf_size, &BytesWritten);
  if (ftStatus != FT_OK) {
    throw string ("faild to write FT232H!");
  } 
  
  // for(int i = 0;i<used_buf_size;i++){
  //   printf("write_buf[%d] : %X\n",i, write_buf[i] );
  // }


  write_buf_next_offset = 0;
  return ;
}

unsigned int FTbuf::read(){
  DWORD BytesReturned;
  FT_STATUS ftStatus;
  if(write_buf_next_offset >0 ) 
    flush();
  
  if (read_command_count > read_buf_size)
    throw string("read_command_count > read_buf_size");
  if (read_command_count > read_buf_size - read_buf_avail_size) //read_command_count > not-used
    throw string("please clear_read_buf()");
  
  ftStatus = FT_Read(ftHandle,&read_buf[read_buf_avail_size], read_command_count, &BytesReturned);

  if (ftStatus != FT_OK ) {
    stringstream s;
    s<<"FTbuf::read(): faild to read FT232H!"<< " ftStatus = "<<ftStatus <<endl;
      throw s.str();
    
  }

  if (read_command_count != BytesReturned) {
    stringstream s;
    s<<"FTbuf::read(): incomplete data receiving from FT232H!"<<"  read_command_count="<<read_command_count<<" BytesReturned="<<BytesReturned <<endl;
      throw s.str();
    
  }

     
  read_buf_avail_size += get_read_command_count() ;
  reset_read_command_count();
  
  return read_buf_avail_size;

}


// unsigned int lowlevel_IO_Fast_D2XX::Async_flush(){
//   auto &used_buf_size = write_buf_next_offset;
//   int available_write_buf_size = write_buf_size - used_buf_size;
//   if(available_write_buf_size <3 )
//     return flush_write_buf();
//   return 0;
  
// }  


void lowlevel_IO_FastIO::Async_write_sub_8bit(unsigned char uc){

  unsigned char buf[] = {0x80,uc,0xFF};
  ftbuf->write(buf,3);
  return ;
}


void lowlevel_IO_FastIO::Async_write(unsigned char uc){
  unsigned char buf[] = {0x82,uc,0xFF};
  ftbuf->write(buf,3);
  return ;
}

unsigned int lowlevel_IO_FastIO::Async_read(){
  unsigned char buf[] = {0x82,0,0,0x83};
  ftbuf->write(buf,4);

  ftbuf->inclement_read_command_count();

  return 0;
}


