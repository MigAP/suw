#ifndef Z_TF_H
#define Z_TF_H
#define _POSIX_C_SOURCE 200809L

#include "util.h"

#define NUMERATOR_CHAR 'N'
#define DENOMINATOR_CHAR 'D'

struct ztf {
     /* numerator and denominator of the transfert function */
     double *num;
     size_t n_num;

     double *denom;
     size_t n_denom;

     /* buffer of previous input and output values */
     double *buf_inputs;
     double *buf_outputs;
};

/* allocates space for the numerator and denominator arrays */
int ztf_allocate( struct ztf *fc, size_t n_num, size_t n_denom );

int ztf_set_num ( struct ztf *fc, double num[], size_t n_num );

int ztf_set_denom ( struct ztf *fc, double denom[], size_t n_denom );

int ztf_set_buf_inputs ( struct ztf *fc, double num[], size_t n_num );

int ztf_set_buf_outputs( struct ztf *fc, double denom[], size_t n_denom);

void ztf_set_buf_input_vals ( struct ztf *fc, double val );

void ztf_set_buf_output_vals( struct ztf *fc, double val);

/* TODO: init function to set all the values of the structure */

void ztf_print( struct ztf *fc );

void ztf_free( struct ztf *fc );

/* updates the input buffer of "fc" by adding "input" at the beginning
 * of the buffer. */
void ztf_update_inputs( struct ztf *fc, double input );


/* updates the output buffer of "fc" by adding "output" at the
 * beginning of the buffer. */
void ztf_update_outputs( struct ztf *fc, double output );


/* the new input is assumed to be already in fc, so there is no need
 * to update fc buffers */
double ztf_compute_output(struct ztf *fc);


/* updates fc with the new input, computes the output and updates fc
 * with the new output */
double ztf_update_and_compute( struct ztf *fc, double input );

int ztf_import(struct ztf *fc, FILE *f );

#endif /* Z_TF_H */
