/*******************************************************************\

Module: SSA Simplification

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include <util/simplify_expr.h>

#include "simplify_ssa.h"

/*******************************************************************\

Function: simplify_ssa

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void simplify(local_SSAt &ssa, const namespacet &ns)
{
  for(local_SSAt::nodest::iterator
      n_it=ssa.nodes.begin();
      n_it!=ssa.nodes.end();
      n_it++)
  {
    local_SSAt::nodet &node=n_it->second;
    
    for(local_SSAt::nodet::equalitiest::iterator
        e_it=node.equalities.begin();
        e_it!=node.equalities.end();
        e_it++)
    {
      e_it->lhs()=simplify_expr(e_it->lhs(), ns);
      e_it->rhs()=simplify_expr(e_it->rhs(), ns);
    }

    for(local_SSAt::nodet::constraintst::iterator
        c_it=node.constraints.begin();
        c_it!=node.constraints.end();
        c_it++)
    {
      *c_it=simplify_expr(*c_it, ns);
    }
    
    if(node.assertion.is_not_nil())
      node.assertion=simplify_expr(node.assertion, ns);
  }
}
