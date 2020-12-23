struct A {
  A();

private:
 A(A& other) {}
};

class B {
  A field_;
};

void Func(const B& t) {}
                                                                                                                  
int main(void) {
  Func(B());
  return 0;
}