/*
 * 
 * 
 * 
 */
#include "ss.h"
#include "dg.h"
#include <cstdlib>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <string>
#include <boost/foreach.hpp>
#include <iostream>

/// An AbstractSpreadsheet object represents the state of a simple spreadsheet.  A 
/// spreadsheet consists of an infinite number of named cells.
/// 
/// In a new spreadsheet, the contents of every cell is the empty string.
/// 
/// Spreadsheets are never allowed to contain a combination of Formulas that establish
/// a circular dependency.  A circular dependency exists when a cell depends on itself.


Spreadsheet::Cell & Spreadsheet::Cell::operator=(const Spreadsheet::Cell &rhs)
{
  if (this == &rhs)
    {      
      return *this; 
    } 

  this->name = rhs.name;
  this->contents = rhs.contents;
}



    
/// A helper for the GetCellsToRecalculate method. (Circular Dependencies happen here.)
void Spreadsheet::Visit(std::string start, std::string name, std::set<std::string> visited, std::list<std::string> changed)
{
  visited.insert(name);
  std::cout<<"Visiting "<< name <<std::endl;
  std::vector<std::string> search = GetDirectDependents(name);
  std::cout << "direct dependents has size of " << search.size() << std::endl;
  std::cout<<"Got direct dependents of "<<std::endl;
  BOOST_FOREACH(std::string str, search)
    {
      std::cout<<"checking visited strings"<<std::endl;
      if (str == start)
	{
	  std::cout<<"Is this the exception being thrown?"<<std::endl;
	  throw Spreadsheet::CircularException("CircEx");
	}
      else if (!(visited.count(str)))
	{
	  Visit(start, str, visited, changed);
	}
    }
  changed.push_front(name);
}
  

/// <summary>
/// Helper method to load the spreadsheet from a file.
/// </summary>
/// <param name="filePath">Path to the spreadsheet file.</param>
void Spreadsheet::loadSpreadsheet(std::string filePath)
{   
       
}

/// <summary>
/// An abstracted helper method that all SetCellContents methods can call.
/// 
/// If contents is null, throw ArgumentNullException.
/// 
/// Else, if name is null or invalid, throw InvalidNameException
/// 
/// </summary>
/// <param name="name">Name of the cell</param>
/// <param name="contents">Object to put into the cell (double, String, Formula)</param>
/// <returns>Set of the cells that need to be updated.</returns>
std::list<std::string> Spreadsheet::SetCellContentsHelper(std::string name, std::string contents)
{
  //Return the value that the InsertCell helper method returns.
  return Spreadsheet::InsertCell(name, contents);
}


 
/// Requires that names be non-null.  Also requires that if names contains s,
/// then s must be a valid non-null cell name.
/// 
/// If any of the named cells are involved in a circular dependency,
/// throws a CircularException.
/// 
/// Otherwise, returns an enumeration of the names of all cells whose values must
/// be recalculated, assuming that the contents of each cell named in names has changed.
/// The names are enumerated in the order in which the calculations should be done.  
/// 
/// For example, suppose that 
/// A1 contains 5
/// B1 contains 7
/// C1 contains the formula A1 + B1
/// D1 contains the formula A1 * C1
/// E1 contains 15
/// 
/// If A1 and B1 have changed, then A1, B1, and C1, and D1 must be recalculated,
/// and they must be recalculated in either the order A1,B1,C1,D1 or B1,A1,C1,D1.
/// The method will produce one of those enumerations.
/// 
/// Please note that this method depends on the abstract GetDirectDependents.
/// It won't work until GetDirectDependents is implemented correctly.
std::list<std::string> Spreadsheet::GetCellsToRecalculate(std::set<std::string> names)
{
  std::cout<<"entered recalculating"<<std::endl;
  std::list<std::string> changed;
  std::set<std::string> visited;
  BOOST_FOREACH(std::string n, names)
    {
      
      if (!(visited.count(n)))
	{
	  std::cout<<"Starting visits"<<std::endl;
	  Visit(n, n, visited, changed);
	}
    }
  return changed;
}


    
/// A convenience method for invoking the other version of GetCellsToRecalculate
/// with a singleton set of names.  See the other version for details.
std::list<std::string> Spreadsheet::GetCellsToRecalculate(std::string name)
{
  std::cout << "Getting cells to recalculate" << std::endl;
  std::set<std::string> to_return;

  to_return.insert(name);
    
  return GetCellsToRecalculate(to_return);
}

  


