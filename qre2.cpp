/* =============================================================================

qre2.cpp

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

#include "qre2.hpp"
#include "./lib/qlib/core/qlib.h"

using namespace qlib::quantum;
using namespace std;


/* write_callback - Required to read data received using curl.

Sources:
- Based on an example at https://stackoverflow.com/questions/51317221/how-to-use-libcurl-in-c-to-send-a-post-request-and-receive-it

 */

static size_t write_callback(void *p_contents, size_t p_size, size_t p_nmemb, void *p_userp)
{
    ((std::string*)p_userp)->append((char*)p_contents, p_size * p_nmemb);
    
    return p_size * p_nmemb;
}


/* ibmqx_qpost- performs a post on q-series quantum computers. This function is 
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
- Arguments correspond to the standard parameters contained in a .qreg file.

Sources:
- https://curl.haxx.se/libcurl/c/simplepost.html
- https://developer.ibm.com/tutorials/os-quantum-computing-shell-game/
- https://stackoverflow.com/questions/22457601/http-post-header-fields-with-libcurl
- https://stackoverflow.com/questions/328281/why-content-length-0-in-post-requests

Output:
- Updated value for request log.

 */
std::string ibmqx_qpost(std::string p_base_verbosity,
		  std::string p_base_method,
		  std::string p_base_data,
		  std::string p_content_type,
		  std::string p_base_results_storage,
		  std::string p_uri,
		  std::string p_login_id)
{
  CURL *curl;
  CURLcode res1 = CURLE_OK;
  std::string res = " ";
  std::string read_buffer;
  std::string file_cookies = "data/cookies/qre_cookies.txt";
  std::string pdata = p_base_data;
  std::string pcontenttype = p_content_type;
  std::string puri = p_uri;
  std::string ploginid = p_login_id;
  std::string pcontenttype2 = "x-access-token: "+p_login_id+";charset=utf-8";
  std::string pcontentlength = "content-length: "+to_string(pdata.length());
  std::string pclientapp = "x-qx-client-application: qiskit-api-py";

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
  
  //Defining headers.
  struct curl_slist *headerlist=NULL;
  headerlist = curl_slist_append(headerlist, ccontenttype);
  headerlist = curl_slist_append(headerlist, "Transfer-Encoding: chunked");
  
  //Final check of data. 
  if (p_base_verbosity == "yes")
    {
      qre_show_string("Data as is just before making the request:");
      qre_show_var("Method: ", p_base_method);
      qre_show_var("Data: ", cdata);
      qre_show_var("Uri: ", curi);
      qre_show_var("Content length: ", ccontentlength);
      qre_show_var("Headers (init): ", ccontenttype);
      qre_show_var("Login id: ", ccloginid);
      qre_show_var("Client application: ", cclientapp);
    }

  res1 = curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if(curl)
    { 
      curl_easy_setopt(curl, CURLOPT_URL, curi);
      curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ccfile_cookies);
      curl_easy_setopt(curl, CURLOPT_COOKIEJAR, ccfile_cookies);
      curl_easy_setopt(curl, CURLOPT_HEADER, true);
      
      //Depending of request.
      if (p_base_method == "post")
	{
	  if (p_login_id == "na")
	    {
	      curl_easy_setopt(curl, CURLOPT_FRESH_CONNECT, 1L);
	    }
	  if (p_login_id != "na")
	    {
	      curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1L);
	      
	      headerlist = curl_slist_append(headerlist, ccontenttype2);
	      headerlist = curl_slist_append(headerlist, ccontentlength);
	      headerlist = curl_slist_append(headerlist, cclientapp);
	      headerlist = curl_slist_append(headerlist, "Accept: application/json");
	       
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
      qre_show_v(p_base_verbosity, ("\nCurl status message: "));    
    }

  curl_slist_free_all(headerlist);
  curl_easy_cleanup(curl);
  curl_global_cleanup();
  
  return res;
}


