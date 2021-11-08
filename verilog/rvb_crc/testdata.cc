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

	for (int k = 0; k < 2; k++)
	{
		char filename[128];
		snprintf(filename, 128, "testdata_%d.hex", k);
		f = fopen(filename, "w");

		bool enable_64bit = (k & 1) == 0;

		for (int i = 0; i < 1000; i++)
		{
			uint32_t din_insn;
			uint64_t din_rs1 = xorshift64();
			uint64_t dout_rd;

			switch (xorshift32() % 8)
			{
			case 0: // CRC32.B
				din_insn = 0x61001013;
				if (!enable_64bit) din_rs1 &= 0xFFFFFFFFLL;
				dout_rd = rv64b::crc32_b(din_rs1);
				break;
			case 1: // CRC32.H
				din_insn = 0x61101013;
				if (!enable_64bit) din_rs1 &= 0xFFFFFFFFLL;
				dout_rd = rv64b::crc32_h(din_rs1);
				break;
			case 2: // CRC32.W
				din_insn = 0x61201013;
				if (!enable_64bit) din_rs1 &= 0xFFFFFFFFLL;
				dout_rd = rv64b::crc32_w(din_rs1);
				break;
			case 3: // CRC32.D
				if (!enable_64bit) { i--; continue; }
				din_insn = 0x61301013;
				dout_rd = rv64b::crc32_d(din_rs1);
				break;
			case 4: // CRC32C.B
				din_insn = 0x61801013;
				if (!enable_64bit) din_rs1 &= 0xFFFFFFFFLL;
				dout_rd = rv64b::crc32c_b(din_rs1);
				break;
			case 5: // CRC32C.H
				din_insn = 0x61901013;
				if (!enable_64bit) din_rs1 &= 0xFFFFFFFFLL;
				dout_rd = rv64b::crc32c_h(din_rs1);
				break;
			case 6: // CRC32C.W
				din_insn = 0x61a01013;
				if (!enable_64bit) din_rs1 &= 0xFFFFFFFFLL;
				dout_rd = rv64b::crc32c_w(din_rs1);
				break;
			case 7: // CRC32C.D
				if (!enable_64bit) { i--; continue; }
				din_insn = 0x61b01013;
				dout_rd = rv64b::crc32c_d(din_rs1);
				break;
			}
			fprintf(f, "%08llx_%016llx_%016llx\n", (long long)din_insn, (long long)din_rs1, (long long)dout_rd);
		}
		fclose(f);
	}

	return 0;
}
