php_var pow (php_var base, php_var exp)
{
  double ans = pow((double) base, (double) exp);
  if(!ans) {
    return false;
  } else {
    return (php_var) ans;
  }
}