/* ibmqx_login - attempts to log in into the IBM QX server.

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
std::vector<std::string> ibmqx_login(std::string p_base_verbosity,
				     std::string p_base_method,
				     std::string p_login_data,
				     std::string p_post_content_type,
				     std::string p_base_results_storage,
				     std::string p_login_uri,
				     std::string p_login_name)
{
  std::vector<std::string> res;
  
  std::string res00 = "na";
  std::string res0 = "";
  std::string res1 = "";
  std::string res2 = "";
  std::string res3 = "";
  
  qre_show_string("Sending log in data...");
  res00 = ibmqx_qpost(p_base_verbosity,
		      p_base_method,
		      p_login_data,
		      p_post_content_type,
		      p_base_results_storage,
		      p_login_uri,
		      "na");
  qre_show_string("\n");
  qre_show_string("Login result\n\n");
  qre_show_string(res00);	      

  qre_store_results(p_base_results_storage, p_login_name, res00);
  
  // Parse the userId value from the received json data.
  res0 = qre_seek_in_json(res00, "\"id\"");
  res.push_back(res0);

  //res1 = qre_seek_in_json(res00, "\"ttl\"");
  //res.push_back(res1);
  res.push_back("na");

  res2 = qre_seek_in_json(res00, "\"created\"");
  res.push_back(res2);
  
  res3 = qre_seek_in_json(res00, "\"userId\"");      
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
        qre_show_var("id", res[0]);
	qre_show_var("ttl", res[1]);
	qre_show_var("created", res[2]);
	qre_show_var("userId", res[3]);
    }
 
  return res;
}


/* ibmqx_post_experiment - posts an experiment.

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
std::string ibmqx_post_experiment(std::string p_base_verbosity,
				  std::string p_base_method,
				  std::string p_post_data,
				  std::string p_post_content_type,
				  std::string p_base_results_storage,			    
				  std::string p_post_uri,
				  std::string p_post_name,
				  std::string p_login_id,
				  std::string p_base_name,
				  std::string p_base_data,
				  std::string p_base_shots,
				  std::string p_base_seed,
				  std::string p_base_device)			  
{
  std::string res = "";
  std::string post_data = "";
  std::string post_uri = p_post_uri;
  std::string header2 = "";
  std::string post_content_type = "";

  if (p_base_verbosity == "yes")
    {  
      qre_show_string("Posting...");
    }
  post_data = "qasm="+p_base_data+"&codeType="+"QASM2"+"&name="+p_base_name;
  post_uri = post_uri+"?access_token="+p_login_id+"&shots="+p_base_shots+"&seed="+p_base_seed+"&deviceRunType="+p_base_device;
  
  if(p_base_verbosity == "yes")
    {
      qre_show_string("\n\n");
      qre_show_string("Data to be posted: ");
      qre_show_var("post_data", post_data);
      qre_show_var("post_content_type", p_post_content_type);
      qre_show_var("base_results_storage", p_base_results_storage);
      qre_show_var("post_uri", post_uri);
      qre_show_string("\n\n");
      qre_show_string("Posting...");
    }
  
  res = ibmqx_qpost(p_base_verbosity,
		    p_base_method,
		    post_data,
		    p_post_content_type,
		    p_base_results_storage,
		    post_uri,
		    p_login_id);
  
  qre_store_results(p_base_results_storage, p_post_name, res);
  
  return res;
}


/* ibmqx_delete_experiment - deletes an experiment.

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
std::string ibmqx_delete_experiment(std::string p_base_verbosity,
				    std::string p_base_method,
				    std::string p_delete_data,
				    std::string p_delete_content_type,
				    std::string p_base_results_storage,			    
				    std::string p_delete_uri,
				    std::string p_delete_name,
				    std::string p_login_id,
				    std::string p_base_name)
{
  std::string res = "";
  std::string delete_uri = p_delete_uri; 
    
  delete_uri = delete_uri + "/users/"+p_login_id+"/codes/"+p_base_name;
  if(p_base_verbosity == "yes")
    {
      qre_show_string("Deletion data: ");
      qre_show_var("delete_data", p_delete_data);
      qre_show_var("delete_content_type", p_delete_content_type);
      qre_show_var("base_results_storage", p_base_results_storage);
      qre_show_var("delete_uri", delete_uri);
      qre_show_string("\n\n");
      qre_show_string("Deleting...");
    }
  
  res = ibmqx_qpost(p_base_verbosity,
		    p_base_method,
		    p_delete_data,
		    p_delete_content_type,
		    p_base_results_storage,
		    p_delete_uri,
		    p_login_id);
  
  qre_store_results(p_base_results_storage, p_delete_name, res);
  
  return res;
}


/* qlib_post_experiment - perform experiment locally on a qlib-based simulator.

Arguments:
- p_base_verbosity: base_verbosity.
- p_base_data: base_data.
- p_base_seed: base_seed.
- p_base_shots: base_shots.
- p_base_name: base_name.
- p_base_device: local backend (qlib simulator, for now ;) ).
  - "simulator": local simulator

 */
