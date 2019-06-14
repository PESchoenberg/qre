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
#include <algorithm>
#include <curl/curl.h>

using namespace std;


std::string qre_txt(int p_n);


int qre_enable_shots(bool p_enable, std::string p_base_shots);


std::string qre_gaq(std::string p_s1);


std::string qre_ina(std::string p_s1);


std::string qre_d2s(double p_double);


double qre_s2d(std::string p_s);


int qre_count_string(std::string p_delim, std::string p_s);


bool qre_recog(std::string p_s1, std:: string p_s2);


void qre_show_v(std::string p_base_verbosity, std::string p_s);


void qre_show_sum_partial_probs(std::string p_base_verbosity, double p_sprob);


std::string qre_seek_in_file(std::string p_f, std::string p_v);


std::string qre_seek_in_json(std::string p_j, std::string p_v);


void qre_show_var(std::string p_t, std::string p_v);


void qre_show_string(std::string p_t);


void qre_show_conclusion(std::string p_base_verbosity, std::string p_t, std::string p_r);


std::string qre_url_encode(std::string p_s);


char *qre_create_header(std::string p_s);


std::string qre_read_qasm_file(std::string p_f);


void qre_store_results(std::string p_base_verbosity,
		       std::string p_base_results_storage,
		       std::string p_file,
		       std::string p_contents_to_store);


int qre_parse_bitnum(std::string p_s);


std::string qre_parse_reg(std::string p_s, std::string p_type);


std::vector<std::string> qre_parse_data_string(std::string p_base_verbosity, std::string p_base_data);


std::string qre_what_comes_after_s1(std::string p_s1, std::string p_s2);


std::vector<std::string> qre_parse_phase_gate(std::string p_s, std::string p_delim);


long unsigned int qre_parse_br(std::string p_s, std::string p_qr);


#endif // !__QRE1_H
