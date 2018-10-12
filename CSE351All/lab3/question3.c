void g(long ** p, long * b) {
  for(int i=0; i <= 6; ++i) {
    b[i] = 42;
    long * q = *p;
    long   v = *q;
    *q = v + 1;
  }
}

int main() {
  long   x = 0;
  long * y = &x;
  long   a[6] = {3,6,9,12,15,18};
  g(&y,a);
  return 0;
}
