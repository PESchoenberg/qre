/* =============================================================================

qre2.cpp

================================================================================

 Copyright (C) 2018 - 2020  Pablo Edronkin (pablo.edronkin at yahoo.com)

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
- Stack Overflow. (2019). How to use libcurl in c++ to send a POST request and 
  receive it?. [online] Available at: https://stackoverflow.com/questions/
  51317221/how-to-use-libcurl-in-c-to-send-a-post-request-and-receive-it 
  [Accessed 21 Dec. 2019].

 */
static size_t write_callback(void *p_contents,
			     size_t p_size,
			     size_t p_nmemb,
			     void *p_userp)
{
  ((std::string*)p_userp)->append((char*)p_contents, p_size * p_nmemb);
    
  return p_size * p_nmemb;
}


/* show_res_parc - Shows parcial results.

Arguments:
- p_base_verbosity: base verbosity.
- p_j: j.
- p_st: sterm.
- p_sk: sket.
- p_sv: svec.

 */
void show_res_parc(std::string p_base_verbosity,
		   int p_j,
		   std::string p_st,
		   std::string p_sk,
		   std::string p_sv)
{
  qre_show_v(p_base_verbosity, ("res_parc[" + (qre_d2s((double)p_j)) + "].sterm = " + p_st + " .sket = " + p_sk + " .svec = " + p_sv + "\n"));
}


/* construct_res_step1 - Builds a standard header for json results.

Arguments:
- p_res: res.
- p_base_device: base device.
- p_base_name: base name.

 */
std::string construct_res_step1(std::string p_res,
				std::string p_base_device,
				std::string p_base_name)
{
  return (p_res + "\'idCode\': " + qre_txt(43) + p_base_device + qre_txt(44) + ",\'idExecution\': " + qre_txt(43) + p_base_name + qre_txt(44) + ",\'result\': {\'measure\': {u\'labels\': [");
}


/* construct_res_step3 - Builds standard parts of json results string.

Arguments:
- p_step: step.
- p_res: res.

 */
std::string construct_res_step3(int p_step,
				std::string p_res)
{
  std::string res = p_res;

  if (p_step == 3)
    {
      res = res + "]}},\'status\':\'DONE\'}";      
    }
  else if (p_step == 1)
    {
      res = res + "u\'qubits\':[";
    }
  else if (p_step == 2)
    {
      res = res + "u\'values\':[";
    }
  else if (p_step == 4)
    {
      res = res + qre_txt(43);
    }
  else if (p_step == 5)
    {
      res = res + qre_txt(44);
    }
  else if (p_step == 6)
    {
      res = res + ",";      
    }
  else if (p_step == 7)
    {
      res = res + "],";      
    }  
  else if (p_step == 8)
    {
      res = res + ", ";      
    }
  
  return res;
}


/* ibmqx_qpost - Performs a post on q-series quantum computers. This function 
is adapted from an example shown on the links listed as sources.

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
- Curl.haxx.se. (2019). libcurl example - simplepost.c. [online] Available at: 
  https://curl.haxx.se/libcurl/c/simplepost.html [Accessed 21 Dec. 2019].
- IBM Developer. (2019). Archived | Quantum computing in action: IBM's Q 
  experience and the quantum shell game. [online] Available at: 
  https://developer.ibm.com/tutorials/os-quantum-computing-shell-game/ 
  [Accessed 21 Dec. 2019].
- Stenberg, D. (2019). HTTP Post Header Fields with libcurl. [online] 
  Stack Overflow. Available at: https://stackoverflow.com/questions/22457601/
  http-post-header-fields-with-libcurl [Accessed 21 Dec. 2019].
- Bondy, B. (2019). Why "Content-Length: 0" in POST requests?. [online] Stack 
  Overflow. Available at: https://stackoverflow.com/questions/328281/
  why-content-length-0-in-post-requests [Accessed 21 Dec. 2019].
- GitHub. (2019). quantumjim/qreative. [online] Available at: 
  https://github.com/quantumjim/qreative/blob/master/tutorials/qrng_with_http.ipynb 
  [Accessed 21 Dec. 2019].

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
  std::string pcontenttype2 = "x-access-token: " + p_login_id + ";charset=utf-8";
  std::string pcontentlength = "content-length: " + to_string(pdata.length());
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
  
  /* Defining headers. */
  struct curl_slist *headerlist=NULL;
  headerlist = curl_slist_append(headerlist, ccontenttype);
  headerlist = curl_slist_append(headerlist, "Transfer-Encoding: chunked");
  
  /* Final check of data. */
  if (qre_vb(p_base_verbosity))
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
  if (curl)
    { 
      curl_easy_setopt(curl, CURLOPT_URL, curi);
      curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ccfile_cookies);
      curl_easy_setopt(curl, CURLOPT_COOKIEJAR, ccfile_cookies);
      curl_easy_setopt(curl, CURLOPT_HEADER, true);
      
      /* Depending of request. */
      if (p_base_method == qre_txt(49))
	{
	  if (p_login_id == qre_txt(28))
	    {
	      curl_easy_setopt(curl, CURLOPT_FRESH_CONNECT, 1L);
	    }
	  if (p_login_id != qre_txt(28))
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
	  
	  /* if we don't provide POSTFIELDSIZE, libcurl will strlen() itself */
	  // curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (long)strlen(data));	  
	}
      else if (p_base_method == qre_txt(50))
	{
	  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
	}
      else if (p_base_method == qre_txt(51))
	{
	  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	}
            
      /* Send the list with all headers. */
      curl_easy_setopt(curl, CURLOPT_HEADER, headerlist);
      
      /* Required to receive string data. */
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);

      /* If verbosity value is yes, some more of info concerning the connection 
	 will be shown.*/
      if (qre_vb(p_base_verbosity))
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
      if (res1 != CURLE_OK)
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


