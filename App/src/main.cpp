#include <iostream>
#include <BFF/Secretary.hpp>
#include <BFF/Checkers/MD5Checker.hpp>

int main(){
  BFF::Secretary secretary;
  secretary.addChecker(BFF::MD5Checker::create("./base.csv"));

  auto result = secretary.checkFolder("../..");

  int files_checked = 0;
  int files_vulnerable = 0;
  int files_error_check = 0;

  for(auto &file_result : result){
    if(file_result.isChecked){
      if(!file_result.isOk){
        files_vulnerable++;
      }
      files_checked++;
    }
    else
    files_error_check++;
  }

  std::cout << "Files analyzed: " << files_checked << std::endl;
  std::cout << "Possible vulnerabilities: " << files_vulnerable << std::endl;
  std::cout << "Files with analyze errors: " << files_error_check << std::endl;

  return 0;
}
