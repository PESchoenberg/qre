/* =============================================================================

qre0.cpp

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

================================================================================

Sources:
- https://developer.ibm.com/tutorials/os-quantum-computing-shell-game/
- https://quantumcomputing.stackexchange.com/questions/5173/submitting-jobs-to-ibm-devices-without-python
- https://stackoverflow.com/questions/51317221/how-to-use-libcurl-in-c-to-send-a-post-request-and-receive-it
- https://github.com/nanowebcoder/NanoQuantumShellGame/blob/master/NanoQuantum.Core/QProcessor.cs
- https://curl.haxx.se/libcurl/c/libcurl-tutorial.html
- https://github.com/XanaduAI/pennylane-pq
- https://arxiv.org/pdf/1710.01794.pdf
- https://arxiv.org/pdf/1805.09607.pdf
- https://arxiv.org/pdf/1808.02449.pdf
- ftp://ftp.cs.princeton.edu/techreports/2012/934.pdf
- http://docs.rigetti.com/en/stable/basics.html

* Compilation (using the gcc compiler family) on Linux:
- See README.md

* Testing:
- See README.md

============================================================================= */


#include "qre0.hpp"

using namespace std;

/* Main program.*/
int main(int argc, char** argv)
{
  int res1 = 0;
  
  std::string res = "";
  std::string res2 = qre_txt(28);
  std::string data = "";
  std::string line = "---------------------------------------------------------------\n";
  std::string cfg_file = "qre.cfg";
  std::string base_file = "";
  std::string base_data = "";
  std::string base_token = "";
  std::string base_uri = "";
  std::string base_results_storage = "";
  std::string base_method = "";
  std::string base_verbosity =  qre_txt(29);
  std::string base_shots = "";
  std::string base_max_credits = "";
  std::string base_device = "";
  std::string base_seed = "1";
  std::string base_name = "";
  std::string login_data = "";
  std::string login_uri = "";
  std::string login_id = "";
  std::string login_name = "";
  std::string login_content_type ="";
  std::string post_data = "";
  std::string post_content_type = "";
  std::string post_uri = "";
  std::string post_name = "";
  std::string get_content_type = "";
  std::string get_uri = "";
  std::string get_data = "";
  std::string get_name = "";
  std::string delete_content_type = "";
  std::string delete_uri = "";
  std::string delete_data = "";
  std::string delete_name = "";
  std::string request_log = "";
  std::string qx_simulator_path = "";
  
  std::vector<std::string>response_login;
  
  // Check number of arguments program_file, comm_file, 
  if (argc == 7)
    {
      // Name of the qasm file to run.
      base_file = argv[1];

      // http method to use (get, post, etc.)
      base_method = argv[2];

      // y: show request data, m: show nothing.
      base_verbosity = argv[3];

      /* Backend to use. Write test if you want to just see the contents of the 
	 qreg file, simulator for a simulator backend, or the name of the 
	 quantum processor to use in case of a real run. */
      base_device = argv[4];

      // Random seed number to use, int >= 1.
      base_seed = argv[5];

      // Name of the experiment to run (choose your own).
      base_name = argv[6];
      
      // Verbosity parameter.
      if((base_verbosity == "y")||(base_verbosity == "Y"))
	{
	  base_verbosity = "yes";
	}
      else if((base_verbosity != "yes")||(base_verbosity == "N"))
	{
	  base_verbosity = "no";
	}
      else if(base_method == "test")
	{
	  base_verbosity = "yes";
	}
      else
	{
	  base_verbosity = "no";
	}
      
      // Read parameters from file.      
      base_data = qre_read_qasm_file(base_file);      
      base_token = qre_seek_in_file(cfg_file, "base-token");
      base_uri = qre_seek_in_file(cfg_file, "base-uri");
      base_results_storage = qre_seek_in_file(cfg_file, "base-results-storage");
      base_shots = qre_seek_in_file(cfg_file, "base-shots");
      base_max_credits = qre_seek_in_file(cfg_file, "base-max-credits");
      login_data = qre_seek_in_file(cfg_file, "login-data")+base_token;
      login_uri = base_uri + qre_seek_in_file(cfg_file, "login-uri");
      login_content_type = login_content_type + qre_seek_in_file(cfg_file, "login-content-type");
      post_content_type = post_content_type + qre_seek_in_file(cfg_file, "post-content-type");
      post_uri = base_uri + qre_seek_in_file(cfg_file, "post-uri");
      get_content_type = get_content_type + qre_seek_in_file(cfg_file, "get-content-type");
      get_uri = base_uri + qre_seek_in_file(cfg_file, "get-uri");
      delete_content_type = delete_content_type + qre_seek_in_file(cfg_file, "delete-content-type");
      delete_uri = base_uri + qre_seek_in_file(cfg_file, "delete-uri");
      qx_simulator_path = qre_seek_in_file(cfg_file, "qx-simulator-path");
      
      // Define some names.
      login_name = base_name + "_login";
      post_name = base_name + "_post";
      get_name = base_name + "_get";
      delete_name = base_name + "_delete";     
      if (base_method == "test")
	{
	  base_verbosity = "yes";
	  qre_show_conclusion(base_verbosity, line, "Testing...");
	  qre_show_string("Arguments entered from the command line:");
	  qre_show_var("File", base_file);
	  qre_show_var("Method", base_method);
	  qre_show_var("Verbosity", base_verbosity);
	  qre_show_var("Device", base_device);
	  qre_show_var("Seed", base_seed);
	  qre_show_var("Name", base_name);	  
	  qre_show_conclusion(base_verbosity, line, "Configuration values:");
	  qre_show_var("base-file", base_file);
	  qre_show_var("base-data", base_data);
	  qre_show_var("base-token", base_token);
	  qre_show_var("base-uri", base_uri);
	  qre_show_var("base-results-storage", base_results_storage);
	  qre_show_var("base-shots", base_shots);
	  qre_show_var("base-max-credits", base_max_credits);
	  qre_show_var("login-data", login_data);
	  qre_show_var("login-uri", login_uri);
	  qre_show_var("login-content-type", login_content_type);
	  qre_show_var("post-content-type", post_content_type);
	  qre_show_var("post-uri", post_uri);
	  qre_show_var("get-content-type", get_content_type);
	  qre_show_var("get-uri", get_uri);
	  qre_show_var("delete-content-type", delete_content_type);
	  qre_show_var("delete-uri", delete_uri);
	  qre_show_var("qx-simulator-path", qx_simulator_path);
	  qre_show_conclusion(base_verbosity, "Test finished", "\n");
	}
      else if (base_device == "qlib_simulator")
	{
	  if(base_method == "post")
	    {
	      res = qlib_post_experiment(base_verbosity,
					 base_data,
					 base_results_storage,
					 base_seed,
					 base_shots,
					 base_name,
					 base_device);	      
	      qre_show_conclusion(base_verbosity, "Post result: \n\n", res);
	    }
	  else
	    {
	      qre_show_conclusion(base_verbosity, "qre qlib_simulator only supports test and post requests.\n\n", " ");
	    }
	}
      else if (base_device == "qx_simulator")
	{
	  if (base_method == "post")
	    {
	      res = qx_post_experiment(base_verbosity,
				       base_data,
				       base_results_storage,
				       base_seed,
				       base_shots,
				       base_name,
				       base_device,
				       qx_simulator_path);	      
	      qre_show_conclusion(base_verbosity, "Post result: \n\n", res);
	    }
	  else
	    {
	      qre_show_conclusion(base_verbosity, "qre qx_simulator only supports test and post requests.\n\n", " ");
	    }
	}       
      else if ((base_device == "ibmqx_simulator")||(base_device == "ibmqx_real_qpu"))
	{
	  qre_show_string(line);

	  // Login first.
	  response_login=ibmqx_login(base_verbosity,
				     base_method,
				     login_data,
				     login_content_type,
				     base_results_storage,
				     login_uri,
				     login_name);	  
	  login_id = response_login[3];	  
	  if (login_id == "na")
	    {
	      qre_show_string("LOGIN FAILED");
	    }
	  else
	    {
	      qre_show_string("LOGIN OK");	  
	      if (base_method == "post")
		{
		  res = ibmqx_post_experiment(base_verbosity,
					      base_method,
					      post_data,
					      post_content_type,
					      base_results_storage,
					      post_uri,
					      post_name,
					      login_id,
					      base_name,
					      base_data,
					      base_shots,
					      base_seed,
					      base_device);
		  qre_show_conclusion(base_verbosity, "Post result:\n\n", res);
		}
	      if (base_method == "get")
		{
		  // Placeholder.
		}
	      if (base_method == "delete")
		{
		  res = ibmqx_delete_experiment(base_verbosity,
						base_method,
						delete_data,
						delete_content_type,
						base_results_storage,
						delete_uri,
						delete_name,
						login_id,
						base_name);
		  qre_show_conclusion(base_verbosity, "Delete result:\n\n", res);	      
		}
	    }	  
	}
      else
	{
	  res1 = 1;
	  qre_show_string("Incorrect method or base device.");
	}
    }
  else
    {
      res1 = 1;
      qre_show_string("Incorrect input.");
    }
  qre_show_conclusion(base_verbosity, line, "\n");
  
  return res1;
}


