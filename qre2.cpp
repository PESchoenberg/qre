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


/* qre_parse_bitnum - Extracts the bit number from a QASM instruction expressed
as an integer, and the instruction to be executed.

Arguments:
- p_string: string.

Output:
- qubit or bit number.

 */
int qre_parse_bitnum(std::string p_string)
{
  int res = 0;
  std::string snum = "";

  snum = p_string.substr(2,(p_string.length()-1));
  stringstream conv(snum);
  conv >> res;
  
  return res;
}


/* qre_parse_qreg - extracts quantum register substring.

Arguments:
- p_string: string from which to extract a register substring.
- p_type:
  - "q".
  - "c".

Output:
- A substring like "q[n]" or "c[n]" being n a qubit or bit number.

 */
std::string qre_parse_reg(std::string p_string, std::string p_type)
{
  std::string res = "//";
  std::string reg = p_type;
  
  reg = reg +"[";
  if (qre_recog(reg, p_string) == true)
    {
      size_t pos1 = p_string.find(reg, 0);
      size_t pos2 = p_string.find("]", pos1);
      res = p_string.substr(pos1,pos2);
    }
  
  return res;
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
  qreg q(1);
  std::vector<double>c;
  std::string res = "";
  std::string line = "";
  std::string comment = "//";
  std::string space = " ";

  long unsigned int rn = 0;
  
  qasm_instructions = qre_parse_data_string(p_base_verbosity, p_base_data);
  int vector_size = qasm_instructions.size();
  for(int i = 0; i < vector_size; i++)
    {
      // Find if line will be ingored or not based on the value of delim.
      if (qre_recog(comment, qasm_instructions[i]) == true)
	{
	  qre_show_v(p_base_verbosity, ("Ignore, comment -> " + qasm_instructions[i]));
	}
      // Find if line will be ingored if it doesn not have spaces.
      else if (qre_recog(space, qasm_instructions[i]) == false)
	{
	  qre_show_v(p_base_verbosity, ("Ignore, error -> " + qasm_instructions[i]));
	}
      else
	{
	  qre_show_v(p_base_verbosity, ("Post   -> " + qasm_instructions[i]));

	  // Process quantum instructions.

	  // ( Instructions that have parenthesis are composites and require secial treatment.
	  if (qre_recog("(", qasm_instructions[i]) == true)
	    {

	    }
	  // If instructions are atomic...
	  else
	    {	  
	      // OPENQASM
	      if (qre_recog("OPENQASM", qasm_instructions[i]) == true)
		{
		  // Do nothing in this case
		  if (qre_recog("2.", qasm_instructions[i]) == true)
		    {
		      cout << " Recognized as OPENQASM 2.*." << endl;
		    }
		  else
		    {
		      cout << " Undefined OPENQASM version." << endl;
		    }
		}

	      // include
	      if (qre_recog("include", qasm_instructions[i]) == true)
		{
		  cout << " Library." << endl;
		}	  

	      // qreg
	      if (qre_recog("qreg", qasm_instructions[i]) == true)
		{
		  rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		  qreg q(rn);
		  cout << " Created qubit register q[" << rn << "]." << endl;
		}

	      // creg
	      if (qre_recog("creg", qasm_instructions[i]) == true)
		{
		  rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "c"));	      
		  for(int j = 0; j < (int)rn; j++)
		    {
		      c.push_back(0.00);
		    }
		  cout << " Created bit register c[" << rn << "]." << endl;
		}	  

	      // h gate.
	      if (qre_recog("h ", qasm_instructions[i]) == true)
		{
		  rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		  q.apply(gates::H, {rn});
		  cout << " h gate to " << rn << endl;
		}

	      // x gate.
	      if ((qre_recog("x ", qasm_instructions[i]) == true)&&(qre_recog("cx ", qasm_instructions[i]) == false))
		{		  	     
		  rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		  q.apply(gates::X, {rn});
		  cout << " x gate to " << rn << endl;
		}
	      
	      // y gate.
	      if ((qre_recog("y ", qasm_instructions[i]) == true)&&(qre_recog("cy ", qasm_instructions[i]) == false))
		{
		  rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		  q.apply(gates::Y, {rn});
		  cout << " y gate to " << rn << endl;
		}

	      // z gate.
	      if ((qre_recog("z ", qasm_instructions[i]) == true)&&(qre_recog("cz ", qasm_instructions[i]) == false))
		{
		  rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		  q.apply(gates::Z, {rn});
		  cout << " z gate to " << rn << endl;
		}

	      // s gate.
	      if (qre_recog("s ", qasm_instructions[i]) == true )
		{
		  /*rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		  q.apply(gates::S, {rn});
		  cout << " s gate to " << rn << endl;*/
		  cout << " s gate not recognized." << endl;
		}

	      // t gate.
	      if (qre_recog("t ", qasm_instructions[i]) == true)
		{
		  rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		  q.apply(gates::T, {rn});
		  cout << " t gate to " << rn << endl;
		}

	      // t gate.
	      if (qre_recog("id ", qasm_instructions[i]) == true)
		{
		  rn = qre_parse_bitnum(qre_parse_reg(qasm_instructions[i], "q"));
		  q.apply(gates::I, {rn});
		  cout << " id gate to " << rn << endl;
		}

	      // s gate.
	      if (qre_recog("barrier ", qasm_instructions[i]) == true )
		{
		  cout << " barrier not recognized." << endl;
		}
	      
	    }
	}
    }

  //Format results on c vector.
  res = "{";
  cout << "c size " << c.size() << endl;
  for(int i = 0; i < (int)c.size(); i++)
    {
      std::ostringstream stro;
      stro << c[i];
      std::string str = stro.str();
      if (i > 0)
	{
	  res = res + ":";
	}
      res = res + str;
    }

  res = res + "}";
  
  return res;
}

