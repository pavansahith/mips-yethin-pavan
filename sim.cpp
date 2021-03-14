#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct labels{
    string label;
    int32_t line_number_of_label;
};

struct memory_elements{
    string name;
    int start_address;
};