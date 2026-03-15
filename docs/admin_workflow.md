# CppAtlas Administrator Workflow

## Entry points

| Method | Description |
|---|---|
| `--admin` CLI flag | Launch the app with `./CppAtlas --admin`. Opens the admin panel immediately after login. |
| `Ctrl+Alt+Shift+M` | Keyboard shortcut while the main window is active. |

## Password requirements

| Build type | Second password gate |
|---|---|
| **Debug** (`QT_DEBUG`) | Disabled — panel opens after a single admin-account login. |
| **Release** | Enabled — admin must pass a second SHA-256 hash check. |

### Setting the release admin password hash

Export the environment variable before launching the app:

```bash
# Compute SHA-256 hex of your chosen admin password:
echo -n "MySecretPassword" | sha256sum   # Linux/macOS
# Example output: 8b8f79a0d0...  -

export CPPATLAS_ADMIN_HASH="8b8f79a0d0..."
./CppAtlas
```

`CPPATLAS_ADMIN_HASH` must be a **SHA-256 hex digest** (64 lowercase hex characters) of the
admin password.  If the variable is absent or empty, `verifyAccess()` returns `Error` in
release builds (access denied).

## Content patch workflow

Patches are plain SQL files placed in a directory you control (not shipped in the app bundle).

### Patch naming convention

```
YYYY_MM_DD_description.sql
```

Examples:
- `2025_01_15_add_cpp20_questions.sql`
- `2025_03_01_fix_typos.sql`

Patches are discovered and sorted **lexicographically by filename**, so the date prefix ensures
correct application order.

### Applying patches

**Via admin UI (QuizAdminPanel → "Apply Content Updates"):**

1. Click **Apply Content Updates**.
2. Select the directory containing your `*.sql` patch files.
3. The panel reports total / already-applied / applied-now counts.
4. The last-used directory is remembered between sessions.

**Via CLI:**

```bash
quiz_admin --db /path/to/cppatlas.db apply-content --content-dir ./patches
```

### Rollback behaviour

Each patch is wrapped in a database transaction.  If SQL execution **or** `commit()` fails:

- The transaction is rolled back automatically.
- The patch is **not** recorded in `content_patches`.
- Processing stops at the failing patch; subsequent patches are not attempted.
- The error message is printed to `stderr` (CLI) or the log panel (UI).

## Backup / Export

Export a deterministic SQL dump of all content tables:

**Via admin UI (QuizAdminPanel → "Export Backup"):**

1. Click **Export Backup**.
2. Choose a destination `.sql` file.
3. The panel reports how many rows were written.

**Via CLI:**

```bash
quiz_admin --db /path/to/cppatlas.db export --out backup_$(date +%Y%m%d).sql
```

Tables exported (in order):  
`topics`, `tags`, `quizzes`, `questions`, `options`, `question_tags`, `quiz_tags`

## Validate content

Run integrity checks without writing anything:

```bash
quiz_admin --db /path/to/cppatlas.db validate --content-dir ./patches
```

Reports:
- Patch status (applied / pending) for each `*.sql` file found.
- MCQ questions without any options.
- MCQ questions without a correct option.
- Orphan options (referencing deleted questions).
