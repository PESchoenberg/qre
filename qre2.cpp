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

using namespace std;


/* qre_parse_qins1 - Extracts the qubit number from a QASM instruction expressed
as astring, and the instruction to be executed.

Arguments:
- p_string: string.

Output:
- An vector containing:
  - Gate name.
  - qubit number.

 */
std::vector<std::string> qre_parse_qins1(std::string p_string)
{

}

/* qre_parse_data_string - parses base_data strings and puts fractions of the
original string parsed using ";" as delimiter on each vector cell.

Arguments:
- p_base_verbosity: base_verbosity.
- p_base_data: base_data.

Output:
- A string vector containing each QASM line on each vector cell.

Sources:
- https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c

 */
std::vector<std::string> qre_parse_data_string(std::string p_base_verbosity, std::string p_base_data)
{
  std::vector<std::string> res;
  
  std::string base_data = p_base_data;
  std::string delim = ";";
  size_t pos = 0;
  std::string line;

  qre_show_v(p_base_verbosity, ("Parsing base_data..."));
  
  while ((pos = base_data.find(delim)) != std::string::npos)
  {
    line = base_data.substr(0, pos);
    res.push_back(line);
    base_data.erase(0, pos + delim.length());
  }

  return res;
}


/* qre_post_experiment - perform experiment locally on p_base_device.

Arguments:
- p_base_verbosity: base_verbosity.
- p_base_data: base_data.
- p_base_seed: base_seed.
- p_base_shots: base_shots.
- p_base_name: base_name.
- p_base_device: local backend (simulator, for now ;) ).
  - "simulator": local simulator

 */
std::string qre_post_experiment(std::string p_base_verbosity,
				std::string p_base_data,
				std::string p_base_seed,
				std::string p_base_shots,
				std::string p_base_name,
				std::string p_base_device)
{
  std::vector<std::string>qasm_instructions;
  std::vector<double>q;
  std::vector<double>c;
  std::string res = "";
  std::string delim = "//";
  
  qasm_instructions = qre_parse_data_string(p_base_verbosity, p_base_data);
  int vector_size = qasm_instructions.size();
  for(int i = 0; i < vector_size; i++)
    {
      // Find if line will be ingored or not based on the value of delim.
      //if (qasm_instructions[i].find(delim) != std::string::npos)
      if (qre_recog(delim, qasm_instructions[i]) == true )
	{
	  qre_show_v(p_base_verbosity, ("Ignore -> " + qasm_instructions[i]));
	}
      else
	{
	  qre_show_v(p_base_verbosity, ("Post   -> " + qasm_instructions[i]));

	  // Process quantum instructions.

	  // OPENQASM
	  if (qre_recog("OPENQASM", qasm_instructions[i]) == true )
	    {
	      // Do nothing in this case
	    }

	  // include
	  if (qre_recog("include", qasm_instructions[i]) == true )
	    {
	      // Do nothing in this case
	    }	  

	  // qreg
	  if (qre_recog("qreg", qasm_instructions[i]) == true )
	    {

	    }

	  // creg
	  if (qre_recog("creg", qasm_instructions[i]) == true )
	    {

	    }	  
	  
	}
    }
  
  return res;
}

