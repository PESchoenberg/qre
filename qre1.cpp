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


/* seek_in_file - Seeks the name of a variable in a flat file and returns its 
associated value.

Arguments:
- p_f: file name.
- p_v: variable.

Output:
- The value associated to p_v, as a string.

 */
string seek_in_file(string p_f, string p_v)
{
  string res = " ";
  string file_line = "";
  string file_name = p_f;
  
  std::ifstream file;
  file.open(file_name.c_str());
  while ( getline (file,file_line))
    {
      if (file_line.find(p_v) != std::string::npos)
	{
	  res = file_line.substr( file_line.find("=") + 1 );
	}
    }
  file.close();
  
  return res;
}


/* seek_in_json - Seeks the value of p_v in json string p_j.

Arguments:
- p_j: jason string passed as a std:string.
- p_v: variable name.

Output:
- The value associated to p_v, as a string; returns "na" if not found.

 */
string seek_in_json(string p_j, string p_v)
{
  string res = "na";
  string res1 = "";
  string j = p_j;
  string v = p_v + ":\"";
  string e = "}";
  size_t pos1;
  size_t pos2;
  
  // Find if p_v is in the string.
  if (j.find(p_v) != std::string::npos)
    {
      res1 = j.substr( j.find(v));

      // Now find the limit of the substring that corresponds to the value of p_v.
      if (res1.find(e) != std::string::npos)
	{
	  pos2 = res1.find(e);
	  res1 = res1.substr(0,pos2-1);
	  pos1 = res1.find(":");
	  res1 = res1.substr(pos1+2);
	  res = res1;
	}
    }

  return res;
}


/* show_var - shows the name and value if a variable contained in a .qreg file.

Aruments:
- p_t: text to show (name of the variable).
- p_v: variable value.

 */
void show_var(string p_t, string p_v)
{
  string j = "\n";
  string t1 = p_t;
  string t2 = " = ";
  string t3 = j + t1 + t2 + p_v + j;
  cout << t3;
}


/* show_string - Shows a string on stdout.

Aruments:
- p_t: string to show.

 */
void show_string(string p_t)
{
  std::cout << p_t << "\n" << std::endl;
}


/* write_callback - Required to read data received.

Sources:
- Based on an example at https://stackoverflow.com/questions/51317221/how-to-use-libcurl-in-c-to-send-a-post-request-and-receive-it

 */
static size_t write_callback(void *p_contents, size_t p_size, size_t p_nmemb, void *p_userp)
{
    ((std::string*)p_userp)->append((char*)p_contents, p_size * p_nmemb);
    
    return p_size * p_nmemb;
}


/* qpost- performs a post on q-series quantum computers. This function is 
based on an example shown on the link listed below/

Arguments:
- p_base_date: base_data.
- p_base_token: base token.
- p_post_content_type: post-content-type.
- p_base_results_storage: base-results-storage.
- p_post_uri: post-uri


Note:
- Arguments correspond to the standar parameters contained in a .qreg file.

Sources:
- https://curl.haxx.se/libcurl/c/simplepost.html
- https://developer.ibm.com/tutorials/os-quantum-computing-shell-game/

Output:
- Updated value for request log.

 */
string qpost(string p_base_data,
	     string p_base_token,
	     string p_content_type,
	     string p_base_results_storage,
	     string p_uri)
{
  CURL *curl;
  CURLcode res1 = CURLE_OK;
  string res = " ";
  string read_buffer;
  static const char *data = p_base_data.c_str();
  static const char *token = p_base_token.c_str();
  static const char *content_type = p_content_type.c_str();
  static const char *uri = p_uri.c_str();
  
  curl = curl_easy_init();
  if(curl)
    {
      curl_easy_setopt(curl, CURLOPT_URL, uri);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
      curl_easy_setopt(curl, CURLOPT_HEADER, content_type); // Mine: add header content-type
      
      /* if we don't provide POSTFIELDSIZE, libcurl will strlen() by itself */
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));

      /* Required to receive string data. */
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
      
      /* Perform the request, res will get the return code */ 
      res1 = curl_easy_perform(curl);
     
      /* Check for errors */ 
      if(res1 != CURLE_OK)
	{
	  fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res1));
	}
      else
	{
	  res = read_buffer;
	}
 
      /* Clean up */ 
      curl_easy_cleanup(curl);
    }
  return res;
}

















