#include "dg.h"
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <boost/foreach.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>
#include <iostream>

/// <summary>
/// (s1,t1) is an ordered pair of strings
/// t1 depends on s1; s1 must be evaluated before t1
/// 
/// A DependencyGraph can be modeled as a set of ordered pairs of strings.  Two ordered pairs
/// (s1,t1) and (s2,t2) are considered equal if and only if s1 equals s2 and t1 equals t2.
/// Recall that sets never contain duplicates.  If an attempt is made to add an element to a 
/// set, and the element is already in the set, the set remains unchanged.
/// 
/// Given a DependencyGraph DG:
/// 
///    (1) If s is a string, the set of all strings t such that (s,t) is in DG is called dependents(s).
///        (The set of things that depend on s)    
///        
///    (2) If s is a string, the set of all strings t such that (t,s) is in DG is called dependees(s).
///        (The set of things that s depends on) 
//
// For example, suppose DG = {("a", "b"), ("a", "c"), ("b", "d"), ("d", "d")}
//     dependents("a") = {"b", "c"}
//     dependents("b") = {"d"}
//     dependents("c") = {}
//     dependents("d") = {"d"}
//     dependees("a") = {}
//     dependees("b") = {"a"}
//     dependees("c") = {"a"}
//     dependees("d") = {"b", "d"}
/// </summary>

/// <summary>
/// This class represents a node on the DependencyGraph.
/// </summary>
            
/// <summary>
/// Constructs a new node associated with the input string.
/// </summary>
/// <param name="nodeName">Unique string to be associated with the new node.</param>
DependencyGraph::DependencyNode::DependencyNode()
{
}

/// <summary>
/// Constructs a new node associated with the input string.
/// </summary>
/// <param name="nodeName">Unique string to be associated with the new node.</param>
DependencyGraph::DependencyNode::~DependencyNode()
{
  delete this;
}


DependencyGraph::DependencyNode::DependencyNode(std::string name)
{
  this->nodeName = name;
}

DependencyGraph::DependencyNode::DependencyNode( const DependencyNode& other ) :
     nodeName( other.nodeName ), dependents( other.dependents ), dependees( other.dependees )
  {}


DependencyGraph::DependencyNode& DependencyGraph::DependencyNode::operator=(const DependencyNode &rhs)
{
  if (this == &rhs)
    {
      return *this; 
    }  

  this->nodeName = rhs.nodeName;
  this->dependees = rhs.dependees;
  this->dependents = rhs.dependents;
}


bool DependencyGraph::DependencyNode::operator==(const DependencyNode &other) const
{
  if(this->nodeName == other.nodeName)
    {
      return true;
    }
  
  return false;
}


/// <summary>
/// Returns a list of the node's dependents.
/// </summary>
/// <returns></returns>
std::vector<std::string> DependencyGraph::DependencyNode::listDependents()
{ 
  std::vector<std::string> stringList;
 
  BOOST_FOREACH(map_t::value_type &pair,  this->dependents)
   {
     stringList.push_back(pair.first);
<<<<<<< HEAD
     std::cout<<pair.first<< " from dependents"<<std::endl;
=======
>>>>>>> 520cb30b2c6b005bbbdd3f39b868bb5a19da5ce1
   }

 return stringList;
}

/// <summary>
/// Returns a list of the node's dependees.
/// </summary>
/// <returns></returns>
std::vector<std::string> DependencyGraph::DependencyNode::listDependees()
{
  std::vector<std::string> stringList;
   
  BOOST_FOREACH(map_t::value_type &pair,  this->dependees)
    {
      stringList.push_back(pair.first);
      std::cout<<pair.first<< " from dependees"<<std::endl;
    }

  return stringList;
}


/// <summary>
/// Add a dependency where the input node is the dependent. The dependency is created both ways.
/// (This node gets a reference to the dependent, and the dependent gets a reference to this node).
/// 
/// Adjusts the size of the DependencyGraph accordingly.
/// </summary>
/// <param name="dependent">The node that becomes the dependent of the current node.</param>
/// <param name="dependencySize">Reference to the DependencyGraph's size count.</param>
void DependencyGraph::DependencyNode::addDependent(DependencyNode* dependent, int &size)
{
  //If the dependency doesn't exist, add it, and adjust the DependencyGraph's "size" accordingly.
  if(dependents.count(dependent->nodeName) == 0)
    {
      this->dependents[dependent->nodeName] = dependent;
      dependent->dependees[this->nodeName] = this;
      size++;
    }
}

