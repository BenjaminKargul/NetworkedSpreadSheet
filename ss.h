#ifndef SS_H
#define SS_H

#include "dg.h"
#include <string>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <stdexcept>

class Spreadsheet
{
  
private:
  // Represents a spreadsheet cell.
  class Cell
  {
  public:
    std::string name;
    std::string contents;

    Cell & operator=(const Cell &rhs);

     Cell()
      {
	this->name = "";
	this->contents = "";                
      }
    
    //Constructs a cell with a name and contents
    Cell(std::string name, std::string contents)
      {
	this->name = name;
	this->contents = contents;                
      }
  };    

  std::map<std::string, Cell> cellSet;
  DependencyGraph dependencies;

  // True if this spreadsheet has been modified since it was created or saved                  
  // (whichever happened most recently); false otherwise.
  bool Changed;

 
  /// A helper for the GetCellsToRecalculate method. (Circular Dependencies happen here.)
  void Visit(std::string start, std::string name, std::set<std::string> visited, std::list<std::string> changed);


  void loadSpreadsheet(std::string filePath);

  std::list<std::string> SetCellContentsHelper(std::string name, std::string contents);

  std::list<std::string> GetCellsToRecalculate(std::set<std::string> names);

  std::list<std::string> GetCellsToRecalculate(std::string name);

  std::list<std::string> InsertCell(std::string name, std::string contents);
  


 public:

  std::vector<std::string> get_variables(std::string s);
  
  class CircularException: public std::runtime_error
   {
   public:
   CircularException(std::string s) : std::runtime_error(s)
       {
       }
   };
 
 Spreadsheet();
 
 std::string GetCellContents(std::string name);
 
 std::string GetCellContentsAsString(std::string name);
 
 std::list<std::string> GetNamesOfAllNonemptyCells();

 std::list<std::string> SetCellContents(std::string name, std::string text);

 std::vector<std::string> GetDirectDependents(std::string name);
 
 void Save(std::string filename);
 
 std::list<std::string> SetContentsOfCell(std::string name, std::string content);
 
 
 
};









#endif
