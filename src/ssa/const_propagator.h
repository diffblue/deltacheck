/*******************************************************************\

Module: Constant propagation

Author: Peter Schrammel

\*******************************************************************/

#ifndef CPROVER_CONST_PROPAGATOR_H
#define CPROVER_CONST_PROPAGATOR_H

#include <analyses/ai.h>
#include <util/replace_symbol.h>

class const_propagator_domaint:public ai_domain_baset
{
public:
  virtual void transform(locationt, locationt, ai_baset &, const namespacet &);
  virtual void output(std::ostream &, const ai_baset &, const namespacet &) const;
  bool merge(const const_propagator_domaint &, locationt, locationt);

  struct valuest
  {
  public:
    // maps variables to constants
    replace_symbolt replace_const;
    
    void output(std::ostream &, const namespacet &) const;
    
    bool merge(const valuest &src);
    
    inline void clear()
    {
      replace_const.expr_map.clear();
      replace_const.type_map.clear();
    }
    
    bool empty() const
    {
      return replace_const.expr_map.empty() && 
	replace_const.type_map.empty();
    }
  };

  const valuest operator()(const exprt &src, const namespacet &ns) const
  {
    valuest tmp;
    assign(tmp, src, ns);
    return tmp;
  }

  valuest values;
  
protected:
  void assign(
    valuest &dest, const exprt &src, 
    const namespacet &) const;

  exprt evaluate_casts_in_constants(const exprt &expr, 
				    const typet& parent_type, 
				    bool &valid);

};

class const_propagator_ait:public ait<const_propagator_domaint>
{
public:
  const_propagator_ait(
    goto_functionst::goto_functiont &goto_function,
    const namespacet &ns)
  {
    operator()(goto_function, ns);
    replace(goto_function, ns);
  }

protected:
  friend class const_propagator_domaint;

void replace(
  goto_functionst::goto_functiont &goto_function,
  const namespacet &ns);

void replace_types_rec(
  const replace_symbolt &replace_const, 
  exprt &expr);
};

#endif