/// <summary>
/// Remove a dependency where the input node is the dependent. The dependency is removed both ways.
/// (This node no longer has a reference to the dependent, and the dependent has no reference to this node).
/// 
/// Adjusts the size of the DependencyGraph accordingly.
/// </summary>
/// <param name="dependent">The node that is the dependent of the current node</param>
/// <param name="dependencySize">Reference to the DependencyGraph's size count</param>
void DependencyGraph::DependencyNode::removeDependent(DependencyNode* dependent, int &size)
{
  std::cout << "**************Removing dependent*******************" << std::endl;
  //If the dependency exists, remove it, and adjust the DependencyGraph's "size" accordingly.
  //find the dependent, S
  std::cout <<"finding dependent " << dependent->nodeName << " on node " << nodeName << std::endl;
  if(dependents.find(dependent->nodeName) != dependents.end())
    {
      std::cout << "found " << dependent->nodeName << ", removing " << dependent->nodeName << "as a dependent on " << nodeName << std::endl;
       //then remove S, its dependent
      dependents.erase(dependent->nodeName);
      
      std::cout << "Removing " << nodeName << " as a dependee on " << dependent->nodeName << std::endl;

      //have s remove it as a dependee
      dependent->dependees.erase(nodeName);
      size--;
    }
}


/// <summary>
/// Remove a dependency where the input node is the dependent. The dependency is removed both ways.
/// (This node no longer has a reference to the dependent, and the dependent has no reference to this node).
/// 
/// Adjusts the size of the DependencyGraph accordingly.
/// </summary>
/// <param name="dependent">The node that is the dependent of the current node</param>
/// <param name="dependencySize">Reference to the DependencyGraph's size count</param>
void DependencyGraph::DependencyNode::removeDependee(DependencyNode* dependee, int &size)
{
  std::cout << "**************Removing dependee*******************" << std::endl;
  //check that the dependee can be found on the node's list of dependees
  //have the dependee remove the caller/dependent from it's list of dependents
  //have the caller remove the dependee from it's list


  //If the dependency exists, remove it, and adjust the DependencyGraph's "size" accordingly.
  //find the dependent, S
  std::cout <<"finding dependee " << dependee->nodeName << " on node " << nodeName << std::endl;
  if(dependees.find(dependee->nodeName) != dependees.end())
    {
      std::cout << "found " << dependee->nodeName << ", removing " << nodeName << " as a dependent on " << dependee->nodeName << std::endl;

      std::cout << "There are " << dependee->dependents.size() << " in dependee's dependents list"<< std::endl;
      int erasedCount = dependee->dependents.erase(nodeName);

      std::cout << "Erased " << erasedCount << " elements from the dependee's dependents list" << std::endl;
      std::cout << "There are " << dependee->dependents.size() << " in dependee's dependents list"<< std::endl;

      std::cout << "Removing " << dependee->nodeName << " as a dependee on " << nodeName << std::endl;

      //have s remove it as a dependee
      dependees.erase(dependee->nodeName);
      size--;
    }
}

/// <summary>
/// Removes the dependencies between this node and all the dependents.
/// </summary>
/// <param name="_size">Reference to the DependencyGraph's size count</param>
void DependencyGraph::DependencyNode::removeAllDependents(int &size)
{
  std::vector<std::string> stringList = listDependents();

  BOOST_FOREACH(std::string s , stringList)
  {
    removeDependent(dependents[s], size);
  }
}

