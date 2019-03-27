/* =============================================================================

qre1.hpp

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

#ifndef __QRE1_H
#define __QRE1_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cstring>
#include <vector>
#include <curl/curl.h>

using namespace std;


string seek_in_file(string p_f, string p_v);

string seek_in_json(string p_j, string p_v);

void show_var(string p_t, string p_v);

void show_string(string p_t);

string url_encode(string p_s);

char *create_header(string p_s);

string qpost(string p_base_verbosity,
	     string p_base_method,
	     string p_base_data,
	     string p_post_content_type,
	     string p_base_results_storage,
	     string p_uri,
	     string p_login_id);

string read_qasm_file(string p_f);

void store_results(string p_base_results_storage, string p_file, string p_contents_to_store);

std::vector<std::string> qx_login(string p_base_verbosity,
		string p_base_method,
		string p_login_data,
		string p_post_content_type,
		string p_base_results_storage,
		string p_login_uri,
		string p_login_name);

string qx_delete_experiment(string p_base_verbosity,
			    string p_base_method,
			    string p_delete_data,
			    string p_delete_content_type,
			    string p_base_results_storage,			    
			    string p_delete_uri,
			    string p_delete_name,
			    string p_login_id,
			    string p_base_name);

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
			  string p_base_device);

#endif // !__QRE1_H
