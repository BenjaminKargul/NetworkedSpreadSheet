#include "ss.cpp"
#include "dg.cpp"
#include <iostream>

int main()
{
  Spreadsheet* ss = new Spreadsheet();
  ss->SetCellContents("A1" , "=A3");
  ss->SetCellContents("A2" , "=A1 +A3");
  ss->SetCellContents("A3" , "=A1");
  std::cout << ss->GetCellContents("A3") << std::endl;

  std::cout << ss->get_variables("=A1 + B2 ")[1] << std::endl;
}