std::string qlib_post_experiment(std::string p_base_verbosity,
				 std::string p_base_data,
				 std::string p_base_results_storage,
				 std::string p_base_seed,
				 std::string p_base_shots,
				 std::string p_base_name,
				 std::string p_base_device)
{
  std::vector<std::string>qasm_instructions;

  int shots = 0;
  int vector_size = 0;
  int i = 0;
  int j = 0;
  int k = 0;

  float svecx = 0;
  float svecy = 0;
  double svect = 0;
  double sprob = 0;
  
  std::vector<double>c;
  std::vector<double>r;
  
  std::string res = "";
  std::string res2 = "";
  std::string str = "";
  std::string str1 = "";
  std::string str2 = "";
  std::string str3 = "";
  std::string str4 = "";
  std::string str5 = "";
  std::string line = "";
  std::string comment = "//";
  std::string space = " ";
  std::string sq = "";
  std::string delim = "";
  
  long unsigned int rn = 0;
  long unsigned int rm = 0;
  
  size_t pos = 0;
  size_t terms = 0;

  struct res_row
  {
    std::string sterm;
    std::string sket;
    std::string svec;
  };

  std::vector<res_row> res_parc;
  std::vector<double> res_sum;
  std::vector<double> res_shots;
  std::vector<double> res_final;

  if (p_base_verbosity == "yes")
    {  
      qre_show_string("Posting...");
    }
  qasm_instructions = qre_parse_data_string(p_base_verbosity, p_base_data);
  vector_size = qasm_instructions.size();
  shots = (int)qre_s2d(p_base_shots);

  // We need to define qreg q and creg c before parsing anything else in the QASM file.
  for(i = 0; i < vector_size; i++)
    {
      if (qre_recog("qreg", qasm_instructions[i]) == true)
	{
	  rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
	}
    }

  qreg q(rn);
  qre_show_v(p_base_verbosity, (" Creating qubit register q."));
  rn = 0;
  
  for(i = 0; i < vector_size; i++)
    {
      if (qre_recog("creg", qasm_instructions[i]) == true)
	{
	  rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "c"));
	  if (c.size() == 0)
	    {
	      for(j = 0; j < (int)rn; j++)
		{
		  c.push_back(0.00);
		  r.push_back(0.00);
		}
	    }
	}
    }

  qre_show_v(p_base_verbosity, (" Creating bit register c."));
  
  // Shots iteration.
  for(k = 0; k < shots; k++)
    {

      // File parsing iteration.
      for(i = 0; i < vector_size; i++)
	{
	  // Find if line will be ignored or not based on the value of delim.
	  if (qre_recog(comment, qasm_instructions[i]) == true)
	    {
	      qre_show_v(p_base_verbosity, ("Ignore, comment -> " + qasm_instructions[i]));
	    }
	  // Find if line will be ignored if it does not have spaces.
	  else if (qre_recog(space, qasm_instructions[i]) == false)
	    {
	      qre_show_v(p_base_verbosity, ("Ignore, error -> " + qasm_instructions[i]));
	    }
	  else
	    {
	      qre_show_v(p_base_verbosity, ("Post   -> " + qasm_instructions[i]));

	      // Process quantum instructions.		  
	      
	      // If instructions are atomic (no parenthesis found).
	      if (qre_recog("(", qasm_instructions[i]) == false)
		{	  
		  // OPENQASM
		  if (qre_recog("OPENQASM", qasm_instructions[i]) == true)
		    {
		      if (qre_recog("2.", qasm_instructions[i]) == true)
			{
			  qre_show_v(p_base_verbosity, " Recognized as OPENQASM 2.*.");
			}
		      else
			{
			  qre_show_v(p_base_verbosity, " OPENQASM version declaration not found. Possibly incompatible code.");
			}
		    }

		  // include
		  if (qre_recog("include", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, " Library");
		    }	  

		  // measurement gate.
		  if (qre_recog("measure", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i].substr(0), "q"));
		      rm = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i].substr(0), "c"));
		      qre_show_v(p_base_verbosity, (" measurement at qubit " + qre_d2s((double)rn) + " to bit " + qre_d2s((double)rm)));
		    }
		  
		  // cx gate.
		  if (qre_recog("cx", qasm_instructions[i]) == true)
		    {
		      pos = qasm_instructions[i].find(",");
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i].substr(0,pos-1), "q"));
		      rm = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i].substr(pos), "q"));
		      q.apply(gates::CX, {rn,rm});
		      qre_show_v(p_base_verbosity, (" cx gate at qubit " + qre_d2s((double)rn) + " to qubit " + qre_d2s((double)rm)));
		    }
		  
		  // h gate.
		  if (qre_recog("h ", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      q.apply(gates::H, {rn});
		      qre_show_v(p_base_verbosity, (" h gate at qubit " + qre_d2s((double)rn)));
		    }

		  // x gate.
		  if ((qre_recog("x ", qasm_instructions[i]) == true)&&(qre_recog("cx ", qasm_instructions[i]) == false))
		    {		  	     
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      q.apply(gates::X, {rn});
		      qre_show_v(p_base_verbosity, (" x gate at qubit " + qre_d2s((double)rn)));
		    }
	      
		  // y gate.
		  if ((qre_recog("y ", qasm_instructions[i]) == true)&&(qre_recog("cy ", qasm_instructions[i]) == false))
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      q.apply(gates::Y, {rn});
		      qre_show_v(p_base_verbosity, (" y gate at qubit " + qre_d2s((double)rn)));
		    }

		  // z gate.
		  if ((qre_recog("z ", qasm_instructions[i]) == true)&&(qre_recog("cz ", qasm_instructions[i]) == false))
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      q.apply(gates::Z, {rn});
		      qre_show_v(p_base_verbosity, (" z gate at qubit " + qre_d2s((double)rn)));
		    }

		  // s gate.
		  if (qre_recog("s ", qasm_instructions[i]) == true )
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      q.apply(gates::S, {rn});
		      qre_show_v(p_base_verbosity, (" s gate at qubit " + qre_d2s((double)rn)));
		    }

		  // sdg gate.
		  if (qre_recog("sdg", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      q.apply(gates::Sdg, {rn});		      
		      qre_show_v(p_base_verbosity, (" sdg gate at qubit " + qre_d2s((double)rn)));
		    }
		  
		  // t gate.
		  if (qre_recog("t ", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      q.apply(gates::T, {rn});
		      qre_show_v(p_base_verbosity, (" t gate at qubit " + qre_d2s((double)rn)));
		    }

		  // tdg gate.
		  if (qre_recog("tdg", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      q.apply(gates::Tdg, {rn});		      
		      qre_show_v(p_base_verbosity, (" tdg gate at qubit " + qre_d2s((double)rn)));
		    }
		  
		  // t gate.
		  if (qre_recog("id ", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      q.apply(gates::I, {rn});
		      qre_show_v(p_base_verbosity, (" id gate at qubit " + qre_d2s((double)rn)));
		    }

		  // s gate.
		  if (qre_recog("barrier ", qasm_instructions[i]) == true )
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      qre_show_v(p_base_verbosity, (" barrier at qubit " + qre_d2s((double)rn)));
		    }
	      
		}

	      // ( Instructions that have parenthesis are composites and require secial treatment.
	      if (qre_recog("(", qasm_instructions[i]) == true)
		{
		  
		  // u1 gate.
		  if (qre_recog("u1", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      //q.apply(gates::phase(M_I), {rn})
		      qre_show_v(p_base_verbosity, " u1 gate not recognized.");
		    }

		  // u2 gate.
		  if (qre_recog("u2", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, " u2 gate not recognized.");
		    }

		  // u3 gate.
		  if (qre_recog("u3", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, " u3 gate not recognized.");
		    }
	       		  
		}	      
	    }
	}

      // This is required to calculate the probabilitiesafter all shots have been performed.
      
      //On first shot, create data set for results.
      if (k == 0)
	{
	  delim = "> + ";
	  
	  //Count number of terms on linear combination.
	  res2 = q.toString();
	  qre_show_v(p_base_verbosity, res2);	  
	  terms = 1;
	  while ((pos = res2.find(delim)) != std::string::npos)
	    {
	      terms++;
	      res2.erase(0, pos + delim.length());   
	    }

	  terms++;
	    
	  /*
	    This gives us a string matrix and two vectors:
	    - res_parc contains strings parsed from res2.
	    - res_sum contains the sum of all values corresponding to each ket.
	    - res_final contains the avg probabilities obtained from res_sum and shots.

	    Each celement of these vectors and matrices will be updated on each shot.
	   */
	  for (i = 0; i < (int)terms; i++)
	    {
	      res_parc.push_back({"","",""});
	      res_sum.push_back(0);
	      res_shots.push_back(0);
	      res_final.push_back(0);
	    }	  
	}

      /* On each iteration, after res_parc has been created, parse res2 and distribute the parsed
      data within the struct of each row.*/

      //Reset some vars.
      j = 0;
      pos = 0;
      res2 = q.toString();      
      delim = ">";
      while ((pos = res2.find(delim)) != std::string::npos)
	{
	  res_parc[j].sterm = res2.substr(0, pos);
	  str1 = res_parc[j].sterm;
	  res_parc[j].sket = str1.substr((str1.find("|"))+1);
	  res_parc[j].svec = str1.substr(0,(str1.find("|"))-1);
	  str1 = res_parc[j].svec;
	  res_parc[j].svec = str1.substr((str1.find("("))+1);
	  res2.erase(0, pos + delim.length());

	  //Cast numbers as two floats.
	  str3 = res_parc[j].svec;
	  str4 = str3.substr(0,str3.find(" + "));
	  str5 = str3.substr(str3.find(" + ")+3);
	  str5 = str5.substr(0,str5.find("i"));
	  svecx = atof(str4.c_str());
	  svecy = atof(str5.c_str());

	  //Square both and sum to get p = x^2 + y^2.
	  svect = (double)(pow(svecx,2) + pow(svecy,2));
	  res_sum[j] = svect;
	  
	  //Increment value of res_sum.
	  res_shots[j] = res_shots[j] + res_sum[j];
	  
	  //Show some info.
	  if (p_base_verbosity == "yes")
	    {
	      cout << "res_parc[" << j << "].sterm =" << res_parc[j].sterm << " .sket ="<< res_parc[j].sket << " .svec=" << res_parc[j].svec << endl;
	    }
	  j++;
	}
      
    } // k

  //After performing all shots, average values, calculate probabilities.
  for (i = 0; i < (int)res_final.size(); i++)
    {
      res_final[i] = res_shots[i] / shots;
      sprob = sprob + res_final[i];
    }  

    if (p_base_verbosity == "yes")
      {  
	cout << "Final avg: " << sprob << endl;
      }

  //Build the json string with results.
  res = "{";
  res = res + "\'idCode\': " + "u\'"+ p_base_device +"\'" + ",\'idExecution\': " + "u\'" + p_base_name + "\'" + ",\'result\': {\'measure\': {u\'labels\': [";

  //Put labels.
  for(i = 0; i < (int)res_final.size(); i++)
  {
    if (i < ((int)res_final.size() - 1))
      {
	res = res + "u\'";
      }
    
    res = res + res_parc[i].sket;

    if (i < ((int)res_final.size() - 1))
      {
	res = res + "\'";
      }
    
    if (i < ((int)res_final.size() - 2))
      {
	res = res + ",";
      }
    
  }
  res = res + "],";

  //Put qubit numbers.
  int nq = c.size();
  res = res + "u\'qubits\':[";
  for(i = 0; i < nq; i++)
    {
      res = res + qre_d2s((double)i);
      if (i < (nq - 1))
	{
	  res = res + ", ";
	}
    }
    
  res = res + "],";

  //Put probability values.
  res = res + "u\'values\':[";
  nq = (int)res_final.size();
  for(i = 0; i < nq; i++)
    {
      if (i < (nq - 1))
	{
	  res = res + qre_d2s(res_final[i]);
	}
      if (i < (nq - 2))
	{
	  res = res + ", ";
	}
    }
    
  res = res + "]}},";

  // Finish off the string.
  res = res + "\'status\':\'DONE\'}";

  //Save.
  qre_store_results(p_base_results_storage, p_base_name, res);
  
  return res;
}


