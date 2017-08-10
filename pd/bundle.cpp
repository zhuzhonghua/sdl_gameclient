#include "bundle.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

const std::string Bundle::CLASS_NAME = "__className";

std::map<std::string, std::string> Bundle::aliases;

Bundle* Bundle::read(std::stringstream& ss)
{
	Bundle* b = new Bundle();
	read_json(ss, b->data);

	return b;
}

bool Bundle::write(Bundle* bundle, std::stringstream& ss)
{
	write_json(ss, bundle->data);
	return true;
}

bool Bundle::getStringArray(const std::string& key, std::list<std::string>& ret)
{
	boost::property_tree::ptree image_array = data.get_child(key);
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, image_array)
	{
		std::stringstream s;
		write_json(s, v.second);
		ret.push_back(s.str());
	}

	return true;
}

bool Bundle::getBoolean(const std::string& key)
{
	return data.get<bool>(key);
}

float Bundle::getFloat(const std::string& key)
{
	return data.get<float>(key);
}

int Bundle::getInt(const std::string& key)
{
	return data.get<int>(key);
}

std::string Bundle::getString(const std::string& key)
{
	return data.get<std::string>(key);
}

Bundle* Bundle::getBundle(const std::string& key)
{
	return new Bundle(data.get_child(key));
}

Bundle::Bundle(const boost::property_tree::ptree& d)
{
	this->data = d;
}

void Bundle::put(const std::string& key, bool value)
{
	data.put(key, value);
}

void Bundle::put(const std::string& key, int value)
{
	data.put(key, value);
}
void Bundle::put(const std::string& key, float value)
{
	data.put(key, value);
}
void Bundle::put(const std::string& key, const std::string& value)
{
	data.put(key, value);
}

void Bundle::put(const std::string& key, Bundle* value)
{
	data.put_child(key, value->data);
}

void Bundle::put(const std::string& key, Bundlable* object)
{
	Bundle bundle;
	bundle.put(CLASS_NAME, object->getClassName());
	object->storeInBundle(&bundle);
	data.put_child(key, bundle.data);
}

void Bundle::put(const std::string& key, const std::vector<int>& arr)
{
	boost::property_tree::ptree array1;
	for (int i = 0; i < arr.size(); i++)
	{
		array1.put("", arr[i]);
	}
	data.put_child(key, array1);
}

void Bundle::put(const std::string& key, const std::vector<bool>& arr)
{
	boost::property_tree::ptree array1;
	for (int i = 0; i < arr.size(); i++)
	{
		array1.put("", arr[i]);
	}
	data.put_child(key, array1);
}

void Bundle::put(const std::string& key, const std::vector<std::string>& arr)
{
	boost::property_tree::ptree array1;
	for (int i = 0; i < arr.size(); i++)
	{
		array1.put("", arr[i]);
	}
	data.put_child(key, array1);
}