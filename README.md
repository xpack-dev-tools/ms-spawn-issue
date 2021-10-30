# ms-spawn-issue

This project provides several test programs to diagnose
the error reported by simple shells like `sh.exe` from BusyBox,
when invoked from GNU `make.exe`.

The error occurs when `sh.exe` is built to use the Microsoft
Universal C Runtime, available since Windows 7 SP1 from 2011.

The error did not occur in the past when `sh.exe` was built
to use the legacy MSVCRT API.

## How to test

Clone this project to a folder of your choice:

```doscmd
C:\Users\ilg>cd tmp
C:\Users\ilg\tmp>git clone https://github.com/xpack-dev-tools/ms-spawn-issue.git
Cloning into 'ms-spawn-issue'...
remote: Enumerating objects: 19, done.
remote: Counting objects: 100% (19/19), done.
remote: Compressing objects: 100% (16/16), done.
remote: Total 19 (delta 2), reused 15 (delta 2), pack-reused 0
Receiving objects: 100% (19/19), 172.08 KiB | 1.83 MiB/s, done.
Resolving deltas: 100% (2/2), done.

C:\Users\ilg\tmp>
```

Change to the `\tests` folder and run the tests:

```doscmd
C:\Users\ilg\tmp>cd ms-spawn-issue\tests

C:\Users\ilg\tmp\ms-spawn-issue\tests>make
spawn-env -c "dump-env one two"
sh_start
sh_env[0]='ALLUSERSPROFILE=C:\ProgramData'
sh_env[1]='APPDATA=C:\Users\ilg\AppData\Roaming'
...
sh_env[47]='USERPROFILE=C:\Users\ilg'
sh_env[48]='windir=C:\WINDOWS'
sh_argv[0]='spawn-env'
sh_argv[1]='-c'
sh_argv[2]='dump-env one two'
sh_spawn 'dump-env.exe'...
make: *** [makefile:3: all] Error -1073741819

C:\Users\ilg\tmp\ms-spawn-issue\tests>make null
spawn-null -c "dump-env one two"
sh_start
sh_env[0]='ALLUSERSPROFILE=C:\ProgramData'
sh_env[1]='APPDATA=C:\Users\ilg\AppData\Roaming'
...
sh_env[47]='USERPROFILE=C:\Users\ilg'
sh_env[48]='windir=C:\WINDOWS'
sh_argv[0]='spawn-null'
sh_argv[1]='-c'
sh_argv[2]='dump-env one two'
sh_spawn 'dump-env.exe'...

envp[0]='ALLUSERSPROFILE=C:\ProgramData'
envp[1]='APPDATA=C:\Users\ilg\AppData\Roaming'
...
envp[47]='USERPROFILE=C:\Users\ilg'
envp[48]='windir=C:\WINDOWS'
argv[0]='dump-env'
argv[1]='one'
argv[2]='two'
sh_ret=0 errno=0

C:\Users\ilg\tmp\ms-spawn-issue>
```

The `spawn-*.exe` binaries behave like a very simple shell used to
start a child process `dump-env.bin`.

The tests show the environment before the spawn and in the child process.

The first test fails exactly at the `_spawnvpe()` call.

Starting exactly the same command that failed from make, but this time
from a terminal, works as expected:

```doscmd
C:\Users\ilg\tmp\ms-spawn-issue\tests>spawn-env -c "dump-env one two"
sh_start
sh_env[0]='ALLUSERSPROFILE=C:\ProgramData'
sh_env[1]='APPDATA=C:\Users\ilg\AppData\Roaming'
...
sh_env[42]='USERPROFILE=C:\Users\ilg'
sh_env[43]='windir=C:\WINDOWS'
sh_argv[0]='spawn-env'
sh_argv[1]='-c'
sh_argv[2]='dump-env one two'
sh_spawn 'dump-env.exe'...

envp[0]='ALLUSERSPROFILE=C:\ProgramData'
envp[1]='APPDATA=C:\Users\ilg\AppData\Roaming'
...
envp[42]='USERPROFILE=C:\Users\ilg'
envp[43]='windir=C:\WINDOWS'
argv[0]='dump-env'
argv[1]='one'
argv[2]='two'
sh_ret=0 errno=0

C:\Users\ilg\tmp\ms-spawn-issue\tests>
```

## Source files

Both tests are based on the same source code (`src/spawn.c`), first compiled to
forward the environment to the child process, then compiled to pass a NULL
pointer as environment, which is a shortcut to inherit the entire environment.

The `spawn-env.exe` and `spawn-null.exe` binaries were compiled with
Visual Studio on a Windows 10 machine.

The code triggers several warnings; to disable them please read:

- <https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4996>

## Conclusion

The current theory is that when Microsoft migrated from MSVCRT to UCRT
they changed the implementation and introduced a subtle bug that does not
show in the common use case.

However, `make.exe` uses an elaborate mechanism to create sub-processes,
and the process running the `spawn-*.exe` is somehow different, and this
difference triggers the bug when this process tries to spawn a new process
with explicit environment.

## Contributions

Please open new GitHub [Issues](https://github.com/xpack-dev-tools/ms-spawn-issue/issues)
with your findings when investigating this issue.

Thank you,

Liviu
