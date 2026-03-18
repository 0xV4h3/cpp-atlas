-- =============================================================
-- CppAtlas Content Patch: OOP & Templates Pack 03 (Part 1)
-- Date: 2026-03-16
-- Questions: 50 (order_index 301..350)
-- Topics: oop, classes, inheritance, templates, best-practices
--
-- IMPORTANT:
-- - No BEGIN/COMMIT (ContentPatchService handles transaction).
-- - Standardized INSERT INTO questions (12 columns).
-- - SQLite-safe escaping only.
-- =============================================================

-- Ensure required tags exist
INSERT OR IGNORE INTO tags (name, color) VALUES
('oop', '#C586C0'),
('classes', '#C586C0'),
('inheritance', '#C586C0'),
('templates', '#BD93F9'),
('best-practices', '#89D185');

-- New quizzes for this part
INSERT OR IGNORE INTO quizzes (title, description, topic_id, difficulty, type, is_active, created_by, version) VALUES
('Class Design Workshop Quiz', 'Class members, constructors, encapsulation and object relationships', 8, 2, 'standard', 1, 0, 1),
('Inheritance Patterns Quiz', 'Base/derived design, access rules, polymorphism basics', 8, 3, 'standard', 1, 0, 1),
('Template Fundamentals Quiz', 'Function/class templates and specialization fundamentals', 9, 3, 'standard', 1, 0, 1);

