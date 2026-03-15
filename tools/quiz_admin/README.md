# quiz_admin — CppAtlas Quiz Admin CLI

Command-line tool for inspecting, validating, patching, and exporting a CppAtlas quiz database without starting the full GUI application.

## Build

`quiz_admin` is built as part of the normal CppAtlas CMake build:

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target quiz_admin
```

The binary is placed in `build/tools/quiz_admin/quiz_admin` (Linux/macOS) or `build\tools\quiz_admin\quiz_admin.exe` (Windows).

---

## Global options

| Option | Description |
|---|---|
| `--db <path>` | Path to the SQLite database file. Defaults to the platform-standard CppAtlas data location (`QStandardPaths::AppDataLocation/cppatlas.db`). |
| `--help`, `-h` | Print usage and exit. |

---

## Commands

### `stats`

Print a summary of database contents.

**Usage**

```sh
quiz_admin [--db <path>] stats
```

**Output includes**

- Schema version
- Content counts: topics, active quizzes, active questions, tags
- User counts: total, admins
- Session counts: total, completed
- Applied content patches

**Example**

```sh
$ quiz_admin stats
Database: /home/user/.local/share/CppAtlas/cppatlas.db

=== CppAtlas Quiz Database Statistics ===

Schema version : 3

Content:
  Topics    : 42
  Quizzes   : 15
  Questions : 320
  Tags      : 8

Users:
  Total  : 12
  Admins : 2

Sessions:
  Total     : 150
  Completed : 130

Content Patches:
  Applied : 3
```

---

### `validate`

Discover `*.sql` patch files in a directory, report their applied/pending status against the database, and run basic content integrity checks.

**Usage**

```sh
quiz_admin [--db <path>] validate --content-dir <dir>
```

| Option | Description |
|---|---|
| `--content-dir <dir>` | Directory containing `*.sql` patch files sorted lexicographically by filename. The patch id for each file is the filename without its `.sql` extension. |

**Exit codes**

| Code | Meaning |
|---|---|
| `0` | All checks passed, no warnings. |
| `1` | Usage or argument error. |
| `2` | One or more integrity warnings found. |

**Integrity checks**

- MCQ questions with no options
- Orphan option rows (no matching question)
- MCQ questions with no correct answer

**Example**

```sh
$ quiz_admin validate --content-dir ./patches
Database: /home/user/.local/share/CppAtlas/cppatlas.db

=== CppAtlas Quiz Content Validation ===

Content directory : ./patches

Patches (3 total):
  001_add_topics  [APPLIED]
  002_questions   [APPLIED]
  003_fix_typos   [PENDING]

  Applied : 2  Pending : 1  Total : 3

Content integrity:
  MCQ questions without options  : 0
  Orphan options                 : 0
  MCQ questions without correct  : 0

Result: OK
```

---

### `apply-content`

Discover `*.sql` patch files in a directory and apply any that have not yet been recorded in the database.  Patches are applied in lexicographic order.  The command stops and returns a non-zero exit code on the first failure.

**Usage**

```sh
quiz_admin [--db <path>] apply-content --content-dir <dir>
```

| Option | Description |
|---|---|
| `--content-dir <dir>` | Directory containing `*.sql` patch files. |

**Exit codes**

| Code | Meaning |
|---|---|
| `0` | Success — all pending patches applied (or none were pending). |
| `1` | Usage or argument error. |
| `2` | A patch failed to apply. |

**Example**

```sh
$ quiz_admin apply-content --content-dir ./patches
Database: /home/user/.local/share/CppAtlas/cppatlas.db

=== CppAtlas Apply Content Patches ===

Content directory : ./patches

Patches total   : 3
Already applied : 2
Pending         : 1

Applied now     : 1

Result: OK
```

---

### `export`

Export all rows from the quiz content tables to a SQL dump file.  Output uses `INSERT INTO` statements with deterministic `ORDER BY id` ordering (or composite key ordering for junction tables).

**Usage**

```sh
quiz_admin [--db <path>] export --out <file>
```

| Option | Description |
|---|---|
| `--out <file>` | Output file path for the SQL dump. The file is created or overwritten. |

**Tables exported**

`topics`, `tags`, `quizzes`, `questions`, `options`, `question_tags`, `quiz_tags`

**Exit codes**

| Code | Meaning |
|---|---|
| `0` | Success. |
| `1` | Usage or argument error. |
| `2` | File or SQL error during export. |

**Example**

```sh
$ quiz_admin export --out backup.sql
Database: /home/user/.local/share/CppAtlas/cppatlas.db

=== CppAtlas Content Export ===

  topics: 42 row(s)
  tags: 8 row(s)
  quizzes: 15 row(s)
  questions: 320 row(s)
  options: 1280 row(s)
  question_tags: 95 row(s)
  quiz_tags: 30 row(s)

Total rows exported : 1790
Output file         : backup.sql

Result: OK
```

