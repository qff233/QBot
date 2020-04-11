#pragma once

namespace qff233 {

template<class T>
class Singleton {
public:
	static T* GetInstance() {
		static T v;
		return &v;
	}
};

}