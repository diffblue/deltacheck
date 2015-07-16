void main()
{
  unsigned x = 5;
  int y, z = 3, w;
  if(!y)
  {
    z += y;
    w = x;
  }
  else
  {
    z -= y;
    w = x;
  }
  float *f;
  //float d[5];
  for(z=0;z<3;z++)
  {
    assert(w==5);
    //f = d;
    f = malloc(w*sizeof(float));
    f[1] = 1.0;
    f[2] = 1.0;
    assert(f[1]==1.0);
    assert(f[2]==0.9);
  }
  assert(f[1]==1.0);
  assert(f[2]==0.9);
}
