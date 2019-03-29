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

std::vector<std::string> qre_parse_qinst1(std::strng p_string);

std::vector<std::string> qre_parse_data_string(std::string p_base_verbosity, std::string p_base_data);

std::string qre_post_experiment(std::string p_base_verbosity,
				std::string p_base_data,
				std::string p_base_seed,
				std::string p_base_shots,
				std::string p_base_name,
				std::string p_base_device);

#endif // !__QRE2_H
