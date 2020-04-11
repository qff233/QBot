#pragma once

#include <CQSDK.h>
#include <memory>
#include "config.h"
#include "singleton.h"

namespace qff233 {
	void InitLogger();
	std::shared_ptr<Logger> GetLogger();
}