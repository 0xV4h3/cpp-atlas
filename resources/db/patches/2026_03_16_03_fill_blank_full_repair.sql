-- Patch 03: Full repair of all fill_blank questions.
-- Inserts canonical short-token accepted answers for every fill_blank question
-- that was reported missing a fill_blank_answers entry by the validator.
--
-- Answers are derived from each question's explanation text.
-- All inserts use INSERT OR IGNORE, so the patch is safe to re-apply.
-- Questions are targeted by (type='fill_blank', order_index) which is a stable
-- surrogate key independent of auto-incremented row IDs.
--
-- Safety guard
CREATE TABLE IF NOT EXISTS fill_blank_answers (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    question_id INTEGER NOT NULL REFERENCES questions(id) ON DELETE CASCADE,
    answer      TEXT    NOT NULL,
    is_active   INTEGER DEFAULT 1,
    order_index INTEGER DEFAULT 0
);
CREATE INDEX IF NOT EXISTS idx_fill_blank_answers_qid
    ON fill_blank_answers(question_id);

-- ── Foundations Pack 01 ──────────────────────────────────────────────────────

-- order_index=109: "The keyword used for explicit compile-time checked conversion is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'static_cast', 0 FROM questions WHERE type='fill_blank' AND order_index=109;

-- order_index=118: "The statement used to exit the nearest loop immediately is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'break', 0 FROM questions WHERE type='fill_blank' AND order_index=118;

-- order_index=126: "The operator that computes remainder is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, '%', 0 FROM questions WHERE type='fill_blank' AND order_index=126;

-- order_index=138: "A named storage location in a program is called a ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'variable', 0 FROM questions WHERE type='fill_blank' AND order_index=138;

-- order_index=149: "The stream used for standard output in C++ is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'std::cout', 0 FROM questions WHERE type='fill_blank' AND order_index=149;
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'cout', 1 FROM questions WHERE type='fill_blank' AND order_index=149;

-- ── Core C++ Pack 02 ─────────────────────────────────────────────────────────

-- order_index=206: "Complete: A function that returns no value has return type ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'void', 0 FROM questions WHERE type='fill_blank' AND order_index=206;

-- order_index=214: "For int a[3][4], the number of columns is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, '4', 0 FROM questions WHERE type='fill_blank' AND order_index=214;

-- order_index=223: "Complete: A pointer that points nowhere should be initialized with ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'nullptr', 0 FROM questions WHERE type='fill_blank' AND order_index=223;

-- order_index=230: "Complete: A function that calls itself is called ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'recursive', 0 FROM questions WHERE type='fill_blank' AND order_index=230;

-- order_index=238: "Complete: int* const p means p is a ____ pointer."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'const', 0 FROM questions WHERE type='fill_blank' AND order_index=238;

-- order_index=246: "Complete: In int a[2][3], there are ____ rows."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, '2', 0 FROM questions WHERE type='fill_blank' AND order_index=246;

-- ── OOP & Templates Pack 03 Part 1 ───────────────────────────────────────────

-- order_index=306: "A constructor that calls another constructor of the same class is called ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'delegating', 0 FROM questions WHERE type='fill_blank' AND order_index=306;

-- order_index=315: "The keyword used to declare a template type parameter is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'typename', 0 FROM questions WHERE type='fill_blank' AND order_index=315;
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'class', 1 FROM questions WHERE type='fill_blank' AND order_index=315;

-- order_index=326: "In C++, access specifier used to expose class API publicly is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'public', 0 FROM questions WHERE type='fill_blank' AND order_index=326;

-- order_index=336: "A class that contains at least one pure virtual function is called ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'abstract', 0 FROM questions WHERE type='fill_blank' AND order_index=336;

-- order_index=346: "A member function template belongs to a ____ template or non-template class."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'class', 0 FROM questions WHERE type='fill_blank' AND order_index=346;

-- ── OOP & Templates Pack 03 Part 2 ───────────────────────────────────────────

-- order_index=356: "Member initializer lists run ____ constructor body execution."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'before', 0 FROM questions WHERE type='fill_blank' AND order_index=356;

-- order_index=366: "A specialization that handles some, but not all, template argument patterns is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'partial', 0 FROM questions WHERE type='fill_blank' AND order_index=366;

