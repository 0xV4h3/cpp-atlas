# quiz_admin — CppAtlas Quiz Admin CLI

Command-line tool for inspecting and validating a CppAtlas quiz database without starting the full GUI application.

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
$ quiz_admin --db /var/data/cppatlas.db validate --content-dir ./patches
Database: /var/data/cppatlas.db

=== CppAtlas Quiz Content Validation ===

Content directory : ./patches

Patches (3 total):
  001_add_topics.sql     [APPLIED]
  002_add_questions.sql  [APPLIED]
  003_fix_typos.sql      [PENDING]

  Applied : 2  Pending : 1  Total : 3

Content integrity:
  MCQ questions without options  : 0
  Orphan options                 : 0
  MCQ questions without correct  : 0

Result: OK
```
