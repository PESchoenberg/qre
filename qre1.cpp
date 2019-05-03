/* =============================================================================

qre1.cpp

================================================================================

 Copyright (C) 2018 - 2019  Pablo Edronkin (pablo.edronkin at yahoo.com)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.

============================================================================= */

#include "qre1.hpp"

using namespace std;


/* qre_d2s - Converts a double to string.

Arguments:
- p_double: double to convert.

Output:
- String.

 */
std::string qre_d2s(double p_double)
{
  double dob = p_double;
  
  std::ostringstream stro;
  stro << dob;
  std::string res = stro.str();  

  return res;
}


/* qre-s2d - Converts a string to a double.

Arguments:
- p_string: string to convert.

Output:
- Double.

 */
double qre_s2d(std::string p_string)
{
  double res = 0.00;
  std::string str = p_string;
  
  stringstream strs(str); 
  strs >> res;
 
  return res;
}


/* qre_count_string

Arguments:
- p_delim: delimiter or substring to be accounted for.
- p_string: string in which to count the ocurrences of p_delim.

Output:
- Integer.

 */
int qre_count_string(std::string p_delim, std::string p_string)
{
  size_t pos;
  
  int res = 0;
  
  std::string str;
  
  while ((pos = str.find(p_delim)) != std::string::npos)
    {
      res++;
      str.erase(0, pos + p_delim.length());
    }

  return res;
}

/* qre_recog - recongizes if p_string1 is found in p_string2.

Arguments:
- p_string1: string to be found as a substring in p_string2.
- p_string2: string to search for p_string_1.

Output:
- Boolean true if found. False otherwise.

 */
bool qre_recog(std::string p_string1, std:: string p_string2)
{
  if (p_string2.find(p_string1) != std::string::npos)
    {
      return true;
    }
  else
    {
      return false;
    }
}


/* qre_show_v - Shows p_string if verbosity is on.

Arguments:
- p_base_verbosity: base_verbosity.
- p_string: string to show.

 */
void qre_show_v(std::string p_base_verbosity, std::string p_string)
{
  if(p_base_verbosity == "yes")
    {
      cout << p_string << endl;
    }
}


/* qre_seek_in_file - Seeks the name of a variable in a flat file and returns its 
associated value.

Arguments:
- p_f: file name.
- p_v: variable.

Output:
- The value associated to p_v, as a string.

 */
std::string qre_seek_in_file(std::string p_f, std::string p_v)
{
  std::string res = " ";
  std::string file_line = "";
  std::string file_name = p_f;
  
  std::ifstream file;
  file.open(file_name.c_str());
  while ( getline (file,file_line))
    {
      if (qre_recog(p_v, file_line) == true)
	{
	  res = file_line.substr( file_line.find("=") + 1 );
	}
    }
  file.close();
  
  return res;
}


/* qre_seek_in_json - seeks the value of p_v in json string p_j.

Arguments:
- p_j: jason string passed as a std:string.
- p_v: variable name.

Output:
- The value associated to p_v, as a string; returns "na" if not found.

 */
std::string qre_seek_in_json(std::string p_j, std::string p_v)
{
  std::string res = "na";
  std::string res1 = "";
  std::string j = p_j;
  std::string v = p_v + ":\"";
  std::string e = "}";
  
  size_t pos1;
  size_t pos2;
  
  // Find if p_v is in the string.
  if (qre_recog(p_v, j) == true)
    {
      res1 = j.substr( j.find(v));

      // Now find the limit of the substring that corresponds to the value of p_v.
      if (qre_recog(e, res1) == true)
	{
	  if (qre_recog("\",", res1) == true)
	    {	  
	      pos2 = res1.find("\",");
	    }
	  else
	    {
	      pos2 = res1.find("\"}");
	    }
	  res1 = res1.substr(0,pos2);
	  pos1 = res1.find(":");
	  res1 = res1.substr(pos1+2);
	  res = res1;	  
	}
    }

  return res;
}


/* qre_show_var - shows the name and value of a variable.

Aruments:
- p_t: text to show (name of the variable).
- p_v: variable value.

 */
void qre_show_var(std::string p_t, std::string p_v)
{
  std::string j = "\n";
  std::string t1 = p_t;
  std::string t2 = " = ";
  std::string t3 = j + t1 + t2 + p_v + j;
  
  cout << t3;
}


/* qre_show_string - Shows a string on stdout.

Aruments:
- p_t: string to show.

 */
void qre_show_string(std::string p_t)
{
  std::cout << p_t << "\n" << std::endl;
}


/* qre_show_conclusion - Shows two strings on different lines after a blank line.

Arguments:
- p_base_verbosity: base_verbosity.
- p_t1: first string.
- p_t2: second string.

 */
void qre_show_conclusion(std::string p_base_verbosity, std::string p_t1, std::string p_t2)
{
  if (p_base_verbosity == "yes")
    {  
      qre_show_string(" ");
      qre_show_string(p_t1);
      qre_show_string(p_t2);
    }
}