-- order_index=376: "A function with the same name as its class and no return type is a ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'constructor', 0 FROM questions WHERE type='fill_blank' AND order_index=376;

-- order_index=386: "A class with at least one pure virtual function cannot be directly ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'instantiated', 0 FROM questions WHERE type='fill_blank' AND order_index=386;

-- order_index=396: "Specialization for exactly one template argument set is called ____ specialization."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'full', 0 FROM questions WHERE type='fill_blank' AND order_index=396;
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'explicit', 1 FROM questions WHERE type='fill_blank' AND order_index=396;

-- ── OOP & Templates Pack 03 Part 3 ───────────────────────────────────────────

-- order_index=406: "A function allowed to access private members without being class member is a ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'friend', 0 FROM questions WHERE type='fill_blank' AND order_index=406;

-- order_index=416: "Template code clarity often improves with type ____ declarations (using ...)."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'alias', 0 FROM questions WHERE type='fill_blank' AND order_index=416;

-- order_index=426: "A relationship where objects cooperate but neither owns the other is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'association', 0 FROM questions WHERE type='fill_blank' AND order_index=426;

-- order_index=436: "Access specifier in inheritance that preserves base public members as public is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'public', 0 FROM questions WHERE type='fill_blank' AND order_index=436;

-- order_index=446: "When compiler infers T from arguments in template call, this is called ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'deduction', 0 FROM questions WHERE type='fill_blank' AND order_index=446;

-- ── OOP & Templates Pack 03 Part 4 ───────────────────────────────────────────

-- order_index=456: "A class member declared with keyword static is shared per ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'class', 0 FROM questions WHERE type='fill_blank' AND order_index=456;

-- order_index=466: "Template definitions are often placed in ____ files so instantiation can work."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'header', 0 FROM questions WHERE type='fill_blank' AND order_index=466;

-- order_index=476: "A constructor with no parameters (or all defaulted) is a ____ constructor."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'default', 0 FROM questions WHERE type='fill_blank' AND order_index=476;

-- order_index=486: "A function marked = 0 is called ____ virtual function."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'pure', 0 FROM questions WHERE type='fill_blank' AND order_index=486;

-- order_index=496: "Template code is generally instantiated at ____ time."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'compile', 0 FROM questions WHERE type='fill_blank' AND order_index=496;

-- ── STL & Modern C++ Pack 04 Part 1 ──────────────────────────────────────────

-- order_index=606: "std::_____ provides O(1) average key lookup using hashing."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'unordered_map', 0 FROM questions WHERE type='fill_blank' AND order_index=606;

-- order_index=616: "Move constructor parameter type is usually ClassName&& (an ____ reference)."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'rvalue', 0 FROM questions WHERE type='fill_blank' AND order_index=616;

-- order_index=626: "Container adaptor with FIFO semantics is std::_____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'queue', 0 FROM questions WHERE type='fill_blank' AND order_index=626;

-- order_index=636: "Lambda capture list syntax uses square ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'brackets', 0 FROM questions WHERE type='fill_blank' AND order_index=636;

-- order_index=646: "Move assignment operator signature — ____ is often recommended."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'noexcept', 0 FROM questions WHERE type='fill_blank' AND order_index=646;

-- ── STL & Modern C++ Pack 04 Part 2 ──────────────────────────────────────────

-- order_index=656: "Container adaptor with LIFO behavior is std::_____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'stack', 0 FROM questions WHERE type='fill_blank' AND order_index=656;

-- order_index=666: "std::_____ can hold a value or no value (empty state)."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'optional', 0 FROM questions WHERE type='fill_blank' AND order_index=666;

-- order_index=676: "Ordered associative container storing unique key-value pairs is std::_____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'map', 0 FROM questions WHERE type='fill_blank' AND order_index=676;

-- order_index=686: "Algorithm returning true if any element matches predicate is std::_____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'any_of', 0 FROM questions WHERE type='fill_blank' AND order_index=686;

-- order_index=696: "An expression that refers to a persistent object identity is generally ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'lvalue', 0 FROM questions WHERE type='fill_blank' AND order_index=696;

-- ── STL & Modern C++ Pack 04 Part 3 ──────────────────────────────────────────

