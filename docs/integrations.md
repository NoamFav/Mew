# Integrations — mew as an ecosystem tool

Short recipes for using `mew` alongside common developer tools.

## Neovim

```vim
" Example: run mew on the current file
nnoremap <leader>m :!mew %<CR>
```

Or via a terminal buffer / toggleterm for interactive sessions.

## fzf

```bash
# Pick a path then hand it to mew
mew "$(fzf)"
```

## Git

```bash
# Review changed files
git diff --name-only | while read -r f; do mew "$f"; done
```

Use a git alias if you run this often:

```gitconfig
[alias]
    mew-changed = !git diff --name-only | xargs -r mew
```

## Shell

```bash
# Fish / zsh / bash helper
mew-here() { mew "${1:-.}"; }
```

Add project-specific wrappers in your shell rc once defaults feel right.

## Notes

Adjust binary name and flags to match your installed `mew` version (`mew --help`).
