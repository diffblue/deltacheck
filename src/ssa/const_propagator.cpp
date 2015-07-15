/*******************************************************************\

Module: Constant Propagation

Author: Peter Schrammel

\*******************************************************************/

#include <iostream>

#include <util/find_symbols.h>
#include <util/arith_tools.h>

#include "const_propagator.h"

/*******************************************************************\

Function: const_propagator_domaint::transform

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void const_propagator_domaint::transform(
  locationt from,
  locationt to,
  ai_baset &ai,
  const namespacet &ns)
{
#if 0
  if(from->is_assign())
  {
    const code_assignt &assignment=to_code_assign(from->code);
    assign_rhs_rec(lhs_deref, rhs_deref, ns);
  }
  else if(from->is_goto())
  {
    // Perhaps look at condition, for stuff like
    // p!=0 or the like.
    //exprt cond_deref=dereference(from->guard, *this, "", ns);
  }
  else if(from->is_dead())
  {
    const code_deadt &code_dead=to_code_dead(from->code);
    assign_lhs_rec(code_dead.symbol(), nil_exprt(), ns);
  }
#endif
}

/*******************************************************************\

Function: const_propagator_domaint::assign_rhs_rec

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void const_propagator_domaint::assign(
  valuest &dest,
  const exprt &src,
  const namespacet &ns) const
{
#if 1
  std::cout << "assign: " << from_expr(ns, "", src) << '\n';
#endif

#if 0
 
  if(rhs.id()==ID_constant)
  {
    if(to_constant_expr(rhs).get_value()==ID_NULL)
    {
      dest.null=true;
    }
  }
  else if(rhs.id()==ID_typecast)
  {
    assign_rhs_rec(dest, to_typecast_expr(rhs).op(), ns, offset, alignment);
  }
#endif
}

/*******************************************************************\

Function: const_propagator_domaint::valuest::output

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void const_propagator_domaint::valuest::output(
  std::ostream &out,
  const namespacet &ns) const
{
  for(replace_symbolt::expr_mapt::const_iterator 
	it=replace_const.expr_map.begin();
      it!=replace_const.expr_map.end();
      ++it)
    out << ' ' << it->first << "=" <<
      from_expr(ns, "", it->second) << std::endl;
}

/*******************************************************************\

Function: const_propagator_domaint::output

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void const_propagator_domaint::output(
  std::ostream &out,
  const ai_baset &ai,
  const namespacet &ns) const
{
  values.output(out,ns);
}

/*******************************************************************\

Function: const_propagator_domaint::valuest::merge

  Inputs:

 Outputs: Return true if "this" has changed.

 Purpose:

\*******************************************************************/

bool const_propagator_domaint::valuest::merge(const valuest &src)
{
  bool changed = false;
  for(replace_symbolt::expr_mapt::const_iterator 
	it=src.replace_const.expr_map.begin();
      it!=src.replace_const.expr_map.end(); ++it)
  {
    replace_symbolt::expr_mapt::iterator 
      c_it = replace_const.expr_map.find(it->first);
    if(c_it != replace_const.expr_map.end())
    {
      if(c_it->second != it->second)
      {
        replace_const.expr_map.erase(c_it);
	changed = true;
      }
    }
    else
    {
      replace_const.expr_map[it->first] = it->second;
      changed = true;
    }
  }
  return changed;
}

/*******************************************************************\

Function: const_propagator_domaint::merge

  Inputs:

 Outputs: Return true if "this" has changed.

 Purpose:

\*******************************************************************/

bool const_propagator_domaint::merge(
  const const_propagator_domaint &other,
  locationt from,
  locationt to)
{
  return values.merge(other.values);
}

/*******************************************************************\

Function: const_propagator_domaint::evaluate_casts_in_constants

  Inputs:

 Outputs: 

 Purpose:

\*******************************************************************/

exprt const_propagator_domaint::evaluate_casts_in_constants(const exprt &expr, 
		    const typet& parent_type, bool &valid)
{
  if(expr.id()==ID_side_effect) valid = false;
  if(expr.type().id()!=ID_signedbv && expr.type().id()!=ID_unsignedbv) return expr;
  exprt r = expr;
  if(expr.id()==ID_typecast) r = evaluate_casts_in_constants(expr.op0(),expr.type(),valid);
  if(r.id()!=ID_constant) return typecast_exprt(r,parent_type);
  mp_integer v;
  to_integer(to_constant_expr(r), v);
  return from_integer(v,parent_type);
}

/*******************************************************************\

Function: const_propagator_ait::replace

  Inputs:

 Outputs: 

 Purpose:

\*******************************************************************/

void const_propagator_ait::replace(
  goto_functionst::goto_functiont &goto_function,
  const namespacet &ns)
{
  Forall_goto_program_instructions(it, goto_function.body)
  {
    state_mapt::iterator s_it = state_map.find(it);
    if(s_it == state_map.end())
      continue;
    replace_types_rec(s_it->second.values.replace_const,
		      it->guard);
  }
}

/*******************************************************************\

Function: const_propagator_ait::replace_types_rec

  Inputs:

 Outputs: 

 Purpose:

\*******************************************************************/

void const_propagator_ait::replace_types_rec(
  const replace_symbolt &replace_const, 
  exprt &expr)
{
  replace_const(expr.type());
  for(exprt::operandst::iterator it = expr.operands().begin(); 
      it!=expr.operands().end();it++)
    replace_types_rec(replace_const,*it);
}