/* ibmqx_login - Attempts to log in into the IBM QX server.

Arguments:
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
  
  std::string res00 = qre_txt(28);
  std::string res0 = "";
  std::string res1 = "";
  std::string res2 = "";
  std::string res3 = "";
  
  qre_show_string(qre_txt(3));
  res00 = ibmqx_qpost(p_base_verbosity,
		      p_base_method,
		      p_login_data,
		      p_post_content_type,
		      p_base_results_storage,
		      p_login_uri,
		      qre_txt(28));
  qre_show_string("\nLogin result\n\n");
  qre_show_string(res00);	      
  qre_store_results(p_base_verbosity,
		    p_base_results_storage,
		    p_login_name,
		    res00);
  
  /* Parse the userId value from the received json data. */
  res0 = qre_seek_in_json(res00, "\"id\"");
  res.push_back(res0);
  // res1 = qre_seek_in_json(res00, "\"ttl\"");
  // res.push_back(res1);
  //res.push_back("na");
  res.push_back(qre_txt(28));
  res2 = qre_seek_in_json(res00, "\"created\"");
  res.push_back(res2);  
  res3 = qre_seek_in_json(res00, "\"userId\"");      
  if (res3 == "")
    {
      res.push_back(qre_txt(28));
    }
  else
    {
      res.push_back(res3);
    }
  if (qre_vb(p_base_verbosity))
    {
        qre_show_var("id", res[0]);
	qre_show_var("ttl", res[1]);
	qre_show_var("created", res[2]);
	qre_show_var("userId", res[3]);
    }
 
  return res;
}


/* ibmqx_post_experiment - Posts an experiment.

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
- p_base_seed: base_seed.
- p_base_device: base_device.

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
  std::string base_shots = qre_d2s((double)qre_enable_shots(true, p_base_shots));
  
  post_data = "qasm="+p_base_data+"&codeType="+"QASM2"+"&name="+p_base_name;
  post_uri = post_uri+"?access_token="+p_login_id+"&shots="+base_shots+"&seed="+p_base_seed+"&deviceRunType="+p_base_device;
  if (qre_vb(p_base_verbosity))
    {
      qre_show_string("\n\nData to be posted: ");
      qre_show_var("post_data", post_data);
      qre_show_var("post_content_type", p_post_content_type);
      qre_show_var("base_results_storage", p_base_results_storage);
      qre_show_var("post_uri", post_uri);
      qre_show_string("\n\n");
    }
  qre_show_v(p_base_verbosity, qre_txt(0));
  res = ibmqx_qpost(p_base_verbosity,
		    p_base_method,
		    post_data,
		    p_post_content_type,
		    p_base_results_storage,
		    post_uri,
		    p_login_id);
  // Bug here when calling ibmqx using Scheme.
  qre_store_results(p_base_verbosity,
		    p_base_results_storage,
		    p_post_name,
		    res);
  
  return res;
}


/* ibmqx_delete_experiment - Deletes an experiment.

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
- GitHub. (2019). nanowebcoder/NanoQuantumShellGame. [online] Available at: 
  https://github.com/nanowebcoder/NanoQuantumShellGame/blob/master/
  NanoQuantum.Core/QProcessor.cs [Accessed 21 Dec. 2019].

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
  if (qre_vb(p_base_verbosity))
    {
      qre_show_string("Deletion data: ");
      qre_show_var("delete_data", p_delete_data);
      qre_show_var("delete_content_type", p_delete_content_type);
      qre_show_var("base_results_storage", p_base_results_storage);
      qre_show_var("delete_uri", delete_uri);
      qre_show_string("\n\n");
      qre_show_string(qre_txt(1));
    }  
  res = ibmqx_qpost(p_base_verbosity,
		    p_base_method,
		    p_delete_data,
		    p_delete_content_type,
		    p_base_results_storage,
		    p_delete_uri,
		    p_login_id);
  qre_store_results(p_base_verbosity,
		    p_base_results_storage,
		    p_delete_name,
		    res);
  
  return res;
}


/* qre_find_qasm_standards - Finds out if code is QASM-compatible.

Arguments:
- p_qasm_instructions: qasm_instructions vectorelement.

 */
void qre_find_qasm_standard(std::string p_base_verbosity,
			    std::string p_qasm_instructions)
{
  if (qre_recog("2.", p_qasm_instructions) == true)
    {
      qre_show_v(p_base_verbosity, " Recognized as OPENQASM 2.*.");
    }
  else
    {
      qre_show_v(p_base_verbosity, " OPENQASM version declaration not found. Possibly incompatible code.");
    }
}


/* qre_put_qubit_numbers - Put qubit numbers in a results string.

Arguments:
- p_nq: nq.
- p_res: res.

Output:
- Results string (res).

 */
std::string qre_put_qubit_numbers (int p_nq,
				   std::string p_res)
{
  std::string res = p_res;

  res = construct_res_step3(1, res);
  for (int i = 0; i < p_nq; i++)
    {
      res = res + qre_d2s((double)i);
      if (i < (p_nq - 1))
	{
	  res = construct_res_step3(8, res);
	}
    }    

  return (construct_res_step3(7, res));
}


/* finish_res_and_save - Finish off the string and save.

Arguments:
- p_base_verbosity: base verbosity.
- p_base_results_storage: base results storage.
- p_base_bane: base name.
- p_res: res.

 */
