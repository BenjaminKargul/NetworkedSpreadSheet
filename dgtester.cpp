#include "dg.cpp"
#include <iostream>


int main()
{
  DependencyGraph *dg = new DependencyGraph();

  std::vector<std::string> vec;
  vec.push_back("X");
  vec.push_back("Y");
  vec.push_back("Z");

  // Test add
  dg->AddDependency("A", "B");
  dg->AddDependency("A", "C");
  dg->AddDependency("A", "D");
  //dg->RemoveDependency("A", "B");
  //dg->RemoveDependency("B", "C");
  dg->ReplaceDependents("A", vec);
			
  std::cout << dg->HasDependents("A") <<std::endl;
  std::cout << dg->HasDependees("A") <<std::endl;

  std::cout << dg->GetDependents("A")[0] <<std::endl;
  std::cout << dg->GetDependents("A")[1] <<std::endl;
  std::cout << dg->GetDependents("A")[2] <<std::endl;

}
