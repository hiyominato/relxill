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

    Copyright 2020 Thomas Dauser, Remeis Observatory & ECAP
*/
#include <stdio.h>
#include "writeOutfiles.h"

static void fclose_errormsg(FILE *fp, char *foutName) {
  if (fclose(fp)) {
    printf(" *** error : failed writing file %s \n", foutName);
  }

}

/**
 *  @function: write_binned_data_to_file
 *  @synopsis: write data to file, where the x-value is interpreted as a binned grid,
 *    with rlo[ii]=rad[ii] and rhi[ii]=rad[ii+1]
 *  @input:
 *   - double[n_rad+1] rad
 *   - double[n_rad] intens
 *   - int n_rad
 **/
void write_binned_data_to_file(char *foutName, double *rad, double *intens, int n_rad) {
  FILE *fp = fopen(foutName, "w+");
  for (int ii = 0; ii < n_rad; ii++) {
    fprintf(fp, " %e \t %e \t %e \n", rad[ii], rad[ii + 1], intens[ii]);
  }
  fclose_errormsg(fp, foutName);
}

/**
 *  @function: write_data_to_file
 *  @synopsis: write data to file, where the x-value and y-value is written
 *  @input:
 *   - double[n_rad] rad
 *   - double[n_rad] intens
 *   - int n_rad
 **/
void write_data_to_file(char *foutName, double *rad, double *intens, int n_rad) {

  FILE *fp = fopen(foutName, "w+");
  for (int ii = 0; ii < n_rad; ii++) {
    fprintf(fp, " %e \t %e \n", rad[ii], intens[ii]);
  }
  if (fclose(fp)) {
    exit(1);
  }
}

void write_radiallyResolvedFluxObs(double *rad, double *intens, int n_rad) {
  char *fname = "test_relline_radialFluxProfile.dat";
  assert(intens != NULL);
  write_data_to_file(fname, rad, intens, n_rad);
}


/** print the relline profile   **/
void save_relline_profile(rel_spec *spec) {

  if (spec == NULL) return;

  FILE *fp = fopen("test_relline_profile.dat", "w+");
  for (int ii = 0; ii < spec->n_ener; ii++) {
    fprintf(fp, " %e \t %e \t %e \n", spec->ener[ii], spec->ener[ii + 1], spec->flux[0][ii]);
  }
  if (fclose(fp)) exit(1);

}


void write_relconv_outfiles(RelSysPar *sysPar, rel_spec *spec, int *status) {
  write_data_to_file("test_emis_profile.dat", sysPar->emis->re, sysPar->emis->emis, sysPar->emis->nr);
  if (sysPar->emisReturn != NULL) {
    write_data_to_file("test_emisReturn_profile.dat", sysPar->emisReturn->re,
                       sysPar->emisReturn->emis, sysPar->emisReturn->nr);
  }
  save_relline_profile(spec);
}

void save_xillver_spectrum(double *ener, double *flu, int n_ener, char *fname) {

  FILE *fp = fopen(fname, "w+");
  int ii;
  for (ii = 0; ii < n_ener; ii++) {
    fprintf(fp, " %e \t %e \t %e \n", ener[ii], ener[ii + 1], flu[ii]);
  }
  if (fclose(fp)) {
    exit(1);
  }
}
