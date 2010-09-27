void main() {
  float x,y;
  x = FBETWEEN(0,10);
  y = x*x-x;
  if (y>=0)
    y = x/10.0;
  else 
    y = x;
}
