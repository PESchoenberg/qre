/* =============================================================================

qre2.hpp

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


#ifndef __QRE2_H
#define __QRE2_H


#include "qre1.hpp"


void show_res_parc(std::string p_base_verbosity, int p_j, std::string p_st, std::string p_sk, std::string p_sv);


std::string construct_res_step1(std::string p_res, std::string p_base_device, std::string p_base_name);


std::string construct_res_step3(int p_step, std::string p_res);


std::string ibmqx_qpost(std::string p_base_verbosity,
			std::string p_base_method,
			std::string p_base_data,
			std::string p_post_content_type,
			std::string p_base_results_storage,
			std::string p_uri,
			std::string p_login_id);


std::vector<std::string> ibmqx_login(std::string p_base_verbosity,
				     std::string p_base_method,
				     std::string p_login_data,
				     std::string p_post_content_type,
				     std::string p_base_results_storage,
				     std::string p_login_uri,
				     std::string p_login_name);


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
				  std::string p_base_device);


std::string ibmqx_delete_experiment(std::string p_base_verbosity,
				    std::string p_base_method,
				    std::string p_delete_data,
				    std::string p_delete_content_type,
				    std::string p_base_results_storage,			    
				    std::string p_delete_uri,
				    std::string p_delete_name,
				    std::string p_login_id,
				    std::string p_base_name);


void qre_find_qasm_standard(std::string p_base_verbosity, std::string p_qasm_instructions);


std::string qre_put_qubit_numbers(int p_nq, std::string p_res);


std::string finish_res_and_save(std::string p_base_verbosity,
				std::string p_base_results_storage,
				std::string p_base_name,
				std::string p_res);


std::string qlib_post_experiment(std::string p_base_verbosity,
				 std::string p_base_data,
				 std::string p_base_results_storage,
				 std::string p_base_seed,
				 std::string p_base_shots,
				 std::string p_base_name,
				 std::string p_base_device);


std::string qx_post_experiment(std::string p_base_verbosity,
			       std::string p_base_data,
			       std::string p_base_results_storage,
			       std::string p_base_seed,
			       std::string p_base_shots,
			       std::string p_base_name,
			       std::string p_base_device,
			       std::string p_simulator_path);


#endif // !__QRE2_H