/// <summary>
/// This method attempts to set the contents of a cell depending on the type of object it is.
/// If it is a Formula, checks for a circular dependency, and throws the appropriate exception.
/// 
/// If an empty string is passed, the cell is removed from the cellSet.
/// </summary>
/// <param name="name">Name of the cell</param>
/// <param name="contents">Object being put into the cell (double, String, Formula)</param>
/// <returns>Set of the cells that need to be updated.</returns>
std::list<std::string> Spreadsheet::InsertCell(std::string name, std::string contents)
{
  //Construct a new cell with the contents that were passed to this method.
  Cell newCell(name, contents);

  //Used to keep track of the old cell contents, if any
  Cell oldCell;
    
  bool setContainedOldCell;
    
  std::cout<< "Inserting new cell, name = " << name << ", contents = " << contents << std::endl;

  std::map<std::string, Cell>::iterator it = cellSet.find(name);
  if (it != cellSet.end())
    {
      setContainedOldCell = true;
      oldCell = it->second;
    }
  else
    {
      setContainedOldCell = false;
    }
    
  //Used to keep track of the old dependees of the cell, if any.
  std::vector<std::string> oldDependees;

  if(contents == "")
    {
      std::cout << "Erasing contents of Cell" << std::endl;
      //Remove the cell (if one previously existed) to allow updated cell to replace it
      std::map<std::string, Cell>::iterator it = cellSet.find(name);
      if (it != cellSet.end())
	{
	  cellSet.erase(it);
	}
	
      //Remove all the dependees of the cell (a string doesn't depend on another cell)
      std::vector<std::string> blank;
      dependencies.ReplaceDependees(name, blank);
    }
  else if(contents[0] == '=')
    {
      //Remove the cell (if one previously existed) to allow updated cell to replace it
      
      std::cout<<"Contents is a formula" <<std::endl;
      cellSet.erase(name);
 

      //Add the new cell in its place
      cellSet[name] =  newCell;

      std::cout<< "Getting old dependies from graph" << std::endl;
      //Get the old dependees from the dependency graph
      oldDependees = dependencies.GetDependees(name);
      std::vector<std::string> variables = get_variables(contents);
      std::cout << "replacing old dependees of cell" << std::endl;
      std::cout << "There are " << variables.size() << " variables in formula " << contents << std::endl;
      //Replace the dependees of the cell with the variables that the Formula uses

      
      dependencies.ReplaceDependees(name, get_variables(contents));
    }
  else
    {
      //Remove the cell (if one previously existed) to allow updated cell to replace it
      std::map<std::string, Cell>::iterator it = cellSet.find(name);
      if (it != cellSet.end())
	{
	  cellSet.erase(it);
	}
      //Replace cell with new cell
      cellSet[name] =  newCell;
      //A single string doesn't depend on any other cells, so remove dependees
      std::vector<std::string> blank;
      dependencies.ReplaceDependees(name, blank);
    }

  try
    {
      std::cout << "getting cells to recalculate" << std::endl;
      std::list<std::string> cellsToRecalculate = GetCellsToRecalculate(name);

      this->Changed = true;

      std::list<std::string> to_return = cellsToRecalculate;

      return to_return;
    }
  catch (std::exception & e)
    {
      std::cout << "Exception" << std::endl;
      
      //Remove the recently added cell
      cellSet.erase(name);

      //If there was an old cell, put it back
      if (setContainedOldCell)
	{
	  cellSet[name] = oldCell;
	}

      //Replace the dependees of the cell with the old ones, if any
      dependencies.ReplaceDependees(name, oldDependees);
      std::cout<<"How about here"<<std::endl;
      //Continue throwing the exception
      throw e;

    }
}

  
std::vector<std::string> Spreadsheet::get_variables(std::string s)
{
  std::cout << "*****************************Getting Variables**********************************" << std::endl;
  std::vector<std::string> to_return;

  std::string read_string = "";
  bool reading = false;
  BOOST_FOREACH(char ch, s)
    {
      std::cout << "Char =  " << ch << std::endl;
      if(ch >= 'a' && ch <= 'z' && (!reading))
	{
	  reading = true;
	  read_string = "";
	  read_string += ch;
	}
      else if(ch >= '0' && ch <= '9' && reading)
	{
	  read_string += ch;
	}
      else if(reading)
	{
	  std::cout<<read_string<<std::endl;
	  to_return.push_back(read_string);
	  reading = false;
	}

    }
  std::cout<<read_string<<std::endl;
  to_return.push_back(read_string);
  reading = false;
  return to_return;
}

  
   


     
// Constructs a new, empty spreadsheet. 
Spreadsheet::Spreadsheet()
{
}

   
/// <summary>
/// If name is null or invalid, throws an InvalidNameException.
/// 
/// Otherwise, returns the contents (as opposed to the value) of the named cell.  The return
/// value should be either a string, a double, or a Formula.
/// </summary>
/// <param name="name">Name of the cell</param>
/// <returns>Cell contents</returns>
std::string Spreadsheet::GetCellContents(std::string name)
{
  //Make sure input string isn't input
  //
  //

  Cell cell;

  //If the cell is in the Dictionary, it has a value. Return the contents of the cell.
  std::map<std::string, Cell>::iterator it = cellSet.find(name);
  if (it != cellSet.end())
    {
      return it->second.contents;
    }
  else
    {
      //Else if cell doesn't contain a value, return empty string
      return "";
    }
}

