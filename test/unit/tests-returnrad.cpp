/*
   This file is part of the RELXILL model code.

   RELXILL is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   any later version.

   RELXILL is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.
   For a copy of the GNU General Public License see
   <http://www.gnu.org/licenses/>.

    Copyright 2021 Thomas Dauser, Remeis Observatory & ECAP
*/

#include "catch2/catch_amalgamated.hpp"
#include "LocalModel.h"
#include "XspecSpectrum.h"
#include "common-functions.h"

#include <vector>


// ------- //
TEST_CASE(" Execute single model with output writing ", "[output]") {
  DefaultSpec default_spec{};
  const char* env_outfiles = "RELXILL_WRITE_OUTFILES";

  setenv(env_outfiles, "1", 1);

  eval_xspec_lmod_default(ModelName::relxilllp, default_spec);
  unsetenv(env_outfiles);


}
