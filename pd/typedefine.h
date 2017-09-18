#pragma once

typedef std::string String;
typedef bool boolean;

template<class T>
class HashSet :public std::set<T>{
public:
	bool contains(T t){
		return this->find(t) != this->end();
	}
};

template<class T>
class List :public std::list<T>{
public:
	bool contains(T t){
		return std::find(this->begin(), this->end(), t) != this->end();
	}
};