-- -------------------------------------------------------------
-- Existing Quiz 8 (OOP Quiz): 10 questions (301..310)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'What is encapsulation in OOP?', NULL, 'Encapsulation combines data and behavior and controls access through interfaces.', 1, 10, 301, 1, 'Think data + methods + access control.', 'https://www.learncpp.com/cpp-tutorial/the-benefits-of-data-hiding-encapsulation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=301), 'Bundling data with methods and restricting direct access', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=301), 'Inheriting from multiple bases', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=301), 'Replacing all functions with macros', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=301), 'Automatic memory deallocation only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'true_false', 'Private members are directly accessible from outside the class.', NULL, 'False. Private members can only be accessed by member/friend context.', 1, 10, 302, 1, 'Access specifiers matter.', 'https://www.learncpp.com/cpp-tutorial/public-and-private-members-and-access-specifiers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=302), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=302), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'code_output', 'What is printed?', 'class A{public: A(){ std::cout << "A"; }}; int main(){ A a; }', 'Constructor executes when object is created.', 1, 10, 303, 1, 'Constructor side effect.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-constructors/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=303), 'A', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=303), 'AA', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=303), 'Nothing', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=303), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'What is the role of a destructor?', NULL, 'Destructor runs when object lifetime ends and is used for cleanup.', 1, 10, 304, 1, 'Cleanup at lifetime end.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-destructors/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=304), 'Cleanup when object is destroyed', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=304), 'Allocate object memory', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=304), 'Declare template parameters', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=304), 'Force inlining', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'multi_select', 'Select all valid reasons to use member initializer lists.', NULL, 'They initialize members directly and are required for const/reference members.', 2, 15, 305, 1, 'Init-list before constructor body.', 'https://www.learncpp.com/cpp-tutorial/constructor-member-initializer-lists/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=305), 'Required for reference members', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=305), 'Required for const members', 1, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=305), 'Always slower than assignment in body', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=305), 'Can avoid redundant default-init then assignment', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'fill_blank', 'A constructor that calls another constructor of the same class is called a ____ constructor.', NULL, 'This is called a delegating constructor.', 2, 10, 306, 1, 'C++11 feature.', 'https://www.learncpp.com/cpp-tutorial/delegating-constructors/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=306), 'delegating', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'What does static data member mean?', NULL, 'Static data members are shared across all instances of a class.', 2, 10, 307, 1, 'Per-class, not per-object.', 'https://www.learncpp.com/cpp-tutorial/static-member-variables/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=307), 'One shared member across all objects of the class', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=307), 'Member that cannot be modified', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=307), 'Member that lives only in constructor', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=307), 'Member that exists only in debug builds', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'true_false', 'Friend functions are members of the class they befriend.', NULL, 'False. Friend functions are non-members with granted access.', 2, 10, 308, 1, 'Friend != member.', 'https://www.learncpp.com/cpp-tutorial/friend-non-member-functions/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=308), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=308), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'Which relationship implies strong ownership and lifetime dependency?', NULL, 'Composition implies owner controls lifetime of contained objects.', 2, 10, 309, 1, 'Whole-part with strict lifetime.', 'https://www.learncpp.com/cpp-tutorial/composition/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=309), 'Composition', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=309), 'Association', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=309), 'Inheritance only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=309), 'Dependency injection', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'code_output', 'What is printed?', 'class B{public: B(){ std::cout<<"B"; } ~B(){ std::cout<<"~B"; }}; int main(){ B b; }', 'Constructor then destructor output in scope lifetime.', 2, 10, 310, 1, 'Enter scope then leave scope.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-destructors/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=310), 'B~B', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=310), '~BB', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=310), 'BB', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=310), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- Existing Quiz 9 (Templates Quiz): 10 questions (311..320)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'What is a function template?', NULL, 'A blueprint used by compiler to generate typed function instantiations.', 2, 10, 311, 1, 'Generic function pattern.', 'https://www.studyplan.dev/pro-cpp/function-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=311), 'A generic function blueprint', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=311), 'A runtime type checker', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=311), 'A macro replacement only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=311), 'A virtual-only member function', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'true_false', 'Template specialization lets you provide custom behavior for specific types.', NULL, 'True. Specialization tailors implementation for selected type(s).', 2, 10, 312, 1, 'Specific type override.', 'https://www.studyplan.dev/pro-cpp/template-specialization');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=312), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=312), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'What does template <typename T, std::size_t N> include?', NULL, 'T is a type parameter, N is a non-type template parameter.', 3, 12, 313, 1, 'Type + value parameter.', 'https://www.learncpp.com/cpp-tutorial/template-non-type-parameters/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=313), 'One type and one non-type parameter', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=313), 'Two type parameters only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=313), 'No parameters', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=313), 'Runtime-only arguments', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'multi_select', 'Select all template-related valid statements.', NULL, 'Templates are compile-time mechanisms; definitions are typically visible in headers.', 3, 15, 314, 1, 'Instantiation requires visible definition.', 'https://www.studyplan.dev/pro-cpp/header-file-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=314), 'Template definitions are often in headers', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=314), 'Instantiation usually occurs at compile time', 1, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=314), 'Templates are evaluated only at runtime', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=314), 'Class templates can be specialized', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'fill_blank', 'The keyword used to declare a template type parameter is ____.', NULL, 'Both typename and class are valid in this context; typename is common in explanations.', 1, 10, 315, 1, 'template <typename T>', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=315), 'typename', 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=315), 'class', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'What is partial class template specialization?', NULL, 'Specializing a class template for a subset/pattern of template arguments.', 4, 15, 316, 1, 'Pattern-based specialization.', 'https://www.learncpp.com/cpp-tutorial/partial-template-specialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=316), 'Specialization for a subset pattern of arguments', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=316), 'Specialization of function template only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=316), 'Runtime polymorphism mechanism', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=316), 'Automatic inlining rule', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'true_false', 'Function templates cannot be overloaded.', NULL, 'False. Function templates can be overloaded, subject to resolution rules.', 3, 12, 317, 1, 'Templates participate in overload resolution.', 'https://www.learncpp.com/cpp-tutorial/function-template-specialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=317), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=317), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'code_output', 'What is printed?', 'template<typename T> T id(T x){ return x; }\nstd::cout << id(7);', 'id(7) instantiates T=int and returns 7.', 2, 10, 318, 1, 'Identity template.', 'https://www.studyplan.dev/pro-cpp/function-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=318), '7', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=318), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=318), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=318), 'id', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'Variable templates are used to define ____.', NULL, 'Variable templates define families of variables parameterized by template args.', 3, 12, 319, 1, 'Template for variables, not functions.', 'https://www.studyplan.dev/pro-cpp/variable-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=319), 'Parameterized variables', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=319), 'Only class constructors', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=319), 'Only constexpr functions', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=319), 'Dynamic libraries', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'multi_select', 'Select template best practices.', NULL, 'Clear constraints, understandable names, and targeted specialization improve maintainability.', 3, 15, 320, 1, 'Readability matters for generic code.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=320), 'Use clear parameter names', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=320), 'Document assumptions/constraints', 1, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=320), 'Specialize everything by default', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=320), 'Prefer simple interfaces when possible', 1, 4);

