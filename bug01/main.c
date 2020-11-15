typedef struct Foo {
        int     a;
        int     b;
        int     c;
} Foo;


int main() {
        Foo f;
        f.a = 17;
        f.b = 23;
        f.c = 19;

        test_foo(f);
}

void test_foo(Foo *pfoo) {
        pfoo->c = 29;
}