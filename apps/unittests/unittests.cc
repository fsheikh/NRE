/*
 * (c) 2012 Nils Asmussen <nils@os.inf.tu-dresden.de>
 *     economic rights: Technische Universität Dresden (Germany)
 *
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */

#include <stream/Serial.h>

#include "tests/Pingpong.h"
#include "tests/UtcbTest.h"
#include "tests/RegionListTest.h"
#include "tests/DelegatePerf.h"
#include "tests/CatchEx.h"
#include "tests/SharedMemory.h"
#include "tests/DataSpaceTest.h"

using namespace nul;
using namespace nul::test;

const TestCase testcases[] = {
	pingpong,
	catchex,
	delegateperf,
	utcbtest,
	regionlist,
	dstest,
	sharedmem,
};

int main() {
	for(size_t i = 0; i < sizeof(testcases) / sizeof(testcases[0]); ++i) {
		Serial::get().writef("Testing %s...\n",testcases[i].name);
		try {
			testcases[i].func();
		}
		catch(const Exception& e) {
			Serial::get() << e;
		}
		Serial::get().writef("Done\n");
	}
}