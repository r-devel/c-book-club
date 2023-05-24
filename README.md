# C book club for R contributors

This is a book club meeting every two weeks for existing/aspiring R contributors that want to learn/refresh their C programming skills.

It will run from May to August 2023, so that people attending R Project Sprint 2023 have at least a basic understanding of C and how it interfaces with R. 
However, you do not have to be a sprint participant to join the book club!

## Readings

Our main reading will be [Modern C](https://gustedt.gitlabpages.inria.fr/modern-c/) by Jens Gustedt. A PDF is available for free download, or there is a 35% discount code for a print or eBook copy (see the [Modern C](https://gustedt.gitlabpages.inria.fr/modern-c/) website). This book is organized in levels: 

 - **0 - Encounter:** summarizes the very basics of programming with C.
 - **1 - Acquaintance:** details most principal concepts and features such as control structures, data types, operators, and functions.
 - **2 - Cognition:** goes to the heart of the C language, covering pointers, C’s memory model, and most of C’s library interface. 
 - **3 - Experience:** goes into detail about specific topics, such as performance, reentrancy, atomicity, threads, and type-generic programming. 
 
This book club will cover **levels 0 to 2** (about 200 pages).
 
The reading will be supplemented with:

 - Selected exercises from [C Programming: A Modern Approach](http://knking.com/books/c2/) by K. N. King (shared under fair use).
 - The [chapter on R's C interface](http://adv-r.had.co.nz/C-interface.html) from Advanced R (1st ed.) by Hadley Wickham (free online).
 
See [exercises](/exercises) for exercises for each session.

## Commitment

There are three levels of commitment:

- **Low (1-2 hrs/wk):** Read the assigned readings, work through the examples in the main text and participate in the online meetings every two weeks.
- **Medium (2-3 hrs/wk, recommended):** Same as Low, but also try (some of) the selected exercises.
- **High (~1 day/wk):** Same as Medium, but also try (some of) the challenges in Modern C.

At the online meetings, one book club member will present a summary of the chapter (~10 minutes), reviewing the main points and highlighting any quotes/examples they found particularly helpful/important. This will be followed by discussion, sharing exercise solutions, etc. 
Book club members are encouraged to sign up to present at one session to spread the load among the group.

## Timing

The book club will meet in two cohorts:

-   **Cohort 1:** 12 noon - 1 pm UTC ([find 12 noon UTC in your local time](https://everytimezone.com/s/b3dd06ec)).
-   **Cohort 2:** 10 am - 11 am PDT ([find 10 am PDT in your local time](https://everytimezone.com/s/1dd66f3f)).

Sign-up to attend the sessions on the [R Contributors Meetup](https://www.meetup.com/r-contributors/events/).

## Schedule

Please sign up to present at one of the sessions, by making a PR to this page. Links to exercises will be added in due course.

| No. | Date      | Level | Book                | Chapters | Page count | Topic(s)                                                 | Cohort 1 Presenter | Cohort 2 Presenter |
|-----|-----------|-------|---------------------|----------|------------|----------------------------------------------------------|--------------------|--------------------|
| 1   | May 16    | 0     | Modern C            | 1-3      | 28         | Compiling & running, program structure, flow control     | Heather Turner     |                    |
| 2   | May 30    | 1     | Modern C            | 4-5      | 36         | Arithmetic & logical operators, data types               | Trang Le           |                    |
| 3   | Jun 13    | 1     | Modern C            | 6-7      | 25         | Derived data types, functions                            |                    | Elio Campitelli    |
| 4   | Jun 27    | 1     | Modern C            | 8-10     | 31         | C library functions, style, organization & documentation | María Nanton       |                    |
| 5   | Jul 11    | 1.5   | Advanced R (1st ed) | 20       | 20         | R's C interface                                          | Roberto Villegas-Diaz |                 |
| 6   | Jul 25    | 2     | Modern C            | 11-12    | 27         | Pointers & the C memory model                            | Malcolm Barrett    |                    |
| 7   | Aug 8     | 2     | Modern C            | 13       | 20         | Storage                                                  | Ivan Krylov        |                    |
| 8   | Aug 22    | 2     | Modern C            | 14       | 19         | More involved processing & IO                            |                    |                    |       |

### August Bonus Exercise

This exercise is recommended for August, when there is not so much to do in the main schedule. You might do the two parts separately, whenever you have time. Getting set up to build R from source can take some time, especially on Windows and MacOS. This is an exercise in building R, making a change to the C code and rebuilding to test the change.

1. Learn to build the development version of R on your OS (see [R Patched and Development Versions](https://contributor.r-project.org/rdevguide/GetStart.html) in the R Development Guide).
2. Review this [patch for `gregexpr(perl=TRUE)`](https://stat.ethz.ch/pipermail/r-devel/2019-February/077315.html), which proposes a single line change to the C code in base R. 
    - Run the [test code](https://github.com/tdhock/namedCapture-article/blob/master/figure-trackDb-Ronly.R) written by Toby Dylan Hocking who proposes the fix, in your build of R-devel. The result should be similar to the "after" plot presented in the [19 Feb 2019 section of the README](https://github.com/tdhock/namedCapture-article/#19-feb-2019). 
    - Modify your copy of `src/main/grep.c` to reverse the patch and rebuild R. Run the test code again to reproduce the old behaviour.
