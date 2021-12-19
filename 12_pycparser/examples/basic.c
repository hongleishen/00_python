int foo() {}
int add(int a, int b)
{
	int tmp;
	tmp = a+ b;
	
	return tmp;
}

int main() {
  int a, b = 10;
  foo();
  add(a, b);
  
  return 0;
}
