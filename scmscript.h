#pragma once

#include <string>
#include "chibi/eval.h"

class ScmScript{
	static ScmScript* _inst;
	static ScmScript* inst();

	ScmScript();
	~ScmScript();

	sexp ctx;
public:
	static std::string getText();
};