-- -------------------------------------------------------------
-- New Quiz: Class Design Workshop Quiz (10 questions, 321..330)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What does data hiding primarily achieve?', NULL, 'It protects invariants by restricting direct external mutation of internals.', 2, 10, 321, 1, 'Protect internal state.', 'https://www.learncpp.com/cpp-tutorial/the-benefits-of-data-hiding-encapsulation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=321), 'Protecting invariants via controlled access', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=321), 'Removing need for constructors', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=321), 'Making all members public', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=321), 'Eliminating dependencies automatically', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'A copy constructor is invoked when an object is initialized from another object of same type.', NULL, 'True, e.g., T b = a; invokes copy constructor (if not elided/moved).', 2, 10, 322, 1, 'Copy initialization case.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-the-copy-constructor/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=322), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=322), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'When is a default constructor used?', NULL, 'When object is default-initialized and no arguments are provided.', 1, 10, 323, 1, 'No constructor args.', 'https://www.learncpp.com/cpp-tutorial/default-constructors-and-default-arguments/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=323), 'When constructing object without arguments', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=323), 'Only when object is const', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=323), 'Only in derived classes', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=323), 'Never used by compiler', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'class C{public: C(int v):x(v){} int x;}; int main(){ C c(9); std::cout<<c.x; }', 'Initializer list sets x to 9.', 1, 10, 324, 1, 'Member-init list.', 'https://www.learncpp.com/cpp-tutorial/constructor-member-initializer-lists/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=324), '9', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=324), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=324), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=324), 'Undefined', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1), 8, 'multi_select', 'Select correct statements about friend declarations.', NULL, 'Friend grants access but does not imply membership or inheritance.', 3, 15, 325, 1, 'Access permission only.', 'https://www.learncpp.com/cpp-tutorial/friend-classes-and-friend-member-functions/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=325), 'Friend can access private/protected members', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=325), 'Friendship is not transitive by default', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=325), 'Friend function becomes class member automatically', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=325), 'Friendship replaces encapsulation entirely', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1), 8, 'fill_blank', 'In C++, access specifier used to expose class API publicly is ____.', NULL, 'The public access specifier exposes members to external users.', 1, 10, 326, 1, 'public/private/protected trio.', 'https://www.learncpp.com/cpp-tutorial/public-and-private-members-and-access-specifiers/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=326), 'public', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'Which relationship is typically weak and non-owning?', NULL, 'Association is typically non-owning relationship.', 2, 10, 327, 1, 'Compare with composition/aggregation.', 'https://www.learncpp.com/cpp-tutorial/association/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=327), 'Association', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=327), 'Composition', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=327), 'Class template', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=327), 'Static storage', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'Aggregation is usually modeled as a has-a relationship where part can outlive whole.', NULL, 'True. Aggregated object may exist independently.', 2, 10, 328, 1, 'Independent lifetime possibility.', 'https://www.learncpp.com/cpp-tutorial/aggregation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=328), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=328), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What does class invariant mean?', NULL, 'A condition that should remain true for object state during valid lifetime/operations.', 3, 12, 329, 1, 'Consistency rule for state.', 'https://www.learncpp.com/cpp-tutorial/the-benefits-of-data-hiding-encapsulation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=329), 'A rule that object state must satisfy', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=329), 'A compiler optimization flag', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=329), 'A template alias', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=329), 'A global variable naming rule', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'class D{public: static int c; D(){++c;}}; int D::c=0; int main(){D a; D b; std::cout<<D::c;}', 'Static member shared across instances; two constructions -> 2.', 3, 12, 330, 1, 'Shared counter.', 'https://www.learncpp.com/cpp-tutorial/static-member-variables/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=330), '2', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=330), '1', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=330), '0', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Design Workshop Quiz' ORDER BY id DESC LIMIT 1) AND order_index=330), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Inheritance Patterns Quiz (10 questions, 331..340)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'Inheritance models which relationship?', NULL, 'Inheritance models an is-a relationship.', 1, 10, 331, 1, 'Base/derived semantics.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-inheritance/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=331), 'is-a', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=331), 'has-a only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=331), 'part-of only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=331), 'friend-of', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'Protected members are accessible in derived classes.', NULL, 'True (subject to inheritance/access context).', 2, 10, 332, 1, 'protected visibility.', 'https://www.studyplan.dev/intro-to-programming/protected-members');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=332), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=332), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'Which constructor runs first when constructing a derived object?', NULL, 'Base subobject is constructed before derived constructor body.', 2, 10, 333, 1, 'Construction order.', 'https://www.learncpp.com/cpp-tutorial/order-of-construction-of-derived-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=333), 'Base class constructor', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=333), 'Derived constructor body first', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=333), 'Destructor first', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=333), 'Undefined order', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'struct B{B(){std::cout<<"B";}}; struct D:B{D(){std::cout<<"D";}}; int main(){D d;}', 'Base then derived output: BD.', 2, 10, 334, 1, 'Ctor order base->derived.', 'https://www.learncpp.com/cpp-tutorial/constructors-and-initialization-of-derived-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=334), 'BD', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=334), 'DB', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=334), 'B', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=334), 'D', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1), 8, 'multi_select', 'Select valid facts about inheritance access specifiers.', NULL, 'Public/protected/private inheritance transform accessibility of base public/protected members in derived interface.', 3, 15, 335, 1, 'Interface visibility changes.', 'https://www.learncpp.com/cpp-tutorial/inheritance-and-access-specifiers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=335), 'Public inheritance keeps base public members public in derived interface', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=335), 'Private inheritance makes inherited public/protected members private to outside interface', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=335), 'Private base members become directly accessible in derived', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=335), 'Protected inheritance exposes inherited members as protected externally', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1), 8, 'fill_blank', 'A class that contains at least one pure virtual function is called ____ class.', NULL, 'Such class is abstract and cannot be instantiated directly.', 3, 12, 336, 1, 'Pure virtual => abstract.', 'https://www.studyplan.dev/pro-cpp/pure-virtual');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=336), 'abstract', 1),
((SELECT id FROM questions WHERE order_index=336), 'an abstract', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What is multiple inheritance?', NULL, 'A class inheriting from more than one base class.', 3, 12, 337, 1, 'Two or more bases.', 'https://www.learncpp.com/cpp-tutorial/multiple-inheritance/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=337), 'Inheriting from more than one base class', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=337), 'Having multiple constructors', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=337), 'Using multiple namespaces', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=337), 'Having multiple files', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'Base class destructor should typically be virtual when deleting derived objects through base pointer.', NULL, 'True for polymorphic base used via base pointer delete.', 4, 15, 338, 1, 'Polymorphic deletion safety.', 'https://www.studyplan.dev/pro-cpp/pure-virtual');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=338), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=338), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What does inherited members page mainly emphasize?', NULL, 'Derived classes inherit accessible members but access and hiding rules apply.', 2, 10, 339, 1, 'Inherited visibility and lookup.', 'https://www.studyplan.dev/intro-to-programming/inherited-members');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=339), 'Inherited members follow access and lookup rules', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=339), 'Derived classes never inherit functions', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=339), 'Private members become public automatically', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=339), 'Inheritance disables constructors', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'struct B{int x{1};}; struct D:B{void f(){ std::cout<<x; }}; int main(){ D d; d.f(); }', 'D inherits B::x and can access it in member function.', 2, 10, 340, 1, 'Inherited data member.', 'https://www.learncpp.com/cpp-tutorial/basic-inheritance-in-c/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=340), '1', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=340), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=340), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Patterns Quiz' ORDER BY id DESC LIMIT 1) AND order_index=340), 'Undefined', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Template Fundamentals Quiz (10 questions, 341..350)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'Class template primarily provides what?', NULL, 'A generic class blueprint parameterized by types/values.', 2, 10, 341, 1, 'Generic class pattern.', 'https://www.learncpp.com/cpp-tutorial/template-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=341), 'A generic blueprint for classes', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=341), 'A runtime object factory only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=341), 'A namespace alias', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=341), 'A preprocessor extension', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 9, 'true_false', 'typename and class are interchangeable in many template type-parameter declarations.', NULL, 'True for type-parameter declaration contexts.', 2, 10, 342, 1, 'template <typename T> vs template <class T>.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=342), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=342), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 9, 'code_output', 'What is printed?', 'template<typename T> T add(T a,T b){return a+b;} int main(){ std::cout<<add<int>(2,3); }', 'Explicit template argument int => 5.', 2, 10, 343, 1, 'Explicit instantiation at call site.', 'https://www.studyplan.dev/pro-cpp/function-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=343), '5', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=343), '23', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=343), '0', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=343), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'What is full specialization?', NULL, 'Providing implementation for one exact template argument set.', 3, 12, 344, 1, 'Exact type/value combination.', 'https://www.learncpp.com/cpp-tutorial/class-template-specialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=344), 'Specialization for one exact argument set', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=344), 'Specialization for broad subset only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=344), 'Runtime virtual dispatch', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=344), 'Type alias only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 9, 'multi_select', 'Select valid uses of type aliases in template-heavy code.', NULL, 'Aliases improve readability and reduce verbosity for complex types.', 2, 15, 345, 1, 'using/typedef readability.', 'https://www.studyplan.dev/pro-cpp/type-alias');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=345), 'Improve readability of long template types', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=345), 'Create semantic names for domain types', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=345), 'Change runtime behavior automatically', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=345), 'Replace all template parameters', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 9, 'fill_blank', 'Complete: A member function template belongs to a ____ template or non-template class.', NULL, 'Member function templates can appear in classes and class templates.', 3, 12, 346, 1, 'Template inside class context.', 'https://www.studyplan.dev/pro-cpp/member-function-templates');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=346), 'class', 1),
((SELECT id FROM questions WHERE order_index=346), 'class template', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'What problem does template specialization for pointers often solve?', NULL, 'Pointer types may need distinct behavior (ownership/format/traits handling).', 4, 15, 347, 1, 'Pointer-specific semantics.', 'https://www.learncpp.com/cpp-tutorial/partial-template-specialization-for-pointers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=347), 'Handling pointer-specific behavior differently', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=347), 'Removing need for headers', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=347), 'Disabling compiler type checking', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=347), 'Replacing inheritance completely', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 9, 'true_false', 'Templates can increase compile times in large codebases if used excessively.', NULL, 'True; heavy instantiation can increase build time and binary size.', 3, 12, 348, 1, 'Compile-time tradeoff.', 'https://www.learncpp.com/cpp-tutorial/dependencies/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=348), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=348), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 9, 'code_output', 'What is printed?', 'template<typename T> struct Box{ T v; }; int main(){ Box<int> b{4}; std::cout<<b.v; }', 'Box<int> stores int value 4.', 2, 10, 349, 1, 'Class template instantiation.', 'https://www.studyplan.dev/pro-cpp/class-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=349), '4', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=349), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=349), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=349), 'Box', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'Best-practice approach for template APIs in educational codebases?', NULL, 'Keep interfaces minimal, document constraints, and provide readable examples.', 2, 10, 350, 1, 'Teachability + maintainability.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=350), 'Small clear interface with documented constraints', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=350), 'Expose all internals publicly by default', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=350), 'Avoid examples in docs', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Fundamentals Quiz' ORDER BY id DESC LIMIT 1) AND order_index=350), 'Use specialization before base template exists', 0, 4);

-- -------------------------------------------------------------
-- question_tags (primary)
-- -------------------------------------------------------------
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id,
       (SELECT id FROM tags WHERE name = CASE
            WHEN q.order_index BETWEEN 301 AND 310 THEN 'oop'
            WHEN q.order_index BETWEEN 311 AND 320 THEN 'templates'
            WHEN q.order_index BETWEEN 321 AND 330 THEN 'classes'
            WHEN q.order_index BETWEEN 331 AND 340 THEN 'inheritance'
            WHEN q.order_index BETWEEN 341 AND 350 THEN 'templates'
            ELSE 'best-practices'
       END)
FROM questions q
WHERE q.order_index BETWEEN 301 AND 350;

-- Secondary tags to improve coverage balance
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=305), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=309), (SELECT id FROM tags WHERE name='classes')),
((SELECT id FROM questions WHERE order_index=320), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=325), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=330), (SELECT id FROM tags WHERE name='oop')),
((SELECT id FROM questions WHERE order_index=335), (SELECT id FROM tags WHERE name='classes')),
((SELECT id FROM questions WHERE order_index=338), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=345), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=350), (SELECT id FROM tags WHERE name='best-practices'));

-- Derive quiz_tags for affected quizzes
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 301 AND 350;