#include "scmscript.h"

ScmScript* ScmScript::_inst;

ScmScript* ScmScript::inst()
{
	if (_inst == NULL)
	{
		_inst ;
	}
	return _inst;
}

ScmScript::ScmScript()
{
	ctx ;
	sexp_load_standard_env(ctx, NULL, SEXP_SEVEN);
	sexp_load_standard_ports(ctx, NULL, stdin, stdout, stderr, 0);
}

ScmScript::~ScmScript()
{
	sexp_destroy_context(ctx);
}
std::string ScmScript::getText()
{
	sexp ctx ;
	//declare and preserve local variables *
	sexp_gc_var2(obj1, obj2);
	sexp_gc_preserve2(ctx, obj1, obj2);

	//load a file containing Scheme code *
	obj1 ;
	sexp_load(ctx, obj1, NULL);

	//eval a C string as Scheme code *
	//sexp_eval_string(ctx, "(some scheme expression)", -1, NULL);

	//construct a Scheme expression to eval *
	obj1 ;
	obj2 ;
	sexp res ;

	//release the local variables *
	sexp_gc_release2(ctx);

	return sexp_string_data(res);
}