std::string Spreadsheet::GetCellContentsAsString(std::string name)
{
  return GetCellContents(name);
}

/// <summary>
/// Enumerates the names of all the non-empty cells in the spreadsheet.
/// </summary>
/// <returns>Enumerable list of nonempty cells</returns>
std::list<std::string> Spreadsheet::GetNamesOfAllNonemptyCells()
{
  std::list<std::string> to_return;
    
  //The dictionary only contains nonempty cells to begin with.
  //Construct a new list with the names of these cells (The "keys" of the dictionary that stores the cells).
  for (std::map<std::string, Cell>::iterator it = cellSet.begin(); it != cellSet.end(); ++it)
    {
      to_return.push_front(it->first);
    }
  return to_return;
}


/// <summary>
/// If text is null, throws an ArgumentNullException.
/// 
/// Otherwise, if name is null or invalid, throws an InvalidNameException.
/// 
/// Otherwise, the contents of the named cell becomes text.  The method returns a
/// set consisting of name plus the names of all other cells whose value depends, 
/// directly or indirectly, on the named cell.
/// </summary>
/// <param name="name">Name of the cell</param>
/// <param name="formula">The string to input</param>
/// <returns>Set of changed cells</returns>
std::list<std::string> Spreadsheet::SetCellContents(std::string name, std::string text)
{
  return SetCellContentsHelper(name, text);
}



/// <summary>
/// If name is null, throws an ArgumentNullException.
/// 
/// Otherwise, if name isn't a valid cell name, throws an InvalidNameException.
/// 
/// Otherwise, returns an enumeration, without duplicates, of the names of all cells whose
/// values depend directly on the value of the named cell.  In other words, returns
/// an enumeration, without duplicates, of the names of all cells that contain
/// formulas containing name.
/// </summary>
/// <param name="name">Name of the cell</param>
std::vector<std::string> Spreadsheet::GetDirectDependents(std::string name)
{
  //Use the dependency graph to return the dependents of a cell
  std::cout<<"Getting direct dependents for "<< name <<std::endl;
  return dependencies.GetDependents(name);
}

  

/// <summary>
/// Saves the spreadsheet to the file.
/// 
/// Directories must be created before calling this method, if they don't exist yet.
/// </summary>
/// <param name="filename">Path to the file</param>
void Spreadsheet::Save(std::string filename)
{

}

/// <summary>
/// Sets the contents of the desired cell.
/// Returns a set of cells that have been changed by this operation.
/// </summary>
/// <param name="name">Name of the cell</param>
/// <param name="content">Contents of the cell</param>
/// <returns>Set of changed cells</returns>
std::list<std::string> Spreadsheet::SetContentsOfCell(std::string name, std::string content)
{
  //Contents is a string, call overloaded method
  return SetCellContents(name, content);
}
      



