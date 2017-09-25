#pragma once

#include <map>
#include <list>
#include <set>

typedef std::string String;
typedef bool boolean;

template<class T>
class HashSet :public std::set<T>{
public:
	bool contains(T t){
		return this->find(t) != this->end();
	}
	void add(T t){
		this->insert(t);
	}
};

template<class T>
class List :public std::list<T>{
public:
	bool contains(T t){
		return std::find(this->begin(), this->end(), t) != this->end();
	}
};

template<class K, class V>
class HashMap :public std::map<K, V>{
public:
	bool containsKey(const K& k){
		return this->find(k) != this->end();
	}
	V get(const K& k){
		return this->operator[](k);
	}
};

template<class T>
class ArrayList :public std::list<T>{
public:
	bool contains(T t){
		return std::find(this->begin(), this->end(), t) != this->end();
	}
};

#define CLASSNAME(name) virtual std::string getClassName() { return #name;}

#define FACTORY(name) \
class Factory##name{ \
public:\
	static name* Create(const std::string& clsName)\
	{\
		return facs[clsName]->Create();\
	}\
protected:\
	virtual name* Create() = 0;\
	static std::map<std::string, Factory##name*> facs;\
};

#define REFLECT(name, type) \
class Factory##type##name:public Factory##type{ \
public: \
	Factory##type##name(){\
		facs.insert(std::make_pair(#name, this));\
	}\
protected:\
	virtual name* Create(){\
		return new name();\
	}\
}; \
namespace{ Factory##type##name reg##name; }

#define REFLECTBUFF(name) REFLECT(name, Buff)
#define REFLECTBLOB(name) REFLECT(name, Blob)