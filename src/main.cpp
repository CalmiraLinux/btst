/*
 * Программа для получения данных об аккумуляторной
 * батарее ноутбука. Из возможностей:
 * 1. Получение статуса (заряжается/разряжается);
 * 2. Получения процента заряда.
 * (C) 2021 Michail Linuxoid85 Krasnov <linuxoid85@gmail.com>
 * For Calmira LX4 1.2 GNU/Linux
 */
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <dirent.h>
#include "str_switch.h"
using namespace std;

string PATH;
string VERSION;
int percentage;

void get_time() {
  time_t now = time(0);
  char* dt = ctime(&now);
  cout << "Launch time: " << dt << endl;
}

bool isdir(const char* path) {
  if(path == NULL) return false;
  
  DIR *pDir;
  bool bExists = false;
  
  pDir = opendir(path);
  
  if(pDir != NULL) {
    bExists = true;
    (void) closedir (pDir);
  }
  
  return bExists;
}

string get_path() {
  string path;

  if(isdir("/sys/class/power_supply/BAT0")) {
    path = "/sys/class/power_supply/BAT0/";
  } else {
    path = "/sys/class/power_supply/BAT1/";
  }

  return path;
}

void get_file(string file) {
  string line;
  ifstream f(get_path()+file);
  
  if(f.is_open()) {
    while(getline(f, line)) {
      cout << line << endl;
    }
  } else {
    cout << "Uknown information\n";
  }
}

void get_status() {
  string status;
  string line;
  
  status = get_path() + "status";
  
  ifstream sf(status);
  
  if(sf.is_open()) {
    while(getline(sf, line)) {
      if(line == "Charging") {
        cout << "\033[32m" << line << "\033[0m\n";
      } else {
        cout << "\033[31m" << line << "\033[0m\n";
      }
    }
  } else {
    cout << "Uknown status\n";
  }
}

void print() {
  cout << "Status: "; get_status();
  cout << "Max energy: " << percentage << endl;
}

int main(int argc, char *argv[]) {
  if(argv[1] == NULL) {
    cout << "Error: you must pass at least one argument to " << argv[0] << "!\n";
    return 1;
  }
  
  VERSION = "0.1 Stable";

  ifstream cp(get_path()+"capacity");
  cp >> percentage;

  SWITCH(argv[1]) {
    CASE("-a"):
      get_time();
      print();

      break;

    CASE("-p"):
      cout << percentage << endl;

      break;
    
    CASE("-i"):
      cout << "Model: "; get_file("model_name");
      cout << "Manufacter: "; get_file("manufacter");
      cout << "Type: "; get_file("technology");
      
      break;
    
    CASE("-v"):
      cout << "btst " << VERSION << endl;
      cout << "btst - A program for obtaining basic information about the laptop battery.\n";
      
      break;
    
    DEFAULT:
      cout << "Uknown argument '" << argv[1] << "'\n";
      return 1;
  }
  
  return 0;
}
