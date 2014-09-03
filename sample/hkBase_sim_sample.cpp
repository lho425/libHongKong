//public domain

#include <cstdlib>
#include <iostream>

using namespace std;
#include "libHongKong.h"
#include "SFC.h"
#include "ch.h"
#include "lowlevel_io.h"

#include "lowlevel_io.h"

int main(int argc, char** argv) {

  try {
    HongKongBase hk(new SFC_mem_ops(
				new channel(new lowlevel_IO_simulator("testdata.txt") )
				)
		);
    hk.LoadROM(cout,0,10);
    cout <<endl;

    hk.LoadROM(cout,1000000,8);
    cout <<endl;


  } catch (string e) {
    std::cout << e << std::endl;
    return 1;
  } catch (...) {
    std::cout << "Exception!" << std::endl;
    return 1;
  }


  return 0;
}



