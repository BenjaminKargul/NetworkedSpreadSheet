#include "ss.h"
#include "dg.h"
#include <iostream>

int main()
{
  Spreadsheet* ss = new Spreadsheet();
  DependencyGraph* dg = new DependencyGraph();
  /*
  std::cout<<"Adding (s,t) and (s,r)"<< std::endl;
  dg->AddDependency("s","t");
  dg->AddDependency("s", "r");
  dg->GetDependents("s");
  dg->GetDependees("r");
  dg->GetDependees("t");
  
  std::cout<<"Removing (s,r)"<< std::endl;
  dg->RemoveDependency("s", "r");
  dg->GetDependents("s");
  dg->GetDependees("r");
  dg->GetDependees("t");

  std::cout<<"Adding (t,s) for a circular dependency"<< std::endl;
  dg->AddDependency("t","s");
  std::cout<<"S dependents:"<< std::endl;
  dg->GetDependents("s");
  std::cout<<"S dependees:"<< std::endl;
  dg->GetDependees("s");
  std::cout<<"T dependents:"<< std::endl;
  dg->GetDependents("t");
  std::cout<<"T dependees:"<< std::endl;
  dg->GetDependees("t");

  std::cout<<std::endl;
  
  std::cout<<"Removing (s,t) to fix circular dependency"<< std::endl;
  dg->RemoveDependency("s", "t");
  std::cout<<"S dependents:"<< std::endl;
  dg->GetDependents("s");
  std::cout<<"S dependees:"<< std::endl;
  dg->GetDependees("s");
  std::cout<<"T dependents:"<< std::endl;
  dg->GetDependents("t");
  std::cout<<"T dependees:"<< std::endl;
  dg->GetDependees("t");


  std::cout<<std::endl;

  dg->AddDependency("t","s");
  std::cout<<"Only dependency is (t,s), s = t + ..." << std::endl;
  
  std::cout<<"Replace S dependees with (k, q)"<< std::endl;
  std::cout<<"Now s = k + q + ..., t is unused"<< std::endl;

  std::vector<std::string> dependees;
  dependees.push_back("k");
  dependees.push_back("q");

  dg->ReplaceDependees("s", dependees);
  std::cout<<"S dependents:"<< std::endl;
  dg->GetDependents("s");
  std::cout<<"S dependees:"<< std::endl;
  dg->GetDependees("s");
  std::cout<<"T dependents:"<< std::endl;
  dg->GetDependents("t");
  std::cout<<"T dependees:"<< std::endl;
  dg->GetDependees("t");
  std::cout<<"k dependents:"<< std::endl;
  dg->GetDependents("k");
  std::cout<<"k dependees:"<< std::endl;
  dg->GetDependees("k");
  std::cout<<"q dependents:"<< std::endl;
  dg->GetDependents("q");
  std::cout<<"q dependees:"<< std::endl;
  dg->GetDependees("q");

  std::cout<<std::endl;
  std::cout<<"Dependencies are (k,s), (q,s), s = k + q +  ..." << std::endl;
  
  std::cout<<"Replace k dependents with (m, n)"<< std::endl;
  std::cout<<"Now s = q + ..., m = k + ..., n = k + ...."<< std::endl;

  std::vector<std::string> dependents;
  dependents.push_back("m");
  dependents.push_back("n");
  
  dg->ReplaceDependents("k", dependents);
  std::cout<<"S dependents:"<< std::endl;
  dg->GetDependents("s");
  std::cout<<"S dependees:"<< std::endl;
  dg->GetDependees("s");
  std::cout<<"k dependents:"<< std::endl;
  dg->GetDependents("k");
  std::cout<<"k dependees:"<< std::endl;
  dg->GetDependees("k");
  std::cout<<"m dependents:"<< std::endl;
  dg->GetDependents("m");
  std::cout<<"m dependees:"<< std::endl;
  dg->GetDependees("m");
  std::cout<<"n dependents:"<< std::endl;
  dg->GetDependents("n");
  std::cout<<"n dependees:"<< std::endl;
  dg->GetDependees("n");
  std::cout<<"q dependents:"<< std::endl;
  dg->GetDependents("q");
  std::cout<<"q dependees:"<< std::endl;
  dg->GetDependees("q");
  */
  std::cout<<"add a1=a3+a2"<<std::endl;
  ss->SetCellContents("a1" , "=a3+a2");
  std::cout<<"add a2=4"<<std::endl;
  ss->SetCellContents("a2" , "4");
  std::cout<<"add a3=8"<<std::endl;
  ss->SetCellContents("a3" , "8");
  //ss->SetCellContents("a3" , "=a1");
  //std::cout<<
  //std::cout << ss->GetCellContents("A3") << std::endl;

  // std::cout << ss->get_variables("=A1 + B2 ")[0] << std::endl;
}
