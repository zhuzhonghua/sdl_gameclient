#pragma once

#include <string>
#include <map>
#include <list>
#include <vector>

#include <boost/property_tree/ptree.hpp>
#include "bundlable.h"

class Bundle{
private:
	static const std::string CLASS_NAME;
	static std::map<std::string, std::string> aliases;

	boost::property_tree::ptree data;

	Bundle(const boost::property_tree::ptree& d);
public:
	Bundle(){}
	std::string toString();
	bool isNull();
	void fields(std::list<std::string>& ret);
	bool contains(const std::string& key);
	bool getBoolean(const std::string& key);
	int getInt(const std::string& key);
	float getFloat(const std::string& key);
	std::string getString(const std::string& key);
	Bundle* getBundle(const std::string& key);
	void getIntArray(const std::string& key, std::list<int>& ret);
	void getBooleanArray(const std::string& key, std::list<bool>& ret);
	bool getStringArray(const std::string& key, std::list<std::string>& ret);

	void put(const std::string& key, bool value);
	void put(const std::string& key, int value);
	void put(const std::string& key, float value);
	void put(const std::string& key, const std::string& value);
	void put(const std::string& key, Bundle* value);
	void put(const std::string& key, Bundlable* object);

	void put(const std::string& key, const std::vector<int>& arr);
	void put(const std::string& key, const std::vector<bool>& arr);
	void put(const std::string& key, const std::vector<std::string>& arr);

	static Bundle* read(std::stringstream& stream);
	static bool write(Bundle* bundle, std::stringstream& stream);
};