#!/bin/bash
set -ex

cbmc --trace --function identity_check    bit_matrix.cc
cbmc --trace --function bswap_check       bit_matrix.cc
cbmc --trace --function brevb_check       bit_matrix.cc
cbmc --trace --function rfill_check       bit_matrix.cc
cbmc --trace --function round_pow2_check  bit_matrix.cc
cbmc --trace --function strlen_check      bit_matrix.cc
cbmc --trace --function conv8x8_check     bit_matrix.cc

echo OK
