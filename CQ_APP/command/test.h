#pragma once

#include "../command.h"

namespace qbot {
	namespace command {

		class Test : public qff233::Command
		{
		public:
			typedef std::shared_ptr<Test> ptr;
			Test();
			void handle(MsgEvent& e) override;
		};

	}
}