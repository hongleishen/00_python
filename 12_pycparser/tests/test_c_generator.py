import os
import sys
import unittest

# Run from the root dir
sys.path.insert(0, '.')

from pycparser import c_parser, c_generator, c_ast, parse_file
from tests.test_util import cpp_supported, cpp_path, cpp_args

_c_parser = c_parser.CParser(
                lex_optimize=False,
                yacc_debug=True,
                yacc_optimize=False,
                yacctab='yacctab')


def compare_asts(ast1, ast2):
    """Compares two ASTs recursively just enough for the purpose of testing.

    Since this function is recursive it also accepts non-ast parameters, in
    which case it compares them literally (with ==) or recursively (for tuples
    or lists).
    """
    # After the initial `if`, all the `elif` clauses assume that the types of
    # ast1 and ast2 are the same.
    if type(ast1) != type(ast2):
        return False
    elif isinstance(ast1, (list, tuple)):
        if len(ast1) != len(ast2):
            return False
        for i in range(len(ast1)):
            if not compare_asts(ast1[i], ast2[i]):
                return False
        return True
    elif isinstance(ast1, c_ast.Node):
        for attr in ast1.attr_names:
            attr1 = getattr(ast1, attr)
            attr2 = getattr(ast2, attr)
            if not compare_asts(attr1, attr2):
                return False
        children1 = ast1.children()
        children2 = ast2.children()
        if len(children1) != len(children2):
            return False
        for i in range(len(children1)):
            if not compare_asts(children1[i], children2[i]):
                return False
        return True
    else:
        return ast1 == ast2


def parse_to_ast(src):
    return _c_parser.parse(src)


class TestFunctionDeclGeneration(unittest.TestCase):
    class _FuncDeclVisitor(c_ast.NodeVisitor):
        def __init__(self):
            self.stubs = []

        def visit_FuncDecl(self, node):
            gen = c_generator.CGenerator()
            self.stubs.append(gen.visit(node))

    def test_partial_funcdecl_generation(self):
        src = r'''
            void noop(void);
            void *something(void *thing);
            int add(int x, int y);'''
        ast = parse_to_ast(src)
        v = TestFunctionDeclGeneration._FuncDeclVisitor()
        v.visit(ast)
        self.assertEqual(len(v.stubs), 3)
        self.assertTrue(r'void noop(void)' in v.stubs)
        self.assertTrue(r'void *something(void *thing)' in v.stubs)
        self.assertTrue(r'int add(int x, int y)' in v.stubs)


