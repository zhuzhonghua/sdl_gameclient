#include "bpt.h"

#include <boost/property_tree/xml_parser.hpp>

BPT* BPT::_inst;
BPT* BPT::inst()
{
	if (_inst == NULL) _inst = new BPT();
	return _inst;
}

BPT::BPT()
{
	using boost::property_tree::ptree;
	read_xml("data/pd/lang.xml", pt);
}

BPT::~BPT()
{
}

std::string BPT::getText(const std::string& key)
{
	return inst()->pt.get<std::string>(key);
}