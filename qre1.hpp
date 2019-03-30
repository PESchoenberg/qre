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

bool qre_recog(std::string p_string1, std:: string p_string2);

void qre_show_v(std::string p_base_verbosity, std::string p_string);

std::string seek_in_file(std::string p_f, std::string p_v);

std::string seek_in_json(std::string p_j, std::string p_v);

void show_var(std::string p_t, std::string p_v);

void show_string(std::string p_t);

std::string url_encode(std::string p_s);

char *create_header(std::string p_s);

std::string qpost(std::string p_base_verbosity,
		  std::string p_base_method,
		  std::string p_base_data,
		  std::string p_post_content_type,
		  std::string p_base_results_storage,
		  std::string p_uri,
		  std::string p_login_id);

std::string read_qasm_file(std::string p_f);

void store_results(std::string p_base_results_storage,
		   std::string p_file,
		   std::string p_contents_to_store);

std::vector<std::string> qx_login(std::string p_base_verbosity,
				  std::string p_base_method,
				  std::string p_login_data,
				  std::string p_post_content_type,
				  std::string p_base_results_storage,
				  std::string p_login_uri,
				  std::string p_login_name);

std::string qx_delete_experiment(std::string p_base_verbosity,
				 std::string p_base_method,
				 std::string p_delete_data,
				 std::string p_delete_content_type,
				 std::string p_base_results_storage,			    
				 std::string p_delete_uri,
				 std::string p_delete_name,
				 std::string p_login_id,
				 std::string p_base_name);

std::string qx_post_experiment(std::string p_base_verbosity,
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
			       std::string p_base_device);

#endif // !__QRE1_H