class TestCtoC(unittest.TestCase):
    def _run_c_to_c(self, src, *args, **kwargs):
        ast = parse_to_ast(src)
        generator = c_generator.CGenerator(*args, **kwargs)
        return generator.visit(ast)

    def _assert_ctoc_correct(self, src, *args, **kwargs):
        """ Checks that the c2c translation was correct by parsing the code
            generated by c2c for src and comparing the AST with the original
            AST.

            Additional arguments are passed to CGenerator.__init__.
        """
        src2 = self._run_c_to_c(src, *args, **kwargs)
        self.assertTrue(compare_asts(parse_to_ast(src), parse_to_ast(src2)),
                        "{!r} != {!r}".format(src, src2))
        return src2

    def test_trivial_decls(self):
        self._assert_ctoc_correct('int a;')
        self._assert_ctoc_correct('int b, a;')
        self._assert_ctoc_correct('int c, b, a;')
        self._assert_ctoc_correct('auto int a;')
        self._assert_ctoc_correct('register int a;')
        self._assert_ctoc_correct('_Thread_local int a;')

    def test_complex_decls(self):
        self._assert_ctoc_correct('int** (*a)(void);')
        self._assert_ctoc_correct('int** (*a)(void*, int);')
        self._assert_ctoc_correct('int (*b)(char * restrict k, float);')
        self._assert_ctoc_correct('int (*b)(char * _Atomic k, float);')
        self._assert_ctoc_correct('int (*b)(char * _Atomic volatile k, float);')
        self._assert_ctoc_correct('int test(const char* const* arg);')
        self._assert_ctoc_correct('int test(const char** const arg);')

    def test_alignment(self):
        self._assert_ctoc_correct('_Alignas(32) int b;')
        self._assert_ctoc_correct('int _Alignas(32) a;')
        self._assert_ctoc_correct('_Alignas(32) _Atomic(int) b;')
        self._assert_ctoc_correct('_Atomic(int) _Alignas(32) b;')
        self._assert_ctoc_correct('_Alignas(long long) int a;')
        self._assert_ctoc_correct('int _Alignas(long long) a;')
        self._assert_ctoc_correct(r'''
            typedef struct node_t {
                _Alignas(64) void* next;
                int data;
            } node;
            ''')
        self._assert_ctoc_correct(r'''
            typedef struct node_t {
                void _Alignas(64) * next;
                int data;
            } node;
            ''')

    def test_ternary(self):
        self._assert_ctoc_correct('''
            int main(void)
            {
                int a, b;
                (a == 0) ? (b = 1) : (b = 2);
            }''')

    def test_casts(self):
        self._assert_ctoc_correct(r'''
            int main() {
                int b = (int) f;
                int c = (int*) f;
            }''')
        self._assert_ctoc_correct(r'''
            int main() {
                int a = (int) b + 8;
                int t = (int) c;
            }
        ''')

    def test_initlist(self):
        self._assert_ctoc_correct('int arr[] = {1, 2, 3};')

    def test_exprs(self):
        self._assert_ctoc_correct('''
            int main(void)
            {
                int a;
                int b = a++;
                int c = ++a;
                int d = a--;
                int e = --a;
            }''')

    def test_statements(self):
        # note two minuses here
        self._assert_ctoc_correct(r'''
            int main() {
                int a;
                a = 5;
                ;
                b = - - a;
                return a;
            }''')

    def test_struct_decl(self):
        self._assert_ctoc_correct(r'''
            typedef struct node_t {
                struct node_t* next;
                int data;
            } node;
            ''')

    def test_krstyle(self):
        self._assert_ctoc_correct(r'''
            int main(argc, argv)
            int argc;
            char** argv;
            {
                return 0;
            }
        ''')

    def test_switchcase(self):
        self._assert_ctoc_correct(r'''
        int main() {
            switch (myvar) {
            case 10:
            {
                k = 10;
                p = k + 1;
                break;
            }
            case 20:
            case 30:
                return 20;
            default:
                break;
            }
        }
        ''')

    def test_nest_initializer_list(self):
        self._assert_ctoc_correct(r'''
        int main()
        {
           int i[1][1] = { { 1 } };
        }''')

    def test_nest_named_initializer(self):
        self._assert_ctoc_correct(r'''struct test
            {
                int i;
                struct test_i_t
                {
                    int k;
                } test_i;
                int j;
            };
            struct test test_var = {.i = 0, .test_i = {.k = 1}, .j = 2};
        ''')

    def test_expr_list_in_initializer_list(self):
        self._assert_ctoc_correct(r'''
        int main()
        {
           int i[1] = { (1, 2) };
        }''')

    def test_issue36(self):
        self._assert_ctoc_correct(r'''
            int main() {
            }''')

    def test_issue37(self):
        self._assert_ctoc_correct(r'''
            int main(void)
            {
              unsigned size;
              size = sizeof(size);
              return 0;
            }''')

    def test_issue66(self):
        # A non-existing body must not be generated
        # (previous valid behavior, still working)
        self._assert_ctoc_correct(r'''
            struct foo;
            ''')
        # An empty body must be generated
        # (added behavior)
        self._assert_ctoc_correct(r'''
            struct foo {};
            ''')

    def test_issue83(self):
        self._assert_ctoc_correct(r'''
            void x(void) {
                int i = (9, k);
            }
            ''')

    def test_issue84(self):
        self._assert_ctoc_correct(r'''
            void x(void) {
                for (int i = 0;;)
                    i;
            }
            ''')

    def test_issue246(self):
        self._assert_ctoc_correct(r'''
            int array[3] = {[0] = 0, [1] = 1, [1+1] = 2};
            ''')

    def test_noreturn(self):
        self._assert_ctoc_correct(r'''
            _Noreturn int x(void) {
                abort();
            }
            ''')

    def test_exprlist_with_semi(self):
        self._assert_ctoc_correct(r'''
            void x() {
                if (i < j)
                    tmp = C[i], C[i] = C[j], C[j] = tmp;
                if (i <= j)
                    i++, j--;
            }
        ''')

    def test_exprlist_with_subexprlist(self):
        self._assert_ctoc_correct(r'''
            void x() {
                (a = b, (b = c, c = a));
            }
        ''')

    def test_comma_operator_funcarg(self):
        self._assert_ctoc_correct(r'''
            void f(int x) { return x; }
            int main(void) { f((1, 2)); return 0; }
        ''')

    def test_comma_op_in_ternary(self):
        self._assert_ctoc_correct(r'''
            void f() {
                (0, 0) ? (0, 0) : (0, 0);
            }
        ''')

    def test_comma_op_assignment(self):
        self._assert_ctoc_correct(r'''
            void f() {
                i = (a, b, c);
            }
        ''')

    def test_pragma(self):
        self._assert_ctoc_correct(r'''
            #pragma foo
            void f() {
                #pragma bar
                i = (a, b, c);
            }
            typedef struct s {
            #pragma baz
           } s;
        ''')

    def test_compound_literal(self):
        self._assert_ctoc_correct('char **foo = (char *[]){ "x", "y", "z" };')
        self._assert_ctoc_correct('int i = ++(int){ 1 };')
        self._assert_ctoc_correct('struct foo_s foo = (struct foo_s){ 1, 2 };')

    def test_enum(self):
        self._assert_ctoc_correct(r'''
            enum e
            {
              a,
              b = 2,
              c = 3
            };
        ''')
        self._assert_ctoc_correct(r'''
            enum f
            {
                g = 4,
                h,
                i
            };
        ''')

    def test_enum_typedef(self):
        self._assert_ctoc_correct('typedef enum EnumName EnumTypedefName;')

    def test_generate_struct_union_enum_exception(self):
        generator = c_generator.CGenerator()
        self.assertRaises(
            AssertionError,
            generator._generate_struct_union_enum,
            n=c_ast.Struct(
                name='TestStruct',
                decls=[],
            ),
            name='',
        )

    def test_array_decl(self):
        self._assert_ctoc_correct('int g(const int a[const 20]){}')
        ast = parse_to_ast('const int a[const 20];')
        generator = c_generator.CGenerator()
        self.assertEqual(generator.visit(ast.ext[0].type),
                         'const int [const 20]')
        self.assertEqual(generator.visit(ast.ext[0].type.type),
                         'const int')

    def test_ptr_decl(self):
        src = 'const int ** const  x;'
        self._assert_ctoc_correct(src)
        ast = parse_to_ast(src)
        generator = c_generator.CGenerator()
        self.assertEqual(generator.visit(ast.ext[0].type),
                         'const int ** const')
        self.assertEqual(generator.visit(ast.ext[0].type.type),
                         'const int *')
        self.assertEqual(generator.visit(ast.ext[0].type.type.type),
                         'const int')

    def test_atomic_qual(self):
        self._assert_ctoc_correct('_Atomic int x;')
        self._assert_ctoc_correct('_Atomic int* x;')
        self._assert_ctoc_correct('int* _Atomic x;')

        # _Atomic specifier gets turned into qualifier.
        s1 = '_Atomic(int) x;'
        c1 = self._run_c_to_c(s1)
        self.assertEqual(c1, '_Atomic int x;\n')
        self._assert_ctoc_correct(s1)

        s2 = '_Atomic(int*) x;'
        c2 = self._run_c_to_c(s2)
        self.assertEqual(c2, 'int * _Atomic x;\n')
        self._assert_ctoc_correct(s2)

        s3 = '_Atomic(_Atomic(int)*) x;'
        c3 = self._run_c_to_c(s3)
        self.assertEqual(c3, '_Atomic int * _Atomic x;\n')
        self._assert_ctoc_correct(s3)

        # TODO: Regeneration with multiple qualifiers is not fully supported.
        # REF: https://github.com/eliben/pycparser/issues/433
        # self._assert_ctoc_correct('auto const _Atomic(int *) a;')

        s4 = 'typedef _Atomic(int) atomic_int;'
        c4 = self._run_c_to_c(s4)
        self.assertEqual(c4, 'typedef _Atomic int atomic_int;\n')
        self._assert_ctoc_correct(s4)

        s5 = 'typedef _Atomic(_Atomic(_Atomic(int (*)(void)) *) *) t;'
        c5 = self._run_c_to_c(s5)
        self.assertEqual(c5, 'typedef int (* _Atomic * _Atomic * _Atomic t)(void);\n')
        self._assert_ctoc_correct(s5)

        self._assert_ctoc_correct(r'''
            typedef struct node_t {
                _Atomic(void*) a;
                _Atomic(void) *b;
                _Atomic void *c;
            } node;
            ''')

    def test_nested_sizeof(self):
        src = '1'
        for _ in range(30):
            src = 'sizeof(' + src + ')'
        src = 'int x = ' + src + ';'
        self._assert_ctoc_correct(src)

    def test_static_assert(self):
        self._assert_ctoc_correct('_Static_assert(sizeof(int) == sizeof(int), "123");')
        self._assert_ctoc_correct('int main() { _Static_assert(sizeof(int) == sizeof(int), "123"); } ')
        self._assert_ctoc_correct('_Static_assert(sizeof(int) == sizeof(int));')

    def test_reduce_parentheses_binaryops(self):
        c1 = 'int x = a + b + c + d;';
        self.assertEqual(self._run_c_to_c(c1), 'int x = ((a + b) + c) + d;\n')
        self.assertEqual(
                self._run_c_to_c(c1, reduce_parentheses=True),
                'int x = a + b + c + d;\n')

        # codes with minimum number of (necessary) parenthesis:
        test_snippets = [
            'int x = a*b*c*d;',
            'int x = a+b*c*d;',
            'int x = a*b+c*d;',
            'int x = a*b*c+d;',
            'int x = (a+b)*c*d;',
            'int x = (a+b)*(c+d);',
            'int x = (a+b)/(c-d);',
            'int x = a+b-c-d;',
            'int x = a+(b-c)-d;'
        ]
        for src in test_snippets:
            src2 = self._assert_ctoc_correct(src, reduce_parentheses=True)
            self.assertTrue(
                src2.count('(') == src.count('('),
                msg="{!r} did not have minimum number of parenthesis, should be like {!r}.".format(
                    src2, src))


