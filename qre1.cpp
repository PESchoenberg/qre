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


/* seek_in_json - seeks the value of p_v in json string p_j.

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
	  /*pos2 = res1.find(e);
	  res1 = res1.substr(0,pos2-1);
	  pos1 = res1.find(":");
	  res1 = res1.substr(pos1+2);
	  res = res1;*/
	  if (res1.find("\",") != std::string::npos)
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


/* show_var - shows the name and value of a variable.

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


/* url_encode - url encodes a string.

Arguments:
- p_s: string to url encode.

 */
string url_encode(string p_s)
{
  CURL *curl;
  string res = p_s;
  
  curl = curl_easy_init();
  if(curl)
    {
      res = curl_easy_escape(curl , res.c_str(), 0);
    }
  
  return res;
}


/* create_heeader - creates a header as a c string.

Arguments:
- p_data: text that will be converted to a header.

Output:
- Header cast as a pure c string.

 */
char *create_header(string p_s)
{
  string ps = p_s;
  const char *s = ps.c_str();
  char cs[ps.length()];

  return strcpy(cs, s);
}


/* qpost- performs a post on q-series quantum computers. This function is 
adapted from an example shown on the links listed as sources.

Arguments:
- p_base_verbosity: base verbosity as set on qre0.cpp.
- p_base_method: request method (post, get or delete).
- p_base_date: base_data.
- p_base_token: base token.
- p_post_content_type: post-content-type.
- p_base_results_storage: base-results-storage.
- p_post_uri: post-uri.
- p_login_id: login_id.


Note:
- Arguments correspond to the standar parameters contained in a .qreg file.

Sources:
- https://curl.haxx.se/libcurl/c/simplepost.html
- https://developer.ibm.com/tutorials/os-quantum-computing-shell-game/
- https://stackoverflow.com/questions/22457601/http-post-header-fields-with-libcurl
- https://stackoverflow.com/questions/328281/why-content-length-0-in-post-requests

Output:
- Updated value for request log.

 */
string qpost(string p_base_verbosity,
	     string p_base_method,
	     string p_base_data,
	     string p_content_type,
	     string p_base_results_storage,
	     string p_uri,
	     string p_login_id)
{
  CURL *curl;
  CURLcode res1 = CURLE_OK;
  string res = " ";
  string read_buffer;
  string file_cookies = "data/cookies/qre_cookies.txt";
  string pdata = p_base_data;
  string pcontenttype = p_content_type;
  string puri = p_uri;
  string ploginid = p_login_id;
  string pcontenttype2 = "X-Access-Token: "+p_login_id+";charset=utf-8";
  string pcontentlength = "Content-Length: "+to_string(pdata.length());
  string pclientapp = "x-qx-client-application: qiskit-api-py";

  const char *cfile_cookies = file_cookies.c_str();
  const char *data = pdata.c_str();
  const char *contenttype = pcontenttype.c_str();
  const char *contenttype2 = pcontenttype2.c_str();
  const char *uri = puri.c_str();
  const char *contentlength = pcontentlength.c_str();
  const char *loginid = ploginid.c_str();
  const char *clientapp = pclientapp.c_str();

  char ccfile_cookies[file_cookies.length()];
  char cdata[pdata.length()];
  char ccontenttype[pcontenttype.length()];
  char ccontenttype2[pcontenttype2.length()];
  char curi[puri.length()];
  char ccontentlength[pcontentlength.length()];
  char ccloginid[ploginid.length()];
  char cclientapp[pclientapp.length()];

  strcpy(ccfile_cookies, cfile_cookies);
  strcpy(cdata, data);
  strcpy(ccontenttype, contenttype);
  strcpy(ccontenttype2, contenttype2);
  strcpy(curi, uri);
  strcpy(ccontentlength, contentlength);
  strcpy(ccloginid, loginid);
  strcpy(cclientapp, clientapp);

  //char *create_header(string p_s);
  // x-qx-client-application: qiskit-api-py
  
  //Defining headers.
  struct curl_slist *headerlist=NULL;
  headerlist = curl_slist_append(headerlist, ccontenttype);
  headerlist = curl_slist_append(headerlist, "Transfer-Encoding: chunked");
  
  //Final check of data.
  if (p_base_verbosity == "yes")
    {
      show_string("Data as is just before the making request:");
      show_var("Method: ", p_base_method);
      show_var("Data: ", cdata);
      show_var("Uri: ", curi);
      show_var("Content length: ", ccontentlength);
      show_var("Headers (init): ", ccontenttype);
      show_var("Login id: ", ccloginid);
      show_var("Client application: ", cclientapp);
    }

  res1 = curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if(curl)
    { 
      curl_easy_setopt(curl, CURLOPT_URL, curi);
      curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ccfile_cookies);
      curl_easy_setopt(curl, CURLOPT_COOKIEJAR, ccfile_cookies);
      curl_easy_setopt(curl, CURLOPT_HEADER, true);
      curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY); //
      curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, ccloginid); //
      //curl_easy_setopt(curl, CURLOPT_COOKIE, "name1=content1; name2=content2;");
      
      //Depending of request.
      if (p_base_method == "post")
	{
	  if (p_login_id != "na")
	    {
	      headerlist = curl_slist_append(headerlist, ccontenttype2);
	      headerlist = curl_slist_append(headerlist, ccontentlength);
	      headerlist = curl_slist_append(headerlist, cclientapp);
	       
	    }
	  curl_easy_setopt(curl,CURLOPT_POST, 1L);
	  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cdata);
	  
	  /* if we don't provide POSTFIELDSIZE, libcurl will strlen() by itself */
	  //curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (long)strlen(data));
	  
	}
      else if (p_base_method == "get")
	{
	  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
	}
      else if (p_base_method == "delete")
	{
	  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	}
            
      // Send the list with all headers.
      curl_easy_setopt(curl, CURLOPT_HEADER, headerlist);
      
      /* Required to receive string data. */
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);

      /* If verbosity value is yes, some more of info concerning the connection will be shown.*/
      if (p_base_verbosity == "yes")
	{
	  curl_easy_setopt(curl, CURLOPT_VERBOSE, true);
	}
      else
	{
	  curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
	}
      
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
      if (p_base_verbosity == "yes")
	{
	  cout << "\nCurl status message: " << res1 << endl;
	}      
    }

  curl_slist_free_all(headerlist);
  curl_easy_cleanup(curl);
  curl_global_cleanup();
  
  return res;
}


