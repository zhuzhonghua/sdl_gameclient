#pragma once

class Bundle;
class Bundlable{
public:
	virtual void restoreFromBundle(Bundle* bundle) = 0;
	virtual void storeInBundle(Bundle* bundle) = 0;
	virtual std::string getClassName() = 0;
};