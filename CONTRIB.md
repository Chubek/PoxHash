# Guidelines for Contributing to PoxHash Repository on Github

There is one major fork in every contribution:

1- New implementations in any language, and major changes to formal implementations belong in [contrib](/contrib/) directory.

2- Non-fundemental changes to the formal implementation have to be PR'd to the exising files.

# Conduct

## Conduct for `contrib` Directory

1- Every new implementation must go inside a new subdirectory of the implementation's paradigm. There's just three of them. Functional, OOP and Miscellaneous Paradigms.

2- The name of that subdirectory must followw this format:

```
<language>-<identifying feature>-<the contributor's alias or name>
```

The identifying feature would be the one thing that makes the implementation unique.

3- The following format rules must be abided by:

- - No tabulations in indentation! Every tab is 4 spaces.
- - The code must follow the most common naming conventions of the language.
- - The 'stock' names of variables and operations in [SPEC.md](/SPEC.md) must be followed. For example you are not allowed to rename operation `alpha` to `alef`.
- - The code must make it claer where each section of the algorithm is.

4 - The coe must not export any unnecessary symbols. The only symbols that the code must export are the interfaces.

5- If you make a new digest standard, you must writee a spec file for it.

6- The final 4 u16 factors of identical messags passed to you code, and the formal implementation, must be the same.

7- The code must be relased under MIT license. All the files have to include this on top:

```
MIT License

Copyright <YEAR> <COPYRIGHT HOLDER>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```

## Conduct for PRs to Formal Implementations

Just some simple rules:

1- Don't change the formatting.
2- Don't change PoxHash-reserved symbol names.
3- Don't introduce bugs.
4- Make your changes beneficial. For example speed boosts etc are welcome.

# My Contacts

If you made a PR to both formal implementations and the `contrib` directory, and I did not respond, please contact me in one of the following ways:

- Chubakbidpaa[at]gmail[dot]com
- Chubak#7400 on Discord
- @bidpaafx on Telegram

**This is a live docment. It will definitely be updated in the future**
