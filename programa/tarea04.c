/*
 * Copyright (C) 2010
 * Pablo Alvarado
 *
 * This file is part of the EL-5805 Digital Signal Processing Lecture at
 * the Costa Rica Institute of Technology
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file   tarea04.c
 *         Contains the processing function
 * \author Pablo Alvarado
 * \author Allan Navarro
 * \date   Oct 19th, 2022
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "tarea04.h"

float *ys; /* array to store previous outputs */
unsigned int _Fs = 48000; /* default sample rate, to be updated with value from init() */
unsigned int k = 0; /* number of output samples to store */

/**
 * This method is called before the real processing starts.
 * You may use it to initialize whatever you need to.
 */
void init(const unsigned int Fs)
{
  _Fs = Fs;
  k = _Fs * K_ms / 1000;

  /*
   * allocate memory for output samples
   * make sure at least 1024 samples can be stored (size of frame)
   */
  int allocate_n =k;
  if(k<MIN_SIZE_BUFFER){
    allocate_n = MIN_SIZE_BUFFER;
  }
  ys = (float *)malloc((allocate_n) * sizeof(float));
}

/**
 * Process a buffer of data.
 *
 * @param Fs sampling rate
 * @param channels number of channels per frame
 * @param frames   number of frames per buffer
 * @param buffer   input and output buffer
 *
 * @return zero if error, or 1 otherwise.
 */
int process(const unsigned int Fs,
            const int nframes,
            const float *in,
            float *out)
{

  /* the 'ys' array will always contain the kth oldest sample in position 0 */

  for (int i = 0; i < nframes; ++i)
  {
    /* perform difference equation */
    out[i] = ALPHA * ys[i] + (1.0f - ALPHA) * in[i];

  }

  /* shift the 'ys' array by k */

  /* if nframes is bigger than k, then just store the values */
  if (nframes>=k){
    memcpy(ys,out,nframes*sizeof(float));
    return 0;
  }

  int src_index = k-nframes;
  int dest_index = src_index - nframes;
  int n_to_copy = nframes;

  if (dest_index<0){
    src_index = src_index - dest_index;
    n_to_copy = k - src_index;
  }
  memmove(ys,ys+src_index,n_to_copy*sizeof(float));
  memcpy(ys+src_index,out,nframes*sizeof(float));

  return 0;
}
