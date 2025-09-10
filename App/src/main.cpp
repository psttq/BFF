#include <iostream>
#include <BFF/Secretary.hpp>
#include <BFF/Checkers/MD5Checker.hpp>

int main(){
  BFF::Secretary secretary;
  secretary.addChecker(BFF::MD5Checker::create("./base.csv"));

  secretary.checkFolder(".");
  return 0;
}
