This directory contains code which should not be distributed to students, such
as test case generators and solutions. It should NOT be included in any project
templates given to students.


# `gentestcase.py`

The `gentestcase.py` script will allow generating a single test case input and
output suitable for this project. You can run it with `python3
private/gentestcase.py --help` to view help documentation.

If you see an error message like:

```
Traceback (most recent call last):
  File "/home/cad/src/shic/labs/lab_2021sp_parallel/private/gentestcase.py", line 18, in <module>
    from waves import Waves
  File "/home/cad/src/shic/labs/lab_2021sp_parallel/utils/python_utils/waves.py", line 10, in <module>
    from pyDigitalWaveTools.vcd.parser import VcdParser
ModuleNotFoundError: No module named 'pyDigitalWaveTools'
```

Try running `sh grade.sh ; . ./.venv/bin/activate`. This script depends on
libraries that are installed automatically by `grade.sh`.

# `compile_labsheet.sh`

This script will compile `../README.md` to `../README.pdf` using `pandoc`.  You
need to have [Eisvogel](https://github.com/Wandmalfarbe/pandoc-latex-template)
installed for this to work, as well as `pandoc`.

# `make_tarballs.sh`

This script will generate the tarballs with the project templates and solutions
for upload into Moodle.
