extern void __VERIFIER_assume(int);
void __VERIFIER_assert(int cond) {
  if (!(cond)) {
    ERROR: assert(0);
  }
  return;
}

int __VERIFIER_nondet_int();

int main() {
  int i=0, x=0, y=0;
  int n=__VERIFIER_nondet_int();
__CPROVER_assume(n>0);
  for(i=0; i<n; i++)
  {
    x = x-y;
assert(x==0);
    y = __VERIFIER_nondet_int();
__CPROVER_assume(y!=0);
    x = x+y;
assert(x!=0);
  }
assert(x==0);
}