-- order_index=706: "Iterator category required by std::sort is ____-access iterator."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'random', 0 FROM questions WHERE type='fill_blank' AND order_index=706;

-- order_index=716: "std::all_of returns true only if ____ elements satisfy predicate."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'all', 0 FROM questions WHERE type='fill_blank' AND order_index=716;

-- order_index=726: "Lambda introducer and capture list begins with square ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'brackets', 0 FROM questions WHERE type='fill_blank' AND order_index=726;

-- order_index=736: "Named variable expression is typically an ____ value category."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'lvalue', 0 FROM questions WHERE type='fill_blank' AND order_index=736;

-- order_index=746: "std::_____ provides type-safe union-style alternatives."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'variant', 0 FROM questions WHERE type='fill_blank' AND order_index=746;

-- ── STL & Modern C++ Pack 04 Part 4 ──────────────────────────────────────────

-- order_index=756: "Rvalue references use the token ____ in declarations."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, '&&', 0 FROM questions WHERE type='fill_blank' AND order_index=756;

-- order_index=766: "C++20 introduced _____ to express template requirements declaratively."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'concepts', 0 FROM questions WHERE type='fill_blank' AND order_index=766;

-- order_index=776: "std::priority_queue::top() returns the element with highest ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'priority', 0 FROM questions WHERE type='fill_blank' AND order_index=776;

-- order_index=786: "Generic lambda parameters use keyword ____ in parameter list."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'auto', 0 FROM questions WHERE type='fill_blank' AND order_index=786;

-- order_index=796: "In big-O notation, average unordered_map lookup is commonly O(_____)."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, '1', 0 FROM questions WHERE type='fill_blank' AND order_index=796;
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'O(1)', 1 FROM questions WHERE type='fill_blank' AND order_index=796;

-- ── Memory & Concurrency Pack 05 Part 1 ──────────────────────────────────────

-- order_index=906: "A pointer that observes shared ownership without increasing ref-count is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'weak_ptr', 0 FROM questions WHERE type='fill_blank' AND order_index=906;

-- order_index=912: "std::shared_ptr and std::weak_ptr coordinate through a shared control ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'block', 0 FROM questions WHERE type='fill_blank' AND order_index=912;

-- order_index=922: "Calling std::this_thread::_____() hints scheduler to run another thread."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'yield', 0 FROM questions WHERE type='fill_blank' AND order_index=922;

-- order_index=932: "Mutex type allowing same thread to lock multiple times is std::_____mutex."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'recursive_', 0 FROM questions WHERE type='fill_blank' AND order_index=932;
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'recursive', 1 FROM questions WHERE type='fill_blank' AND order_index=932;

-- order_index=942: "std::promise communicates a result to associated std::_____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'future', 0 FROM questions WHERE type='fill_blank' AND order_index=942;

-- order_index=952: "std::future pairs naturally with std::_____ as producer."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'promise', 0 FROM questions WHERE type='fill_blank' AND order_index=952;

-- ── Memory & Concurrency Pack 05 Part 2 ──────────────────────────────────────

-- order_index=961: "Manual memory release for new T uses keyword ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'delete', 0 FROM questions WHERE type='fill_blank' AND order_index=961;

-- order_index=971: "Function returning exclusive owner should typically return std::_____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'unique_ptr', 0 FROM questions WHERE type='fill_blank' AND order_index=971;

-- order_index=981: "std::future::wait() blocks until shared state becomes ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'ready', 0 FROM questions WHERE type='fill_blank' AND order_index=981;

-- order_index=991: "Read-mostly workload with many readers often uses std::shared_____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'mutex', 0 FROM questions WHERE type='fill_blank' AND order_index=991;

-- order_index=1001: "Atomic compare-and-swap style API commonly uses name compare_exchange_____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'weak', 0 FROM questions WHERE type='fill_blank' AND order_index=1001;
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'strong', 1 FROM questions WHERE type='fill_blank' AND order_index=1001;

-- ── Memory & Concurrency Pack 05 Part 3 ──────────────────────────────────────

-- order_index=1011: "A race-free increment of shared integer can use std::atomic<_____>."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'int', 0 FROM questions WHERE type='fill_blank' AND order_index=1011;

-- order_index=1021: "Future operation to retrieve result value is _____()."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'get', 0 FROM questions WHERE type='fill_blank' AND order_index=1021;

