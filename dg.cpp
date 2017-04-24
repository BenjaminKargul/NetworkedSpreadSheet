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
  this->nodeName = "";
}

DependencyGraph::DependencyNode::DependencyNode(std::string name)
{
  this->nodeName = name;
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
  std::vector<std::string> stringList; //TODO: How does all of this get affected by null strings?
 
 BOOST_FOREACH(DependencyNode node, this->dependents)
  {
  stringList.push_back(node.nodeName);
 }
 //std::cout << stringList[0] << std::endl; 
 return stringList;
}

/// <summary>
/// Returns a list of the node's dependees.
/// </summary>
/// <returns></returns>
std::vector<std::string> DependencyGraph::DependencyNode::listDependees()
{
  std::vector<std::string> stringList;
 
 BOOST_FOREACH (DependencyNode node,  this->dependees)
   {
    stringList.push_back(node.nodeName);
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
void DependencyGraph::DependencyNode::addDependent(DependencyNode dependent, int &size)
{
  //If the dependency doesn't exist, add it, and adjust the DependencyGraph's "size" accordingly.
  //!(std::find(this->dependents.begin(), this->dependents.end(), dependent) != this->dependents.end())
  bool item_present = boost::algorithm::any_of_equal(this->dependents, dependent);
  if (!item_present)
  {
    this->dependents.push_back(dependent);
    dependent.dependees.push_back(*this);
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
void DependencyGraph::DependencyNode::removeDependent(DependencyNode dependent, int &size)
{
  //If the dependency exists, remove it, and adjust the DependencyGraph's "size" accordingly.
  if (std::find(this->dependents.begin(), this->dependents.end(), dependent) != this->dependents.end())
    {
      this->dependents.erase(std::remove(this->dependents.begin(), this->dependents.end(), dependent), this->dependents.end());
      dependent.dependees.erase(std::remove(dependent.dependees.begin(), dependent.dependees.end(), *this), dependent.dependees.end());
      size--;
    }
}

/// <summary>
/// Removes the dependencies between this node and all the dependents.
/// </summary>
/// <param name="_size">Reference to the DependencyGraph's size count</param>
void DependencyGraph::DependencyNode::removeAllDependents(int &size)
{
//Can't use foreach loop, so iterate through the list of dependents with a while loop instead.
  while(dependents.size() > 0)
  {
    int endDependentIndex = dependents.size() - 1;
    removeDependent(dependents[endDependentIndex], size);
  }
}

/// <summary>
/// Removes the dependencies between this node and all the dependees.
/// </summary>
/// <param name="_size"></param>
void DependencyGraph::DependencyNode::removeAllDependees(int &size)
{
//Can't use foreach loop, so iterate through the list of dependees with a while loop instead.
  while (dependees.size() > 0)
  {
    int endDependeeIndex = dependees.size() - 1;
    //Remove each of the dependees' reference to this node. Calling the removeDependent method through the dependees ensures a 
    //bidirectional deletion of the references.
    dependees[endDependeeIndex].removeDependent(*this, size); 
  }
}
  

/// <summary>
/// Retrieves a node that is associated with the string. If it doesn't exist on the graph, a new node is created and placed on the graph.
/// </summary>
/// <param name="variable"></param>
/// <returns></returns>
DependencyGraph::DependencyNode DependencyGraph::retrieveNode(std::string variable)
{

  std::map<std::string, DependencyNode>::iterator it;

  it = graph.find(variable);
  if(it == graph.end())
    {
      DependencyNode new_node(variable);
      graph[variable] = new_node;
      return new_node;
    }

  DependencyNode node("");
  return node;
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
void DependencyGraph::AddDependency(DependencyNode s, std::string t)
{
  //TODO: Label this overload as being slightly better, since you don't need to lookup the node for the recurring dependee node in replaceDependents
  DependencyNode dependent = retrieveNode(t);

  s.addDependent(dependent, graph_size);
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
void DependencyGraph::AddDependency(std::string s, DependencyNode t)
{
  //TODO: Label this overload as being slightly better, since you don't need to lookup the node for the recurring dependent node in replaceDependees
  DependencyNode dependee = retrieveNode(s);

  dependee.addDependent(t, graph_size);
}


/// <summary>
/// Creates an empty DependencyGraph.
/// </summary>
DependencyGraph::DependencyGraph()
{
  this->graph_size = 0;
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

  std::map<std::string, DependencyNode>::iterator it;

  it = graph.find(s);
  if(it != graph.end())
    {
      if(graph[s].dependents.size() > 0)
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

  std::map<std::string, DependencyNode>::iterator it;

  it = graph.find(s);
  if(it != graph.end())
    {
      if(graph[s].dependees.size() > 0)
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

  std::map<std::string, DependencyNode>::iterator it;

  it = graph.find(s);
  if(it != graph.end())
    {
      //std::cout << graph[s].listDependents()[0] << std::endl;
      return graph[s].listDependents();
    }
  
  std::vector<std::string> to_return;
  return to_return;
}

/// <summary>
/// Enumerates dependees(s).
/// </summary>
std::vector<std::string> DependencyGraph::GetDependees(std::string s)
{

  std::map<std::string, DependencyNode>::iterator it;

  it = graph.find(s);
  if(it != graph.end())
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
  DependencyNode dependee = retrieveNode(s);
  DependencyNode dependent = retrieveNode(t);

  dependee.addDependent(dependent, graph_size);
}



/// <summary>
/// Removes the ordered pair (s,t), if it exists
/// </summary>
/// <param name="s"></param>
/// <param name="t"></param>
void DependencyGraph::RemoveDependency(std::string s, std::string t)
{
  DependencyNode dependee = retrieveNode(s);
  DependencyNode dependent = retrieveNode(t);

  dependee.removeDependent(dependent, graph_size);
}


/// <summary>
/// Removes all existing ordered pairs of the form (s,r).  Then, for each
/// t in newDependents, adds the ordered pair (s,t).
/// </summary>
void DependencyGraph::ReplaceDependents(std::string s, std::vector<std::string> newDependents)
{
  DependencyNode dependee = retrieveNode(s);

  dependee.removeAllDependents(graph_size);

  BOOST_FOREACH(std::string dependent, newDependents)
    {
      //Calls the overloaded method of AddDependency that takes a reference to the dependee node.
      //This way, the same node (s) doesn't have to constantly be retrieved.
      AddDependency(dependee, dependent);
    }
}


/// <summary>
/// Removes all existing ordered pairs of the form (r,s).  Then, for each 
/// t in newDependees, adds the ordered pair (t,s).
/// </summary>
void DependencyGraph::ReplaceDependees(std::string s, std::vector<std::string> newDependees)
{
  
  DependencyNode dependent = retrieveNode(s);

  dependent.removeAllDependees(graph_size);

  BOOST_FOREACH(std::string dependee, newDependees)
    {
      std::cout << "in rd" << std::endl;
     //Calls the overloaded method of AddDependency that takes a reference to the dependent node.
      //This way, the same node (s) doesn't have to constantly be retrieved.
      AddDependency(dependee, dependent);
    }
}


