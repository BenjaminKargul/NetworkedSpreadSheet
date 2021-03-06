#ifndef DG_H
#define DG_H

#include <string>
#include <cstdlib>
#include <vector>
#include <map>



class DependencyGraph
{
 private:
  
   class DependencyNode
   {
     typedef std::map<std::string, DependencyNode* > map_t;

   public:
     
     std::string nodeName;
     map_t dependents;
     map_t dependees;

     DependencyNode();
     ~DependencyNode();
     DependencyNode(std::string name);

     DependencyNode( const DependencyNode& other );

     DependencyNode& operator=(const DependencyNode &rhs);

     bool operator==(const DependencyNode &other) const;

     std::vector<std::string> listDependents();

     std::vector<std::string> listDependees();

     void addDependent(DependencyNode* dependent, int &size);

     void removeDependent(DependencyNode* dependent, int &size);

     void removeDependee(DependencyNode* dependent, int &size);

     void removeAllDependents(int &size);

     void removeAllDependees(int &size);
   };

   std::map<std::string, DependencyNode*> graph;

   int graph_size;

   DependencyNode* retrieveNode(std::string variable);

   void AddDependency(DependencyNode* s, std::string t);

   void AddDependency(std::string s, DependencyNode* t);

   

 public:

   DependencyGraph();
   ~DependencyGraph();

   int get_size();

   bool HasDependents(std::string s);

   bool HasDependees(std::string s);

   std::vector<std::string> GetDependents(std::string s);

   std::vector<std::string> GetDependees(std::string s);

   void AddDependency(std::string s, std::string t);

   void RemoveDependency(std::string s, std::string t);

   void ReplaceDependents(std::string s, std::vector<std::string> newDependents);

   void ReplaceDependees(std::string s, std::vector<std::string> newDependees);
  
};

#endif
