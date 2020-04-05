VVhitespace Syntax Highlighting
===============================

![VVS Syntax Highlighting Example](vvs_syntax_highlighting.gif)

I find it useful to highlight each IMP a different color as well as highlight
code comments and label definitions. The above image is typical of my
VVhitespace editing environment.

If you would like something similar, the configuration examples below can be
used with `vim` or `neatvi`. Other editors that use regex matching can reuse
the regular expressions from these examples to achieve similar results.

This syntax highlighting definition works only with the way I write VVhitespace
code in this repository. Comments in particular can take many forms other
than specified here.

If you take the time to put together a configuration for other editors, or
improve the regex definitions, let me know.


VIM Highlighting
----------------

If they don't already exist, create the folders `~/.vim/syntax` and
`~/.vim/ftdetect`.

    % mkdir -p ~/.vim/syntax
    % mkdir -p ~/.vim/ftdetect

Create a file named `~/.vim/ftdetect/vvhitespace.vim` to detect the filetype
based on the file extension. It should contain a single line:

    au BufRead,BufNewFile *.pvvs set filetype=vvhitespace

Create a file named `~/.vim/syntax/vvhitespace.vim` containing the following:

    if exists("b:current_syntax")
      finish
    endif
    
    let b:current_syntax = "vvhitespace"
    
    " Stack IMP
    syn match stackIMP '^[sS][sS]'
    syn match stackIMP '^[sS][nN][sStTnN]'
    
    " Arithmetic IMP
    syn match arithIMP '^[tT][sS][sStT][sStTnN]'
    
    " Heap IMP
    syn match heapIMP '^[tT][tT][sStT]'
    
    " Control Flow IMP
    syn match controlIMP '^[nN][nNsStT][nNsStT]'
    
    " IO IMP
    syn match ioIMP '^[tT][nN][sStT][sStT]'
    
    " Label Definition
    syn match labelDef 'V[sStT]\+N'
    
    " Comments
    syn match commentBlock '|.*$'
    syn match commentBlock '@.*$'
    
    " Define colors using existing Highlight Groups.
    " To see a list of other Highlight Groups, type `:highlight` in vim.
    hi def link stackIMP     Type
    hi def link arithIMP     ModeMsg
    hi def link heapIMP      Special
    hi def link controlIMP   Constant
    hi def link ioIMP        Keyword
    hi def link labelDef     Function
    hi def link commentBlock NonText

That's it! You should now see syntax highlighting when opening files with names
ending in `*.pvvs`.

If you don't see any syntax highlighting, check your `.vimrc` and other config
locations for conflicting options. Try manually enabling syntax highlighting
with the `syntax on` configuration directive.


NeatVI Highlighting
-------------------

Open the file `conf.h` for editing.

Add the following line to the `filetypes` array, matching all `*.pvvs` files.

    {"pvvs", "\\.pvvs$"},    /* VVhitespace */

Add the following block to the `highlights` array.

    {"pvvs", {2}, "^[sS][sS]"},                  /* IMP: Stack Manipulation */
    {"pvvs", {2}, "^[sS][nN][sStTnN]"},          /* IMP: Stack Manipulation */
    {"pvvs", {8}, "^[tT][sS][sStT][sStTnN]"},    /* IMP: Arithmetic */
    {"pvvs", {5}, "^[tT][tT][sStT]"},            /* IMP: Heap Access */
    {"pvvs", {1}, "^[nN][nNsStT][nNsStT]"},      /* IMP: Control Flow */
    {"pvvs", {4}, "^[tT][nN][sStT][sStT]"},      /* IMP: Input/Output */
    {"pvvs", {6 | SYN_BD}, "V[sStT]+N "},        /* Label Definitions */
    {"pvvs", {3 | SYN_IT}, "\\|.*$"},            /* Comments */
    {"pvvs", {3 | SYN_IT}, "\\@.*$"},            /* Comments */

Recompile NeatVI and syntax highlighting should work on all files with names
ending in `*.pvvs`.