/* qre_url_encode - url encodes a string.

Arguments:
- p_s: string to url encode.

 */
std::string qre_url_encode(std::string p_s)
{
  CURL *curl;
  
  std::string res = p_s;
  
  curl = curl_easy_init();
  if(curl)
    {
      res = curl_easy_escape(curl , res.c_str(), 0);
    }  
  curl_easy_cleanup(curl);
  
  return res;
}


/* qre_create_header - creates a header as a c string.

Arguments:
- p_data: text that will be converted to a header.

Output:
- Header cast as a pure c string.

 */
char *qre_create_header(std::string p_s)
{
  std::string ps = p_s;
  
  const char *s = ps.c_str();
  
  char cs[ps.length()];

  return strcpy(cs, s);
}


/* qre_read_qasm_file - reads a qasm file.

Arguments:
- p_f: qasm file to read.

 */
std::string qre_read_qasm_file(std::string p_f)
{

  std::string res = "";
  std::string file_line = "";
  std::string file_name = p_f;
  
  std::ifstream file;
  file.open(file_name.c_str());
  while ( getline (file,file_line))
    {
      res = res + file_line;
    }
  file.close();
  
  return res;
}


/* qre_store_results - stores results from remote execution in the selected format and file.

Arguments:
- p_base_results_storage: base_results_storage.
- p_db: selected database.
- p_file: file contained in p_db that will hold the contents to be stored.
- p_contents_to_store.

 */
void qre_store_results(std::string p_base_results_storage,
		       std::string p_file,
		       std::string p_contents_to_store)
{
  std::string pathj = "data/json/";
  std::string paths = "data/sqlite3";
  std::string db = "qre.db";
  std::string file = p_file;

  if (p_base_results_storage == "json")
    {
      file = file + ".json";
      pathj = pathj + file;     
      ofstream json_file;
      json_file.open(pathj, std::ios::out);
      json_file << p_contents_to_store << "\n";
      json_file.close();    
    }
  else if (p_base_results_storage == "sqlite3")
    {;
      //Here we will put all routines to store stuff on a sqlite3 database.
    }
}


/* qre_parse_bitnum - Extracts the bit number from a QASM instruction expressed
as an integer, and the instruction to be executed.

Arguments:
- p_string: string.

Output:
- qubit or bit number.

 */
int qre_parse_bitnum(std::string p_string)
{
  int res = 0;
  
  std::string snum = "";

  snum = p_string.substr(2,(p_string.length()-1));
  stringstream conv(snum);
  conv >> res;
  
  return res;
}


/* qre_parse_reg - extracts quantum register substring.

Arguments:
- p_string: string from which to extract a register substring.
- p_type:
  - "q".
  - "c".

Output:
- A substring like "q[n]" or "c[n]" being n a qubit or bit number.

 */
std::string qre_parse_reg(std::string p_string, std::string p_type)
{
  std::string res = "//";
  std::string reg = p_type;
  
  reg = reg +"[";
  if (qre_recog(reg, p_string) == true)
    {
      size_t pos1 = p_string.find(reg, 0);
      size_t pos2 = p_string.find("]", pos1);
      res = p_string.substr(pos1,pos2);
    }
  
  return res;
}


/* qre_parse_data_string - parses base_data strings and puts fractions of the
original string parsed using ";" as delimiter on each vector cell.

Arguments:
- p_base_verbosity: base_verbosity.
- p_base_data: base_data.

Output:
- A string vector containing each QASM line on each vector cell.

Sources:
- https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c

 */
std::vector<std::string> qre_parse_data_string(std::string p_base_verbosity, std::string p_base_data)
{
  std::vector<std::string> res;
  
  std::string base_data = p_base_data;
  std::string delim = ";";
  std::string line;
  
  size_t pos = 0;
   
  qre_show_v(p_base_verbosity, ("Parsing base_data..."));  
  while ((pos = base_data.find(delim)) != std::string::npos)
  {
    line = base_data.substr(0, pos);
    res.push_back(line);
    base_data.erase(0, pos + delim.length());
  }

  return res;
}


/* qre_what_comes_after_s1 - returns the substring of p_s2 that comes after p_s1. 

Arguments:
- p_s1: string whose comlement is sought after in p_s2.
- p_s2: string wher to look for the complement of p_s1.

Output:
- Returns p_s2 - p_s1. If p_s1 is non-exsitent or is shorter than p_ss1, 
returns complete p_s2.

 */
std::string qre_what_comes_after_s1(std::string p_s1, std::string p_s2)
{
  std::string s1 = p_s1;
  std::string s2 = p_s2;
  std::string res = s2;
  
  int ls1 = s1.length();
  int ls2 = s2.length();
  
  if ((qre_recog(s1, s2) == true)&&(ls2 >= ls1))
    {
      res = s2.substr(ls1);
    }

  return res;
}
