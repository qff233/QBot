#include "common.h"

namespace qff233 {
	static std::shared_ptr<Logger> g_logger;

	void InitLogger() {
		g_logger = std::make_shared<Logger>("qff233");
	}

	std::shared_ptr<Logger> GetLogger() {
		return g_logger;
	}
}
