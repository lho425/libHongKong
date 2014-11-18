/*

    Copyright 2014 LHO425.

This file is distributed under the MIT License.
See LICENSE/MIT.TXT for details.

*/

#ifndef LOWLEVEL_IO_H
#define LOWLEVEL_IO_H

#include <iostream>
#include <string>
using namespace std;

class lowlevel_IO
{
public:
	virtual void write_sub_8bit(unsigned char data) = 0;
	virtual void write(unsigned char data) = 0;
	virtual unsigned char read() = 0;

	virtual
	~lowlevel_IO()
	{}



};

class DUMMY_lowIO : public lowlevel_IO
{
public:

	void
	write_sub_8bit(unsigned char data)
	{
	};

	void
	write(unsigned char data)
	{
	};

	unsigned char
	read()
	{
	  return char();
	};
};

class DUMMY_EXCEPTION_lowIO : public lowlevel_IO
{

	void
	write_sub_8bit(unsigned char data)
	{
	};

	void
	write(unsigned char data)
	{
	};

	unsigned char
	read()
	{
	  return char();
	};

public:

	DUMMY_EXCEPTION_lowIO()
	{
		throw string("DUMMY_EXCEPTION_lowIO");
	}
};


#include <fstream>

class lowlevel_IO_simulator : public lowlevel_IO
{
private:
	int _sub8bit;
	unsigned char _ch_data_not_persisted;
	unsigned char _ch_data[8];
	std::fstream ROM_img;
	int open_ROM_Image_File();
	int setup_lowlevel_IO_func_ptr();
	int close_ROM_Image_File();

public:
	void write_sub_8bit(unsigned char data);
	void write(unsigned char data);
	unsigned char read();

	lowlevel_IO_simulator(string ROM_Path) : ROM_img(ROM_Path)
	{
	};

	~lowlevel_IO_simulator()
	{
	};

};





#include <windows.h>
#include "ftd2xx.h"

class lowlevel_IO_D2XX : public lowlevel_IO
{
protected:
	FT_HANDLE ftHandle;

	int connect_FT232H();

	int setup_FT232H();

	int setup_lowlevel_IO_func_ptr();
	int disconnect_FT232H();
public:
	lowlevel_IO_D2XX();

	virtual ~lowlevel_IO_D2XX();


	void write_sub_8bit(unsigned char data);

	void write(unsigned char data);

	unsigned char read();
		
};





#endif //LOWLEVEL_IO_H