//*******************************************************************************************************Remove Dependees
/// <summary>
/// Removes the dependencies between this node and all the dependees.
/// </summary>
/// <param name="_size"></param>
void DependencyGraph::DependencyNode::removeAllDependees(int &size)
{
  //get the list of all dependees for this node
  std::vector<std::string> stringList = listDependees();

  BOOST_FOREACH(std::string s , stringList)
  {
    //reaches into the list of dependees, calls remove dependent on S
    //this doesn't work because s doesn't have this node listed as a dependent, it's a dependee
    //replace this with calling removing dependent on the dependee
    //removeDependent(dependees[s], size);

    //removeDependee(&dependees[s], size);

    std::cout <<"finding dependee " << dependees[s]->nodeName << " on node " << nodeName << std::endl;
    if(dependees.find(dependees[s]->nodeName) != dependees.end())
    {
      std::cout << "found " << dependees[s]->nodeName << ", removing " << nodeName << " as a dependent on " << dependees[s]->nodeName << std::endl;

      std::cout << "There are " << dependees[s]->dependents.size() << " in dependee's dependents list"<< std::endl;
      int erasedCount = dependees[s]->dependents.erase(nodeName);

      //std::cout << "Erased " << erasedCount << " elements from the dependee's dependents list" << std::endl;
      //std::cout << "There are " << dependee->dependents.size() << " in dependee's dependents list"<< std::endl;

      //std::cout << "Removing " << dependee->nodeName << " as a dependee on " << nodeName << std::endl;

      //have s remove it as a dependee
      dependees.erase(s);
      size--;
    }


    //std::cout << "Finished removing " << s << " as a dependee on " << nodeName << std::endl;
    //std::cout << "There are " << dependees[s].dependents.size() << "elements in " << s << "'s dependents list"<< std::endl;
    //dependees[s].removeDependent(*this, size);
  }
  std::cout << "Removed all dependees from " << nodeName << std::endl;
}
  

/// <summary>
/// Retrieves a node that is associated with the string. If it doesn't exist on the graph, a new node is created and placed on the graph.
/// </summary>
/// <param name="variable"></param>
/// <returns></returns>
//This code should be 100% correct
DependencyGraph::DependencyNode* DependencyGraph::retrieveNode(std::string variable)
{
  std::cout <<"Retrieving node " << variable << std::endl;
  if(graph.find(variable) == graph.end())
    {
      std::cout << "Making a new node" << std:: endl;
      DependencyNode* new_node = new DependencyNode(variable);
      std::cout << "Saving node to graph" <<std::endl;
      graph[variable] = new_node;
      return graph[variable];
    }

  return graph[variable];
}

/// <summary>
/// <para>Adds the ordered pair (s,t), if it doesn't exist</para>
/// 
/// <para>This should be thought of as:</para>   
/// 
///   t depends on s
///
/// </summary>
/// <param name="s">Dependee node</param>
/// <param name="t">String associated with dependent node</param>
void DependencyGraph::AddDependency(DependencyNode* s, std::string t)
{ 
  //TODO: Label this overload as being slightly better, since you don't need to lookup the node for the recurring dependee node in replaceDependents
  DependencyNode* dependent = retrieveNode(t);
  
  s->addDependent(dependent, graph_size);
}

/// <summary>
/// <para>Adds the ordered pair (s,t), if it doesn't exist</para>
/// 
/// <para>This should be thought of as:</para>   
/// 
///   t depends on s
///
/// </summary>
/// <param name="s">String associated with dependee node</param>
/// <param name="t">Dependent node</param>
void DependencyGraph::AddDependency(std::string s, DependencyNode* t)
{ 
  //TODO: Label this overload as being slightly better, since you don't need to lookup the node for the recurring dependent node in replaceDependees
  DependencyNode* dependee = retrieveNode(s);

  dependee->addDependent(t, graph_size);
}


/// <summary>
/// Creates an empty DependencyGraph.
/// </summary>
DependencyGraph::DependencyGraph()
{ 
  std::cout << "Making empty dependency graph" << std::endl;
  this->graph_size = 0;
}

DependencyGraph::~DependencyGraph()
{ 
  typedef std::map<std::string, DependencyNode*> map_t;
  BOOST_FOREACH(map_t::value_type &pair,  this->graph)
   {
     delete(pair.second);
   }
}


/// <summary>
/// The number of ordered pairs in the DependencyGraph.
/// </summary>
int DependencyGraph::get_size()
{
  return this->graph_size;
}


/// <summary>
/// The size of dependees(s).
/// This property is an example of an indexer.  If dg is a DependencyGraph, you would
/// invoke it like this:
/// dg["a"]
/// It should return the size of dependees("a")
/// </summary>

//WTF CHECK THIS////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int this[std::string s]
//{
//  get
//    {
//	DependencyNode node;
//	graph.TryGetValue(s, out node);
//	//If the node exists on the graph, return the dependees count
//	if (node != null)
//	  {
//	    return node.dependees.Count;
//	  }
//
//	return 0;
//    }
//}


