#pragma once

#include <CQSDK.h>
#include "util/common.h"

namespace qff233 {
	void init();
	void reload();
	void handle(MsgEvent& e);
}