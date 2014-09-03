/*

    Copyright 2014 LHO425.

This file is distributed under the MIT License.
See LICENSE/MIT.TXT for details.

*/

#ifndef FASTIO_H
#define FASTIO_H

#include <memory>
#include "lowlevel_io.h"

struct noncopyable {
  noncopyable() = default;

  noncopyable(noncopyable&) = delete;
  noncopyable & operator =(noncopyable&)  = delete;
  noncopyable(const noncopyable&) = delete;
  noncopyable & operator =(const noncopyable&)  = delete;
};


#include "ftd2xx.h"

class FTbuf : private noncopyable{

  FT_HANDLE ftHandle;


  unsigned int read_command_count=0;
  unsigned int read_command_count_Max = 64*1024;

  const unsigned int read_buf_size ;
  unsigned char *read_buf;
  unsigned int read_buf_avail_size = 0 ;

  const unsigned int write_buf_size ;
  unsigned char *write_buf;
  unsigned int write_buf_next_offset =0;// == used buf size.

protected:
  inline void reset_read_command_count(){
    read_command_count =0;
  }

public:
  /** constructor, destructor **/
  FTbuf (FT_HANDLE && initialized_ftHandle, unsigned int read_buf_size,unsigned int write_buf_size)
    : ftHandle(initialized_ftHandle), read_buf_size(read_buf_size) , write_buf_size(write_buf_size)
  {
    FT_DEVICE ftDevice;
    FT_STATUS ftStatus;
    DWORD deviceID;
    char SerialNumber[16];
    char Description[64];

    ftStatus = FT_GetDeviceInfo(
				ftHandle,
				&ftDevice,
				&deviceID,
				SerialNumber,
				Description,
				NULL
				);

    if (ftStatus != FT_OK)  {
      throw string("invailed ftHandle!"); 
    }


    read_buf = new unsigned char[read_buf_size];
    write_buf = new unsigned char [write_buf_size];
   
  }

  virtual ~FTbuf(){
    
    FT_Close(ftHandle);
    
    delete[] read_buf;
    delete[] write_buf;
  }

  /** io operations **/

  /*
    バッファにsizeの余裕がなければflushしてから、バッファに書き込む
  */
  void write(unsigned char *buf, unsigned int size){
    if(size > write_buf_size - write_buf_next_offset )
      flush();
    
    for(unsigned int i = 0;i<size;i++){
      write_buf[write_buf_next_offset++]=buf[i];
    }
    //flush(); //slow
    //flush();read();  //very slow
  }

  unsigned int read();
  /*
    書き込みバッファをflushしてからread_command_countだけ readバッファに読み込む
    return : バッファの未読数
  */

  void flush();
  /*
    write bufをflushする
  */

  inline unsigned char* get_read_buf(){
    return read_buf;
  }

  //read_buf がクリアされた後、何が入っているかは保証しない。
  inline void clear_read_buf(){
    read_buf_avail_size =0;
  }


  /******* counter operations ******/

  //when overflowed,read() and ret 1
  int inclement_read_command_count(){
    read_command_count++; 
    if(read_command_count == read_buf_size || read_command_count == read_command_count_Max){
      read();
      return 1;
    }
    return 0;
  }

  // inline void add_read_command_count(unsigned int count){
  //   read_command_count+=count;
  // }

  inline unsigned int get_read_buf_size(){
    return read_buf_size;
  }

  inline unsigned int get_read_buf_avail_size(){
    return read_buf_avail_size;
  }


  inline unsigned int  get_read_command_count(){
    return read_command_count;
  }


};

class FTbuf_Reader : private noncopyable{
  std::shared_ptr<FTbuf> ftbuf;
  
public :
  FTbuf_Reader(const std::shared_ptr<FTbuf>& ftbuf_sptr)
    : ftbuf(ftbuf_sptr)
  {}

  inline unsigned int read(){
    return ftbuf->read();
  }
  inline unsigned char* get_read_buf(){
    return ftbuf->get_read_buf();
  }
  
  inline unsigned int get_read_buf_avail_size(){
    return ftbuf->get_read_buf_avail_size();
  }
  //read_buf がクリアされた後、何が入っているかは保証しない。
  inline void clear_read_buf(){
    ftbuf->clear_read_buf();
  }


};
	    

class FTbuf_Writer : private noncopyable{
  std::shared_ptr<FTbuf> ftbuf;
  
public :
  FTbuf_Writer(const std::shared_ptr<FTbuf>& ftbuf_sptr)
    : ftbuf(ftbuf_sptr)
  {}



  inline void write(unsigned char *buf, unsigned int size){
    ftbuf->write(buf, size);
  }


  /******* counter operations ******/

  //when overflowed,read() and ret 1
  inline int inclement_read_command_count(){
    return ftbuf->inclement_read_command_count();
  }

  // inline void add_read_command_count(unsigned int count){
  //   read_command_count+=count;
  // }


};


class lowlevel_IO_FastIO : public lowlevel_IO
{
protected:

  std::shared_ptr<FTbuf_Writer> ftbuf;

public:

  lowlevel_IO_FastIO(const std::shared_ptr<FTbuf_Writer>& ftbuf_W_sptr)
    : ftbuf(ftbuf_W_sptr)
  {}



  virtual ~lowlevel_IO_FastIO() = default;


	void write_sub_4bit(unsigned char data){
	  Async_write_sub_4bit(data);
	}

	void write(unsigned char data){
	  Async_write(data);
	}


	//reurn non-zero if Async_read() != 0
	unsigned char read(){
	  return static_cast<unsigned char>(Async_read() );
	}

	void write_sub_8bit(unsigned char data) //for debug.
	{
	  Async_write_sub_8bit(data);
	}


	unsigned int flush_write_buf();

	unsigned int receive_Async_read_buf();
	

	unsigned int Async_flush();

	void Async_write_sub_4bit(unsigned char data);

	void Async_write(unsigned char data);

	unsigned int Async_read();

	void Async_write_sub_8bit(unsigned char data) ;//for debug.

		
};

#endif //FASTIO_H
