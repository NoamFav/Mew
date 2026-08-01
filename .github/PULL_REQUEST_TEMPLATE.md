## What does this PR do?

<!-- A short description of the change. What problem does it solve, or what feature does it add? -->

## Type of change

<!-- Check the one that applies -->

- [ ] `feat` — new feature or behaviour
- [ ] `fix` — bug fix
- [ ] `refactor` — restructuring without changing behaviour
- [ ] `docs` — documentation only
- [ ] `build` — Makefile, compiler flags, CI

## Testing

<!-- Describe how you tested this. List the commands you ran. -->

```sh
# example:
echo "hello" | ./mew
./mew main.c src/mew.c
./mew /nonexistent   # should print error to stderr, exit 1
```

## Checklist

- [ ] `make re` compiles cleanly with zero warnings
- [ ] Tested with files, stdin (`./mew -`), and a missing file
- [ ] No debug prints or commented-out code left behind
- [ ] No `stdio.h` / `printf` / `malloc` introduced
- [ ] Commit messages follow the `type(scope): description` format

## Related issue

<!-- Link with: Closes #N -->