//************************************************************************************************************************


/* qx_post_experiment - perform experiment locally on the qx simulator.

Arguments:
- p_base_verbosity: base_verbosity.
- p_base_data: base_data.
- p_base_seed: base_seed.
- p_base_shots: base_shots.
- p_base_name: base_name.
- p_base_device: local backend (qx simulator) ).
  - "qx_simulator": local simulator

Sources:
- https://www.tudelft.nl/eemcs/the-faculty/departments/quantum-computer-engineering/quantum-computer-architecture-lab/research/quantum-programming/

 */
std::string qx_post_experiment(std::string p_base_verbosity,
			       std::string p_base_data,
			       std::string p_base_results_storage,
			       std::string p_base_seed,
			       std::string p_base_shots,
			       std::string p_base_name,
			       std::string p_base_device,
			       std::string p_simulator_path)
{
  std::vector<std::string>qasm_instructions;

  int shots = 0;
  int vector_size = 0;
  int i = 0;
  int j = 0;
  int k = 0;
  int res1 = 0;
  
  float svecx = 0;
  float svecy = 0;
  double svect = 0;
  double sprob = 0;
  
  std::vector<double>c;
  std::vector<double>r;
  
  std::string res = "";
  std::string res2 = "";
  std::string str = "";
  std::string str1 = "";
  std::string str2 = "";
  std::string str3 = "";
  std::string str4 = "";
  std::string str5 = "";
  std::string line = "";
  std::string comment = "//";
  std::string space = " ";
  std::string sq = "";
  std::string delim = "";
  std::string file ="";
  std::string patha = "data/temp/";
  std::string pathj = "";
  std::string sys_command = p_simulator_path;
  
  long unsigned int rn = 0;
  long unsigned int rm = 0;
  
  size_t pos = 0;
  size_t terms = 0;

  struct res_row
  {
    std::string sterm;
    std::string sket;
    std::string svec;
  };

  std::vector<res_row> res_parc;
  std::vector<double> res_sum;
  std::vector<double> res_shots;
  std::vector<double> res_final;

  if (p_base_verbosity == "yes")
    {  
      qre_show_string("Posting...");
    }
  qasm_instructions = qre_parse_data_string(p_base_verbosity, p_base_data);
  vector_size = qasm_instructions.size();
  shots = (int)qre_s2d(p_base_shots);

  //(re)Create temp.qc
  file = "qx_temp.qc";
  pathj = patha + file;     
  std::ofstream qc_file_ini(pathj);
  qc_file_ini << "# qx_temp.qc" << endl;
  
  // We need to define qreg q and creg c before parsing anything else in the QASM file.
  for(i = 0; i < vector_size; i++)
    {
      if (qre_recog("qreg", qasm_instructions[i]) == true)
	{
	  rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
	  qc_file_ini << "qubits " << rn << endl;
	}
    }

  qreg q(rn);
  qre_show_v(p_base_verbosity, (" Creating qubit register q."));
  rn = 0;
  
  for(i = 0; i < vector_size; i++)
    {
      if (qre_recog("creg", qasm_instructions[i]) == true)
	{
	  rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "c"));
	  if (c.size() == 0)
	    {
	      for(j = 0; j < (int)rn; j++)
		{
		  c.push_back(0.00);
		  r.push_back(0.00);
		}
	    }
	}
    }

  qc_file_ini.close();
  
  qre_show_v(p_base_verbosity, (" Creating bit register c."));
  
  // Complile qc file from qasm file.
  if (k == 0)
    {
      //Reopen temp.qc, but to append stuff.
      std::ofstream qc_file_app;
      qc_file_app.open(pathj, std::ios::app);
      
      // File parsing iteration.
      for(i = 0; i < vector_size; i++)
	{
	  // Find if line will be ignored or not based on the value of delim.
	  if (qre_recog(comment, qasm_instructions[i]) == true)
	    {
	      qre_show_v(p_base_verbosity, ("Ignore, comment -> " + qasm_instructions[i]));
	    }
	  // Find if line will be ingored if it does not have spaces.
	  else if (qre_recog(space, qasm_instructions[i]) == false)
	    {
	      qre_show_v(p_base_verbosity, ("Ignore, error -> " + qasm_instructions[i]));
	    }
	  else
	    {
	      qre_show_v(p_base_verbosity, ("Post   -> " + qasm_instructions[i]));

	      // Process quantum instructions.		  
	      
	      // If instructions are atomic (no parenthesis found).
	      if (qre_recog("(", qasm_instructions[i]) == false)
		{	  
		  // OPENQASM
		  if (qre_recog("OPENQASM", qasm_instructions[i]) == true)
		    {
		      if (qre_recog("2.", qasm_instructions[i]) == true)
			{
			  qre_show_v(p_base_verbosity, " Recognized as OPENQASM 2.*.");
			}
		      else
			{
			  qre_show_v(p_base_verbosity, " OPENQASM version declaration not found. Possibly incompatible code.");
			}
		    }

		  // include
		  if (qre_recog("include", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, " Library");
		    }	  

		  // measurement gate.
		  if (qre_recog("measure", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i].substr(0), "q"));
		      rm = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i].substr(0), "c"));
		      qc_file_app << "measure q" << rm << endl;
		      qre_show_v(p_base_verbosity, (" measurement at qubit " + qre_d2s((double)rn) + " to bit " + qre_d2s((double)rm)));
		    }
		  
		  // cx gate.
		  if (qre_recog("cx", qasm_instructions[i]) == true)
		    {
		      pos = qasm_instructions[i].find(",");
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i].substr(0,pos-1), "q"));
		      rm = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i].substr(pos), "q"));
		      qc_file_app << "cx q" << rn << ", q" << rm << endl;
		      qre_show_v(p_base_verbosity, (" cx gate at qubit " + qre_d2s((double)rn) + " to qubit " + qre_d2s((double)rm)));
		    }
		  
		  // h gate.
		  if (qre_recog("h ", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      qc_file_app << "h q" << rn << endl;
		      qre_show_v(p_base_verbosity, (" h gate at qubit " + qre_d2s((double)rn)));
		    }

		  // x gate.
		  if ((qre_recog("x ", qasm_instructions[i]) == true)&&(qre_recog("cx ", qasm_instructions[i]) == false))
		    {		  	     
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      qc_file_app << "x q" << rn << endl;
		      qre_show_v(p_base_verbosity, (" x gate at qubit " + qre_d2s((double)rn)));
		    }
	      
		  // y gate.
		  if ((qre_recog("y ", qasm_instructions[i]) == true)&&(qre_recog("cy ", qasm_instructions[i]) == false))
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      qc_file_app << "y q" << rn << endl;
		      qre_show_v(p_base_verbosity, (" y gate at qubit " + qre_d2s((double)rn)));
		    }

		  // z gate.
		  if ((qre_recog("z ", qasm_instructions[i]) == true)&&(qre_recog("cz ", qasm_instructions[i]) == false))
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      qc_file_app << "z q" << rn << endl;
		      qre_show_v(p_base_verbosity, (" z gate at qubit " + qre_d2s((double)rn)));
		    }

		  // s gate.
		  if (qre_recog("s ", qasm_instructions[i]) == true )
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      qc_file_app << "s q" << rn << endl;
		      qre_show_v(p_base_verbosity, (" s gate at qubit " + qre_d2s((double)rn)));
		    }

		  // sdg gate.
		  if (qre_recog("sdg", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      qre_show_v(p_base_verbosity, " sdg gate not recognized.");		      
		      qre_show_v(p_base_verbosity, (" sdg gate at qubit " + qre_d2s((double)rn)));
		    }
		  
		  // t gate.
		  if (qre_recog("t ", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      qc_file_app << "t q" << rn << endl;
		      qre_show_v(p_base_verbosity, (" t gate at qubit " + qre_d2s((double)rn)));
		    }

		  // tdg gate.
		  if (qre_recog("tdg", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      qc_file_app << "tdag q" << rn << endl;
		      qre_show_v(p_base_verbosity, (" tdg gate at qubit " + qre_d2s((double)rn)));
		    }
		  
		  // t gate.
		  if (qre_recog("id ", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      qre_show_v(p_base_verbosity, " id gate not recognized.");
		      qre_show_v(p_base_verbosity, (" id gate at qubit " + qre_d2s((double)rn)));
		    }

		  // s gate.
		  if (qre_recog("barrier ", qasm_instructions[i]) == true )
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      qre_show_v(p_base_verbosity, " barrier not recognized.");
		      qre_show_v(p_base_verbosity, (" barrier at qubit " + qre_d2s((double)rn)));
		    }
	      
		}

	      // ( Instructions that have parenthesis are composites and require secial treatment.
	      if (qre_recog("(", qasm_instructions[i]) == true)
		{
		  
		  // u1 gate.
		  if (qre_recog("u1", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      //q.apply(gates::phase(M_I), {rn})
		      qre_show_v(p_base_verbosity, " u1 gate not recognized.");
		    }

		  // u2 gate.
		  if (qre_recog("u2", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, " u2 gate not recognized.");
		    }

		  // u3 gate.
		  if (qre_recog("u3", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, " u3 gate not recognized.");
		    }
	       		  
		}	      
	    }
	}

      //Add these lines to the end of the file
      qc_file_app << "display" << endl;
      qc_file_app << "# error_model depolarizing_channel,0.001" << endl;
            
      //Once finished parsing qasm_instructions file and compiling temp.qc file, close latter.
      qc_file_app.close();

    }
  
  // Shots iteration.
  shots = 1; //temp.
  sys_command = sys_command + " " + patha + "qx_temp.qc" + " > " + patha + "qx_temp.txt";
  for(k = 0; k < shots; k++)
    {
      //call qx simulator with created temp.qc file
      res1 = (system(sys_command.c_str()));
      
      //Read results on each shot iteration and accummulate
      
    }

  //cout << res1 << endl;
  res = "qx_simulator test ok.";

  //Save.
  //qre_store_results(p_base_results_storage, p_base_name, res);
  
  return res;
}




