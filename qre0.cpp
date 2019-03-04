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
- https://stackoverflow.com/questions/51317221/how-to-use-libcurl-in-c-to-send-a-post-request-and-receive-it

* Compilation (using the gcc compiler family) on Linux:

- g++ -std=c++17 -Wall -O3 -c qre0.cpp qre1.cpp 
- g++ -std=c++17 -Wall -O3 qre0.o qre1.o -o qre -lcurl

* Testing:
- You should update the api token on the provided example1.qreg file and
use your own:
  - Open examples/example1.qreg
  - Replace the string "your-token-goes-here" wherever it appears on the file
  with your new token.
  - Save the file as it is, without changin anything else.
  - See the included README.md file for further instructions on how
  to get a token on the IBM QX website.
- On your console, type 
  ./qre examples/example1.qreg test y simulator 1 example1.1 <ENT>
- The example file provided is precompiled using g2q. If you want to test run
your own programs, you will have to get a copy of g2q and write your programs
in Scheme (Lisp) or get  or develop your own compatible compiler.
============================================================================= */


#include "qre0.hpp"
#include "qre1.hpp"

using namespace std;

int main(int argc, char** argv)
{
  int res1 = 0;
  string res = "";
  string res2 = "na";
  string data = "";
  string line = "---------------------------------------------------------------\n";
  string base_file = "";
  string base_data = "";
  string base_token = "";
  string base_uri = "";
  string base_results_storage = "";
  string base_method = "";
  string base_verbosity = "no";
  string base_shots = "";
  string base_max_credits = "";
  string base_device = "";
  string base_seed = "1";
  string base_name = "";
  string login_data = "";
  string login_uri = "";
  string login_id = "";
  string post_data = "";
  string post_content_type = "";
  string post_uri = "";
  string get_content_type = "";
  string get_uri = "";
  string delete_content_type = "";
  string delete_uri = "";
  string request_log = "";
  
  //Check number of arguments program_file, comm_file, 
  if (argc == 7)
    {
      base_file = argv[1];
      base_method = argv[2];
      base_verbosity = argv[3];
      base_device = argv[4];
      base_seed = argv[5];
      base_name = argv[6];
      
      //Some checks on input parameters.

      //Check verbosity parameter.
      if(base_verbosity == "y")
	{
	  base_verbosity = "yes";
	}
      
      if(base_verbosity != "yes")
	{
	  base_verbosity = "no";
	}
      
      show_string(line);
      show_string("Argumens entered from the command line:");
      show_var("File", base_file);
      show_var("Method", base_method);
      show_var("Verbosity", base_verbosity);
      show_var("Device", base_device);
      show_var("Seed", base_seed);
      show_var("Name", base_name);
      
      //Read parameters from file.      
      base_data = seek_in_file(base_file, "base-data");
      base_token = seek_in_file(base_file, "base-token");
      base_uri = seek_in_file(base_file, "base-uri");
      base_results_storage = seek_in_file(base_file, "base-results-storage");
      base_shots = seek_in_file(base_file, "base-shots");
      base_max_credits = seek_in_file(base_file, "base-max-credits");
      login_data = seek_in_file(base_file, "login-data");
      login_uri = seek_in_file(base_file, "login-uri");
      login_id = seek_in_file(base_file, "login-id");
      post_content_type = seek_in_file(base_file, "post-content-type");
      post_uri = seek_in_file(base_file, "post-uri");
      get_content_type = seek_in_file(base_file, "get-content-type");
      get_uri = seek_in_file(base_file, "get-uri");
      delete_content_type = seek_in_file(base_file, "delete-content-type");
      delete_uri = seek_in_file(base_file, "delete-uri");
           
      // Show parameters if verbosity is set to yes.
      if(base_verbosity == "yes")
	{
	  show_string(line);
	  show_string("Values from .qreg file:");	  
	  show_var("base-file", base_file);
	  show_var("base-data", base_data);
	  show_var("base-token", base_token);
	  show_var("base-uri", base_uri);
	  show_var("base-results-storage", base_results_storage);
	  show_var("base-shots", base_shots);
	  show_var("base-max-credits", base_max_credits);
	  show_var("login-data", login_data);
	  show_var("login-uri", login_uri);
	  show_var("login-id", login_id);
	  show_var("post-content-type", post_content_type);
	  show_var("post-uri", post_uri);
	  show_var("get-content-type", get_content_type);
	  show_var("get-uri", get_uri);
	  show_var("delete-content-type", delete_content_type);
	  show_var("delete-uri", delete_uri);	  
	}

      if(base_method == "test")
	{
	  //Nothing more to do.
	  cout << "\n Test finished.";
	}
      else
	{
	  show_string(line);
	  show_string("Requesting, stand by...");
	  if(base_method == "post")
	    {
	      // Login.	      
	      res = qpost(login_data, base_token, post_content_type, base_results_storage, login_uri);
	      cout << "Login: \n" << res << "\n" << line << endl;

	      // Get login id if login was correct.
	      login_id = seek_in_json(res, "\"userId\"");
	      cout << "\n login_id = " << login_id << endl;

	      // If we get a correct id, then proceed.
	      // https://github.com/nanowebcoder/NanoQuantumShellGame/blob/master/NanoQuantum.Core/QProcessor.cs
	      // ./qre example1.qreg post y simulator 100 example1_1

	      if (login_id != "na")
		{
		  show_string(line);
		  post_data = "qasm="+base_data+"&codeType="+"QASM2"+"&name="+base_name;
		  post_uri = base_uri+"codes/execute?shots="+base_shots+"&seed="+base_seed+"&deviceRunType="+base_device+"&access_token="+login_id;
		  //post_data = "qasm=\""+base_data+"\"&codeType=\"QASM2\"&name=\""+base_name+"\"";
		  //post_uri = base_uri+"codes/execute?shots="+base_shots+"&seed="+base_seed+"\"&deviceRunType=\""+base_device+"\"&access_token=\""+login_id+"\"";		  
		  cout << "\n post_data = " << post_data << endl;
		  cout << "\n post_content_type = " << post_content_type << endl;
		  cout << "\n base_results_storage = " << base_results_storage << endl;
		  cout << "\n post_uri = " << post_uri << endl;
		  
		  res = qpost(post_data, base_token, post_content_type, base_results_storage, post_uri);
		  cout << "Post: \n" << res << "\n" << line << endl;
		}
	      
	      
	    }
	  if(base_method == "get")
	    {
	      
	    }
	  if(base_method == "delete")
	    {
	      
	    }
	  
	}
    }
  else
    {
      res1 = 1;
      cout << "Incorrect input.";
    }
  cout << "\n\n";
  
  return res1;
}