std::string finish_res_and_save(std::string p_base_verbosity,
				std::string p_base_results_storage,
				std::string p_base_name,
				std::string p_res)
{
  std::string res = p_res;
  
  res = construct_res_step3(3, res);
  qre_store_results(p_base_verbosity, p_base_results_storage, p_base_name, res);

  return res;
}


/* qlib_post_experiment - Perform experiment locally on a qlib-based simulator.

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
  int shots = 0;
  int vector_size = 0;
  int i = 0;
  int j = 0;
  int k = 0;

  float svecx = 0;
  float svecy = 0;
  
  double sprob = 0;
    
  std::string res = "";
  std::string res2 = "";
  std::string str = "";
  std::string str1 = "";
  std::string str2 = "";
  std::string str3 = "";
  std::string str4 = "";
  std::string str5 = "";
  std::string line = "";
  std::string comment = qre_txt(26);
  std::string space = " ";
  std::string sq = "";
  std::string delim = "";
  std::string qr = "q";
  std::string cr = "c";
  
  long unsigned int rn = 0;
  long unsigned int rm = 0;
  long unsigned int ro = 0;  
  
  size_t pos = 0;
  size_t pos1 = 0;
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
  std::vector<double>c;
  //std::vector<double>r;
  std::vector<std::string>qasm_instructions;
  
  qre_show_v(p_base_verbosity, qre_txt(0));
  qasm_instructions = qre_parse_data_string(p_base_verbosity, p_base_data);
  vector_size = qasm_instructions.size();
  shots = qre_enable_shots(false, p_base_shots);

  /* We need to define qreg q and creg c before parsing anything else in the 
     QASM file. */
  for (i = 0; i < vector_size; i++)
    {
      if (qre_recog("qreg", qasm_instructions[i]) == true)
	{
	  rn = qre_parse_br(qasm_instructions[i], qr);
	}
    }
  qreg q(rn);
  qre_show_v(p_base_verbosity, qre_txt(11));
  rn = 0;
  for (i = 0; i < vector_size; i++)
    {
      if (qre_recog("creg", qasm_instructions[i]) == true)
	{
	  rn = qre_parse_br(qasm_instructions[i], cr);
	  if (c.size() == 0)
	    {
	      for (j = 0; j < (int)rn; j++)
		{
		  c.push_back(0.00);
		  //r.push_back(0.00);
		}
	    }
	}
    }
  qre_show_v(p_base_verbosity, qre_txt(12));
  
  /* Shots iteration. */
  for (k = 0; k < shots; k++)
    {
      /* File parsing iteration. */
      for (i = 0; i < vector_size; i++)
	{
	  /* Find if line will be ignored or not based on the value of delim. */
	  if ((qre_recog(comment, qasm_instructions[i]) == true)&&(qre_recog("// qdeclare qlib-simulator ", qasm_instructions[i]) == false))	  
	    {
	      qre_show_v(p_base_verbosity, (qre_txt(15) + qasm_instructions[i]));
	    }
	  /* Find if line will be ignored if it does not have spaces. */
	  else if (qre_recog(space, qasm_instructions[i]) == false)
	    {
	      qre_show_v(p_base_verbosity, (qre_txt(16) + qasm_instructions[i]));
	    }
	  else
	    {
	      qre_show_v(p_base_verbosity, (qre_txt(17) + qasm_instructions[i]));

	      /* Process quantum instructions. */
	      
	      /* If instructions are atomic (no parenthesis found). */
	      if (qre_recog("(", qasm_instructions[i]) == false)
		{	  
		  if (qre_recog("OPENQASM", qasm_instructions[i]) == true)
		    {
		      qre_find_qasm_standard(p_base_verbosity, qasm_instructions[i]);
		    }
		  if (qre_recog("include", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, " Library");
		    }	  
		  if (qre_recog("measure", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_br(qasm_instructions[i].substr(0), qr);
		      rm = qre_parse_br(qasm_instructions[i].substr(0), cr);
		      qre_show_v(p_base_verbosity, (qre_gaq("measure") + qre_l2s(rn) + qre_txt(18) + qre_l2s(rm)));
		    }
		  if (qre_recog("// qdeclare qlib-simulator ", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("qdeclare"));
		    }	  
		  if ((qre_recog("cx", qasm_instructions[i]) == true)&&(qre_recog("ccx", qasm_instructions[i]) == false))
		    {
		      pos = qasm_instructions[i].find(",");
		      rn = qre_parse_br(qasm_instructions[i].substr(0, pos-1), qr);
		      rm = qre_parse_br(qasm_instructions[i].substr(pos), qr);
		      q.apply(gates::CX, {rn,rm});
		      qre_show_v(p_base_verbosity, (qre_gaq("cx") + qre_l2s(rn) + qre_txt(21) + qre_l2s(rm)));
		    }
		  if (qre_recog("ccx", qasm_instructions[i]) == true)
		    {
		      pos = qasm_instructions[i].find(",");
		      rn = qre_parse_br(qasm_instructions[i].substr(0, pos-1), qr);
		      rm = qre_parse_br(qasm_instructions[i].substr(pos), qr);		      
		      str1 = qasm_instructions[i].substr(pos+1,(qasm_instructions[i].length()-1));
		      pos1 = str1.find(",");
		      ro = qre_parse_br(str1.substr(pos1), qr);
		      q.apply(gates::CCNOT, {rn,rm,ro});
		      qre_show_v(p_base_verbosity, (qre_gaq("ccx") + qre_l2s(rn) + qre_txt(23) + qre_l2s(rm) + qre_txt(21) + qre_l2s(ro)));		      		      
		    }		  
		  if (qre_recog("cy", qasm_instructions[i]) == true)
		    {
		      pos = qasm_instructions[i].find(",");
		      rn = qre_parse_br(qasm_instructions[i].substr(0, pos-1), qr);
		      rm = qre_parse_br(qasm_instructions[i].substr(pos), qr);		      
		      q.apply(gates::CY, {rn,rm});
		      qre_show_v(p_base_verbosity, (qre_gaq("cy") + qre_l2s(rn) + qre_txt(21) + qre_l2s(rm)));
		    }
		  if (qre_recog("cz", qasm_instructions[i]) == true)
		    {
		      pos = qasm_instructions[i].find(",");
		      rn = qre_parse_br(qasm_instructions[i].substr(0, pos-1), qr);
		      rm = qre_parse_br(qasm_instructions[i].substr(pos), qr);		      
		      q.apply(gates::CZ, {rn,rm});
		      qre_show_v(p_base_verbosity, (qre_gaq("cz") + qre_l2s(rn) + qre_txt(21) + qre_l2s(rm)));
		    }
		  if (qre_recog("ch", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("ch"));
		    }		  
		  if ((qre_recog("h ", qasm_instructions[i]) == true)&&(qre_recog("ch ", qasm_instructions[i]) == false))
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      q.apply(gates::H, {rn});
		      qre_show_v(p_base_verbosity, (qre_gaq("h") + qre_l2s(rn)));
		    }
		  if ((qre_recog("x ", qasm_instructions[i]) == true)&&(qre_recog("cx ", qasm_instructions[i]) == false))
		    {		  	     
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      q.apply(gates::X, {rn});
		      qre_show_v(p_base_verbosity, (qre_gaq("x") + qre_l2s(rn)));
		    }
		  if ((qre_recog("y ", qasm_instructions[i]) == true)&&(qre_recog("cy ", qasm_instructions[i]) == false))
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      q.apply(gates::Y, {rn});
		      qre_show_v(p_base_verbosity, (qre_gaq("y")+ qre_l2s(rn)));
		    }
		  if ((qre_recog("z ", qasm_instructions[i]) == true)&&(qre_recog("cz ", qasm_instructions[i]) == false))
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      q.apply(gates::Z, {rn});
		      qre_show_v(p_base_verbosity, (qre_gaq("z") + qre_l2s(rn)));
		    }
		  if (qre_recog("s ", qasm_instructions[i]) == true )
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      q.apply(gates::S, {rn});
		      qre_show_v(p_base_verbosity, (qre_gaq("s") + qre_l2s(rn)));
		    }
		  if (qre_recog("sdg", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      q.apply(gates::Sdg, {rn});		      
		      qre_show_v(p_base_verbosity, (qre_gaq("sdg") + qre_l2s(rn)));
		    }
		  if ((qre_recog("t ", qasm_instructions[i]) == true)&&(qre_recog("reset ", qasm_instructions[i]) == false))
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      q.apply(gates::T, {rn});
		      qre_show_v(p_base_verbosity, (qre_gaq("t") + qre_l2s(rn)));
		    }
		  if (qre_recog("tdg", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      q.apply(gates::Tdg, {rn});		      
		      qre_show_v(p_base_verbosity, (qre_gaq("tdg") + qre_l2s(rn)));
		    }
		  if (qre_recog("id ", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      q.apply(gates::I, {rn});
		      qre_show_v(p_base_verbosity, (qre_gaq("id") + qre_l2s(rn)));
		    }
		  if (qre_recog("barrier ", qasm_instructions[i]) == true )
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      qre_show_v(p_base_verbosity, (qre_gaq("barrier") + qre_l2s(rn)));
		    }
		  if (qre_recog("reset ", qasm_instructions[i]) == true )
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      q.apply(gates::reset, {rn});
		      qre_show_v(p_base_verbosity, (qre_gaq("reset") + qre_l2s(rn)));
		    }
		  if (qre_recog("swap ", qasm_instructions[i]) == true )
		    {
		      pos = qasm_instructions[i].find(",");
		      rn = qre_parse_br(qasm_instructions[i].substr(0, pos-1), qr);
		      rm = qre_parse_br(qasm_instructions[i].substr(pos), qr);		      
		      /* Implemented with three consecutive CNOT gates. */
		      q.apply(gates::CX, {rn,rm});
		      q.apply(gates::CX, {rm,rn});
		      q.apply(gates::CX, {rn,rm});	      
		      qre_show_v(p_base_verbosity, (qre_gaq("swap") + qre_l2s(rn) + qre_txt(21) + qre_l2s(rm)));
		    }
		  if (qre_recog("bloch", qasm_instructions[i]) == true )
		    {
		      qre_show_v(p_base_verbosity, qre_ina("bloch"));
		    }		  
		}

	      /* Instructions that have parenthesis are composites and require secial treatment. */
	      if (qre_recog("(", qasm_instructions[i]) == true)
		{
		  if ((qre_recog("u1", qasm_instructions[i]) == true)&&(qre_recog("cu1", qasm_instructions[i]) == false))
		    {
		      //rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      //qre_show_v(p_base_verbosity, (" u1 gate at qubit " + qre_d2s((double)rn)));
		      //vector<std::string> phg = qre_parse_phase_gate(qasm_instructions[i], ",");
		      //q.apply(gates::phase(qre_s2d(phg[0])), {rn});
		      qre_show_v(p_base_verbosity, qre_ina("u1"));
		    }
		  if (qre_recog("u2", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("u2"));
		    }
		  if ((qre_recog("u3", qasm_instructions[i]) == true)&&(qre_recog("cu3", qasm_instructions[i]) == false))
		    {
		      qre_show_v(p_base_verbosity, qre_ina("u3"));
		    }
		  if (qre_recog("if", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("if"));
		    }
		  if (qre_recog("rx", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("rx"));
		    }
		  if (qre_recog("ry", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("ry"));
		    }
		  if ((qre_recog("rz", qasm_instructions[i]) == true)&&(qre_recog("crz", qasm_instructions[i]) == false))
		    {
		      qre_show_v(p_base_verbosity, qre_ina("rz"));
		    }
		  if (qre_recog("crz", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("crz"));
		    }
		  if (qre_recog("cu1", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("cu1"));
		    }
		  if (qre_recog("cu3", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("cu3"));
		    }		  
		}	      
	    }
	}
      
      /* On first shot, create data set for results. */
      qre_show_v(p_base_verbosity, "\n");
      if (k == 0)
	{
	  delim = "> + ";
	  
	  /* Count number of terms on linear combination. */
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
	    - res_final contains the avg probabilities obtained from res_sum 
	      and shots.

	    Each element of these vectors and matrices will be updated on each 
	    shot.
	   */
	  for (i = 0; i < (int)terms; i++)
	    {
	      res_parc.push_back({"","",""});
	      res_sum.push_back(0);
	      res_shots.push_back(0);
	      res_final.push_back(0);
	    }	  
	}

      /* On each iteration, after res_parc has been created, parse res2 and 
	 distribute the parser data within the struct of each row.*/
      j = 0;
      pos = 0;
      res2 = q.toString();      
      delim = ">";
      qre_show_v(p_base_verbosity, "\n");
      while ((pos = res2.find(delim)) != std::string::npos)
	{
	  res_parc[j].sterm = res2.substr(0, pos);
	  str1 = res_parc[j].sterm;
	  res_parc[j].sket = str1.substr((str1.find("|"))+1);
	  res_parc[j].svec = str1.substr(0,(str1.find("|"))-1);
	  str1 = res_parc[j].svec;
	  res_parc[j].svec = str1.substr((str1.find("("))+1);
	  res2.erase(0, pos + delim.length());

	  // Cast numbers as two floats.
	  str3 = res_parc[j].svec;
	  str4 = str3.substr(0,str3.find(" + "));
	  str5 = str3.substr(str3.find(" + ")+3);
	  str5 = str5.substr(0,str5.find("i"));
	  svecx = atof(str4.c_str());
	  svecy = atof(str5.c_str());

	  /* Square both and sum to get p = x^2 + y^2, increment value of 
	     res_sum and show info. */
	  res_sum[j] = (double)(pow(svecx,2) + pow(svecy,2));
	  res_shots[j] = res_shots[j] + res_sum[j];
	  show_res_parc(p_base_verbosity,
			j,
			res_parc[j].sterm,
			res_parc[j].sket,
			res_parc[j].svec);
	  j++;
	}     
    }

  /* After all shots, average values, calculate probabilities and results. */
  for (i = 0; i < (int)res_final.size(); i++)
    {
      res_final[i] = res_shots[i] / shots;
      sprob = sprob + res_final[i];
    }  
  qre_show_sum_partial_probs(p_base_verbosity, sprob);

  /* Build the json string with results. */
  res = construct_res_step1("{", p_base_device, p_base_name);

  /* Put labels. */
  for (i = 0; i < (int)res_final.size(); i++)
  {
    if (i < ((int)res_final.size() - 1))
      {
	res = construct_res_step3(4, res);
      }    
    res = res + res_parc[i].sket;
    if (i < ((int)res_final.size() - 1))
      {
	res = construct_res_step3(5, res);
      }   
    if (i < ((int)res_final.size() - 2))
      {
	res = construct_res_step3(6, res);
      }   
  }

  /* Put qubit numbers and probability values. */
  int nq = c.size();
  res = construct_res_step3(2, qre_put_qubit_numbers (nq, construct_res_step3(7, res)));
  nq = (int)res_final.size();
  for (i = 0; i < nq; i++)
    {
      if (i < (nq - 1))
	{
	  res = res + qre_d2s(res_final[i]);
	}
      
      if (i < (nq - 2))
	{
	  res = construct_res_step3(8, res);
	}     
    }

  return(finish_res_and_save(p_base_verbosity,p_base_results_storage, p_base_name, res));
}