/* read_qasm_file - reads a qasm file.

Arguments:
- p_f: qasm file to read.

 */
string read_qasm_file(string p_f)
{

  string res = "";
  string file_line = "";
  string file_name = p_f;
  
  std::ifstream file;
  file.open(file_name.c_str());
  while ( getline (file,file_line))
    {
      res = res + file_line;
    }
  file.close();
  
  return res;
}


/* store_results - stores results from remote execution in the selected format and file.

Arguments:
- p_base_results_storage: base_results_storage.
- p_db: selected database.
- p_file: file contained in p_db that will hold the contents to be stored.
- p_contents_to_store.

 */
void store_results(string p_base_results_storage, string p_file, string p_contents_to_store)
{
  string pathj = "data/json/";
  string paths = "data/sqlite3";
  string db = "qre.db";
  string file = p_file;

  if (p_base_results_storage == "json")
    {
      file = file + ".json";
      pathj = pathj + file;     
      ofstream json_file;
      json_file.open(pathj, std::ios::app);
      json_file << p_contents_to_store << "\n";
      json_file.close();    
    }
  else if (p_base_results_storage == "sqlite3")
    {;
      //Here we will put all routines to store stuff on a sqlite3 database.
    }
}

/* qx_login - attempts to log in into the IBM QX server.

Arguments
- p_base_verbosity.
- p_base_method.
- p_login_data.
- p_post_content_type.
- p_base_results_storage.
- p_login_uri.
- p_login_name: login_name.

Output:
- login_id as res.

 */
std::vector<std::string> qx_login(string p_base_verbosity,
		string p_base_method,
		string p_login_data,
		string p_post_content_type,
		string p_base_results_storage,
		string p_login_uri,
		string p_login_name)
{
  std::vector<std::string> res;
  
  string res00 = "na";
  string res0 = "";
  string res1 = "";
  string res2 = "";
  string res3 = "";
  
  show_string("Sending log in data...");
  res00 = qpost(p_base_verbosity, p_base_method, p_login_data, p_post_content_type, p_base_results_storage, p_login_uri, "na");
  show_string("\n");
  show_string("Login result\n\n");
  show_string(res00);	      

  store_results(p_base_results_storage, p_login_name, res00);
  
  // Parse the userId value from the received json data.
  res0 = seek_in_json(res00, "\"id\"");
  res.push_back(res0);

  //res1 = seek_in_json(res00, "\"ttl\"");
  //res.push_back(res1);
  res.push_back("na");

  res2 = seek_in_json(res00, "\"created\"");
  res.push_back(res2);
  
  res3 = seek_in_json(res00, "\"userId\"");      
  if (res3 == "")
    {
      res.push_back("na");
    }
  else
    {
      res.push_back(res3);
    }
  if (p_base_verbosity == "yes")
    {
        show_var("id", res[0]);
	show_var("ttl", res[1]);
	show_var("created", res[2]);
	show_var("userId", res[3]);
    }
 
  return res;
}


