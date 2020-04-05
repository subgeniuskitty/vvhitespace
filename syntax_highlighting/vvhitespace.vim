" Vim syntax file
" Language: VVhitespace
" Based on: https://vim.fandom.com/wiki/Creating_your_own_syntax_files

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