class TestCasttoC(unittest.TestCase):
    def _find_file(self, name):
        test_dir = os.path.dirname(__file__)
        name = os.path.join(test_dir, 'c_files', name)
        assert os.path.exists(name)
        return name

    def test_to_type(self):
        src = 'int *x;'
        generator = c_generator.CGenerator()
        test_fun = c_ast.FuncCall(c_ast.ID('test_fun'), c_ast.ExprList([]))

        ast1 = parse_to_ast(src)
        int_ptr_type = ast1.ext[0].type
        int_type = int_ptr_type.type
        self.assertEqual(generator.visit(c_ast.Cast(int_ptr_type, test_fun)),
                         '(int *) test_fun()')
        self.assertEqual(generator.visit(c_ast.Cast(int_type, test_fun)),
                         '(int) test_fun()')

    @unittest.skipUnless(cpp_supported(), 'cpp only works on Unix')
    def test_to_type_with_cpp(self):
        generator = c_generator.CGenerator()
        test_fun = c_ast.FuncCall(c_ast.ID('test_fun'), c_ast.ExprList([]))
        memmgr_path = self._find_file('memmgr.h')

        ast2 = parse_file(memmgr_path, use_cpp=True,
            cpp_path=cpp_path(), cpp_args=cpp_args())
        void_ptr_type = ast2.ext[-3].type.type
        void_type = void_ptr_type.type
        self.assertEqual(generator.visit(c_ast.Cast(void_ptr_type, test_fun)),
                         '(void *) test_fun()')
        self.assertEqual(generator.visit(c_ast.Cast(void_type, test_fun)),
                         '(void) test_fun()')

    def test_nested_else_if_line_breaks(self):
        generator = c_generator.CGenerator()
        test_ast1 = c_ast.If(None, None, None)
        test_ast2 = c_ast.If(None, None, c_ast.If(None, None, None))
        test_ast3 = c_ast.If(None, None, c_ast.If(None, None, c_ast.If(None, None, None)))
        test_ast4 = c_ast.If(None, c_ast.Compound([]), c_ast.If(None, c_ast.Compound([]), c_ast.If(None, c_ast.Compound([]), None)))

        self.assertEqual(generator.visit(test_ast1),
                         'if ()\n  \n')
        self.assertEqual(generator.visit(test_ast2),
                         'if ()\n  \nelse\n  if ()\n  \n')
        self.assertEqual(generator.visit(test_ast3),
                         'if ()\n  \nelse\n  if ()\n  \nelse\n  if ()\n  \n')
        self.assertEqual(generator.visit(test_ast4),
                         'if ()\n{\n}\nelse\n  if ()\n{\n}\nelse\n  if ()\n{\n}\n')

if __name__ == "__main__":
    unittest.main()