/* qx_delete_experiment - deletes an experiment.

Arguments:
- p_base_verbosity: base_verbosity.
- p_base_method: base_method.
- p_delete_data: delete data.
- p_delete_content_type: delete_content_type.
- p_base_results_storage: base_results_storage.
- p_delete_uri: delete_uri.
- p_delete_name: delete_name.
- p_login_id: login_id.
- p_base_name: nase_name.

Output:
-  Result of the operation.

Sources:
- https://github.com/nanowebcoder/NanoQuantumShellGame/blob/master/NanoQuantum.Core/QProcessor.cs

 */
string qx_delete_experiment(string p_base_verbosity,
			    string p_base_method,
			    string p_delete_data,
			    string p_delete_content_type,
			    string p_base_results_storage,			    
			    string p_delete_uri,
			    string p_delete_name,
			    string p_login_id,
			    string p_base_name)
{
  string res = "";
  string delete_uri = p_delete_uri; 
    
  delete_uri = delete_uri + "/users/"+p_login_id+"/codes/"+p_base_name;
  if(p_base_verbosity == "yes")
    {
      show_string("Deletion data: ");
      show_var("delete_data", p_delete_data);
      show_var("delete_content_type", p_delete_content_type);
      show_var("base_results_storage", p_base_results_storage);
      show_var("delete_uri", delete_uri);
      show_string("\n\n");
      show_string("Deleting...");
    }
  
  res = qpost(p_base_verbosity,
	      p_base_method,
	      p_delete_data,
	      p_delete_content_type,
	      p_base_results_storage,
	      p_delete_uri,
	      p_login_id);
  
  store_results(p_base_results_storage, p_delete_name, res);
  
  return res;
}


/* qx_post_experiment - posts an experiment.

Arguments:
- p_base_verbosity: base_verbosity.
- p_base_method: base_method.
- p_post_data: post data.
- p_post_content_type: post_content_type.
- p_post_results_storage: post_results_storage.
- p_post_uri: post_uri.
- p_post_name: post_name.
- p_login_id: login_id.
- p_base_name: nase_name.
- p_base_data: base_data.
- p_base_shots: base_shots.
- p_base_seedL base_seed.
- p_base_deviceL base_device.


Output:
-  Result of the operation.

 */
string qx_post_experiment(string p_base_verbosity,
			  string p_base_method,
			  string p_post_data,
			  string p_post_content_type,
			  string p_base_results_storage,			    
			  string p_post_uri,
			  string p_post_name,
			  string p_login_id,
			  string p_base_name,
			  string p_base_data,
			  string p_base_shots,
			  string p_base_seed,
			  string p_base_device)			  
{
  string res = "";
  string post_data = "";
  string post_uri = p_post_uri;
  string header2 = "";
  string post_content_type = "";
  
  post_data = "qasm="+p_base_data+"&codeType="+"QASM2"+"&name="+p_base_name;
  post_uri = post_uri+"?access_token="+p_login_id+"&shots="+p_base_shots+"&seed="+p_base_seed+"&deviceRunType="+p_base_device;

  //post_data = "qasm="+p_base_data+"&codeType="+"QASM2"+"&name="+p_base_name+"&shots="+p_base_shots+"&seed="+p_base_seed+"&deviceRunType="+p_base_device;
  //post_uri = post_uri+"?access_token="+p_login_id;
  
  if(p_base_verbosity == "yes")
    {
      show_string("\n\n");
      show_string("Data to be posted: ");
      show_var("post_data", post_data);
      show_var("post_content_type", p_post_content_type);
      show_var("base_results_storage", p_base_results_storage);
      show_var("post_uri", post_uri);
      show_string("\n\n");
      show_string("Posting...");
    }
  
  res = qpost(p_base_verbosity,
	      p_base_method,
	      post_data,
	      p_post_content_type,
	      p_base_results_storage,
	      post_uri,
	      p_login_id);
  
  store_results(p_base_results_storage, p_post_name, res);
  
  return res;
}







