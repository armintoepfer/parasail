# parasail: Pairwise Sequence Alignment Library

Author: Jeff Daily (jeff.daily@pnnl.gov)

## Table of Contents

  * [Introduction](#introduction)
  * [A Note About Instruction Sets and CPU Dispatching](#a-note-about-instruction-sets-and-cpu-dispatching)
  * [Installing](#installing)
  * [C Interface Example](#c-interface-example)
    * [Low-Level Functions](#low-level-functions)
    * [Higher-Level Functions](#higher-level-functions)
    * [Substitution Matrices](#substitution-matrices)
    * [Function Lookup](#function-lookup)
  * [Language Bindings](#language-bindings)
    * [C/C++](#cc)
    * [Python](#python)
  * [Example Applications](#example-applications)
  * [Windows Build](#windows-build)
  * [Citing parasail](#citing-parasail)
  * [License: Battelle BSD-style](#license-battelle-bsd-style)

## Introduction

[back to top](#parasail-pairwise-sequence-alignment-library)

parasail is a SIMD C (C99) library containing implementations of the Smith-Waterman (local), Needleman-Wunsch (global), and semi-global pairwise sequence alignment algorithms.  Here, semi-global means insertions before the start or after the end of either the query or target sequence are not penalized.  parasail implements most known algorithms for vectorized pairwise sequence alignment, including diagonal [Wozniak, 1997], blocked [Rognes and Seeberg, 2000], striped [Farrar, 2007], and prefix scan [Daily, 2015].  Therefore, parasail is a reference implementation for these algorithms in addition to providing an implementation of the best-performing algorithm(s) to date on today's most advanced CPUs.

parasail implements the above algorithms currently in two variants, 1) returning only the alignment score, and 2) returning the alignment score with additional alignment statistics (number of exact matches, number of similarities, and alignment length).  The two variants exist because parasail is intended to be high-performing and caculating additional statistics is not free. Select the appropriate implementation for your needs.

Note: When any of the algorithms open a gap, only the gap open penalty alone is applied.

## A Note About Instruction Sets and CPU Dispatching

[back to top](#parasail-pairwise-sequence-alignment-library)

parasail supports the SSE2, SSE4.1, AVX2, and KNC (Xeon Phi) instruction sets.  In many cases, your compiler can compile source code for an instruction set which is not supported by your host CPU.  The code is still compiled, however, parasail uses CPU dispatching at runtime to correctly select the appropriate implementation for the highest level of instruction set supported.

Occasionally, configure will report that your compiler supports an instruction set, e.g., AVX2, when in fact it does not.  This is a bug; please file an issue and I will amend the configure script.  You likely won't discover this until you attempt to compile all of the sources.  If you run into issues (or if for some other reason you wish to disable one of the instruction sets), and after reporting the issue, you can disable the offending code as a temporary work-around using one or more of the following configure flags.
```bash
configure SSE2_FLAGS=choke SSE41_FLAGS=choke AVX2_CFLAGS=choke
```

## Installing

[back to top](#parasail-pairwise-sequence-alignment-library)

parasail follows the typical configure, make, make install steps of other GNU autotools-based installations.  There are no external dependencies.

## C Interface Example

[back to top](#parasail-pairwise-sequence-alignment-library)

All parasail functions have identical function signatures with respect to return types and parameters.

```C
parasail_result_t* the_parasail_function_name(
        const char * const restrict s1, const int s1Len,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap,
        const parasail_matrix_t* matrix);
```
You must free the returned parasail result using `void parasail_result_free(parasail_result_t *result)`.

### Low-Level Functions

[back to top](#parasail-pairwise-sequence-alignment-library)

The function names for the low-level, instruction set-aware functions follow a naming convention which includes
  0. the "parasail_" prefix a.k.a. namespace,
  1. the algorithm (nw/sg/sw),
  2. optionally if the statistics are requested (_stats),
  3. optionally the vectorized approach (_scan/_striped/_blocked/_diag), which requires
     1. the instruction set and vector width (_sse2_128/_sse41_128/_avx2_256/_knc_512), and
     2. the vector element widths (_8/_16/_32/_64 for {sse2,sse41,avx2}, and _32 only for knc).

For example:

- `parasail_nw_stats_striped_sse41_128_16` would use Needleman-Wunsch, with alignment statistics, using striped vectors for sse41 16-bit integers.
- `parasail_sg` would use semi-global, no alignment statistics, no vectorized code (i.e. serial).
- `parasail_sw_scan_avx2_256_8` would use Smith-Waterman, no alignment statistics, using prefix scan vectors for avx2 8-bit integers.

Note: The blocked vector implementation only exist for sse41 16-bit and 32-bit integer elements.

### Higher-Level Functions

[back to top](#parasail-pairwise-sequence-alignment-library)

There are higher-level, instruction set-unaware functions which automatically dispatch to the best available instruction set of your host CPU.  The function names are similar in convention to the low-level functions
  0. the "parasail_" prefix a.k.a. namespace,
  1. the algorithm (nw/sg/sw),
  2. optionally if the statistics are requested (_stats),
  3. optionally the vectorized approach (_scan/_striped/_diag), which requires
    - the desired integer element bit width (_8/_16/_32/_64)

For example:

- `parasail_nw_stats_striped_16` would use Needleman-Wunsch, with alignment statistics, using striped vectors and 16-bit integers.
- `parasail_sg` would use semi-global, no alignment statistics, no vectorized code (i.e. serial).
- `parasail_sw_scan_8` would use Smith-Waterman, no alignment statistics, using prefix scan vectors for 8-bit integers.

Note: The blocked vector implementations do not have a higher-level dispatching function.

Note: The dispatcher for the KNC instruction set will always dispatch to the 32-bit integer element implementation since it is the only one supported on that platform.

### Substitution Matrices

[back to top](#parasail-pairwise-sequence-alignment-library)

parasails bundles a number of substitution matrices including PAM and BLOSUM.  To use them, include the appropriate header, or look them up by name (useful for command-line parsing). For example

```C
#include "parasail.h"
#include "parasail/matrices/blosum62.h"
#include "parasail/matrix_lookup.h"

int main(int argc, char **argv) {
        const char s1 = "asdf";
        const char s2 = "asdf";
        int s1Len = (int)strlen(s1);
        int s2Len = (int)strlen(s2);
        parasail_result_t *result = NULL;
        const parasail_matrix_t *matrix = NULL;
        
        /* note the address-of operator '&' */
        result = parasail_sw(s1, s1Len, s2, s2Len, -11, -1, &parasail_blosum62);
        parasail_result_free(result);
        
        matrix = parasail_matrix_lookup("pam100");
        result = parasail_sw(s1, s1Len, s2, s2Len, -11, -1, matrix);
        parasail_result_free(result);
}
```

### Function Lookup

[back to top](#parasail-pairwise-sequence-alignment-library)

Typically used for command-line or user-input parsing, the parasail functions can be accessed using their string name.  For example

```C
#include "parasail.h"
#include "parasail/matrices/blosum62.h"

int main(int argc, char **argv) {
        const char s1 = "asdf";
        const char s2 = "asdf";
        int s1Len = (int)strlen(s1);
        int s2Len = (int)strlen(s2);
        parasail_result_t *result = NULL;
        const parasail_matrix_t *matrix = NULL;
        parasail_function_t *function = NULL;
        
        function = parasail_lookup_function(argv[1]);
        result = function(s1, s1Len, s2, s2Len, -11, -1, &parasail_blosum62);
        parasail_result_free(result);
        
        /* 'parasail_' prefix is optional */
        function = parasail_lookup_function("nw_striped_32");
        result = function(s1, s1Len, s2, s2Len, -11, -1, &parasail_blosum62);
        parasail_result_free(result);
}
```

## Language Bindings

[back to top](#parasail-pairwise-sequence-alignment-library)

### C/C++

[back to top](#parasail-pairwise-sequence-alignment-library)

C is the native API for parasail.  C++ is supported by using the common include guards (#ifdef __cplusplus).  Once you have installed parasail, #include "parasail.h" into your sources.  

### Python

[back to top](#parasail-pairwise-sequence-alignment-library)

Once you have installed parasail into --prefix=$PREFIX, you can also compile the Python bindings.  Don't forget to add $PREFIX/lib to your LD_LIBRARY_PATH, if needed.  The Python bindings are in the <parasail>/bindings/python directory.  To build, run:

```
PARASAIL_PREFIX=$PREFIX python setup.py build
```

This will correctly setup the necessary CPPFLAGS, LDFLAGS, and LIBS variables during the build.  Becuase the parasail.h header uses C99 keywords, e.g., restrict, the setup.py process will test your C compiler for the correct use of restrict, automatically.

The Python interface only includes bindings for the dispatching functions, not the low-level instruction set-specific function calls.  The Python interface also includes wrappers for the various PAM and BLOSUM matrices included in the distribution.

Example:

```python
import parasail
result = parasail.sw_scan_16("asdf", "asdf", -11, -1, parasail.blosum62)
result = parasail.sw_stats_striped_8("asdf", "asdf", -11, -1, parasail.pam100)
```

## Windows Build

[back to top](#parasail-pairwise-sequence-alignment-library)

A parasail.dll was successfully created using a combination of Cygwin and its mingw64 package.

```bash
cd parasail
mkdir bld # it's cleaner to keep build and source directories separated
cd bld
../configure --prefix=`pwd` --host=x86_64-w64-mingw32 --build=x86_64-pc-cygwin
make
make install # installs DLL into `pwd`/bin
ldd bin/parasail.dll
        ntdll.dll => /cygdrive/c/Windows/SYSTEM32/ntdll.dll (0x77910000)
        kernel32.dll => /cygdrive/c/Windows/system32/kernel32.dll (0x777f0000)
        KERNELBASE.dll => /cygdrive/c/Windows/system32/KERNELBASE.dll (0x7fefd790000)
        msvcrt.dll => /cygdrive/c/Windows/system32/msvcrt.dll (0x7fefdbc0000)
        libwinpthread-1.dll => /usr/x86_64-w64-mingw32/sys-root/mingw/bin/libwinpthread-1.dll (0x64940000)
        USER32.dll => /cygdrive/c/Windows/system32/USER32.dll (0x776f0000)
        GDI32.dll => /cygdrive/c/Windows/system32/GDI32.dll (0x7fefdc60000)
        LPK.dll => /cygdrive/c/Windows/system32/LPK.dll (0x7fefde30000)
        USP10.dll => /cygdrive/c/Windows/system32/USP10.dll (0x7fefde40000)
        IMM32.DLL => /cygdrive/c/Windows/system32/IMM32.DLL (0x7feff370000)
        MSCTF.dll => /cygdrive/c/Windows/system32/MSCTF.dll (0x7feff000000)
        KATRK64.DLL => /cygdrive/c/Windows/KATRK64.DLL (0x180000000)
        WTSAPI32.dll => /cygdrive/c/Windows/system32/WTSAPI32.dll (0x7fefc920000)
```
Note that parasail.dll depends on a `libwinpthread-1.dll` which is located in the sys-root of the corresponding mingw installation.  See the `ldd` output above for an example.  If it weren't for the `parasail_time(void)` function, parasail would have zero external dependencies.

## Example Applications

[back to top](#parasail-pairwise-sequence-alignment-library)

In addition to the parasail library, there are a few binaries that are also compiled and installed.
See [the apps README](apps/README.md) for more details.

## Citing parasail

[back to top](#parasail-pairwise-sequence-alignment-library)

If needed, please cite the following paper.

Daily, Jeff. 2015. "Scalable Parallel Methods for Analyzing Metagenomic Data at Extreme Scale". PhD dissertation, Washington State University.  http://hpc.pnl.gov/tascel/papers/PNNL-24266.pdf

## License: Battelle BSD-style

[back to top](#parasail-pairwise-sequence-alignment-library)

Copyright (c) 2015, Battelle Memorial Institute

1.  Battelle Memorial Institute (hereinafter Battelle) hereby grants
    permission to any person or entity lawfully obtaining a copy of this
    software and associated documentation files (hereinafter “the
    Software”) to redistribute and use the Software in source and binary
    forms, with or without modification.  Such person or entity may use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and may permit others to do so, subject to
    the following conditions:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimers.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.

    - Other than as used herein, neither the name Battelle Memorial
      Institute or Battelle may be used in any form whatsoever without
      the express written consent of Battelle.

    - Redistributions of the software in any form, and publications
      based on work performed using the software should include the
      following citation as a reference:

    Daily, Jeff. 2015. "Scalable Parallel Methods for Analyzing
    Metagenomic Data at Extreme Scale". PhD dissertation, Washington
    State University.  http://hpc.pnl.gov/tascel/papers/PNNL-24266.pdf

2.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BATTELLE
    OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
    USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.

