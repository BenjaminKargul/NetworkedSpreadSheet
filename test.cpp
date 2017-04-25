#include "ss.h"
#include "dg.h"
#include <iostream>

int main()
{
  Spreadsheet* ss = new Spreadsheet();
  std::cout<<"add =a3+a2"<<std::endl;
  //ss->SetCellContents("a1" , "=a3 + a2");
ss->SetCellContents("A2" , "=1");
ss->SetCellContents("A3" , "=1");
  ss->SetCellContents("A1" , "=a3 + a2");
  // std::cout<<"add =a1"<<std::endl;
  // ss->SetCellContents("a2" , "=a1");
  // ss->SetCellContents("a3" , "=a1");
  //std::cout << ss->GetCellContents("a3") << std::endl;
std::cout << ss->GetCellContents("A1") << std::endl;
  //std::cout << ss->get_variables("=a1 + b2 ")[0] << std::endl;
}