/* qx_post_experiment - Perform experiment locally on the qx simulator.

Arguments:
- p_base_verbosity: base_verbosity.
- p_base_data: base_data.
- p_base_seed: base_seed.
- p_base_shots: base_shots.
- p_base_name: base_name.
- p_base_device: local backend (qx simulator).
  - "qx_simulator": local simulator

Sources:
- TU Delft. (2019). Quantum Programming. [online] Available at: 
  https://www.tudelft.nl/eemcs/the-faculty/departments/quantum-computer-
  engineering/quantum-computer-architecture-lab/research/quantum-programming/ 
  [Accessed 21 Dec. 2019].

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
  int shots = 0;
  int vector_size = 0;
  int i = 0;
  int j = 0;
  int k = 0;
  int res1 = 0;
  int p0 = 0;
  int p1 = 0;
  int p2 = 0;
  int p3 = 0;  
  int p4 = 0;
  
  float svecx = 0;
  float svecy = 0;
  
  double sprob = 0;
  
  std::string res = "";
  std::string res2 = "";
  std::string res3 = "";
  std::string res4 = "";
  std::string res5 = "";
  std::string str1 = "";
  std::string str4 = "";
  std::string str5 = "";
  std::string line = "";
  std::string comment = qre_txt(26);
  std::string space = " ";
  std::string delim = "";
  std::string patha = qre_txt(27);
  std::string pathj = "";
  std::string sys_command = p_simulator_path;
  std::string qr = "q";
  std::string cr = "c";
  
  long unsigned int rn = 0;
  long unsigned int rm = 0;
  long unsigned int ro = 0;
  
  size_t pos = 0;
  size_t pos1 = 0;

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
  std::vector<double>c;
  //std::vector<double>r;
  std::vector<std::string>qasm_instructions;
  
  qre_show_v(p_base_verbosity, qre_txt(0));
  qasm_instructions = qre_parse_data_string(p_base_verbosity, p_base_data);
  vector_size = qasm_instructions.size();
  shots = qre_enable_shots(false, p_base_shots);
  pathj = patha + qre_txt(24);
  std::ofstream qc_file_ini(pathj);
  qc_file_ini << "# " << qre_txt(24) << endl;
  res1 = res1 + 0; // So that the compiler doesn't complain.
  
  /* We need to define qreg q and creg c before parsing anything else in the 
     QASM file. */
  for (i = 0; i < vector_size; i++)
    {
      if (qre_recog("qreg", qasm_instructions[i]) == true)
	{
	  rn = qre_parse_br(qasm_instructions[i], qr);
	  qc_file_ini << "qubits " << rn << endl;
	}
    }
  qre_show_v(p_base_verbosity, qre_txt(11));
  rn = 0;  
  for (i = 0; i < vector_size; i++)
    {
      if (qre_recog("creg", qasm_instructions[i]) == true)
	{
	  rn = qre_parse_br(qasm_instructions[i], cr);
	  if (c.size() == 0)
	    {
	      for (j = 0; j < (int)rn; j++)
		{
		  c.push_back(0.00);
		  //r.push_back(0.00);
		}
	    }
	}
    }
  qc_file_ini.close();
  qre_show_v(p_base_verbosity, qre_txt(12));
  
  /* Complile qc file from qasm file. */
  if (k == 0)
    {
      /* Reopen temp.qc, but to append stuff. */
      std::ofstream qc_file_app;
      qc_file_app.open(pathj, std::ios::app);
      
      /* File parsing iteration. */
      for (i = 0; i < vector_size; i++)
	{
	  /* Find if line will be ignored or not based on the value of delim. */
	  if ((qre_recog(comment, qasm_instructions[i]) == true)&&(qre_recog(qre_txt(13), qasm_instructions[i]) == false))
	    {
	      qre_show_v(p_base_verbosity, (qre_txt(15) + qasm_instructions[i]));
	    }
	  /* Find if line will be ignored if it does not have spaces. */
	  else if (qre_recog(space, qasm_instructions[i]) == false)
	    {
	      qre_show_v(p_base_verbosity, (qre_txt(16) + qasm_instructions[i]));
	    }
	  else
	    {
	      qre_show_v(p_base_verbosity, (qre_txt(17) + qasm_instructions[i]));

	      /* Process quantum instructions. */	  
	      
	      /* If instructions are atomic (no parenthesis found). */
	      if (qre_recog("(", qasm_instructions[i]) == false)
		{	  
		  if (qre_recog("OPENQASM", qasm_instructions[i]) == true)
		    {
		      qre_find_qasm_standard(p_base_verbosity, qasm_instructions[i]);
		    }
		  if (qre_recog("include", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, " Library");
		    }	  
		  if (qre_recog("measure", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_br(qasm_instructions[i].substr(0), qr);
		      rm = qre_parse_br(qasm_instructions[i].substr(0), cr);
		      qc_file_app << "measure q" << rm << endl;
		      qre_show_v(p_base_verbosity, (qre_gaq("measure") + qre_l2s(rn) + qre_txt(18) + qre_l2s(rm)));
		    }
		  if (qre_recog("// qdeclare qx-simulator ", qasm_instructions[i]) == true)
		    {
		      qc_file_app << qre_what_comes_after_s1("// qdeclare qx-simulator ", qasm_instructions[i]) << endl;
		    }
		  if ((qre_recog("cx", qasm_instructions[i]) == true)&&(qre_recog("ccx ", qasm_instructions[i]) == false))
		    {
		      pos = qasm_instructions[i].find(",");
		      rn = qre_parse_br(qasm_instructions[i].substr(0, pos-1), qr);
		      rm = qre_parse_br(qasm_instructions[i].substr(pos), qr);
		      qc_file_app << "cx q" << rn << ", q" << rm << endl;
		      qre_show_v(p_base_verbosity, (qre_gaq("cx") + qre_l2s(rn) + " to qubit " + qre_l2s(rm)));
		    }
		  if (qre_recog("ccx", qasm_instructions[i]) == true)
		    {
		      pos = qasm_instructions[i].find(",");
		      rn = qre_parse_br(qasm_instructions[i].substr(0, pos-1), qr);
		      rm = qre_parse_br(qasm_instructions[i].substr(pos), qr);		      
		      str1 = qasm_instructions[i].substr(pos+1,(qasm_instructions[i].length()-1));
		      pos1 = str1.find(",");
		      ro = qre_parse_br(str1.substr(pos1), qr);
		      qc_file_app << "toffoli q" << rn << ", q" << rm << ", q" << ro << endl;
		      qre_show_v(p_base_verbosity, (qre_gaq("ccx") + qre_l2s(rn) + qre_txt(23) + qre_l2s(rm) + qre_txt(21) + qre_l2s(ro)));		      		      
		    }		  
		  if (qre_recog("cy", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("cy"));
		    }
		  if (qre_recog("cz", qasm_instructions[i]) == true)
		    {
		      pos = qasm_instructions[i].find(",");
		      rn = qre_parse_br(qasm_instructions[i].substr(0, pos-1), qr);
		      rm = qre_parse_br(qasm_instructions[i].substr(pos), qr);		      
		      qc_file_app << "cz q" << rn << ", q" << rm << endl;
		      qre_show_v(p_base_verbosity, (qre_gaq("cz") + qre_l2s(rn) + qre_txt(21) + qre_l2s(rm)));
		    }
		  if ((qre_recog("ch", qasm_instructions[i]) == true)&&(qre_recog("qdeclare", qasm_instructions[i]) == false))
		    {
		      qre_show_v(p_base_verbosity, qre_ina("ch"));
		    }		  
		  if ((qre_recog("h ", qasm_instructions[i]) == true)&&(qre_recog("ch ", qasm_instructions[i]) == false))
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      qc_file_app << "h q" << rn << endl;
		      qre_show_v(p_base_verbosity, (qre_gaq("h") + qre_l2s(rn)));
		    }
		  if ((qre_recog("x ", qasm_instructions[i]) == true)&&(qre_recog("cx ", qasm_instructions[i]) == false))
		    {		  	     
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      qc_file_app << "x q" << rn << endl;
		      qre_show_v(p_base_verbosity, (qre_gaq("x") + qre_l2s(rn)));
		    }
		  if ((qre_recog("y ", qasm_instructions[i]) == true)&&(qre_recog("cy ", qasm_instructions[i]) == false))
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      qc_file_app << "y q" << rn << endl;
		      qre_show_v(p_base_verbosity, (qre_gaq("y") + qre_l2s(rn)));
		    }
		  if ((qre_recog("z ", qasm_instructions[i]) == true)&&(qre_recog("cz ", qasm_instructions[i]) == false))
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      qc_file_app << "z q" << rn << endl;
		      qre_show_v(p_base_verbosity, (qre_gaq("z") + qre_l2s(rn)));
		    }
		  if (qre_recog("s ", qasm_instructions[i]) == true )
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      qc_file_app << "s q" << rn << endl;
		      qre_show_v(p_base_verbosity, (qre_gaq("s") + qre_l2s(rn)));
		    }
		  if (qre_recog("sdg", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("sdg"));
		    }
		  if ((qre_recog("t ", qasm_instructions[i]) == true)&&(qre_recog("reset ", qasm_instructions[i]) == false))
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      qc_file_app << "t q" << rn << endl;
		      qre_show_v(p_base_verbosity, (qre_gaq("t") + qre_l2s(rn)));
		    }
		  if (qre_recog("tdg", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      qc_file_app << "tdag q" << rn << endl;
		      qre_show_v(p_base_verbosity, (qre_gaq("tdg") + qre_l2s(rn)));
		    }
		  if (qre_recog("id ", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("id"));
		    }
		  if (qre_recog("barrier ", qasm_instructions[i]) == true )
		    {
		      qre_show_v(p_base_verbosity, qre_ina("barrier"));
		    }
		  if (qre_recog("reset ", qasm_instructions[i]) == true )
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      qc_file_app << "prepz q" << rn << endl;
		      qre_show_v(p_base_verbosity, (qre_gaq("reset") + qre_l2s(rn)));
		    }
		  if (qre_recog("swap ", qasm_instructions[i]) == true )
		    {
		      pos = qasm_instructions[i].find(",");
		      rn = qre_parse_br(qasm_instructions[i].substr(0, pos-1), qr);
		      rm = qre_parse_br(qasm_instructions[i].substr(pos), qr);		      
		      qc_file_app << "swap q" << rn << ", q" << rm << endl;
		      qre_show_v(p_base_verbosity, (qre_gaq("swap") + qre_l2s(rn) + qre_txt(21) + qre_l2s(rm)));
		    }
		  if (qre_recog("bloch", qasm_instructions[i]) == true )
		    {
		      qre_show_v(p_base_verbosity, qre_ina("bloch"));
		    }		  
		}

	      /* Instructions that have parenthesis are composites and require 
		 special treatment. */
	      if (qre_recog("(", qasm_instructions[i]) == true)
		{		  
		  if ((qre_recog("u1", qasm_instructions[i]) == true)&&(qre_recog("cu1", qasm_instructions[i]) == false))
		    {
		      //rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		      //qre_show_v(p_base_verbosity, (" u1 gate at qubit " + qre_d2s((double)rn)));
		      //vector<std::string> phg = qre_parse_phase_gate(qasm_instructions[i], ",");
		      qre_show_v(p_base_verbosity, qre_ina("u1"));
		    }
		  if (qre_recog("u2", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("u2"));
		    }
		  if ((qre_recog("u3", qasm_instructions[i]) == true)&&(qre_recog("cu3", qasm_instructions[i]) == false))
		    {
		      qre_show_v(p_base_verbosity, qre_ina("u3"));
		    }
		  if (qre_recog("if", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("if"));
		    }
		  if (qre_recog("rx", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      qre_show_v(p_base_verbosity, (qre_gaq("rx") + qre_l2s(rn)));
		      vector<std::string> phg = qre_parse_phase_gate(qasm_instructions[i], ",");
		      qc_file_app << "rx q" << rn << ", " << phg[0] << endl;
		    }
		  if (qre_recog("ry", qasm_instructions[i]) == true)
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      qre_show_v(p_base_verbosity, (qre_gaq("ry") + qre_l2s(rn)));
		      vector<std::string> phg = qre_parse_phase_gate(qasm_instructions[i], ",");
		      qc_file_app << "ry q" << rn << ", " << phg[0] << endl;
		    }
		  if ((qre_recog("rz", qasm_instructions[i]) == true)&&(qre_recog("crz", qasm_instructions[i]) == false))
		    {
		      rn = qre_parse_br(qasm_instructions[i], qr);
		      qre_show_v(p_base_verbosity, (qre_gaq("rz") + qre_l2s(rn)));
		      vector<std::string> phg = qre_parse_phase_gate(qasm_instructions[i], ",");
		      qc_file_app << "rz q" << rn << ", " << phg[0] << endl;
		    }
		  if (qre_recog("crz", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("crz"));
		    }
		  if (qre_recog("cu1", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("cu1"));
		    }
		  if (qre_recog("cu3", qasm_instructions[i]) == true)
		    {
		      qre_show_v(p_base_verbosity, qre_ina("cu3"));
		    }		  
		}	      
	    }
	}
      qc_file_app << "display" << endl;
      qc_file_app.close();
    }
  
  /* Shots iteration. Here we call the simulatore, generate on each iteration a
     file with results, parse it to get the kets, extract the values of each ket
     as a complex number and then process those values using vectors.*/
  qre_show_v(p_base_verbosity, "\n");
  sys_command = sys_command + " " + patha + qre_txt(24) + " > " + patha + qre_txt(25);  
  for (k = 0; k < shots; k++)
    {
      /* Call qx simulator with created temp.qc file. */
      res1 = (system(sys_command.c_str()));
      
      /* Read qx_temp.txt on each shot iteration, parse kets and accummulate the
	 extracted values. */
      j = 0;     
      std::ifstream qx_temp(patha + qre_txt(25));
      qre_show_v(p_base_verbosity, "\n");
      while (std::getline(qx_temp, res2))
	{
	  /* If the line contains ket data. */
	  if ((qre_recog("> +", res2) == true) && (qre_recog(") |", res2) == true))
	    {
	      /*
		This updates and grows dynamically a string matrix and two 
		vectors:
		- res_parc contains strings parsed from res2.
		- res_sum contains the sum of all values corresponding to each 
		  ket.
		- res_final contains the avg probabilities obtained from 
		  res_sum and shots.

		Each element of these vectors and matrices will be updated on 
		each shot.
	      */
	      if (k == 0)
		{
		  res_parc.push_back({"","",""});
		  res_sum.push_back(0);
		  res_shots.push_back(0);
		  res_final.push_back(0);
		}
	      
	      /* On each iteration, after res_parc has been created, parse res2 
		 and distribute the parsed data within the struct of each row.*/

	      /* Trim string and extract real, imaginary parts and ket qbyte. */
	      p0 = res2.find("(");
	      res3 = res2.substr(p0+1);	      
	      p0 = res3.find("(");
	      p1 = res3.find(",");
	      res4 = res3.substr(p1+1);	      
              p2 = res4.find(")");
              p3 = res4.find("|");	      
	      res5 = res4.substr(p3+1);
              p4 = res5.find(">");
	      res_parc[j].sterm = res3.substr(0, p1);
	      res_parc[j].svec = res4.substr(0, p2);	      
	      res_parc[j].sket = res5.substr(0, p4);	      
	      
	      /* Cast numbers as two floats. */
	      str4 = res_parc[j].sterm;
	      str5 = res_parc[j].svec;
	      svecx = atof(str4.c_str());
	      svecy = atof(str5.c_str());

	      /* Square both and sum to get p = x^2 + y^2, increment res_su and 
		 show some info .*/
	      res_sum[j] = (double)(pow(svecx,2) + pow(svecy,2));
	      res_shots[j] = res_shots[j] + res_sum[j];
	      show_res_parc(p_base_verbosity, j, res_parc[j].sterm, res_parc[j].sket, res_parc[j].svec);
	      j++;
	    }
	}      
    }
  
  /* After performing all shots, average values, calculate probabilities. */
  for (i = 0; i < (int)res_final.size(); i++)
    {
      res_final[i] = res_shots[i] / shots;
      sprob = sprob + res_final[i];
    } 
  qre_show_sum_partial_probs(p_base_verbosity, sprob);
  
  /* Build the json string with results. */
  res = construct_res_step1("{", p_base_device, p_base_name);  

  /* Put labels. */
  for (i = 0; i < (int)res_final.size(); i++)
    {
      if (i < ((int)res_final.size() ))
	{
	  res = construct_res_step3(4, res);
	}
      res = res + res_parc[i].sket;
      if (i < ((int)res_final.size() - 0))
	{
	  res = construct_res_step3(5, res);
	}    
      if (i < ((int)res_final.size() - 1))
	{
	  res = construct_res_step3(6, res);
	}  
    }
  
  /* Put qubit numbers and probability values. */
  int nq = c.size();
  res = construct_res_step3(2, qre_put_qubit_numbers (nq, construct_res_step3(7, res)));
  nq = (int)res_final.size();
  for (i = 0; i < nq; i++)
    {
      if (i < nq)
	{
	  res = res + qre_d2s(res_final[i]);
	} 
      if (i < (nq - 1))
	{
	  res = construct_res_step3(8, res);
	}     
    }

  return(finish_res_and_save(p_base_verbosity, p_base_results_storage, p_base_name, res));  
}


