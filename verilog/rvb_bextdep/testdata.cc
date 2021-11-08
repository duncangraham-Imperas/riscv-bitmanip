/*
 *  Copyright (C) 2019  Claire Wolf <claire@symbioticeda.com>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <stdio.h>
#include "../../cproofs/common.h"

int main()
{
	FILE *f;

	for (int k = 0; k < 8; k++)
	{
		char filename[128];
		snprintf(filename, 128, "testdata_%d.hex", k);
		f = fopen(filename, "w");

		bool enable_64bit = (k & 1) == 0;
		bool enable_grev = (k & 2) == 0;
		bool enable_shfl = (k & 4) == 0;

		for (int i = 0; i < 1000; i++)
		{
			uint32_t din_insn;
			uint64_t din_rs1 = xorshift64();
			uint64_t din_rs2 = xorshift64();
			uint64_t dout_rd;

			switch (xorshift32() % 12)
			{
			case 0: // BDEP
				if (!enable_64bit) { i--; continue; }
				din_insn = 0x48006033;
				dout_rd = rv64b::bdep(din_rs1, din_rs2);
				break;
			case 1: // BEXT
				if (!enable_64bit) { i--; continue; }
				din_insn = 0x08006033;
				dout_rd = rv64b::bext(din_rs1, din_rs2);
				break;
			case 2: // SHFL
				if (!enable_64bit) { i--; continue; }
				if (!enable_shfl) { i--; continue; }
				din_insn = 0x08001033;
				dout_rd = rv64b::shfl(din_rs1, din_rs2);
				break;
			case 3: // UNSHFL
				if (!enable_64bit) { i--; continue; }
				if (!enable_shfl) { i--; continue; }
				din_insn = 0x08005033;
				dout_rd = rv64b::unshfl(din_rs1, din_rs2);
				break;
			case 4: // GORC
				if (!enable_64bit) { i--; continue; }
				if (!enable_grev) { i--; continue; }
				din_insn = 0x28005033;
				dout_rd = rv64b::gorc(din_rs1, din_rs2);
				break;
			case 5: // GREV
				if (!enable_64bit) { i--; continue; }
				if (!enable_grev) { i--; continue; }
				din_insn = 0x68005033;
				dout_rd = rv64b::grev(din_rs1, din_rs2);
				break;
			case 6: // BDEPW
				din_insn = enable_64bit ? 0x4800603b : 0x48006033;
				dout_rd = int32_t(rv32b::bdep(din_rs1, din_rs2));
				break;
			case 7: // BEXTW
				din_insn = enable_64bit ? 0x0800603b : 0x08006033;
				dout_rd = int32_t(rv32b::bext(din_rs1, din_rs2));
				break;
			case 8: // SHFLW
				if (!enable_shfl) { i--; continue; }
				din_insn = enable_64bit ? 0x0800103b : 0x08001033;
				dout_rd = int32_t(rv32b::shfl(din_rs1, din_rs2));
				break;
			case 9: // UNSHFLW
				if (!enable_shfl) { i--; continue; }
				din_insn = enable_64bit ? 0x0800503b : 0x08005033;
				dout_rd = int32_t(rv32b::unshfl(din_rs1, din_rs2));
				break;
			case 10: // GORCW
				if (!enable_grev) { i--; continue; }
				din_insn = 0x28005033 | (enable_64bit ? 8 : 0);
				dout_rd = int32_t(rv32b::gorc(din_rs1, din_rs2));
				break;
			case 11: // GREVW
				if (!enable_grev) { i--; continue; }
				din_insn = 0x68005033 | (enable_64bit ? 8 : 0);
				dout_rd = int32_t(rv32b::grev(din_rs1, din_rs2));
				break;
			}
			fprintf(f, "%08llx_%016llx_%016llx_%016llx\n", (long long)din_insn, (long long)din_rs1, (long long)din_rs2, (long long)dout_rd);
		}
		fclose(f);
	}

	return 0;
}