/// <summary>
/// Reports whether dependents(s) is non-empty.
/// </summary>
bool DependencyGraph::HasDependents(std::string s)
{
  if(graph.find(s) != graph.end())
    {
      if(graph[s]->dependents.size() > 0)
	{
	  return true;
	}
      else
	{
	  return false;
	}
    }
  return false;
}


/// <summary>
/// Reports whether dependees(s) is non-empty.
/// </summary>
bool DependencyGraph::HasDependees(std::string s)
{ 
  if(graph.find(s) != graph.end())
    {
      if(graph[s]->dependees.size() > 0)
	{
	  return true;
	}
      else
	{
	  return false;
	}
    }
  return false;
}


/// <summary>
/// Enumerates dependents(s).
/// </summary>
std::vector<std::string> DependencyGraph::GetDependents(std::string s)
{
  if(graph.find(s) != graph.end())
    {
      return graph[s]->listDependents();
    }
  
  std::vector<std::string> to_return;
  return to_return;
}

/// <summary>
/// Enumerates dependees(s).
/// </summary>
std::vector<std::string> DependencyGraph::GetDependees(std::string s)
{

  if(graph.find(s) != graph.end())
    {
      return graph[s].listDependees();
    }
 
  std::vector<std::string> to_return;
  return to_return;
}


/// <summary>
/// <para>Adds the ordered pair (s,t), if it doesn't exist</para>
/// 
/// <para>This should be thought of as:</para>   
/// 
///   t depends on s
///
/// </summary>
/// <param name="s"> s must be evaluated first. T depends on S</param>
/// <param name="t"> t cannot be evaluated until s is</param>        /// 
void DependencyGraph::AddDependency(std::string s, std::string t)
{ 
  DependencyNode* dependee = retrieveNode(s);
  DependencyNode* dependent = retrieveNode(t);
  
  dependee->addDependent(dependent, graph_size);
}



/// <summary>
/// Removes the ordered pair (s,t), if it exists
/// </summary>
/// <param name="s"></param>
/// <param name="t"></param>
void DependencyGraph::RemoveDependency(std::string s, std::string t)
{ 
  DependencyNode* dependee = retrieveNode(s);
  DependencyNode* dependent = retrieveNode(t);

  dependee->removeDependent(dependent, graph_size);
}


/// <summary>
/// Removes all existing ordered pairs of the form (s,r).  Then, for each
/// t in newDependents, adds the ordered pair (s,t).
/// </summary>
void DependencyGraph::ReplaceDependents(std::string s, std::vector<std::string> newDependents)
{ 
  DependencyNode* dependee = retrieveNode(s);

  dependee->removeAllDependents(graph_size);

  BOOST_FOREACH(std::string dependent, newDependents)
    {
      
      //Calls the overloaded method of AddDependency that takes a reference to the dependee node.
      //This way, the same node (s) doesn't have to constantly be retrieved.

      AddDependency(s, dependent);
    }
}


/// <summary>
/// Removes all existing ordered pairs of the form (r,s).  Then, for each 
/// t in newDependees, adds the ordered pair (t,s).
/// </summary>
void DependencyGraph::ReplaceDependees(std::string s, std::vector<std::string> newDependees)
{
  
  DependencyNode* dependent = retrieveNode(s);

  dependent->removeAllDependees(graph_size);


  //std::cout << "Finished removing " << "t" << " as a dependee on " << dependent->nodeName << std::endl;
  ///std::cout << "There are " << dependent->dependees["t"].dependents.size() << "elements in " << "t" << "'s dependents list"<< std::endl;
  
    std::cout<< std::endl << "********************Adding new dependees******************************" << std::endl;
  BOOST_FOREACH(std::string dependee, newDependees)
    {
      std::cout << "Adding " << dependee << " as a dependee" << std::endl;
      std::cout << "new dependency ("<< dependee << ", " << s << ")" << std::endl;
      //Calls the overloaded method of AddDependency that takes a reference to the dependent node.
      //This way, the same node (s) doesn't have to constantly be retrieved.
     
      //This is adding a dependency of (s, s), when we should be adding (dependee, s)
      //AddDependency(s, *dependent);
      AddDependency(dependee,s);
    }

}



