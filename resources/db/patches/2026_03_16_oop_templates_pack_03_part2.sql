-- =============================================================
-- CppAtlas Content Patch: OOP & Templates Pack 03 (Part 2)
-- Date: 2026-03-16
-- Questions: 50 (order_index 351..400)
-- Topics: oop, classes, inheritance, templates, best-practices
-- =============================================================

INSERT OR IGNORE INTO tags (name, color) VALUES
('oop', '#C586C0'),
('classes', '#C586C0'),
('inheritance', '#C586C0'),
('templates', '#BD93F9'),
('best-practices', '#89D185');

INSERT OR IGNORE INTO quizzes (title, description, topic_id, difficulty, type, is_active, created_by, version) VALUES
('Class Lifecycle Mastery Quiz', 'Copy/move basics, construction patterns, and object lifecycle', 8, 3, 'standard', 1, 0, 1),
('Inheritance & Polymorphism Practice Quiz', 'Overriding, abstract classes, and inheritance design choices', 8, 3, 'standard', 1, 0, 1),
('Template Patterns Practice Quiz', 'Specialization, deduction, and generic design patterns', 9, 4, 'standard', 1, 0, 1);

-- -------------------------------------------------------------
-- Existing Quiz 8 (OOP Quiz): 10 questions (351..360)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'What is the main purpose of access specifiers in classes?', NULL, 'They define visibility boundaries and protect internal invariants.', 1, 10, 351, 1, 'Think API surface control.', 'https://www.learncpp.com/cpp-tutorial/public-and-private-members-and-access-specifiers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=351), 'Control visibility of members', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=351), 'Force compiler optimization', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=351), 'Create template instances', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=351), 'Replace constructors', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'true_false', 'A class can have multiple constructors with different parameter lists.', NULL, 'True. This is constructor overloading.', 1, 10, 352, 1, 'Overloading applies to constructors too.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-constructors/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=352), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=352), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'code_output', 'What is printed?', 'class X{public: X(){std::cout<<"X";} X(const X&){std::cout<<"C";}}; int main(){ X a; X b=a; }', 'Default constructor prints X, copy constructor prints C.', 2, 10, 353, 1, 'Construction + copy init.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-the-copy-constructor/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=353), 'XC', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=353), 'XX', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=353), 'CX', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=353), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'Which statement about destructor overloading is correct?', NULL, 'A class has only one destructor; it cannot be overloaded.', 2, 10, 354, 1, 'Destructor signature is fixed.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-destructors/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=354), 'Destructors cannot be overloaded', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=354), 'Destructors may take int parameter', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=354), 'Each constructor needs separate destructor', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=354), 'Destructor must be static', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'multi_select', 'Select valid statements about default constructors.', NULL, 'They can be user-defined, compiler-generated, or deleted.', 2, 15, 355, 1, 'Think generated vs user-provided.', 'https://www.learncpp.com/cpp-tutorial/default-constructors-and-default-arguments/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=355), 'Compiler may generate one', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=355), 'You can explicitly delete one', 1, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=355), 'It can have required non-default parameters', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=355), 'It can be user-defined', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'fill_blank', 'Member initializer lists run ____ constructor body execution.', NULL, 'They execute before constructor body.', 2, 10, 356, 1, 'Initialization phase ordering.', 'https://www.learncpp.com/cpp-tutorial/constructor-member-initializer-lists/');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'Why are delegating constructors useful?', NULL, 'They reduce duplicated initialization logic across constructors.', 2, 10, 357, 1, 'DRY for constructors.', 'https://www.learncpp.com/cpp-tutorial/delegating-constructors/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=357), 'To reuse initialization logic inside the same class', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=357), 'To bypass access control', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=357), 'To inherit from templates only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=357), 'To avoid constructors entirely', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'true_false', 'Static member variables are allocated per-object instance.', NULL, 'False. They are allocated once per class.', 2, 10, 358, 1, 'Shared storage.', 'https://www.learncpp.com/cpp-tutorial/static-member-variables/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=358), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=358), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'Which relationship is best when an object should not own another object''s lifetime?', NULL, 'Association or aggregation are non-owning relative to composition.', 3, 12, 359, 1, 'Ownership semantics.', 'https://www.learncpp.com/cpp-tutorial/object-relationships/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=359), 'Association', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=359), 'Composition', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=359), 'Private inheritance always', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=359), 'Friendship', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'code_output', 'What is printed?', 'class E{public: E(){std::cout<<"1";} E(int){std::cout<<"2";}}; int main(){ E e; E f(0); }', 'Default ctor prints 1 then int ctor prints 2.', 2, 10, 360, 1, 'Ctor overload selection.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-constructors/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=360), '12', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=360), '21', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=360), '11', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=360), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- Existing Quiz 9 (Templates Quiz): 10 questions (361..370)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'What is template argument deduction?', NULL, 'Compiler infers template parameters from function call arguments.', 2, 10, 361, 1, 'Inference from call-site types.', 'https://www.studyplan.dev/pro-cpp/function-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=361), 'Inferring template params from call arguments', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=361), 'Runtime JIT type analysis', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=361), 'Macro text replacement only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=361), 'Debugger-only feature', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'true_false', 'Class templates may have member function templates inside them.', NULL, 'True. This is common in generic containers/adapters.', 3, 12, 362, 1, 'Template inside template.', 'https://www.studyplan.dev/pro-cpp/member-function-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=362), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=362), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'code_output', 'What is printed?', 'template<typename T> T max2(T a,T b){ return (a>b)?a:b; }\nstd::cout<<max2(3,8);', 'Deduced T=int; returns 8.', 2, 10, 363, 1, 'Simple generic compare.', 'https://www.learncpp.com/cpp-tutorial/function-template-specialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=363), '8', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=363), '3', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=363), '38', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=363), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'Why are template definitions commonly placed in header files?', NULL, 'Compiler needs full template definition at instantiation point.', 3, 12, 364, 1, 'Instantiation visibility.', 'https://www.studyplan.dev/pro-cpp/header-file-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=364), 'Because instantiation requires visible definition', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=364), 'Because linker cannot process .cpp files', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=364), 'Because templates are runtime scripts', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=364), 'Only style preference, never required', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'multi_select', 'Select valid kinds of template parameters.', NULL, 'Type, non-type (value), and template-template parameters are valid categories.', 4, 15, 365, 1, 'Parameter categories.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=365), 'Type parameters', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=365), 'Non-type parameters', 1, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=365), 'Template template parameters', 1, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=365), 'Destructor parameters', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'fill_blank', 'A specialization that handles some, but not all, template argument patterns is called ____ specialization.', NULL, 'That is partial specialization.', 3, 12, 366, 1, 'Between primary and full specialization.', 'https://www.learncpp.com/cpp-tutorial/partial-template-specialization/');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'Which statement about function template partial specialization is correct?', NULL, 'Function templates do not support partial specialization directly; overloading is used instead.', 4, 15, 367, 1, 'Function template nuance.', 'https://www.learncpp.com/cpp-tutorial/function-template-specialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=367), 'Use overloads instead; partial specialization is for class templates', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=367), 'Function templates fully support partial specialization syntax', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=367), 'Only constexpr function templates can be specialized', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=367), 'Specialization is forbidden for all templates', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'true_false', 'Template code can increase binary size due to multiple instantiations.', NULL, 'True. Multiple distinct type instantiations may generate more code.', 3, 12, 368, 1, 'Code bloat tradeoff.', 'https://www.learncpp.com/cpp-tutorial/dependencies/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=368), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=368), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'code_output', 'What is printed?', 'template<typename T> struct V{ static constexpr int n=1; };\nstd::cout<<V<int>::n;', 'Accessing static constexpr template member yields 1.', 3, 12, 369, 1, 'Class template static member.', 'https://www.studyplan.dev/pro-cpp/variable-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=369), '1', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=369), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=369), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=369), 'V', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'Template best-practice for educational code is to ____.', NULL, 'Prefer small, well-documented abstractions with clear examples and constraints.', 2, 10, 370, 1, 'Readability first.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=370), 'Keep generic interfaces simple and documented', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=370), 'Maximize clever metaprogramming always', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=370), 'Hide all template constraints', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=370), 'Avoid tests for template code', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Class Lifecycle Mastery Quiz (10 questions, 371..380)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What does Rule of Zero encourage?', NULL, 'Prefer designs where compiler-generated special members are sufficient.', 3, 12, 371, 1, 'Resource ownership via RAII helpers.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-the-copy-constructor/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=371), 'Rely on generated special members when possible', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=371), 'Always write destructor manually', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=371), 'Disable constructors', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=371), 'Use only global variables', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'A user-declared copy constructor may suppress implicit move constructor generation in some cases.', NULL, 'True under special member generation rules.', 4, 15, 372, 1, 'Special member interactions.', 'https://cppreference.com/w/cpp/language/classes.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=372), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=372), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'class M{public: M(){std::cout<<"M";} ~M(){std::cout<<"D";}}; int main(){ M m; }', 'Constructor then destructor in reverse lifetime.', 2, 10, 373, 1, 'Object scope lifetime.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-destructors/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=373), 'MD', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=373), 'DM', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=373), 'M', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=373), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What is object slicing?', NULL, 'Copying derived object into base object by value, losing derived-specific part.', 3, 12, 374, 1, 'Base-by-value pitfall.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-inheritance/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=374), 'Losing derived part when copied into base by value', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=374), 'Cutting arrays at runtime', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=374), 'Virtual dispatch failure only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=374), 'Template specialization issue', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'multi_select', 'Select best practices for class invariants.', NULL, 'Validate constructor inputs, keep state private, and expose controlled mutators.', 3, 15, 375, 1, 'Invariant enforcement patterns.', 'https://www.learncpp.com/cpp-tutorial/the-benefits-of-data-hiding-encapsulation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=375), 'Validate state at boundaries', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=375), 'Expose all fields as public for speed', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=375), 'Use member functions to preserve consistency', 1, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=375), 'Document assumptions', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'fill_blank', 'A function with the same name as its class and no return type is a ____.', NULL, 'That is a constructor.', 1, 10, 376, 1, 'Creation-time function.', 'https://www.studyplan.dev/intro-to-programming/constructors');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'When is copy constructor typically called?', NULL, 'During pass-by-value, return-by-value (subject to elision), and copy init.', 2, 10, 377, 1, 'Think value semantics paths.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-the-copy-constructor/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=377), 'Object initialization from same type by value', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=377), 'Only when destructor runs', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=377), 'Only in templates', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=377), 'Never if constructor exists', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'Delegating constructors can call base class constructors directly.', NULL, 'False. Delegating constructor calls another constructor in same class; base ctor call is via initializer list in selected constructor.', 3, 12, 378, 1, 'Same-class delegation.', 'https://www.learncpp.com/cpp-tutorial/delegating-constructors/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=378), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=378), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'Which statement best reflects robust class design?', NULL, 'Small cohesive classes with clear responsibilities are easier to test and maintain.', 2, 10, 379, 1, 'Single responsibility principle.', 'https://www.studyplan.dev/intro-to-programming/classes');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=379), 'Clear responsibilities and minimal public surface', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=379), 'Expose internals to avoid getters/setters', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=379), 'Store all state as globals', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=379), 'Prefer cyclic dependencies', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'class Z{public: Z(){std::cout<<"z";}}; int main(){ Z a; Z b; }', 'Two objects created => constructor runs twice.', 1, 10, 380, 1, 'Object count == ctor calls.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=380), 'zz', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=380), 'z', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=380), 'zzz', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Class Lifecycle Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=380), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Inheritance & Polymorphism Practice Quiz (10 questions, 381..390)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What enables runtime polymorphism in C++?', NULL, 'Virtual functions + base reference/pointer dispatch.', 2, 10, 381, 1, 'dynamic dispatch.', 'https://www.studyplan.dev/pro-cpp/pure-virtual');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=381), 'virtual functions', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=381), 'templates only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=381), 'friend classes only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=381), 'static member variables', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'A pure virtual function is declared with = 0.', NULL, 'True.', 2, 10, 382, 1, 'Syntax detail.', 'https://www.studyplan.dev/pro-cpp/pure-virtual');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=382), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=382), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'struct B{virtual void f(){std::cout<<"B";}}; struct D:B{void f() override {std::cout<<"D";}}; int main(){ B* p=new D; p->f(); }', 'Virtual dispatch calls D::f.', 3, 12, 383, 1, 'override through base pointer.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-inheritance/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=383), 'D', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=383), 'B', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=383), 'BD', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=383), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What does override keyword help with?', NULL, 'It asks compiler to verify function overrides a virtual base function.', 3, 12, 384, 1, 'Compile-time correctness check.', 'https://www.studyplan.dev/pro-cpp/pure-virtual');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=384), 'Ensures intended virtual override is valid', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=384), 'Makes function static', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=384), 'Disables vtable', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=384), 'Forces inline expansion', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1), 8, 'multi_select', 'Select pitfalls of inheritance misuse.', NULL, 'Deep hierarchies, weak is-a modeling, and fragile base assumptions are common pitfalls.', 4, 15, 385, 1, 'Modeling quality matters.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-inheritance/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=385), 'Using inheritance where composition is better', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=385), 'Very deep and rigid hierarchies', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=385), 'Clear interface boundaries', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=385), 'Ignoring base-class invariants', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1), 8, 'fill_blank', 'A class with at least one pure virtual function cannot be directly ____.', NULL, 'It cannot be instantiated.', 2, 10, 386, 1, 'Abstract class rule.', 'https://www.studyplan.dev/pro-cpp/pure-virtual');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'Which is true about base references?', NULL, 'Base references can bind to derived objects and enable polymorphic calls for virtual funcs.', 3, 12, 387, 1, 'Reference polymorphism.', 'https://www.learncpp.com/cpp-tutorial/basic-inheritance-in-c/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=387), 'They can refer to derived objects', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=387), 'They always slice objects', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=387), 'They cannot call virtual functions', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=387), 'They must be nullptr-initialized', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'Private inheritance still means derived has access to inherited base interface internally (subject to access rules).', NULL, 'True, but public interface exposure differs.', 3, 12, 388, 1, 'Internal use vs external API.', 'https://www.learncpp.com/cpp-tutorial/inheritance-and-access-specifiers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=388), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=388), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'Best practice when designing polymorphic base classes?', NULL, 'Provide virtual destructor and clear stable interface contract.', 3, 12, 389, 1, 'Lifetime + contract.', 'https://www.studyplan.dev/pro-cpp/pure-virtual');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=389), 'Virtual destructor + clear interface', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=389), 'Expose all data as public', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=389), 'Disable all overrides', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=389), 'Avoid base classes entirely always', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'struct B{virtual ~B(){} virtual void f(){std::cout<<"B";}}; struct D:B{void f() override{std::cout<<"D";}}; int main(){ B& r=*new D; r.f(); }', 'Reference dispatches to D::f due to virtual override.', 3, 12, 390, 1, 'Virtual call via reference.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-inheritance/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=390), 'D', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=390), 'B', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=390), 'BD', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance & Polymorphism Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=390), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Template Patterns Practice Quiz (10 questions, 391..400)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'What is primary template?', NULL, 'The main generic template definition before specializations.', 2, 10, 391, 1, 'Base template form.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=391), 'The initial generic template definition', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=391), 'A runtime-generated function', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=391), 'A specialized destructor', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=391), 'A namespace alias', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1), 9, 'true_false', 'Class templates can be partially specialized, function templates cannot.', NULL, 'True (function templates use overloading instead).', 4, 15, 392, 1, 'Key distinction.', 'https://www.learncpp.com/cpp-tutorial/partial-template-specialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=392), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=392), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1), 9, 'code_output', 'What is printed?', 'template<typename T> struct S{ static constexpr int v=2; }; int main(){ std::cout<<S<double>::v; }', 'v is constexpr static member =2 for any T unless specialized.', 2, 10, 393, 1, 'Primary template static value.', 'https://www.studyplan.dev/pro-cpp/class-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=393), '2', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=393), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=393), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=393), 'S', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'Template non-type parameter is best described as ____.', NULL, 'A compile-time constant parameter (e.g., int N).', 3, 12, 394, 1, 'Value parameter.', 'https://www.learncpp.com/cpp-tutorial/template-non-type-parameters/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=394), 'Compile-time constant template parameter', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=394), 'Runtime user input parameter', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=394), 'Pointer-only parameter', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=394), 'Preprocessor macro value', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1), 9, 'multi_select', 'Select good template design practices.', NULL, 'Clear constraints, simpler APIs, and meaningful diagnostics improve usability.', 3, 15, 395, 1, 'Usability of generic code.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=395), 'Constrain assumptions/document requirements', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=395), 'Prefer understandable names', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=395), 'Maximize metaprogram complexity by default', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=395), 'Provide focused examples', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1), 9, 'fill_blank', 'Specialization for exactly one template argument set is called ____ specialization.', NULL, 'That is full specialization.', 3, 12, 396, 1, 'Exact-case specialization.', 'https://www.learncpp.com/cpp-tutorial/class-template-specialization/');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'Why do dependencies matter in template-heavy code?', NULL, 'Templates can amplify compile dependencies and rebuild cost.', 3, 12, 397, 1, 'Build scalability concern.', 'https://www.learncpp.com/cpp-tutorial/dependencies/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=397), 'They can increase rebuild/compile time impact', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=397), 'They only affect runtime speed', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=397), 'They are irrelevant for headers', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=397), 'They apply only to virtual classes', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1), 9, 'true_false', 'Type aliases can improve readability for nested template types.', NULL, 'True.', 2, 10, 398, 1, 'using helps readability.', 'https://www.studyplan.dev/pro-cpp/type-alias');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=398), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=398), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1), 9, 'code_output', 'What is printed?', 'template<typename T> T sq(T x){ return x*x; }\nstd::cout<<sq(4);', 'sq(4) => 16.', 2, 10, 399, 1, 'Generic square.', 'https://www.studyplan.dev/pro-cpp/function-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=399), '16', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=399), '8', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=399), '4', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=399), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'Best-practice for introducing templates in education?', NULL, 'Start from concrete implementation, then generalize progressively.', 2, 10, 400, 1, 'Concept-first teaching sequence.', 'https://www.learncpp.com/cpp-tutorial/template-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=400), 'Teach concrete case first, then generalize', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=400), 'Begin with advanced specialization edge cases only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=400), 'Avoid examples completely', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Patterns Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=400), 'Hide all constraints and assumptions', 0, 4);

-- -------------------------------------------------------------
-- Primary question_tags mapping
-- -------------------------------------------------------------
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id, (SELECT id FROM tags WHERE name = CASE
    WHEN q.order_index BETWEEN 351 AND 360 THEN 'oop'
    WHEN q.order_index BETWEEN 361 AND 370 THEN 'templates'
    WHEN q.order_index BETWEEN 371 AND 380 THEN 'classes'
    WHEN q.order_index BETWEEN 381 AND 390 THEN 'inheritance'
    WHEN q.order_index BETWEEN 391 AND 400 THEN 'templates'
    ELSE 'best-practices'
END)
FROM questions q
WHERE q.order_index BETWEEN 351 AND 400;

-- Secondary tags
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=355), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=359), (SELECT id FROM tags WHERE name='classes')),
((SELECT id FROM questions WHERE order_index=370), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=375), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=379), (SELECT id FROM tags WHERE name='oop')),
((SELECT id FROM questions WHERE order_index=385), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=389), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=395), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=400), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=384), (SELECT id FROM tags WHERE name='classes'));

-- Derive quiz_tags for affected quizzes
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 351 AND 400;