-- order_index=1031: "Tag used to indicate lock ownership already acquired is std::adopt_____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'lock', 0 FROM questions WHERE type='fill_blank' AND order_index=1031;

-- order_index=1041: "Type wrapper for lock-free style scalar synchronization is std::_____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'atomic', 0 FROM questions WHERE type='fill_blank' AND order_index=1041;

-- order_index=1051: "Move semantics can help avoid expensive deep ____ of resources."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'copies', 0 FROM questions WHERE type='fill_blank' AND order_index=1051;
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'copy', 1 FROM questions WHERE type='fill_blank' AND order_index=1051;

-- ── Foundations Pack 01 Part 2 ────────────────────────────────────────────────

-- order_index=1206: "C++ standard library symbols like cout are in namespace ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'std', 0 FROM questions WHERE type='fill_blank' AND order_index=1206;

-- order_index=1216: "Keyword often used for compile-time constant variables is _____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'constexpr', 0 FROM questions WHERE type='fill_blank' AND order_index=1216;

-- order_index=1226: "Operator with highest common arithmetic precedence among + and * is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, '*', 0 FROM questions WHERE type='fill_blank' AND order_index=1226;

-- order_index=1236: "Statement that exits current loop immediately is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'break', 0 FROM questions WHERE type='fill_blank' AND order_index=1236;

-- order_index=1246: "Pseudo-random generation utilities are provided in header <____>."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'random', 0 FROM questions WHERE type='fill_blank' AND order_index=1246;

-- ── Foundations Pack 01 Part 3 ────────────────────────────────────────────────

-- order_index=1256: "Directive used to include declarations from another file is #_____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'include', 0 FROM questions WHERE type='fill_blank' AND order_index=1256;

-- order_index=1266: "Named compile-time constant keyword is often _____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'constexpr', 0 FROM questions WHERE type='fill_blank' AND order_index=1266;

-- order_index=1276: "Comparison operators like < and > return type ____ in C++."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'bool', 0 FROM questions WHERE type='fill_blank' AND order_index=1276;

-- order_index=1286: "Keyword that starts alternative branch after if is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'else', 0 FROM questions WHERE type='fill_blank' AND order_index=1286;

-- order_index=1296: "Loop statement that may execute zero times due to precondition is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'while', 0 FROM questions WHERE type='fill_blank' AND order_index=1296;

-- ── Core C++ Pack 02 Part 2 ───────────────────────────────────────────────────

-- order_index=1306: "Method that adds substring at specific position is string::_____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'insert', 0 FROM questions WHERE type='fill_blank' AND order_index=1306;

-- order_index=1316: "Reference that cannot modify referenced object is ____ lvalue reference."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'const', 0 FROM questions WHERE type='fill_blank' AND order_index=1316;

-- order_index=1326: "A reference parameter that should not modify argument is usually qualified ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'const', 0 FROM questions WHERE type='fill_blank' AND order_index=1326;

-- order_index=1336: "Expression p[i] is equivalent to *(p + ____)."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'i', 0 FROM questions WHERE type='fill_blank' AND order_index=1336;

-- order_index=1346: "Method returning pointer to null-terminated char array view is ____()."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'c_str', 0 FROM questions WHERE type='fill_blank' AND order_index=1346;

-- ── Core C++ Pack 02 Part 3 ───────────────────────────────────────────────────

-- order_index=1356: "When passed to function as parameter, C-style array often decays to ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'pointer', 0 FROM questions WHERE type='fill_blank' AND order_index=1356;

-- order_index=1366: "Modern null pointer literal in C++ is _____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'nullptr', 0 FROM questions WHERE type='fill_blank' AND order_index=1366;

-- order_index=1376: "Reference declaration uses symbol ____ after type."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, '&', 0 FROM questions WHERE type='fill_blank' AND order_index=1376;

-- order_index=1386: "Function that returns pointer suitable for C APIs is string::_____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'c_str', 0 FROM questions WHERE type='fill_blank' AND order_index=1386;

-- order_index=1396: "If a function should modify caller variable directly, parameter style is ____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'reference', 0 FROM questions WHERE type='fill_blank' AND order_index=1396;
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, '&', 1 FROM questions WHERE type='fill_blank' AND order_index=1396;
